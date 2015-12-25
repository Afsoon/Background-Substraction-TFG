/*
 * MediaGaussianaColor.cpp
 *
 *  Created on: 14 de oct. de 2015
 *      Author: saidatrahouchecharrouti
 */

#include "MediaGaussianaColor.h"

MediaGaussianaColor::MediaGaussianaColor() {
	// TODO Auto-generated constructor stub

}

MediaGaussianaColor::~MediaGaussianaColor() {
	// TODO Auto-generated destructor stub
}

void MediaGaussianaColor::process(cv::Mat& input, cv::Mat& output){
	processMatrix(input, output);
}

void MediaGaussianaColor::processMatrix(cv::Mat& input, cv::Mat& output){

		mask_foreground = cv::Mat::zeros(input.rows, input.cols, CV_8UC1);

		cv::Mat diff;
		cv::Mat desv;
		cv::Mat desv_float;
		cv::Mat diff_pow;
		cv::Mat pow_matrix;


		bool first_model = false;
		if(model_pdf.empty()){
			model_pdf.create(input.rows, input.cols, input.type());
			if(seleccion_pixeles){
				first_model = true;
			}
		}

		if(desviacion_tipica.empty()){
			desviacion_tipica.create(input.rows, input.cols, input.type());
		}

		cv::absdiff(input, model_pdf, diff);
		desviacion_tipica.convertTo(desv_float, CV_64F);
		cv::pow(desv_float, 0.5,desv);
		desv = k * desv;
		desv.convertTo(desv, diff.type());
		crearMascara(desv, diff, input);


		cv::pow(diff, 2, pow_matrix);
		cv::pow(desviacion_tipica, 2, desviacion_tipica);
		desviacion_tipica = learning_rate*pow_matrix +(1-learning_rate)*desviacion_tipica;

		if(!seleccion_pixeles){
			model_pdf = learning_rate*input + (1-learning_rate)*model_pdf;
		}

		output = mask_foreground;
}

void MediaGaussianaColor::crearMascara(cv::Mat& desv, cv::Mat& diff, cv::Mat& input){
	int channels = input.channels();
	int cols  = input.cols * channels;
	int rows = input.rows;

	if(input.isContinuous()){
		cols *= rows;
		rows = 1;
		input_contiua = true;
	}

		uchar* input_;
		uchar* model;
		uchar* desviacion;
		uchar* mask;

		for(int i = 0; i < rows; ++i){
			input_ = input.ptr<uchar>(i);
			model = diff.ptr<uchar>(i);
			desviacion = desv.ptr<uchar>(i);
			mask = mask_foreground.ptr<uchar>(i);
			for(int j = 0; j < cols; ++j){
				int indice = calcularIndice(rows,i,j);
				if(model[indice] > desviacion[indice] || model[indice +1] > desviacion[indice+1] || model[indice+2] > desviacion[indice+2]){
					mask[indice/3] = 255;
				}
				j+=2;

			}
		}
}

void MediaGaussianaColor::setKValue(double k_value){
	k = k_value;
}

void MediaGaussianaColor::setLearning_rate(double learning){
	learning_rate = learning;
}


void MediaGaussianaColor::setSeleccionPixelesMode(bool state){
	seleccion_pixeles = state;
}

int MediaGaussianaColor::calcularIndice(int filas, int i, int j){
	if(input_contiua){
		return j;
	}else{
		return filas*i + j;
	}
}
