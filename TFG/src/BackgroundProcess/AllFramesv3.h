/*
 * AllFramesv3.h
 *
 *  Created on: 19/9/2015
 *      Author: Said
 */

#ifndef BACKGROUNDPROCESS_ALLFRAMESV3_H_
#define BACKGROUNDPROCESS_ALLFRAMESV3_H_
#include <imgproc/imgproc.hpp>
#include "FrameProcessor.h"
#include <iostream>

class AllFramesv3: public FrameProcessor {
public:
	AllFramesv3();
	~AllFramesv3();
	void process(cv::Mat& input, cv::Mat& output);
};

#endif /* BACKGROUNDPROCESS_ALLFRAMESV3_H_ */
