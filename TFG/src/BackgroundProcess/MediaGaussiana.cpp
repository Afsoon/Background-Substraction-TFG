/*
 * MediaGaussiana.cpp
 *
 *  Created on: 2/10/2015
 *      Author: Said
 */

#include "MediaGaussiana.h"

MediaGaussiana::MediaGaussiana() {

}

MediaGaussiana::~MediaGaussiana() {
}

/**
 * Asigamos el valor de K que queremos que se usa durante el proceso
 * @param k_value
 */
void MediaGaussiana::setKValue(double k_value){
	k = k_value;
}

/**
 * Recibe el factor de aprendizaje que queremos aplicar durante el proceso
 * @param learning un valor comprendido entre 0-1.
 */
void MediaGaussiana::setLearning_rate(double learning){
	this->learningRate = learning;
}

/**
 * Indicamos si queremos que el PDF se actualice mendiante selección de pixeles o no.
 * @param state true o false
 */
void MediaGaussiana::setSeleccionPixelesMode(bool state){
	this->seleccionPixeles = state;
}

void MediaGaussiana::setProcessColorVideo(bool processColorVideo){
	this->processColorVideo = processColorVideo;
}

/**
 * Si el usuario no ha modificado ningún parámetro, por defecto se realizará la substracción
 * de fondo con selección de píxeles, en color, con un <<alpha>> de 0.05.
 *
 *
 * @param input Fotograma, del vídeo, que se va a manipular
 * @param output Máscara del primer plano obtenido tras manipular el fotograma de entrada
 */
void MediaGaussiana::process(cv::Mat& input, cv::Mat& output){
	if(!processColorVideo){
		cv::Mat gray;
		cv::cvtColor(input, gray, CV_BGR2GRAY);
		processGrayImages(gray, output);
	}else{
		processColorImages(input, output);
	}

}

/**
 * En el estado inicial, se inicializa las desviaciones típicas y la función de probabilidad de densidad
 * y se devuelve una máscara negra.
 *
 * En los siguietnes estados, se calcula la máscara de primer plano con los valores calculados previamente y
 * se pasa a la actualización.
 *
 * @param input Fotograma, del vídeo, que se va a manipular
 * @param output Máscara del primer plano obtenido tras manipular el fotograma de entrada
 */
void MediaGaussiana::processGrayImages(cv::Mat& input, cv::Mat& output){

	this->maskForeground = cv::Mat::zeros(input.rows, input.cols, CV_8UC1);

	cv::Mat diff;
	cv::Mat desv;
	cv::Mat desv_float;
	cv::Mat diff_pow;
	cv::Mat pow_matrix;


	bool first_model = false;
	if(this->modelPdf.empty()){
		this->modelPdf.create(input.rows, input.cols, input.type());
		if(this->seleccionPixeles){
			first_model = true;
		}
	}

	if(this->desviacionTipica.empty()){
		this->desviacionTipica.create(input.rows, input.cols, input.type());
	}

	cv::absdiff(input, this->modelPdf, diff);
	this->desviacionTipica.convertTo(desv_float, CV_64F);
	cv::pow(desv_float, 0.5,desv);
	desv = k * desv;
	desv.convertTo(desv, diff.type());
	if(first_model){
		cv::compare(diff, desv, this->maskForeground, CV_CMP_LT);
	}else{
		cv::compare(diff, desv, this->maskForeground, CV_CMP_GT);
	}

	cv::pow(diff, 2, pow_matrix);
	cv::pow(this->desviacionTipica, 2, this->desviacionTipica);
	this->desviacionTipica = this->learningRate*pow_matrix +(1-this->learningRate)*this->desviacionTipica;

	if(!this->seleccionPixeles){
		this->modelPdf = this->learningRate*input + (1-this->learningRate)*this->modelPdf;
	}else{
		actualizarPdfSeleccionPixeles(this->maskForeground, input, first_model);
	}

	output = this->maskForeground;

}

/**
 * Esté método se encarga de actualizar PDF según el criterio de selección de píxeles.
 *
 * @param model Mascara obtenida para la actualización de pixeles.
 * @param input Fotograma de entrada, en escala de grisis, de donde se obtendrá los valores para actualizar.
 * @param first para saber si nos encontramos en el primer fotograma o no.
 */

