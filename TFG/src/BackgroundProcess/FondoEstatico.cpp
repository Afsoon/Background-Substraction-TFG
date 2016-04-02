/*
 * FondoEstatico.cpp
 *
 *  Created on: 17/9/2015
 *      Author: Said
 */

#include "FondoEstatico.h"

FondoEstatico::FondoEstatico() {
	// TODO Auto-generated constructor stub

}

FondoEstatico::~FondoEstatico() {
	// TODO Auto-generated destructor stub
}

/**
 * En el estado inicial, el algoritmo no tiene guardado el primer fotograma del vídeo. Al carecer del
 * primer fotograma, este se guarda y se devuelve una máscara negra, si se calculase se obtendría eso.
 *
 * En los estados consecutivos, se va comparando con el frame guardado y obtenemos la máscara de
 * primer plano asociada.
 *
 * @param input Fotograma, del vídeo, que se va a manipular
 * @param output Máscara del primer plano obtenido tras manipular el fotograma de entrada
 */
void FondoEstatico::process(cv::Mat& input, cv::Mat& output){
	if(output.empty()){
		output = cv::Mat::zeros(input.rows, input.cols, CV_8UC1);
		cv::cvtColor(input, this->previousFrame, CV_BGR2GRAY);
		return;
	}

	cv::Mat differnce;
	cv::cvtColor(input, input, CV_BGR2GRAY);
	cv::absdiff(input, this->previousFrame, differnce);

	float threshold = 30.0f;

	cv::threshold(differnce, output, threshold, 255, cv::THRESH_BINARY);


}
