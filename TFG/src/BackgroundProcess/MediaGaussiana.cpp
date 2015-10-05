/*
 * MediaGaussiana.cpp
 *
 *  Created on: 2/10/2015
 *      Author: Said
 */

#include "MediaGaussiana.h"

MediaGaussiana::MediaGaussiana() {
	// TODO Auto-generated constructor stub

}

MediaGaussiana::~MediaGaussiana() {
	// TODO Auto-generated destructor stub
}

void MediaGaussiana::setKValue(double k_value){
	k = k_value;
}

void MediaGaussiana::setLearning_rate(double learning){
	learning_rate = learning;
}

void MediaGaussiana::setThresold(float thresold){
	threshold = thresold;
}

void MediaGaussiana::setSeleccionPixelesMode(bool state){
	seleccion_pixeles = state;
}

void MediaGaussiana::process(cv::Mat& input, cv::Mat& output){
	cv::Mat gray;
	cv::cvtColor(input, gray, CV_BGR2GRAY);
	processMatrix(gray, output);
}

void MediaGaussiana::setDif(cv::Mat &diff, cv::Mat &input, cv::Mat &output, cv::Mat &desviacion){
	cv::absdiff(input, output, diff);
	desviacion = k * desviacion_tipica;


}

//Este proceso es para seleccion de pixeles o no;
void MediaGaussiana::processMatrix(cv::Mat& input, cv::Mat& output){
	bool clone = false;
	if(oldFrame.empty()){
		oldFrame = input.clone();
		clone = true;
	}

	if(!seleccion_pixeles){
		int channels = input.channels();
		int cols  = input.cols * channels;
		int rows = input.rows;

		if(input.isContinuous()){
			cols *= rows;
			rows = 1;
			input_contiua = true;
		}

		if(!desviacion_tipica.empty() && desviacion_tipica.isContinuous()){
			desviacion_continua = true;
		}


		mask_foreground = cv::Mat::zeros(input.rows, input.cols, CV_8UC1);

		if(!mask_foreground.empty() && mask_foreground.isContinuous()){
			mask_continua = true;
		}

		if(!model_pdf.empty() && model_pdf.isContinuous()){
			model_continua = true;
		}

		uchar* input_;
		uchar* model;
		uchar* desviacion;
		uchar* mask;

		cv::Mat pow_matrix;
		cv::pow((oldFrame-model_pdf), 2, pow_matrix);
		cv::pow(desviacion_tipica, 2, desviacion_tipica);
		desviacion_tipica = learning_rate*pow_matrix +(1-learning_rate)*desviacion_tipica;
		model_pdf = learning_rate*input + (1-learning_rate)*model_pdf;

		cv::Mat diff;
		cv::Mat desv;

		setDif(diff, input, model_pdf, desv);

		for(int i = 0; i < rows; ++i){
			input_ = input.ptr<uchar>(i);
			model = diff.ptr<uchar>(i);
			desviacion = desv.ptr<uchar>(i);
			mask = mask_foreground.ptr<uchar>(i);
			for(int j = 0; j < cols; ++j){
				int indice = calcularIndice(rows,i,j);
				if(model[indice] > desviacion[indice]){
					mask[indice] = 255;
				}
			}
		}

		if(!clone){
			oldFrame = input.clone();
		}
		output = mask_foreground;
	}

}



int MediaGaussiana::calcularIndice(int filas, int i, int j){
	if(input_contiua){
		return j;
	}else{
		return filas*i + j;
	}
}
