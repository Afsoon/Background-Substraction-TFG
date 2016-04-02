/*
 * FondoEstatico.h
 *
 *  Created on: 17/9/2015
 *      Author: Said
 */

#ifndef ALLFRAMES_H_
#define ALLFRAMES_H_
#include <imgproc/imgproc.hpp>
#include <iostream>
#include "FrameProcessor.h"

/**
 * El modelo de susbtración implementado en esta clase es el modelo de Fondo estático.
 *
 * Este proceso se basa en la siguiente fórmula matemática:
 * <<Insertar después en Latex>>
 *
 * El modelo se basa en la comparación continua con el primer frame del vídeo. Este modelo
 * computacionalmente es bajo pero sufre de carencias en entornos donde el inicio del vídeo contiene
 * objetos que están en movimiento.
 *
 * El problema previamente mencionado se le llama `ghosting`, que consiste que el objeto
 * en movimiento, capturado en el primer frame, siempre se mostrará su sombra, o estela,
 * a no ser que el objeto vuelva a su posición inicial.
 *
 * Este modelo es dependiente a (Forma lista) :
 * La imagen de fondo tomada: si hay un objeto en movimiento en ese frame, se formará su sombra
 * La eleción del umbral: Un valor muy alto genera falsos-negativos pero uno muy bajo genera falsos-positivos
 * Las condiciones de la escena que se esta tratando.
 */
class FondoEstatico: public FrameProcessor {
private:
	cv::Mat previousFrame;
	float threshold;
public:
	FondoEstatico();
	~FondoEstatico();
	void process(cv::Mat &input, cv::Mat &output);
	void setThreshold(float threshold);
};

#endif /* ALLFRAMES_H_ */
