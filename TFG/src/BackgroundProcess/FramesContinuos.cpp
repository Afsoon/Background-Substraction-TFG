/*
 * FramesContinuos.cpp
 *
 *  Created on: 19/9/2015
 *      Author: Said
 */

#include "FramesContinuos.h"

FramesContinuos::FramesContinuos() {
}

FramesContinuos::~FramesContinuos() {
}

/**
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
void FramesContinuos::process(cv::Mat& input, cv::Mat& output){
	if(output.empty()){
		output = cv::Mat::zeros(input.rows, input.cols, CV_8UC1);
		cv::cvtColor(input, input, CV_BGR2GRAY);
		return;
	}

	cv::Mat differnce;
	cv::cvtColor(input, input, CV_BGR2GRAY);
	cv::absdiff(input, output, differnce);

	float threshold = 30.0f;

	cv::threshold(differnce, output, threshold, 255, cv::THRESH_BINARY_INV);

}
