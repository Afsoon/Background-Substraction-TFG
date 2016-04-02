/*
 * MediaGaussiana.h
 *
 *  Created on: 2/10/2015
 *      Author: Said
 */

#ifndef BACKGROUNDPROCESS_MEDIAGAUSSIANA_H_
#define BACKGROUNDPROCESS_MEDIAGAUSSIANA_H_
#include <imgproc/imgproc.hpp>
#include "FrameProcessor.h"

/**
 * El modelo de susbtración implementado en esta clase es el modelo de fondo con media móvil gaussiana.
 *
 * Esta técnica sigue las siguiente formúla matemática:
 *
 * <<Incluir la formulas en Latex>>
 *
 * Llegamos al primer modelo de substracción de fondo que nos podemos permitir tratar las imagenes en color,
 * cosa que no podíamos hacer en los anteriores. En este modelo de substracción de fondo tenemos suficiente
 * información para poder tratar la imagen en RGB y no en escala de grises.
 *
 * Se calcula una función de densidad de probabilidad (pdf), una media y una desviación típica.
 *
 * La desviación típica, de cada píxel, se calcula de la siguiente forma:
 *
 * <<Incluir formula en Latex>>
 *
 * La media móvil, de cada píxel, se calcula de la siguiente forma:
 *
 * <<Incluir fórmula en Latex>>
 *
 * La actualización de la función de densidad de probabilidad se hace de la siguiente manera:
 *
 * <<Insertar fórmula en Latex>>
 *
 * Si se hace una selección de píxeles para la actualización de la función de densidad de probabilidad,
 * entonces se aplica también la siguiente fórmula.
 *
 * <<Insertar fórmula en Latex>>
 */
class MediaGaussiana: public FrameProcessor {
private:
	cv::Mat desviacionTipica;
	cv::Mat modelPdf;
	cv::Mat maskForeground;
	cv::Mat oldFrame;
	double k;
	double learningRate;
	void processGrayImages(cv::Mat& input, cv::Mat& output);
	void processColorImages(cv::Mat& input, cv::Mat& output);
	void actualizarPdfSeleccionPixeles(cv::Mat& mask, cv::Mat& input, bool first);
	int calcularIndice(int filas, int i, int j);
	void crearMascara(cv::Mat& desv, cv::Mat& diff, cv::Mat& input);
	bool inputContinua;
	bool seleccionPixeles;
	bool processColorVideo = true;
public:
	MediaGaussiana();
	~MediaGaussiana();
	void setLearning_rate(double learning_rate);
	void setKValue(double k_value);
	void process(cv::Mat& input, cv::Mat& output);
	void setSeleccionPixelesMode(bool state);
	void setProcessColorVideo(bool processColorVideo);

};

#endif /* BACKGROUNDPROCESS_MEDIAGAUSSIANA_H_ */
