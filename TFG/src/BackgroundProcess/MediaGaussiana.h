/*
 * MediaGaussiana.h
 *
 *  Created on: 2/10/2015
 *      Author: Said
 */

#ifndef BACKGROUNDPROCESS_MEDIAGAUSSIANA_H_
#define BACKGROUNDPROCESS_MEDIAGAUSSIANA_H_
#include <imgproc/imgproc.hpp>
#include "FrameProcessor.h"

class MediaGaussiana: public FrameProcessor {
private:
	cv::Mat desviacion_tipica;
	cv::Mat model_pdf;
	cv::Mat mask_foreground;
	cv::Mat oldFrame;
	double k;
	double learning_rate;
	void processMatrix(cv::Mat& input, cv::Mat& output);
	void actualizarPdfSeleccionPixeles(cv::Mat& mask, cv::Mat& input, bool first);
	int calcularIndice(int filas, int i, int j);
	bool input_contiua;
	bool seleccion_pixeles;
public:
	MediaGaussiana();
	~MediaGaussiana();
	void setLearning_rate(double learning_rate);
	void setKValue(double k_value);
	void process(cv::Mat& input, cv::Mat& output);
	void setSeleccionPixelesMode(bool state);
};

#endif /* BACKGROUNDPROCESS_MEDIAGAUSSIANA_H_ */
