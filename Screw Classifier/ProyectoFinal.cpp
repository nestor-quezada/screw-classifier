#include "ProyectoFinal.h"
#include "opencv2/opencv.hpp"


ProyectoFinal::ProyectoFinal(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(abrirFichero()));
	connect(ui.buttonCapturarImagen, SIGNAL(toggled(bool)), this, SLOT(iniciarPararCaptura(bool)));
	connect(ui.buttonGuardarImagen, SIGNAL(clicked()), this, SLOT(guardarImagen()));
	connect(ui.buttonSegmentar, SIGNAL(clicked()), this, SLOT(clasificar()));
		
	m_cap.open(0);

	clasificadorSVM = cv::Algorithm::load<cv::ml::SVM>("clasificador_svm.yml");
	m_mean = cv::Mat(1, 4, CV_32F);
	m_std = cv::Mat(1, 4, CV_32F);
	//[0.03022702, 4.0839653, 0.067446686, 7.6189709][0.024217753, 3.234998, 0.090186708, 10.399039]
	m_mean.at<float>(0, 0)= 0.03022702;
	m_mean.at<float>(0, 1) = 4.0839653;
	m_mean.at<float>(0, 2) = 0.067446686;
	m_mean.at<float>(0, 3) = 7.6189709;

	m_std.at<float>(0, 0) = 0.024217753;
	m_std.at<float>(0, 1) = 3.234998;
	m_std.at<float>(0, 2) = 0.090186708;
	m_std.at<float>(0, 3) = 10.399039;
	   	   	 
}

void ProyectoFinal::abrirFichero() {
		
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Save Address Book"), "",
		tr("JPEG (*.jpg *.jpeg);;PNG (*.png)"));
	
	m_imagenCaptura = cv::imread(fileName.toUtf8().constData(), CV_LOAD_IMAGE_COLOR);
		
	ui.imagenmostrar->setVisualicedImage(m_imagenCaptura);
	
}

void ProyectoFinal::capturarImagen() {
	
	if (m_isCapturando) {
			
		// Leer el frame actual de la cámara y almacenarlo en la matriz.
		m_cap.read(m_imagenCaptura);
		
		// Verificar si ha habido un error en la lectura.
		if (m_imagenCaptura.empty()) {
			std::cerr << "ERROR! blank frame grabbed\n";
		}
		
		// Timer para leer de forma continua el frame actual.
		QTimer::singleShot(50, this, SLOT(capturarImagen()));
		
		clasificar();
		
						
	}
		
}

void ProyectoFinal::iniciarPararCaptura(bool capturando) {
	
	m_isCapturando = capturando;

	if (m_isCapturando) {
		
		capturarImagen();
		
	}
}

void ProyectoFinal::guardarImagen() {
	
	// Vector con los parámetros de configuración de la compresión.
	std::vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(5);

	QString strFecha, strHora, nombreImagen;
	QTime hora;
	QDate fecha;
	
	// Se crea el nombre y ruta de la imagen.  
	strFecha = fecha.currentDate().toString("yyyy-MM-dd");
	strHora = hora.currentTime().toString("hh-mm-ss-zzz");
	nombreImagen = "C:/imagenes/ferr_"+ strFecha + "__"+ strHora +".png";
	
	// Guarda la imagen.
	cv::imwrite(nombreImagen.toUtf8().constData(), m_imagenCaptura, compression_params);

}

