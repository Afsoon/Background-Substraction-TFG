/*
 * KGaussians.cpp
 *
 *  Created on: 25 de mar. de 2016
 *      Author: saidatrahouchecharrouti
 */

#include "KGaussians.h"

/**
* Metodo usado para la comparacion de las gaussianas asociadas
* a un pixel. Se ordena de forma descendente en funcion de
* w/sqrt(varianza)
* */
int compareKGM(const void* _kgm1, const void* _kgm2){
	KGM kgm1 = *(KGM*) _kgm1;
	KGM kgm2 = *(KGM*) _kgm2;

	if(kgm1.significants < kgm2.significants){
		return 1;
	}else if(kgm1.significants == kgm2.significants){
		return 0;
	}else{
		return -1;
	}
}

/**
* Método para inicilizar nuestra matriz de numero de gaussianas
* asociadas a 0 y rellnear nuestra matriz de gaussianas que podrá haber
* a la vez durante la ejecución
* */
void KGaussians::initModel(){
	this->m_modes_per_pixel = cv::Mat::zeros(this->height, this->widht, CV_64F);
	int numberModels = this->size*this->kGaussians;
	for(int i = 0; i < numberModels; ++i){
		this->fillEmptyModel(i);
	}
}

/** Método de configuración, el ususario tiene que indicar el número de gaussianas
que puede tener cada píxel asociado.
*/
void KGaussians::setKGaussians(int kGaussian){
	this->kGaussians = kGaussian;
}

/**
 * Factor de aprendizaje del modelo de substracción de fondo
 * */
void KGaussians::setAlpha(float alpha){
	this->alpha = alpha;
}

/**
 * Varianza que se le asignará a cada nueva gaussiana que se añada
 * */
void KGaussians::setVariance(float variance){
	this->m_variance = variance;
}

/**
 * Valor de T para el proceso de estimación del modelo de fondo.
 * Tiene un valor comprendido entre 0-1. Ya que se normaliza los pesos
 * para que sea más fácil para el usuario incluir un valor de T.
 * */
void KGaussians::setTValue(float t){
	this->t = t;
}

/**
 * Metodo llamado cuando nos encontramos en el primer frame.
 * Se encarga de inicializar todos los valores que no depende del usuario
 * */
void KGaussians::Initialize(int widht, int height){
	this->widht = widht;
	this->height = height;
	this->size = widht * height;

	this->m_modes = new KGM[this->size*this->kGaussians];

	this->m_modes_per_pixel = cvCreateImage(cvSize(this->widht, this->height),IPL_DEPTH_8U, 1);
	this->m_background = cvCreateImage(cvSize(this->widht, this->height), IPL_DEPTH_8U, 3);

}

/**
 * Añade una gaussiana con todos sus valores a 0. Se usa para la inicializar
 * la matriz de gaussianas.
 * */
void KGaussians::fillEmptyModel(int pos){
	this->m_modes[pos].muB = 0;
	this->m_modes[pos].muG = 0;
	this->m_modes[pos].muR = 0;
	this->m_modes[pos].significants = 0;
	this->m_modes[pos].variance = 0;
	this->m_modes[pos].weight = 0;
}

/**
 * Método usado para añadir una gaussiana nueva cuando el pixel no tenga asociada ninguna gaussiana.
 * Esta gaussiana si que se usará para el modelo.
 * */
void KGaussians::fillNewModel(int pos, const cv::Vec3b& pixel){
	this->m_modes[pos].muB = pixel[2];
	this->m_modes[pos].muG = pixel[1];
	this->m_modes[pos].muR = pixel[0];
	this->m_modes[pos].significants = 0;
	this->m_modes[pos].variance = this->m_variance;
	this->m_modes[pos].weight = 1;
}

/**
 * Se añade una gaussiana nueva cuando no ha habido coincidencias con las existentes, este
 * permite indicar con que peso debe empezar la nueva gaussiana.
 * */
void KGaussians::fillNewModelLowWeigth(int pos, const cv::Vec3b& pixel, float weight){
	this->m_modes[pos].muB = pixel[2];
	this->m_modes[pos].muG = pixel[1];
	this->m_modes[pos].muR = pixel[0];
	this->m_modes[pos].significants = 0;
	this->m_modes[pos].variance = this->m_variance;
	this->m_modes[pos].weight = weight;
}

