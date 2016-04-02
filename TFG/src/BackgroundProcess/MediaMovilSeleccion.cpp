/*
 * MediaMovilSeleccion.cpp
 *
 *  Created on: 1/10/2015
 *      Author: Said
 */

#include "MediaMovilSeleccion.h"

MediaMovilSeleccion::MediaMovilSeleccion() {
	// TODO Auto-generated constructor stub

}

MediaMovilSeleccion::~MediaMovilSeleccion() {
	// TODO Auto-generated destructor stub
}


/**
 * Recibe el factor de aprendizaje que queremos aplicar durante el proceso
 * @param learning un valor comprendido entre 0-1.
 */
void MediaMovilSeleccion::setLearning_rate(double learning){
	learning_rate = learning;
}

/**
 * Recibe el umbral que queremos que se aplique durante el proceso
 * @param thresold un valor comprendido entre 0 y 255
 */
void MediaMovilSeleccion::setThresold(float thresold){
	threshold = thresold;
}

/**
 * Es la misma técnica mencionada anteriormente pero en este caso se le añade una selección de píxeles
 *
 * Solo se actualizan los píxeles asociadas al primer plano, es decir, si el píxel calculado no pertenece
 * al primer plano, este se descarta de la actualización y no varía su valor.
 *
 * @param input Fotograma, del vídeo, que se va a manipular
 * @param output Máscara del primer plano obtenido tras manipular el fotograma de entrada
 */
void MediaMovilSeleccion::process(cv::Mat& input, cv::Mat& output){
	cv::Mat gray;
	cv::Mat backImage;
	cv::Mat foreground;

	cv::cvtColor(input, gray, CV_BGR2GRAY);
	if(background_model.empty()){
		gray.convertTo(background_model, CV_32F, 1./255.);
	}
	bool clone = false;
	if(old_frame.empty()){
		old_frame = gray.clone();
		clone = true;
	}

	if(!clone){
		cv::accumulateWeighted(old_frame, background_model, learning_rate, mask_background);
	}

	background_model.convertTo(backImage, CV_8U);

	cv::absdiff(backImage, gray, foreground);

	cv::threshold(foreground, output, threshold, 255, cv::THRESH_BINARY);

	if(clone){
		cv::threshold(foreground, mask_background, threshold, 255, cv::THRESH_BINARY);
	}

	if(!clone){
		old_frame = gray.clone();
		cv::threshold(old_frame, mask_background, threshold, 255, cv::THRESH_BINARY);
	}


}