std::vector<std::vector<cv::Point>> ProyectoFinal::segmentarImagen(cv::Mat m_imagenCaptura, std::vector<int> &h, std::vector<cv::Vec4i> &hierarchy) {
	cv::Mat imagen_binarizada,
		rgbchannel[3], kernel, imagen_topHat, imagen_contornos,
		imagen_open, imagen_close, kernel_open, imagen_filtro_mediana,
		imagen_filtro_bilateral, kernel_close, kernel_erosion, image_HSV, imagen_negative;
	
	// Array de vectores para almacenar los contornos
	std::vector<std::vector<cv::Point>> contours;
	
		
	float height, tmp_width, min_side_value;

	// Divide la imagen en sus componentes RGB
	split(m_imagenCaptura, rgbchannel);

	// Elemento estructurante
	kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(35, 35));
	
	cv::cvtColor(m_imagenCaptura, image_HSV, cv::COLOR_BGR2HSV);

	std::vector<cv::Mat> hsv_planes;
	split(image_HSV, hsv_planes);
	
	// Filtro paso bajo bilateral.
	cv::bitwise_not(hsv_planes[1], imagen_negative);
	cv::bilateralFilter(imagen_negative, imagen_filtro_bilateral, 35, 25, 10);
	cv::medianBlur(imagen_filtro_bilateral, imagen_filtro_mediana, 5);

	// Top hat
	cv::morphologyEx(imagen_filtro_mediana, imagen_topHat, CV_MOP_TOPHAT, kernel);

	// Binarización de la imagen
	cv::adaptiveThreshold(imagen_topHat, imagen_binarizada, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 21, -3);

	// Aperura y cierre de la imagen.
	//kernel_open = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(1,1));

	//kernel_close = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(1, 1));

	//cv::morphologyEx(imagen_binarizada, imagen_open, CV_MOP_OPEN, kernel_open);

	//cv::morphologyEx(imagen_open, imagen_close, CV_MOP_CLOSE, kernel_close);

	// Crea la matriz para almacenar los bordes.
	imagen_contornos = imagen_binarizada.clone();
	//std::vector<cv::Vec4i> hierarchy;
	// Obtenemos los contornos
	cv::findContours(imagen_contornos, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

	std::vector<std::vector<cv::Point>> contornos_final;
	//std::vector<std::vector<cv::Point>> contours_inner(contours.size());
	
	h.clear();
	min_side_value = 10;
	int cont = 0;
	for (size_t idx = 0; idx < contours.size(); idx++) {
		cv::RotatedRect minRect;
		
		minRect = cv::minAreaRect(cv::Mat(contours[idx]));
		if (minRect.size.height > min_side_value && minRect.size.width > min_side_value) {
			
			
			int index = hierarchy[idx][3];
			
			h.push_back(index);
			contornos_final.push_back(contours[idx]);
			
			//drawContours(m_imagenCaptura, contours, idx, cv::Scalar(255,0,0), 2, 8);
			
						
		}
		else {
			h.push_back(-2);
			std::vector<cv::Point> contorno_vacio;
			contornos_final.push_back(contorno_vacio);
		}
	}
		
	
	
	//ui.imagenmodificada->setVisualicedImage(imagen_binarizada);

	return contornos_final;
}

void ProyectoFinal::clasificar() {
	cv::Mat vector_car;
	float clase_detectada;
	std::vector<int> h;
	std::vector<cv::Vec4i> hierarchy;
	if (!clasificadorSVM->isTrained())
		entrenar();
	std::vector<std::vector<cv::Point>> contours = segmentarImagen(m_imagenCaptura, h, hierarchy);
	int inner_index = -1;
	for (int i = 0; i < contours.size(); i++) {
		
		if (h[i] == -1) {
			/*for (int j = 0; j < h.size(); j++) {

				if (h[j] == i) {
					inner_index = j;
					break;
				}
			}*/
			inner_index = hierarchy[i][2];
			std::vector<cv::Point> p;
			if (inner_index > -1)
				vector_car = obtenerVectorCaracteristicas(contours[i], contours[inner_index]);
			if (inner_index == -1)
				vector_car = obtenerVectorCaracteristicas(contours[i], p);
			inner_index = -1;

			
			for (int i = 0; i < vector_car.cols; i++) {

				vector_car.col(i) = (vector_car.col(i) - m_mean.at<float>(0, i)) / m_std.at<float>(0, i);

			}

			clase_detectada = clasificadorSVM->predict(vector_car);
			dibujarEtiquetas(clase_detectada, contours[i]);
			


			//clase_detectada = clasificadorSVM->predict(vector_car);
			//dibujarEtiquetas(clase_detectada, contours[i]);
		}
		//for (int i = 0; i < vector_car.cols; i++) {
				
			//vector_car.col(i) = (vector_car.col(i) - m_mean.at<float>(0,i)) / m_std.at<float>(0, i);
			
		//}
		
		
	}

	ui.imagenmostrar->setVisualicedImage(m_imagenCaptura);

	contours.clear();

}

