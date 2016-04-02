/*
 * FrameProcessor.h
 *
 *  Created on: 16/9/2015
 *      Author: Said
 */

#ifndef FRAMEPROCESSOR_H_
#define FRAMEPROCESSOR_H_
#include <highgui/highgui.hpp>
#include <core/core.hpp>


/**
 * Clase de la que heredan todas los modelos de subtracción de fondo que se van a implementar.
 * Se ha planteado usar el patrón Strategy, es decir, a la clase SustractBackgroundProcess se
 * le pasa la referencia del modelo de substracción de fondo que se quiere aplicar.
 *
 * Mas adelante se explicará como estra estructurado el uso de las implementaciones de los modelos
 * de substracción de fondo.
 */
class FrameProcessor {
public:
	FrameProcessor();
	~FrameProcessor();
	virtual void process(cv::Mat &input, cv::Mat &output) = 0;

};

#endif /* FRAMEPROCESSOR_H_ */
