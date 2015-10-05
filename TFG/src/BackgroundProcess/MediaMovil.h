/*
 * MediaMovil.h
 *
 *  Created on: 1/10/2015
 *      Author: Said
 */

#ifndef BACKGROUNDPROCESS_MEDIAMOVIL_H_
#define BACKGROUNDPROCESS_MEDIAMOVIL_H_
#include <imgproc/imgproc.hpp>
#include "FrameProcessor.h"

class MediaMovil: public FrameProcessor {
private:
	cv::Mat background_model;
	cv::Mat old_frame;
	double threshold;
	float learning_rate;
public:
	MediaMovil();
	~MediaMovil();
	void setThresold(float threshold);
	void setLearning_rate(double learning_rate);
	void process(cv::Mat& input, cv::Mat& output);
};

#endif /* BACKGROUNDPROCESS_MEDIAMOVIL_H_ */