void ProyectoFinal::dibujarEtiquetas(int clase_detectada, std::vector<cv::Point> contour) {

	cv::RotatedRect minRect = cv::minAreaRect(cv::Mat(contour));

	//cv::RotatedRect ellipse = cv::fitEllipse(contour);
	//cv::ellipse(m_imagenCaptura, ellipse, (0, 255, 0), 2);
	int baseline = 0;
		
	cv::putText(m_imagenCaptura, m_etiquetas[clase_detectada - 1], minRect.center, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0, 0), 1);

}

void ProyectoFinal::entrenar() {

	std::string path = "C:/imagenes/imagenes psm final/entrenar";
	std::string path_image;
	QString	filename;
	cv::Mat imagen_entrenar, TD2, TL2;
	std::vector<cv::Mat> TD_vector;
	std::vector<int> TL_vector;
	std::vector<std::vector<cv::Point>> contours;

				
	for (auto & p : std::experimental::filesystem::directory_iterator(path)) {

		path_image = p.path().string();

		filename = QString::fromStdString(p.path().filename().string());

		imagen_entrenar = cv::imread(path_image, CV_LOAD_IMAGE_COLOR);

		QStringList pieces = filename.split("_");
		QString claseElemento_str = pieces.at(1);

		int claseElemento = claseElemento_str.toInt();
		std::vector<int> h;std::vector<cv::Vec4i> hierarchy;
		// Hallar contornos de la imagen(Array de contornos)
		contours = segmentarImagen(imagen_entrenar, h, hierarchy);

		//std::vector<std::vector<cv::Point>> contours = segmentarImagen(m_imagenCaptura, h);
		int inner_index = -1;
		

			
			//for (int i = 0; i < vector_car.cols; i++) {

				//vector_car.col(i) = (vector_car.col(i) - m_mean.at<float>(0,i)) / m_std.at<float>(0, i);


		// Recorrer con un for el array de contornos y meterlo en la matriz de TD Y TL
		for (size_t idx = 0; idx < contours.size(); idx++) {

			cv::Mat vector_caracteristica;
			if (h[idx] == -1) {
				for (int j = 0; j < h.size(); j++) {

					if (h[j] == idx) {
						inner_index = j;
						break;
					}
				}
				std::vector<cv::Point> vacio_contour;
				if (inner_index > -1)
					vector_caracteristica = obtenerVectorCaracteristicas(contours[idx], contours[inner_index]);
				if (inner_index == -1)
					vector_caracteristica = obtenerVectorCaracteristicas(contours[idx], vacio_contour);
				inner_index = -1;
				
				TD_vector.push_back(vector_caracteristica);
				TL_vector.push_back(claseElemento);

				
			}
			
			
		}

		contours.clear();
	
	}

	TD2 = cv::Mat(TD_vector.size(), TD_vector[0].cols, CV_32F);
	TL2 = cv::Mat(TL_vector.size(), 1, CV_32S);
	for (int i = 0; i < TD_vector.size() ; i++) {
	
		for(int j=0; j < TD2.cols; j++)
			TD2.at<float>(i,j) = TD_vector[i].at<float>(0,j);
		
		
		TL2.at<int>(i,0) = TL_vector[i];
	}
	
	m_mean = cv::Mat(1, TD2.cols, CV_32F);
	m_std = cv::Mat(1, TD2.cols, CV_32F);
	for (int i = 0; i<TD2.cols; i++) {
		cv::Scalar mean, std;
		cv::meanStdDev(TD2.col(i), mean, std);
		
		TD2.col(i) = (TD2.col(i) - mean[0]) / std[0];
		m_mean.at<float>(0,i)= mean[0];
		m_std.at<float>(0, i) = std[0];
	}
		
	volcadoMatriz(TD2, TL2);
	
	// Train the SVM
	clasificadorSVM = cv::ml::SVM::create();
	clasificadorSVM->setType(cv::ml::SVM::C_SVC);
	clasificadorSVM->setKernel(cv::ml::SVM::LINEAR);
	clasificadorSVM->setC(50);
	clasificadorSVM->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));
	clasificadorSVM->train(TD2, cv::ml::ROW_SAMPLE, TL2);
	clasificadorSVM->save("clasificador_svm.yml");
		
}

