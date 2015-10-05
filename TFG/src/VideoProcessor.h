/*
 * VideoProcessor.h
 *
 *  Created on: 16/9/2015
 *      Author: Said
 */

#ifndef VIDEOPROCESSOR_H_
#define VIDEOPROCESSOR_H_
#include <highgui/highgui.hpp>
#include <core/core.hpp>
#include "BackgroundProcess/FrameProcessor.h"


namespace vp {

class VideoProcessor {
private:
	cv::VideoCapture capture;
	void (*process) (cv::Mat&, cv::Mat&);
	FrameProcessor *frameProcesor;
	bool callIt;
	std::string windowNameInput;
	std::string windowNameOutput;
	int delay;
	long fnumber;
	long frameToStop;
	bool stop;

public:
	VideoProcessor();
	void setFrameProcessor(void (*frameProcessingCallback) (cv::Mat&, cv::Mat&));
	void setFrameProcessor(FrameProcessor *frameProcessorPtr);
	bool setInput(std::string filename);
	void displayInput(std::string wn);
	void displayOutput(std::string wn);
	void run(void);
	void stopIt(void);
	bool isStopped(void);
	bool isOpened(void);
	void setDelay(int d);
	bool readNextFrame(cv::Mat& frame);
	void callProcess(void);
	void dontCallProcess(void);
	void stopAtFrameNo(long frame);
	long getFrameNumber(void);
	double getFrameRate(void);
	~VideoProcessor();
};

} /* namespace vp */

#endif /* VIDEOPROCESSOR_H_ */
