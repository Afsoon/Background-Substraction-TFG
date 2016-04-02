/*
 * MediaMovil.cpp
 *
 *  Created on: 1/10/2015
 *      Author: Said
 */

#include "MediaMovil.h"

MediaMovil::MediaMovil() {
	// TODO Auto-generated constructor stub

}

MediaMovil::~MediaMovil() {
	// TODO Auto-generated destructor stub
}
/**
 * Recibe el umbral que queremos que se aplique durante el proceso
 * @param thresold un valor comprendido entre 0 y 255
 */
void MediaMovil::setThresold(float thresold){
	this->threshold = thresold;
}
/**
 * Recibe el factor de aprendizaje que queremos aplicar durante el proceso
 * @param learning un valor comprendido entre 0-1.
 */
void MediaMovil::setLearning_rate(double learning){
	this->learningRate = learning;
}

void MediaMovil::setPixelSelection(bool pixelSelection){
	this->pixelSelection = pixelSelection;
}

/**
 * En el estado inicial, copiamos los valores actuales, en escala de grises, de cada píxel e inicializamos el modelo de fondo.
 * El resultado de la primera pasada es una imagen totalmente negra.
 *
 * En las siguientes pasadas, se va procesando cada frame de entrada con el modelo de fondo. El proceso de actualización que se realiza
 * es el indicado por el usuario. Si no ha indicado nada, por defecto se realiza una selección de píxeles.
 *
 * No se deja por defecto el proceso de actualización sin selección de píxeles ya que se contamina los valores del fondo
 * con píxeles que pertenecen al primer plano.
 *
 * @param input Fotograma, del vídeo, que se va a manipular
 * @param output Máscara del primer plano obtenido tras manipular el fotograma de entrada
 */
void MediaMovil::process(cv::Mat& input, cv::Mat& output){
	if(this->pixelSelection){
		processWithSelectionPixel(input, output);
	}else{
		processWithoutSelectionPixel(input, output);
	}
}

void MediaMovil::processWithoutSelectionPixel(cv::Mat& input, cv::Mat& output){
	cv::Mat gray;
	cv::Mat backImage;
	cv::Mat foreground;

	cv::cvtColor(input, gray, CV_BGR2GRAY);
	if(this->backgroundModel.empty()){
		gray.convertTo(this->backgroundModel, CV_32F,  1./255.);
	}

	bool clone = false;
	if(this->oldFrame.empty()){
		this->oldFrame = gray.clone();
		clone = true;
	}

	this->backgroundModel.convertTo(backImage, CV_8U);

	cv::absdiff(backImage, gray, foreground);

	cv::threshold(foreground, output, this->threshold, 255, cv::THRESH_BINARY);

	cv::accumulateWeighted(this->oldFrame, this->backgroundModel, this->learningRate);
	if(clone){
		clone = false;
	}else{
		this->oldFrame = gray.clone();
	}

}

void MediaMovil::processWithSelectionPixel(cv::Mat& input, cv::Mat& output){
	cv::Mat gray;
	cv::Mat backImage;
	cv::Mat foreground;

	cv::cvtColor(input, gray, CV_BGR2GRAY);
	if(this->backgroundModel.empty()){
		gray.convertTo(this->backgroundModel, CV_32F, 1./255.);
	}
	bool clone = false;
	if(this->oldFrame.empty()){
		this->oldFrame = gray.clone();
		clone = true;
	}

	if(!clone){
		cv::accumulateWeighted(this->oldFrame, this->backgroundModel, this->learningRate, this->maskBackground);
	}

	this->backgroundModel.convertTo(backImage, CV_8U);

	cv::absdiff(backImage, gray, foreground);

	cv::threshold(foreground, output, threshold, 255, cv::THRESH_BINARY);

	if(clone){
		cv::threshold(foreground, this->maskBackground, threshold, 255, cv::THRESH_BINARY);
	}

	if(!clone){
		this->oldFrame = gray.clone();
		cv::threshold(this->oldFrame, this->maskBackground, threshold, 255, cv::THRESH_BINARY);
	}
}