void MediaGaussiana::actualizarPdfSeleccionPixeles(cv::Mat& model, cv::Mat& input, bool first){
	int channels = input.channels();
	int cols  = input.cols * channels;
	int rows = input.rows;

	if(input.isContinuous()){
		cols *= rows;
		rows = 1;
		this->inputContinua = true;
	}

	uchar* pdf;
	uchar* mask;
	uchar* input_;
	for(int i = 0; i < rows; ++i){
		input_ = input.ptr<uchar>(i);
		mask = this->maskForeground.ptr<uchar>(i);
		pdf = this->modelPdf.ptr<uchar>(i);
		for(int j = 0; j < cols; ++j){
			int indice = calcularIndice(rows,i,j);
			if(first){
				int valor = 0;
				if(mask[indice] == 255){
					valor = 1;
				}
				pdf[indice] = valor*pdf[indice]+(1-valor)*(this->learningRate*input_[indice] + (1 - this->learningRate)*pdf[indice]);
			}else{
				if(mask[indice] != 255){
					pdf[indice] = this->learningRate*input_[indice] + (1 - this->learningRate)*pdf[indice];
				}
			}
		}
	}
}

void MediaGaussiana::processColorImages(cv::Mat& input, cv::Mat& output){
	this->maskForeground = cv::Mat::zeros(input.rows, input.cols, CV_8UC1);

	cv::Mat diff;
	cv::Mat desv;
	cv::Mat desv_float;
	cv::Mat diff_pow;
	cv::Mat pow_matrix;


	bool first_model = false;
	if(this->modelPdf.empty()){
		this->modelPdf.create(input.rows, input.cols, input.type());
		if(this->seleccionPixeles){
			first_model = true;
		}
	}

	if(this->desviacionTipica.empty()){
		this->desviacionTipica.create(input.rows, input.cols, input.type());
	}

	cv::absdiff(input, this->modelPdf, diff);
	this->desviacionTipica.convertTo(desv_float, CV_64F);
	cv::pow(desv_float, 0.5,desv);
	desv = k * desv;
	desv.convertTo(desv, diff.type());
	crearMascara(desv, diff, input);


	cv::pow(diff, 2, pow_matrix);
	cv::pow(this->desviacionTipica, 2, this->desviacionTipica);
	this->desviacionTipica = this->learningRate*pow_matrix +(1-this->learningRate)*this->desviacionTipica;

	if(!this->seleccionPixeles){
		this->modelPdf = this->learningRate*input + (1-this->learningRate)*this->modelPdf;
	}else{
		actualizarPdfSeleccionPixeles(this->maskForeground, input, first_model);
	}

	output = this->maskForeground;
}

/**
 * Método para crear la máscara de primer plano para imagenes en color. Cuando hay una
 * coincidencia en cualquier de los canales, se pinta blanco el píxel. Si no hay
 * coincidencia entonces se pinta como negro el píxel.
 *
 * @param desv Matriz con los valores actuales de la desviación típica
 * @param diff Matriz con los valores obtenidos tras hacer la diferencia entre PDF y el frame de entrada
 * @param input Frame que se esta procesando
 */
void MediaGaussiana::crearMascara(cv::Mat& desv, cv::Mat& diff, cv::Mat& input){
	int channels = input.channels();
	int cols  = input.cols * channels;
	int rows = input.rows;

	if(input.isContinuous()){
		cols *= rows;
		rows = 1;
		this->inputContinua = true;
	}

	uchar* input_;
	uchar* model;
	uchar* desviacion;
	uchar* mask;

	for(int i = 0; i < rows; ++i){
		input_ = input.ptr<uchar>(i);
		model = diff.ptr<uchar>(i);
		desviacion = desv.ptr<uchar>(i);
		mask = this->maskForeground.ptr<uchar>(i);
		for(int j = 0; j < cols; ++j){
			int indice = calcularIndice(rows,i,j);
			if(model[indice] > desviacion[indice] || model[indice +1] > desviacion[indice+1] || model[indice+2] > desviacion[indice+2]){
				mask[indice/3] = 255;
			}
			j+=2;

		}
	}
}


int MediaGaussiana::calcularIndice(int filas, int i, int j){
	if(this->inputContinua){
		return j;
	}else{
		return filas*i + j;
	}
}

