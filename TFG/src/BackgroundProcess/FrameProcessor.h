/*
 * FrameProcessor.h
 *
 *  Created on: 16/9/2015
 *      Author: Said
 */

#ifndef FRAMEPROCESSOR_H_
#define FRAMEPROCESSOR_H_
#include <highgui/highgui.hpp>
#include <core/core.hpp>

class FrameProcessor {
public:
	FrameProcessor();
	~FrameProcessor();
	virtual void process(cv::Mat &input, cv::Mat &output) = 0;

};

#endif /* FRAMEPROCESSOR_H_ */
