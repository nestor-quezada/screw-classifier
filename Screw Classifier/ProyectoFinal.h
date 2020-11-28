#pragma once

#define _USE_MATH_DEFINES 

#include <QtWidgets/QMainWindow>
#include "ui_ProyectoFinal.h"
#include "QtImageManager.h"
#include <qfiledialog.h>
#include <vector>
#include <QTimer>
#include <cmath>
#include <experimental/filesystem> 
#include <iostream>

struct Caracteristicas {
	
	double area;
	double relacionAspecto;
	float circularidad;
	double width;
	double height;
	cv::Point2f center;
	int clase;
		
};

class ProyectoFinal : public QMainWindow
{
	Q_OBJECT

public:
	ProyectoFinal(QWidget *parent = Q_NULLPTR);
	cv::VideoCapture m_cap;

private:
	Ui::ProyectoFinalClass ui;

protected:
	
	
	bool m_isCapturando=0;
	cv::Mat m_imagenCaptura, m_mean, m_std;
	std::vector<Caracteristicas> m_caracteristicas;
	cv::Ptr<cv::ml::SVM> clasificadorSVM;
	cv::Mat obtenerVectorCaracteristicas(std::vector<cv::Point> contour, std::vector<cv::Point> contours_inner);
	void entrenar();
	void volcadoMatriz(cv::Mat a, cv::Mat b);
	std::vector<std::vector<cv::Point>> segmentarImagen(cv::Mat m_imagenCaptura, std::vector<int> &h, std::vector<cv::Vec4i> &hierarchy);
	void dibujarEtiquetas(int clase_detectada, std::vector<cv::Point> contour);
	std::string m_etiquetas[8] = { "Tuercas", "Arandela", "Alcayata cerr.", "Alcayata ab.",
		"Tirafondos", "Torn. neg. peq.", "Torn. neg. gran.", "Torn. cab. red." };
	

private slots:
	
	void abrirFichero();

	void capturarImagen();

	void guardarImagen();

	void iniciarPararCaptura(bool capturando);

	void clasificar();
			
};



