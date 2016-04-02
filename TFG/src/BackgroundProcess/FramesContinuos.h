/*
 * FramesContinuos.h
 *
 *  Created on: 19/9/2015
 *      Author: Said
 */

#ifndef BACKGROUNDPROCESS_FRAMESCONTINUOS_H_
#define BACKGROUNDPROCESS_FRAMESCONTINUOS_H_
#include <imgproc/imgproc.hpp>
#include <iostream>
#include "FrameProcessor.h"

/**
 * El modelo de susbtración implementado en esta clase es el modelo de Fondo continuo.
 *
 * Este proceso se basa en la siguiente fórmula matemática:
 * <<Insertar después en Latex>>
 *
 * El modelo se basa en la comparación continua con el frame anterior. Aunque no se de
 * el caso de `ghosting`  como el modelo anterior, este modelo sigue siendo dependiente a
 * varios factores. Estos factores son lo siguientes:
 *
 * - Velocidad del objeto que esta en movimiento.
 * - Frecuencia de captura.
 * - El valor elegido para el umbral
 */
class FramesContinuos: public FrameProcessor {
private:
	cv::Mat previousFrame;
	float threshold;
public:
	FramesContinuos();
	~FramesContinuos();
	void process(cv::Mat &input, cv::Mat &output);
	void setThreshold(float threshold);
};

#endif /* BACKGROUNDPROCESS_FRAMESCONTINUOS_H_ */
