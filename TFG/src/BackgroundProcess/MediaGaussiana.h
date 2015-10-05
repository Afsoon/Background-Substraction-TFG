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
	cv::Mat media;
	cv::Mat model_pdf;
	cv::Mat mask_foreground;
	cv::Mat oldFrame;
	double k;
	double learning_rate;
	float threshold;
	void processMatrix(cv::Mat& input, cv::Mat& output);
	int calcularIndice(int filas, int i, int j);
	void setDif(cv::Mat &diff, cv::Mat &input, cv::Mat &output, cv::Mat &desviacion);
	bool desviacion_continua;
	bool model_continua;
	bool mask_continua;
	bool input_contiua;
	bool seleccion_pixeles;
public:
	MediaGaussiana();
	~MediaGaussiana();
	void setThresold(float threshold);
	void setLearning_rate(double learning_rate);
	void setKValue(double k_value);
	void process(cv::Mat& input, cv::Mat& output);
	void setSeleccionPixelesMode(bool state);
};

#endif /* BACKGROUNDPROCESS_MEDIAGAUSSIANA_H_ */
