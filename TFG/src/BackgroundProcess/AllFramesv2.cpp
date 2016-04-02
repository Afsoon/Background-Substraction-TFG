/*
 * AllFramesv2.cpp
 *
 *  Created on: 19/9/2015
 *      Author: Said
 */

#include "AllFramesv2.h"

AllFramesv2::AllFramesv2() {
	// TODO Auto-generated constructor stub

}

AllFramesv2::~AllFramesv2() {
	// TODO Auto-generated destructor stub
}

/**
 * La técnica que ha sido aplicada para obtener la máscara del plano ha sido el modelo del fondo estático
 *
 * Este proceso se basa en la siguiente fórmula matemática:
 * <<Insertar después en Latex>>
 *
 * En el estado inicial, el algoritmo no tiene guardado el primer fotograma del vídeo. Al carecer del
 * primer fotograma, este se guarda y se devuelve una máscara negra, si se calculase se obtendría eso.
 *
 * En los estados consecutivos, se va comparando con el frame guardado y obtenemos la máscara asociada.
 *
 * Esta técnica tiene un problema que es el `ghosting`. El problema aparece cuando el primer fotograma hay
 * un objeto en movimiento, las máscarás consecutivas obtenidas se formará la sombra del objeto en movimiento.
 *
 * @param input Fotograma, del vídeo, que se va a manipular
 * @param output Máscara del primer plano obtenido tras manipular el fotograma de entrada
 */
void AllFramesv2::process(cv::Mat& input, cv::Mat& output){
	if(output.empty()){
		output = cv::Mat::zeros(input.rows, input.cols, CV_8UC1);
		cv::cvtColor(input, input, CV_BGR2GRAY);
		return;
	}

	cv::Mat differnce;
	cv::cvtColor(input, input, CV_BGR2GRAY);
	cv::absdiff(input, output, differnce);

	float threshold = 30.0f;

	const int64 start = cv::getTickCount();

	cv::threshold(differnce, output, threshold, 255, cv::THRESH_BINARY_INV);

	double duration = (cv::getTickCount()-start)/cv::getTickFrequency();
	std::cout << duration << std::endl;
}
