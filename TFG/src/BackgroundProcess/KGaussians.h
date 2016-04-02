/*
 * KGaussians.h
 *
 *  Created on: 25 de mar. de 2016
 *      Author: saidatrahouchecharrouti
 */

#ifndef BACKGROUNDPROCESS_KGAUSSIANS_H_
#define BACKGROUNDPROCESS_KGAUSSIANS_H_

#include "FrameProcessor.h"

typedef struct KGaussian{
	float variance;
	float muR;
	float muG;
	float muB;
	float weight;
	float significants;
}KGM;

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
