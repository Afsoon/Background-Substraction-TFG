/*
 * MediaGaussianaColor.h
 *
 *  Created on: 14 de oct. de 2015
 *      Author: saidatrahouchecharrouti
 */

#ifndef BACKGROUNDPROCESS_MEDIAGAUSSIANACOLOR_H_
#define BACKGROUNDPROCESS_MEDIAGAUSSIANACOLOR_H_
#include <imgproc/imgproc.hpp>
#include "FrameProcessor.h"

class MediaGaussianaColor: public FrameProcessor {
private:
	cv::Mat desviacion_tipica;
	cv::Mat model_pdf;
	cv::Mat mask_foreground;
	cv::Mat oldFrame;
	double k;
	double learning_rate;
	void processMatrix(cv::Mat& input, cv::Mat& output);
	//void actualizarPdfSeleccionPixeles(cv::Mat& mask, cv::Mat& input, bool first);
	void crearMascara(cv::Mat& desv, cv::Mat& diff, cv::Mat& input);
	bool esPrimerPlano(uchar& desv, uchar& diff);
	int calcularIndice(int filas, int i, int j);
	bool input_contiua;
	bool seleccion_pixeles;
public:
	MediaGaussianaColor();
	~MediaGaussianaColor();
	void setLearning_rate(double learning_rate);
	void setKValue(double k_value);
	void process(cv::Mat& input, cv::Mat& output);
	void setSeleccionPixelesMode(bool state);
};

#endif /* BACKGROUNDPROCESS_MEDIAGAUSSIANACOLOR_H_ */
