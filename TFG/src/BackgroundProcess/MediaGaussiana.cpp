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
	learning_rate = learning;
}

/**
 * Indicamos si queremos que el PDF se actualice mendiante selección de pixeles o no.
 * @param state true o false
 */
void MediaGaussiana::setSeleccionPixelesMode(bool state){
	seleccion_pixeles = state;
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
	cv::Mat gray;
	cv::cvtColor(input, gray, CV_BGR2GRAY);
	processMatrix(gray, output);
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
void MediaGaussiana::processMatrix(cv::Mat& input, cv::Mat& output){

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
	if(first_model){
		cv::compare(diff, desv, mask_foreground, CV_CMP_LT);
	}else{
		cv::compare(diff, desv, mask_foreground, CV_CMP_GT);
	}

	cv::pow(diff, 2, pow_matrix);
	cv::pow(desviacion_tipica, 2, desviacion_tipica);
	desviacion_tipica = learning_rate*pow_matrix +(1-learning_rate)*desviacion_tipica;

	if(!seleccion_pixeles){
		model_pdf = learning_rate*input + (1-learning_rate)*model_pdf;
	}else{
		actualizarPdfSeleccionPixeles(mask_foreground, input, first_model);
	}

	output = mask_foreground;

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
		input_contiua = true;
	}

	uchar* pdf;
	uchar* mask;
	uchar* input_;
	for(int i = 0; i < rows; ++i){
		input_ = input.ptr<uchar>(i);
		mask = mask_foreground.ptr<uchar>(i);
		pdf = model_pdf.ptr<uchar>(i);
		for(int j = 0; j < cols; ++j){
			int indice = calcularIndice(rows,i,j);
			if(first){
				int valor = 0;
				if(mask[indice] == 255){
					valor = 1;
				}
				pdf[indice] = valor*pdf[indice]+(1-valor)*(learning_rate*input_[indice] + (1 - learning_rate)*pdf[indice]);
			}else{
				if(mask[indice] != 255){
					pdf[indice] = learning_rate*input_[indice] + (1 - learning_rate)*pdf[indice];
				}
			}
		}
	}
}



int MediaGaussiana::calcularIndice(int filas, int i, int j){
	if(input_contiua){
		return j;
	}else{
		return filas*i + j;
	}
}

