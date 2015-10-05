/*
 * AllFramesv2.h
 *
 *  Created on: 19/9/2015
 *      Author: Said
 */

#ifndef BACKGROUNDPROCESS_ALLFRAMESV2_H_
#define BACKGROUNDPROCESS_ALLFRAMESV2_H_
#include <imgproc/imgproc.hpp>
#include <iostream>
#include "FrameProcessor.h"

class AllFramesv2: public FrameProcessor {
public:
	AllFramesv2();
	~AllFramesv2();
	void process(cv::Mat &input, cv::Mat &output);
};

#endif /* BACKGROUNDPROCESS_ALLFRAMESV2_H_ */