/**
 * En el estado inicial, a cada pixel se le asociada una gaussiana, ya que no tienen ninguna. Todos los
 * píxel son considerados fondo.
 * @param input Fotograma, del vídeo, que se va a manipular
 * @param output Máscara del primer plano obtenido tras manipular el fotograma de entrada
 */
void KGaussians::process(cv::Mat& input, cv::Mat& output){
	if(!this->initialized){
		this->Initialize(input.cols, input.rows);
		this->initialized = true;
	}
	this->substractFrame(input, output);
}

/**
 * Se encarga de calcular la distancia euclídea de los valores RGB del píxel
 * y los valores RGB que contiene la gaussiana
 * */
float KGaussians::getValuePixel(const cv::Vec3b& pixel, int pos){
	float muR = this->m_modes[pos].muR;
	float muG = this->m_modes[pos].muG;
	float muB = this->m_modes[pos].muB;

	float dR = muR - pixel[0];
	float dG = muG - pixel[1];
	float dB = muB - pixel[2];
	return std::sqrt(dR*dR + dG*dG + dB*dB);
}

/**
 * Se encarga de actualizar los valores de la gaussiana que ha producido una coincidencia
 * */
void KGaussians::updateModelCoincidence(int pos, const cv::Vec3b& pixel, float intensity){
	this->m_modes[pos].weight = (1 - this->alpha)*this->m_modes[pos].weight + this->alpha;
	float rho = 0.05; /**cambiarlo por la formula buena**/
	//float rho = m_params.Alpha()*exp(dist/(-2*var_m))/(pow(2*M_PI,3/2)*std::sqrt(var_m)); // 3 - no of channels
	this->m_modes[pos].muR = (1-rho)*this->m_modes[pos].muR + rho*pixel[0];
	this->m_modes[pos].muG = (1-rho)*this->m_modes[pos].muG + rho*pixel[1];
	this->m_modes[pos].muB = (1-rho)*this->m_modes[pos].muB + rho*pixel[2];
	this->m_modes[pos].variance = (1-rho)*this->m_modes[pos].variance + rho*intensity;
	this->m_modes[pos].significants = this->m_modes[pos].weight/ std::sqrt(this->m_modes[pos].variance);
}

/**
 * Se enarga de actualizar los valores de las gaussianas que no ha producido coincidencia o
 * ya ha habido una coincidencia y se ha actualizado la gaussiana correspondiente
 * */
void KGaussians::updateModelWithoutCoincidence(int pos){
	this->m_modes[pos].weight = (1 - this->alpha)*this->m_modes[pos].weight;
	this->m_modes[pos].significants = this->m_modes[pos].weight/ std::sqrt(this->m_modes[pos].variance);
}


/**
 * Interfaz del método del ordenación
 * */
void KGaussians::orderKGaussians(long posPixel, const cv::Vec3b& numModes){
	qsort(&this->m_modes[posPixel], numModes[0], sizeof(KGM), compareKGM);
}

/**
 * Nos indica si ha habido coincidencia con la gaussiana correspondiente o no
 * */
bool KGaussians::fitInTheBackgroundModel(float intensity, int pos){
	return intensity < 2.5*std::sqrt(this->m_modes[pos].variance);
}

/**
 * Nos indica si el pixel que estamos procesando ha producido coincidencia con
 * algunas de las gaussianas asociadas al pixel
 * */
bool KGaussians::thePixelFitInAnyBackgroundModel(long posPixel, const cv::Vec3b& pixel, const cv::Vec3b& numModes, float& sumWeight){
	bool fit = false;
	long pos;

	for(int iMode = 0; iMode < numModes[0]; iMode++){
			pos = posPixel + iMode;

			if(!fit){
				float intensity = this->getValuePixel(pixel, pos);
				if(fitInTheBackgroundModel(intensity, pos)){
					fit = true;
					this->updateModelCoincidence(pos, pixel, intensity);
				}else{
					this->updateModelWithoutCoincidence(pos);
				}
			}else{
				this->updateModelWithoutCoincidence(pos);
			}
			sumWeight += this->m_modes[pos].weight;
	}

	return fit;
}

/**
 * Si se requiere de una actualizacion de gaussianas, se encarga del proceso.
 * Tanto si se ha llegado al limite de gaussianas asociadas al pixel como si hay
 * aún se puede añadir más gaussianas al píxel
 * */
