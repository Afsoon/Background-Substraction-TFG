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
 * La técnica que ha sido aplicada para obtener la máscara del plano ha sido el modelo de fondo móvil.
 *
 * Este proceso se basa en la siguiente fórmula matemática:
 *
 * <<Insertar fórmula en Latex>>
 *
 * Por cada nuevo fotograma que se recibe, se procesa y se actualiza el modelo del fotograma anterior, excepto cuando
 * es el primer fotograma.
 *
 * Cuando estamos en el primer fotograma, la máscara que obtenemos es una totalmente negra, no ha habido diferencia,
 * pero al acabar de procesarla, nos evitamos volver a guardar la imagen.
 *
 * En el resto de fotogramas, hacemos siempre la comparación con el anterior y, como se ha dicho antes, se guardar
 * el fotograma, ya en escala de grises, para que cuando llegue el siguiente fotograma nos evitemos volver a pasarlo
 * a escala de grises y aumentar el coste computacional.
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