cv::Mat ProyectoFinal::obtenerVectorCaracteristicas(std::vector<cv::Point> contour, std::vector<cv::Point> contours_inner) {
	cv::Mat vector_caracteristicas(1, 4, CV_32F);
	float tmp_width;
	cv::RotatedRect minRect;
	minRect = cv::minAreaRect(cv::Mat(contour));
	cv::RotatedRect ellipse = cv::fitEllipse(contour);
	// Hallamos el perimetro
	float perimeter = cv::arcLength(contour, true);
	// Set del ratio perimetro bounding box / perimetro contorno
	//vector_caracteristicas.at<float>(0, 2) = (minRect.size.height * 2 + minRect.size.width * 2)/ perimeter;
	
	// Set del area.
	vector_caracteristicas.at<float>(0,0) = cv::contourArea(contour)/ (perimeter * perimeter);
	//vector_caracteristicas.at<float>(0, 0) = cv::contourArea(contour);

	// La altura siempre será el valor mayor.
	if (minRect.size.height < minRect.size.width) {
		tmp_width = minRect.size.width;
		minRect.size.width = minRect.size.height;
		minRect.size.height = tmp_width;
	}
	
	// Set del aspect ratio
	vector_caracteristicas.at<float>(0, 1) = minRect.size.height / minRect.size.width;
	
	
	
	// Set del ratio perimetro bounding box / perimetro contorno
	//vector_caracteristicas.at<float>(0, 2) = (minRect.size.height * 2 + minRect.size.width * 2)/ perimeter;
	if (ellipse.size.height < ellipse.size.width) {
		tmp_width = ellipse.size.width;
		ellipse.size.width = ellipse.size.height;
		ellipse.size.height = tmp_width;
	}
	
	//vector_caracteristicas.at<float>(0, 2) = ellipse.size.height / ellipse.size.width;

	float minRectPerimeter = perimeter;
	if (contours_inner.size() == 0) {
		
		vector_caracteristicas.at<float>(0, 2) = 0;
	}
	else {
		//vector_caracteristicas.at<float>(0, 2) = cv::contourArea(contour) / cv::contourArea(contours_inner);
		vector_caracteristicas.at<float>(0, 2) =  cv::contourArea(contours_inner)/minRect.size.area();
		//vector_caracteristicas.at<float>(0, 2) = cv::arcLength(contour, true) / cv::arcLength(contours_inner, true);
		//vector_caracteristicas.at<float>(0, 2) = cv::contourArea(contours_inner, true)  /( cv::arcLength(contours_inner, true) * cv::arcLength(contours_inner, true));
	}
	//vector_caracteristicas.at<float>(0, 3) = minRect.size.area() / (minRectPerimeter*minRectPerimeter);
	vector_caracteristicas.at<float>(0, 3) = ellipse.size.height / ellipse.size.width;
	return vector_caracteristicas;

}

void ProyectoFinal::volcadoMatriz(cv::Mat a, cv::Mat b) {

	QString nombreFichero = "C:/imagenes/ficheros-caracteristicas/volcado_caracteristicas.csv";

	std::ofstream fichero_caracteristicas;
	fichero_caracteristicas.open(nombreFichero.toUtf8().constData(), std::ofstream::app);
	for (int i = 0; i < a.rows; i++) {
		std::string slabel = std::to_string(b.at<int>(i));
		fichero_caracteristicas << slabel << ",";
		fichero_caracteristicas << a.at<float>(i, 0) << ",";
		fichero_caracteristicas << a.at<float>(i, 1) << ",";
		fichero_caracteristicas << a.at<float>(i, 2) << ",";
		fichero_caracteristicas << a.at<float>(i, 3) << "\n";

	}

	fichero_caracteristicas.close();

}