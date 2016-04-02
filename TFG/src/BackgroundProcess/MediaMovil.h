/*
 * MediaMovil.h
 *
 *  Created on: 1/10/2015
 *      Author: Said
 */

#ifndef BACKGROUNDPROCESS_MEDIAMOVIL_H_
#define BACKGROUNDPROCESS_MEDIAMOVIL_H_
#include <imgproc/imgproc.hpp>
#include "FrameProcessor.h"

/**
 * La técnica que ha sido aplicada para obtener la máscara del plano ha sido el modelo de fondo móvil.
 *
 * Este proceso se basa en la siguiente fórmula matemática:
 *
 * <<Insertar fórmula en Latex>>
 *
 * Ya es el primero de los modelos que tiene memorización y aprendizaje. El modelo de fondo se inicializa con
 * los valores del primer fotograma.
 *
 * La actualización del modelo de fondo se realiza mediante una selección de píxeles. Ya que se quiere evitar
 * que el modelo de fondo se contamine con valores que no pertenecen al fondo. La actualización se realiza de la
 * siguiente manera:
 *
 * <Insertar fórmula en Latex>>
 *  */
class MediaMovil: public FrameProcessor {
private:
	cv::Mat backgroundModel;
	cv::Mat oldFrame;
	double threshold;
	float learningRate;
public:
	MediaMovil();
	~MediaMovil();
	void setThresold(float threshold);
	void setLearning_rate(double learning_rate);
	void process(cv::Mat& input, cv::Mat& output);
};

#endif /* BACKGROUNDPROCESS_MEDIAMOVIL_H_ */
