/*
 * FondoEstatico.cpp
 *
 *  Created on: 17/9/2015
 *      Author: Said
 */

#include "FondoEstatico.h"

FondoEstatico::FondoEstatico() {
	// TODO Auto-generated constructor stub

}

FondoEstatico::~FondoEstatico() {
	// TODO Auto-generated destructor stub
}

void FondoEstatico::process(cv::Mat& input, cv::Mat& output){
	if(output.empty()){
		output = cv::Mat::zeros(input.rows, input.cols, CV_8UC1);
		return;
	}
	cv::Mat differnce;
	cv::Mat foregroundMask = cv::Mat::zeros(input.rows, input.cols, CV_8UC1);
	cv::absdiff(input, output, differnce);

	float threshold = 30.0f;


}
