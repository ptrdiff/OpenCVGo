#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>
#include <stdio.h> 

// глобальные переменные 
cv::Mat src;// матрица для исходного изображения 
cv::Mat src_grey;// матрица для изображения в градациях серого
cv::Mat dst;//матрица после размытия
cv::Mat sec_dst;//матрица после второго размытия
cv::Mat grany;//матрица после детектора границ 
int karnelsize,sec_karnelsize;// параметр для размытия
int canny_theshold_min;// нижняя граница для детектора канни 
int canny_theshold_max;// верхняя граница для детектора канни 


void CannyThreshold(int, void*)
{   //размытие гаусса
	cv::GaussianBlur(src_grey, dst, cv::Size(abs(2 * karnelsize - 1), abs(2 * karnelsize - 1)), 0, 0);

	// Детектор границ канни 
	cv::Canny(dst, grany, canny_theshold_min, canny_theshold_max);

	//dst1 = cv::Scalar::all(0);  Создание массива из нулей 

	//src.copyTo(dst1, grany); копирование с исходного изображения в пустую матрицу по краницам канни 
	cv::imshow("Result", grany);
}

//функция для ползунка размытия методом Гаусса 
void GaussinBlurTrackbar(int,void*) {

	// размытие Гаусса
	cv::GaussianBlur(src_grey, dst, cv::Size(abs(2*karnelsize-1),abs( 2*karnelsize-1)), 0, 0);

	//детектор границ
	cv::Canny(dst, grany, canny_theshold_min, canny_theshold_max);

	//вывод изображения после размытия и детектора границ
	cv::imshow("Result", grany);

}

void SecondGaussinBlurTrackbar(int, void*) {

	// размытие Гаусса
	cv::GaussianBlur(dst, sec_dst, cv::Size(abs(2 * sec_karnelsize - 1), abs(2 * sec_karnelsize - 1)), 0, 0);

	//детектор границ
	cv::Canny(sec_dst, grany, canny_theshold_min, canny_theshold_max);

	//вывод изображения после размытия и детектора границ
	cv::imshow("After second blur", grany);

}

//функция для вывода созданного нами изображения в консоль в виде бинарной матрицы
void printmatrix(int, void*) {

	// создаем ещё одну матрицу в которую копируем матрицу после детектора границ
	cv::Mat matrix = grany.clone();

	for (int i = 0; i < matrix.rows; ++i)
		for (int j = 0; j < matrix.cols; ++j)
			if (matrix.at<uchar>(i, j) > 0)
				matrix.at<uchar>(i, j) = 255;

	//вывод матрицы после преобразования  на экран 
	cv::imshow("Before", matrix);

	//вывод матрицы в консоль
	std::cout << matrix << std::endl;
}

int main(int argc, char** argv)
{
	int x;
	//Создание окон для вывода
	cv::namedWindow("Before");
	cv::namedWindow("Result");
	cv::namedWindow("After second blur");

	// Загрузка изображение, создание матрицы и перенос в неё изображения
	 src = cv::imread("Anamorphic_by_Brusspup.jpg",1);// >0 - трехканельное, <0 - исходное кол-во каналов, =0 - оттенки серого
	if (!src.data)
	{
		return -1;
	}

	// Вывод в окне ДО матрицы с исходным изображением
	cv::imshow("Before", src);

	//преобразование избражения в оттенки серого
	cv::cvtColor(src, src_grey, CV_BGR2GRAY); // в opencv не RGB, а BGR

	cv::imshow("Result", src_grey);

	//создание ползунков
	cv::createTrackbar("Blur", "Result", &karnelsize,51, GaussinBlurTrackbar);
	cv::createTrackbar("Second Blur", "Result", &sec_karnelsize, 51, SecondGaussinBlurTrackbar);
	cv::createTrackbar("Canny min", "Result", &canny_theshold_min, 100, CannyThreshold);
	cv::createTrackbar("Canny max", "Result", &canny_theshold_max, 300, CannyThreshold);
	cv::createTrackbar("Print", "Result",&x, 2 , printmatrix);

	//ожидание нажатия клавиши
	cv::waitKey(0);
	

	return 0;
}