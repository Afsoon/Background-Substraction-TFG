//============================================================================
// Name        : TFG.cpp
// Author      : Said
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "VideoProcessor.h"
#include "BackgroundProcess/FondoEstatico.h"
#include "BackgroundProcess/FramesContinuos.h"
#include "BackgroundProcess/MediaMovil.h"
#include "BackgroundProcess/MediaGaussiana.h"
#include "BackgroundProcess/KGaussians.h"
#define VIDEO "videos/1.mp4"
#define VIDEO2 "videos/2.mp4"
#define VIDEO3 "videos/3.mp4"
using namespace std;

void mediaGaussiana(MediaGaussiana &media);
void mediaMovil(MediaMovil &mediamovil);
void kGassuians(KGaussians &kGaussian);

int main() {
 	vp::VideoProcessor videoProcessor;
	FramesContinuos framesContinuos;
	FondoEstatico fondoEstatico;
	MediaMovil mediamovil;
	MediaGaussiana gauss;
	KGaussians kGaussian;
	mediaGaussiana(gauss);
	mediaMovil(mediamovil);
	kGassuians(kGaussian);
	videoProcessor.setInput(VIDEO2);
	videoProcessor.displayInput("Input");
	videoProcessor.displayOutput("Output");
	videoProcessor.setDelay(1000./videoProcessor.getFrameRate()); //Nunca puede ser 0 porque se congela la imagen
	videoProcessor.setFrameProcessor(&gauss);
	videoProcessor.run();
	return 0;
}

void kGassuians(KGaussians &kGaussian){
	kGaussian.setAlpha(0.05);
	kGaussian.setVariance(36.0);
	kGaussian.setKGaussians(5);
	kGaussian.setTValue(0.9);
}


void mediaGaussiana(MediaGaussiana &media){
	media.setKValue(2.5);
	media.setSeleccionPixelesMode(true);
	media.setLearning_rate(0.05);
	media.setProcessColorVideo(true);
}

void mediaMovil(MediaMovil &mediamovil){
	mediamovil.setLearning_rate(0.05);
	mediamovil.setThresold(30.0f);
	mediamovil.setPixelSelection(false);
}

