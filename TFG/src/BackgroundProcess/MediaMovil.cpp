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
	threshold = thresold;
}
/**
 * Recibe el factor de aprendizaje que queremos aplicar durante el proceso
 * @param learning un valor comprendido entre 0-1.
 */
void MediaMovil::setLearning_rate(double learning){
	learning_rate = learning;
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
	cv::Mat gray;
	cv::Mat backImage;
	cv::Mat foreground;

	cv::cvtColor(input, gray, CV_BGR2GRAY);
	if(background_model.empty()){
		gray.convertTo(background_model, CV_32F,  1./255.);
	}

	bool clone = false;
	if(old_frame.empty()){
		old_frame = gray.clone();
		clone = true;
	}

	background_model.convertTo(backImage, CV_8U);

	cv::absdiff(backImage, gray, foreground);

	cv::threshold(foreground, output, threshold, 255, cv::THRESH_BINARY);

	cv::accumulateWeighted(old_frame, background_model, learning_rate);
	if(clone){
		clone = false;
	}else{
		old_frame = gray.clone();
	}

}
