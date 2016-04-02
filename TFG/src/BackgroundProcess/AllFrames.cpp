/*
 * AllFrames.cpp
 *
 *  Created on: 17/9/2015
 *      Author: Said
 */

#include "AllFrames.h"

AllFrames::AllFrames() {
	// TODO Auto-generated constructor stub

}

AllFrames::~AllFrames() {
	// TODO Auto-generated destructor stub
}

void AllFrames::process(cv::Mat& input, cv::Mat& output){
	if(output.empty()){
		output = cv::Mat::zeros(input.rows, input.cols, CV_8UC1);
		return;
	}
	cv::Mat differnce;
	cv::Mat foregroundMask = cv::Mat::zeros(input.rows, input.cols, CV_8UC1);
	cv::absdiff(input, output, differnce);

	float threshold = 30.0f;
	float dist;

	const int64 start = cv::getTickCount();
	for(int j = 0; j < differnce.rows; j++){
		for(int i = 0; i < differnce.cols; i++){
			cv::Vec3b pix = differnce.at<cv::Vec3b>(j, i);

			dist = (pix[0]*pix[0] + pix[1]*pix[1] + pix[2]*pix[2]);
			dist = sqrt(dist);

			if(dist>threshold)
			{
				foregroundMask.at<unsigned char>(j,i) = 255;
			}
		}
	}

	output = foregroundMask;
	double duration = (cv::getTickCount()-start)/cv::getTickFrequency();

	std::cout << duration << std::endl;
}
