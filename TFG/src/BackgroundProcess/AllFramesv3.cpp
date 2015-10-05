/*
 * AllFramesv3.cpp
 *
 *  Created on: 19/9/2015
 *      Author: Said
 */

#include "AllFramesv3.h"

AllFramesv3::AllFramesv3() {
	// TODO Auto-generated constructor stub

}

AllFramesv3::~AllFramesv3() {
	// TODO Auto-generated destructor stub
}

void AllFramesv3::process(cv::Mat& input, cv::Mat& output){
	if(output.empty()){
		output = cv::Mat::zeros(input.rows, input.cols, CV_8UC1);
		cv::cvtColor(input, input, CV_BGR2GRAY);
		return;
	}

	cv::cvtColor(input, input, CV_BGR2GRAY);
	cv::Mat difference;
	cv::absdiff(input, output, output);

	const int64 start = cv::getTickCount();

	cv::threshold(output, difference, 30.0f, 255, cv::THRESH_BINARY);
	cv::morphologyEx(difference, output, cv::MORPH_OPEN, cv::Mat(2,2, CV_8U, cv::Scalar(1)));

	double duration = (cv::getTickCount()-start)/cv::getTickFrequency();
	std::cout << duration << std::endl;
}
