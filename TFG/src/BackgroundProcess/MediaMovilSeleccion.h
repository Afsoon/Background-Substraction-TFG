/*
 * MediaMovilSeleccion.h
 *
 *  Created on: 1/10/2015
 *      Author: Said
 */

#ifndef BACKGROUNDPROCESS_MEDIAMOVILSELECCION_H_
#define BACKGROUNDPROCESS_MEDIAMOVILSELECCION_H_
#include <imgproc/imgproc.hpp>
#include "FrameProcessor.h"

class MediaMovilSeleccion: public FrameProcessor {
private:
	cv::Mat background_model;
	cv::Mat old_frame;
	cv::Mat mask_background;
	double learning_rate;
	float threshold;
public:
	MediaMovilSeleccion();
	~MediaMovilSeleccion();
	void setThresold(float threshold);
	void setLearning_rate(double learning_rate);
	void process(cv::Mat& input, cv::Mat& output);
};

#endif /* BACKGROUNDPROCESS_MEDIAMOVILSELECCION_H_ */
