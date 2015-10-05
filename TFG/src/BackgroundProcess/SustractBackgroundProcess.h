/*
 * SustractBackgroundProcess.h
 *
 *  Created on: 17/9/2015
 *      Author: Said
 */

#ifndef SUSTRACTBACKGROUNDPROCESS_H_
#define SUSTRACTBACKGROUNDPROCESS_H_
#include <imgproc/imgproc.hpp>
#include <iostream>
#include "FrameProcessor.h"

class SustractBackgroundProcess: public FrameProcessor {
private:
	cv::Mat background;
	void diference(cv::Mat& input, cv::Mat& output);
public:
	SustractBackgroundProcess();
	~SustractBackgroundProcess();
	void process(cv::Mat &input, cv::Mat &output);
};

#endif /* SUSTRACTBACKGROUNDPROCESS_H_ */
