//============================================================================
// Name        : TFG.cpp
// Author      : Said
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "VideoProcessor.h"
#include "BackgroundProcess/SustractBackgroundProcess.h"
#include "BackgroundProcess/AllFramesv2.h"
#include "BackgroundProcess/MediaMovil.h"
#include "BackgroundProcess/MediaMovilSeleccion.h"
#include "BackgroundProcess/MediaGaussiana.h"
#include "BackgroundProcess/MediaGaussianaColor.h"
#include "BackgroundProcess/KGaussians.h"
#define VIDEO "videos/1.mp4"
#define VIDEO2 "videos/2.mp4"
#define VIDEO3 "videos/3.mp4"
using namespace std;

void mediaGaussiana(MediaGaussiana &media);
void mediaMovil(MediaMovil &mediamovil);
void mediaMovilSeleccion(MediaMovilSeleccion &media);
void mediaGaussianaColor(MediaGaussianaColor &media);
void kGassuians(KGaussians &kGaussian);

int main() {
 	vp::VideoProcessor videoProcessor;
	SustractBackgroundProcess sustract;
	AllFramesv2 all;
	MediaMovil mediamovil;
	MediaMovilSeleccion media;
	MediaGaussiana gauss;
	MediaGaussianaColor gaussColor;
	KGaussians kGaussian;
	mediaGaussiana(gauss);
	mediaMovil(mediamovil);
	mediaMovilSeleccion(media);
	mediaGaussianaColor(gaussColor);
	kGassuians(kGaussian);
	videoProcessor.setInput(VIDEO);
	videoProcessor.displayInput("Input");
	videoProcessor.displayOutput("Output");
	videoProcessor.setDelay(1000./videoProcessor.getFrameRate()); //Nunca puede ser 0 porque se congela la imagen
	videoProcessor.setFrameProcessor(&kGaussian);
	videoProcessor.run();
	return 0;
}

void kGassuians(KGaussians &kGaussian){
	kGaussian.setAlpha(0.05);
	kGaussian.setVariance(36.0);
	kGaussian.setKGaussians(5);
	kGaussian.setTValue(0.93 );
}

void mediaGaussianaColor(MediaGaussianaColor &media){
	media.setKValue(2.5);
	media.setSeleccionPixelesMode(false);
	media.setLearning_rate(0.5);
}

void mediaGaussiana(MediaGaussiana &media){
	media.setKValue(2.5);
	media.setSeleccionPixelesMode(false);
	media.setLearning_rate(0.5);
}

void mediaMovil(MediaMovil &mediamovil){
	mediamovil.setLearning_rate(0.05);
	mediamovil.setThresold(30.0f);
}

void mediaMovilSeleccion(MediaMovilSeleccion &media){
	media.setLearning_rate(0.05);
	media.setThresold(30.0f);
}
