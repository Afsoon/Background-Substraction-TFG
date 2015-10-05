/*
 * SustractBackgroundProcess.cpp
 *
 *  Created on: 17/9/2015
 *      Author: Said
 */

#include "SustractBackgroundProcess.h"

SustractBackgroundProcess::SustractBackgroundProcess() {
	// TODO Auto-generated constructor stub

}

SustractBackgroundProcess::~SustractBackgroundProcess() {
	// TODO Auto-generated destructor stub
}

void SustractBackgroundProcess::process(cv::Mat& input, cv::Mat& output){
	cv::Mat difference;
	cv::Mat backImage;
	cv::Mat foreground;
	cv::cvtColor(input, difference, CV_BGR2GRAY);

			// initialize background to 1st frame
	if (background.empty())
		difference.convertTo(background, CV_32F);

	// convert background to 8U
	background.convertTo(backImage,CV_8U);

	// compute difference between current image and background
	cv::absdiff(backImage,difference,foreground);

	// apply threshold to foreground image
	cv::threshold(foreground,output,30.0f,255,cv::THRESH_BINARY);

	/**
	if(background.empty()){
		background = input.clone();
		cv::cvtColor(background, background, CV_BGR2GRAY);
		output = cv::Mat::zeros(input.rows, input.cols, CV_8UC1);
	}else{
		cv::Mat difference;
		cv::cvtColor(input, difference, CV_BGR2GRAY);
		//cv::absdiff(difference, background, output);
		diference(difference, output);
	}**/
}

void SustractBackgroundProcess::diference(cv::Mat& input, cv::Mat& output){
	output = cv::Mat::zeros(input.rows, input.cols, CV_8UC1);
	float threshold = 30.0f;
	float dist;
	const int64 start = cv::getTickCount();
	for(int j = 0; j < input.rows; j++){
		for(int i = 0; i < input.cols; i++){
			cv::Vec3b pix = input.at<cv::Vec3b>(j, i);
			cv::Vec3b back = background.at<cv::Vec3b>(j,i);

			dist = (pix[0]*back[0] + pix[1]*back[1] + pix[2]*back[2]);
			dist = sqrt(dist);

			if(dist>threshold)
			{
			     output.at<unsigned char>(j,i) = 255;
			}
		}
	}

	double duration = (cv::getTickCount()-start)/cv::getTickFrequency();

	std::cout << duration << std::endl;
}

/**
void SustractBackgroundProcess::diference(cv::Mat& input, cv::Mat& output){
	output = cv::Mat::zeros(input.rows, input.cols, CV_8UC1);
	float threshold = 30.0f;
	float dist;

	const int64 start = cv::getTickCount();
	for(int j = 0; j < input.rows; j++){
		for(int i = 0; i < input.cols; i++){
			cv::Vec3b pix = input.at<cv::Vec3b>(j, i);
			cv::Vec3b back = background.at<cv::Vec3b>(i,j);

			dist = (pix[0]*back[0] + pix[1]*back[1] + pix[2]*back[2]);
			dist = sqrt(dist);

			if(dist>threshold)
			{
			     output.at<unsigned char>(j,i) = 255;
			}
		}
	}

	double duration = (cv::getTickCount()-start)/cv::getTickFrequency();

	std::cout << duration << std::endl;
}**/
