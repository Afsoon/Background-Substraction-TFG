/*
 * VideoProcessor.cpp
 *
 *  Created on: 16/9/2015
 *      Author: Said
 */

#include "VideoProcessor.h"

namespace vp {

VideoProcessor::VideoProcessor() {
	// TODO Auto-generated constructor stub

}

VideoProcessor::~VideoProcessor() {
	// TODO Auto-generated destructor stub
}

void VideoProcessor::setFrameProcessor(void (*frameProcessingCallback) (cv::Mat&, cv::Mat&)){
	process = frameProcessingCallback;
	callProcess();
}

void VideoProcessor::setFrameProcessor(FrameProcessor *frameProcessorPtr){
	process = 0;
	frameProcesor = frameProcessorPtr;
	callProcess();
}

void VideoProcessor::callProcess(){
	callIt = true;
}

void VideoProcessor::dontCallProcess(){
	callIt = false;
}

bool VideoProcessor::setInput(std::string filename){
	fnumber = 0;
	capture.release();
	return capture.open(filename);
}

void VideoProcessor::displayInput(std::string wn){
	windowNameInput = wn;
	cv::namedWindow(windowNameInput);
}

void VideoProcessor::displayOutput(std::string wn){
	windowNameOutput = wn;
	cv::namedWindow(windowNameOutput);
}

bool VideoProcessor::readNextFrame(cv::Mat& frame){
	return capture.read(frame);
}

void VideoProcessor::stopIt(){
	stop = true;
}

bool VideoProcessor::isStopped(){
	return stop;
}

bool VideoProcessor::isOpened(){
	return capture.isOpened();
}

void VideoProcessor::setDelay(int d){
	delay = d;
}

void VideoProcessor::stopAtFrameNo(long frame){
	frameToStop = frame;
}

long VideoProcessor::getFrameNumber(){
	long fnumber = static_cast<long>(capture.get(CV_CAP_PROP_POS_FRAMES));
	return fnumber;
}

double VideoProcessor::getFrameRate(){


	double r = capture.get(CV_CAP_PROP_FPS);

	if(r <= 0){
		return 30;
	}

	return r;
}

void VideoProcessor::run(){

	cv::Mat frame;
	cv::Mat output;
	//cv::Mat old;

	if(!isOpened()){
		return ;
	}

	stop = false;

	while(!isStopped()){

		/**
		if(!frame.empty()){
			old = frame.clone();
		}**/
		if(!readNextFrame(frame)){
			break;
		}

		if(windowNameInput.length() != 0){
			cv::imshow(windowNameInput, frame);
		}

		if(callIt){
			if(process){
				process(frame, output);
			}else if (frameProcesor){
				frameProcesor->process(frame, output);
			}
			fnumber++;
		}else{
			output = frame;
		}

		if(windowNameOutput.length() != 0){
			cv::imshow(windowNameOutput, output);
		}

		if(delay >= 0 && cv::waitKey(delay) >= 0){
			stopIt();
		}
		if(frameToStop >= 0 && getFrameNumber() == frameToStop){
			stopIt();
		}
	}
}

} /* namespace vp */
