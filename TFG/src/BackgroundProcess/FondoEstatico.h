/*
 * FondoEstatico.h
 *
 *  Created on: 17/9/2015
 *      Author: Said
 */

#ifndef ALLFRAMES_H_
#define ALLFRAMES_H_
#include <imgproc/imgproc.hpp>
#include <iostream>
#include "FrameProcessor.h"


class FondoEstatico: public FrameProcessor {
public:
	FondoEstatico();
	~FondoEstatico();
	void process(cv::Mat &input, cv::Mat &output);
};

#endif /* ALLFRAMES_H_ */
