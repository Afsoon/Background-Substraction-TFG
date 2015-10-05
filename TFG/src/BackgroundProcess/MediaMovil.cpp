/*
 * MediaMovil.cpp
 *
 *  Created on: 1/10/2015
 *      Author: Said
 */

#include "MediaMovil.h"

MediaMovil::MediaMovil() {
	// TODO Auto-generated constructor stub

}

MediaMovil::~MediaMovil() {
	// TODO Auto-generated destructor stub
}

void MediaMovil::setThresold(float thresold){
	threshold = thresold;
}

void MediaMovil::setLearning_rate(double learning){
	learning_rate = learning;
}

void MediaMovil::process(cv::Mat& input, cv::Mat& output){
	cv::Mat gray;
	cv::Mat backImage;
	cv::Mat foreground;

	cv::cvtColor(input, gray, CV_BGR2GRAY);
	if(background_model.empty()){
		gray.convertTo(background_model, CV_32F,  1./255.);
	}

	bool clone = false;
	if(old_frame.empty()){
		old_frame = gray.clone();
		clone = true;
	}

	background_model.convertTo(backImage, CV_8U);

	cv::absdiff(backImage, gray, foreground);

	cv::threshold(foreground, output, threshold, 255, cv::THRESH_BINARY);

	cv::accumulateWeighted(old_frame, background_model, learning_rate);
	if(clone){
		clone = false;
	}else{
		old_frame = gray.clone();
	}

}
