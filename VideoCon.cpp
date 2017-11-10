#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>

// глобальные переменные 
cv::Mat src;// матрица дл€ исходного изображени€ 
cv::Mat src_grey;// матрица дл€ изображени€ в градаци€х серого
cv::Mat dst;//матрица после размыти€ 
cv::Mat grany;//матрица после детектора границ 
int karnelsize;// параметр дл€ размыти€
int canny_theshold_min;// нижн€€ граница дл€ детектора канни 
int canny_theshold_max;// верхн€€ граница дл€ детектора канни


void CannyThreshold(int, void*)
{   //размытие гаусса
	cv::GaussianBlur(src_grey, dst, cv::Size(abs(2 * karnelsize - 1), abs(2 * karnelsize - 1)),0,0);

	// ƒетектор границ канни 
	cv::Canny(dst, grany, canny_theshold_min, canny_theshold_max);

	//dst1 = cv::Scalar::all(0); //—оздание массива из нулей 
	//src_grey.copyTo(dst1, grany); //копирование с исходного изображени€ в пустую матрицу по границам канни 

	cv::imshow("Result", grany);
}

//функци€ дл€ ползунка размыти€ методом √аусса 
void GaussinBlurTrackbar(int, void*) {

	// размытие √аусса
	cv::GaussianBlur(src_grey, dst, cv::Size(abs(2 * karnelsize - 1), abs(2 * karnelsize - 1)),0,0);

	//детектор границ
	cv::Canny(dst, grany, canny_theshold_min, canny_theshold_max);

	//вывод изображени€ после размыти€ и детектора границ
	cv::imshow("Result", grany);

}

//функци€ дл€ вывода созданного нами изображени€ в консоль в виде бинарной матрицы
void printmatrix(int, void*) {

	// создаем ещЄ одну матрицу в которую копируем матрицу после детектора границ
	cv::Mat matrix = grany.clone();

	for (int i = 0; i < matrix.rows; ++i)
		for (int j = 0; j < matrix.cols; ++j)
			if (matrix.at<uchar>(i, j) > 0)
				matrix.at<uchar>(i, j) = 255;

	//вывод матрицы после преобразовани€  на экран 
	cv::imshow("Before", matrix);

	//вывод матрицы в консоль
	std::cout << matrix << std::endl;
}

int main(int argc, char** argv)
{
	int x=0;//переменна€ дл€ функции вывода матрицы в консоль, не несЄт ценности 

	//std::cout << "write image name" << std::endl;

	// «агрузка изображение, перенос в матрицу изображени€
	//char filename[128];//создание строки дл€ имени файла 
	//std::cin >> filename; //считывание имени файла
	//src = cv::imread(filename, 1);// загрузка в матрицу изображени€
								  //>0 - трехканальное, <0 - исходное кол-во каналов, =0 - оттенки серого


	//if (!src.data)
	//{
		//return -1;
	//}

	cv::VideoCapture cap(0); // подключение к камере 
	if (!cap.isOpened())  // проверка 
		return -1;


	//—оздание окон дл€ вывода
	cv::namedWindow("Before");
	cv::namedWindow("Result");
	cv::namedWindow("Tools");

	for (;;) {

		cap >> src;

		// ¬ывод в окне ƒќ матрицы с исходным изображением
		cv::imshow("Before", src);

		//преобразование избражени€ в оттенки серого
		cv::cvtColor(src, src_grey, CV_BGR2GRAY); // в opencv не RGB, а BGR

		//создание ползунков
		cv::createTrackbar("Blur", "Tools", &karnelsize, 51, GaussinBlurTrackbar);
		cv::createTrackbar("Canny min", "Tools", &canny_theshold_min, 256, CannyThreshold);
		cv::createTrackbar("Canny max", "Tools", &canny_theshold_max, 256, CannyThreshold);
		cv::createTrackbar("Print", "Tools", &x, 2, printmatrix);

		cv::GaussianBlur(src_grey, dst, cv::Size(abs(2 * karnelsize - 1), abs(2 * karnelsize - 1)),0,0);
		cv::Canny(dst, grany, canny_theshold_min, canny_theshold_max);
		cv::imshow("Result", grany);

		//ожидание нажати€ клавиши
		if (cv::waitKey(1) >= 0) break;

	}

	return 0;
}