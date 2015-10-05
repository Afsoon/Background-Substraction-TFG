/*
 * AllFramesv2.cpp
 *
 *  Created on: 19/9/2015
 *      Author: Said
 */

#include "AllFramesv2.h"

AllFramesv2::AllFramesv2() {
	// TODO Auto-generated constructor stub

}

AllFramesv2::~AllFramesv2() {
	// TODO Auto-generated destructor stub
}

void AllFramesv2::process(cv::Mat& input, cv::Mat& output){
	if(output.empty()){
		output = cv::Mat::zeros(input.rows, input.cols, CV_8UC1);
		cv::cvtColor(input, input, CV_BGR2GRAY);
		return;
	}

	cv::Mat differnce;
	cv::cvtColor(input, input, CV_BGR2GRAY);
	cv::absdiff(input, output, differnce);

	float threshold = 30.0f;

	const int64 start = cv::getTickCount();

	cv::threshold(differnce, output, threshold, 255, cv::THRESH_BINARY);

	double duration = (cv::getTickCount()-start)/cv::getTickFrequency();
	std::cout << duration << std::endl;
}
