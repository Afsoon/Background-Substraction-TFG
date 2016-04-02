/*
 * KGaussians.h
 *
 *  Created on: 25 de mar. de 2016
 *      Author: saidatrahouchecharrouti
 */

#ifndef BACKGROUNDPROCESS_KGAUSSIANS_H_
#define BACKGROUNDPROCESS_KGAUSSIANS_H_

#include "FrameProcessor.h"

/**
 * Estructura de datos que usamos para mantener los valores de cada gaussiana
 * asociada a un pixel. Guardamos su varianza, los valores RGB, su peso y su
 * significante, más adelante se sabrá para que es.
 */
typedef struct KGaussian{
	float variance;
	float muR;
	float muG;
	float muB;
	float weight;
	float significants;
}KGM;

/**
 * La técnica que ha sido aplicada para obtener la máscara del plano ha sido el modelo de
 * mezcla de K gaussianas.
 *
 * Este modelo se basa en la probabilidad de un píxel pertezca al proceso de fondo basado en
 * su serie temporal de valores que toma a lo larto de un tiempo t.
 *
 * Por cada píxel tiene asociada un número máximo de gaussianas, este valor esta comprendido entre [3-5].
 * El píxel se considera que ha habido una coincidencia cuando el valor del píxel tiene una desviación
 * para una distribución menor que 2.5 su desviación estándar.
 *
 * En el caso de no producir una coincidencia, se añade una gaussiana o se sustituye la distribución
 * menos probable con las siguientes características:
 *  - Con el valor actual como su valor medio
 *  - una varianza inicial alta
 *  - un peso bajo
 *
 * El proceso de actualización de las gaussianas es el siguiente:
 *
 * Si se ha producido una coincidencia, la varianza, media y peso se actualiza de la siguiente forma:
 *
 * <<Insertar fórmulas matemáticas>>
 * Peso:
 *
 * Media:
 *
 * Varianza:
 *
 * <rho>:
 *
 * Si es una gaussiana que no ha producido coincidencia o ya ha habido una, solo se actualiza su peso:
 *
 * Tras la actualización, comprobación de si ha habido una coincidencia, y su ordenación, se estima
 * el modelo de fondo mediante la siguienet fórmula:
 *
 * <<Formula>>
 *
 * Si T es grande, entonces el modelo de fondo será multimodal.
 * Si T es pequeña, entonces el modelo de fondo sera unimodal.
 */
class KGaussians: public FrameProcessor {
private:
	bool initialized;
	int widht;
	int height;
	int size;
	float t;
	int kGaussians;
	//Array dinámico de K gaussians.
	KGM* m_modes;
	float alpha;
	//variannza
	float m_variance;
	//numero de modelos por pixel
	cv::Mat m_modes_per_pixel;
	//modelo del fondo
	cv::Mat m_background;
	const unsigned char FOREGROUND = 255;
	const unsigned char BACKGROUND = 0;
	void initModel();
	void fillEmptyModel(int pos);
	void fillNewModel(int pos, const cv::Vec3b& pixel);
	void fillNewModelLowWeigth(int pos, const cv::Vec3b& pixel, float weight);
	void Initialize(int widht, int height);
	void substractFrame(cv::Mat& input, cv::Mat &output);
	void subtractPixel(long posPixel, const cv::Vec3b& pixel, cv::Vec3b& numModes, unsigned char& threshold);
	void updateModelCoincidence(int pos, const cv::Vec3b& pixel, float intensity);
	void updateModelWithoutCoincidence(int pos);
	void calculateSignificants(long posPixel, const cv::Vec3b& numModes);
	void orderKGaussians(long posPixel, const cv::Vec3b& numModes);
	void updateKGaussians(bool isUpdateRequired, long posPixel, const cv::Vec3b& pixel, cv::Vec3b& numModes, float& sumWeight);
	void normalizeWeight(const float sumWeight, long posPixel, const cv::Vec3b& numModes);
	void setColorMask(const unsigned char threshold, const cv::Vec3b& pixel, int row, int column);
	float getValuePixel(const cv::Vec3b& pixel, int pos);
	bool fitInTheBackgroundModel(float intensity, int pos);
	bool thePixelFitInAnyBackgroundModel(long posPixel, const cv::Vec3b& pixel, const cv::Vec3b& numModes, float& sumWeight);
	bool thePixelHaveACoincidence(long posPixel, const cv::Vec3b& pixel, const cv::Vec3b& numModes);

public:
	KGaussians();
	~KGaussians();
	void process(cv::Mat &input, cv::Mat &output);
	void setKGaussians(int kGaussian);
	void setAlpha(float alpha);
	void setVariance(float variance);
	void setTValue(float t);


};

#endif /* BACKGROUNDPROCESS_KGAUSSIANS_H_ */