void KGaussians::updateKGaussians(bool isUpdateRequired, long posPixel, const cv::Vec3b& pixel, cv::Vec3b& numModes, float& sumWeight){
	if(!isUpdateRequired){
		if(numModes[0] < this->kGaussians){
			numModes[0]++;
		}else{
			this->orderKGaussians(posPixel, numModes);
			sumWeight -= this->m_modes[(posPixel+numModes[0])-1].weight;
		}
		this->fillNewModelLowWeigth((posPixel+numModes[0])-1, pixel, this->alpha);
		sumWeight += this->alpha;
	}
}

/**
 * Normalización de los pesos de las gaussianas, para que estos tengan un valor comprendido
 * entre 0-1 y sea más fácil para el usuario introducir un valor de T.
 * */
void KGaussians::normalizeWeight(const float sumWeight, long posPixel, const cv::Vec3b& numModes){
	for(int iMode = 0; iMode < numModes[0]; iMode++){
		long pos = iMode + posPixel;
		this->m_modes[pos].weight = this->m_modes[pos].weight/sumWeight;
		this->m_modes[pos].significants = this->m_modes[pos].weight/ std::sqrtf(this->m_modes[pos].variance);
	}
}

/**
 * Este método nos dice si el pixel finalmente se clasifica como FOREGROUND o BACKGROUND
 * */
bool KGaussians::thePixelHaveACoincidence(long posPixel, const cv::Vec3b& pixel, const cv::Vec3b& numModes){
	long pos;
	bool fit = false;
	float weight_actual = 0;
	for(int iMode = 0; iMode < numModes[0]; iMode++){
		pos = posPixel + iMode;

		float intensity = this->getValuePixel(pixel, pos);
		weight_actual += this->m_modes[pos].weight;

		if(fitInTheBackgroundModel(intensity, pos)){
			fit = true;
		}

		if(weight_actual > this->t){
			break;
		}

	}

	return fit;

}

/**
 * Por cada píxel que se trate, se llama a este método. Este método nos dice
 * si el pixel procesado es un píxel de fondo o de primer plano */
void KGaussians::subtractPixel(long posPixel, const cv::Vec3b& pixel, cv::Vec3b& numModes, unsigned char& threshold){
	bool backgroundFit = false;
	float sumWeight = 0;

	if(numModes[0] == 0){
		this->fillNewModel(posPixel, pixel);
		numModes[0]++;
		threshold = this->BACKGROUND;
		return;
	}

	backgroundFit = this->thePixelFitInAnyBackgroundModel(posPixel, pixel, numModes, sumWeight);
	this->updateKGaussians(backgroundFit, posPixel, pixel, numModes, sumWeight);
	this->normalizeWeight(sumWeight, posPixel, numModes);
	this->orderKGaussians(posPixel, numModes);
	backgroundFit = this->thePixelHaveACoincidence(posPixel, pixel, numModes);

	threshold = backgroundFit ? this->BACKGROUND : this->FOREGROUND;
}

/**
 * Se encarga de dibujar la máscada de primer plano, en funcion de si el píxel ha sido
 * de FOREGROUND o de BACKGROUND
 * */
void KGaussians::setColorMask(const unsigned char threshold, const cv::Vec3b& pixel, int row, int column){
	if(threshold == this->BACKGROUND){
		this->m_background.at<cv::Vec3b>(row, column)[0] = this->BACKGROUND;
		this->m_background.at<cv::Vec3b>(row, column)[1] = this->BACKGROUND;
		this->m_background.at<cv::Vec3b>(row, column)[2] = this->BACKGROUND;
	}else{
		this->m_background.at<cv::Vec3b>(row, column)[0] = pixel[0];
		this->m_background.at<cv::Vec3b>(row, column)[1] = pixel[1];
		this->m_background.at<cv::Vec3b>(row, column)[2] = pixel[2];
	}
}

/**
 * Recorre todos los pixeles del frame de entrada y llama a los métodos
 * para construir la máscara de primer plano
 * */
void KGaussians::substractFrame(cv::Mat& input, cv::Mat& output){

	unsigned char threshold;
	long posPixel;
	for(int i = 0; i < this->height; ++i){
		for(int j = 0; j < this->widht; ++j){
			posPixel = (i*this->widht+j)*this->kGaussians;

			this->subtractPixel(posPixel, input.at<cv::Vec3b>(i,j), m_modes_per_pixel.at<cv::Vec3b>(i,j), threshold);
			this->setColorMask(threshold, input.at<cv::Vec3b>(i,j), i, j);
		}
	}

	output = this->m_background;
}


KGaussians::KGaussians() {
	// TODO Auto-generated constructor stub

}

KGaussians::~KGaussians() {
	// TODO Auto-generated destructor stub
}
