#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>
#include <stdio.h> 

// ���������� ���������� 
cv::Mat src;// ������� ��� ��������� ����������� 
cv::Mat src_grey;// ������� ��� ����������� � ��������� ������
cv::Mat dst;//������� ����� ��������
cv::Mat sec_dst;//������� ����� ������� ��������
cv::Mat grany;//������� ����� ��������� ������ 
int karnelsize,sec_karnelsize;// �������� ��� ��������
int canny_theshold_min;// ������ ������� ��� ��������� ����� 
int canny_theshold_max;// ������� ������� ��� ��������� ����� 


void CannyThreshold(int, void*)
{   //�������� ������
	cv::GaussianBlur(src_grey, dst, cv::Size(abs(2 * karnelsize - 1), abs(2 * karnelsize - 1)), 0, 0);

	// �������� ������ ����� 
	cv::Canny(dst, grany, canny_theshold_min, canny_theshold_max);

	//dst1 = cv::Scalar::all(0);  �������� ������� �� ����� 

	//src.copyTo(dst1, grany); ����������� � ��������� ����������� � ������ ������� �� �������� ����� 
	cv::imshow("Result", grany);
}

//������� ��� �������� �������� ������� ������ 
void GaussinBlurTrackbar(int,void*) {

	// �������� ������
	cv::GaussianBlur(src_grey, dst, cv::Size(abs(2*karnelsize-1),abs( 2*karnelsize-1)), 0, 0);

	//�������� ������
	cv::Canny(dst, grany, canny_theshold_min, canny_theshold_max);

	//����� ����������� ����� �������� � ��������� ������
	cv::imshow("Result", grany);

}

void SecondGaussinBlurTrackbar(int, void*) {

	// �������� ������
	cv::GaussianBlur(dst, sec_dst, cv::Size(abs(2 * sec_karnelsize - 1), abs(2 * sec_karnelsize - 1)), 0, 0);

	//�������� ������
	cv::Canny(sec_dst, grany, canny_theshold_min, canny_theshold_max);

	//����� ����������� ����� �������� � ��������� ������
	cv::imshow("After second blur", grany);

}

//������� ��� ������ ���������� ���� ����������� � ������� � ���� �������� �������
void printmatrix(int, void*) {

	// ������� ��� ���� ������� � ������� �������� ������� ����� ��������� ������
	cv::Mat matrix = grany.clone();

	for (int i = 0; i < matrix.rows; ++i)
		for (int j = 0; j < matrix.cols; ++j)
			if (matrix.at<uchar>(i, j) > 0)
				matrix.at<uchar>(i, j) = 255;

	//����� ������� ����� ��������������  �� ����� 
	cv::imshow("Before", matrix);

	//����� ������� � �������
	std::cout << matrix << std::endl;
}

int main(int argc, char** argv)
{
	int x;
	//�������� ���� ��� ������
	cv::namedWindow("Before");
	cv::namedWindow("Result");
	cv::namedWindow("After second blur");

	// �������� �����������, �������� ������� � ������� � �� �����������
	 src = cv::imread("Anamorphic_by_Brusspup.jpg",1);// >0 - �������������, <0 - �������� ���-�� �������, =0 - ������� ������
	if (!src.data)
	{
		return -1;
	}

	// ����� � ���� �� ������� � �������� ������������
	cv::imshow("Before", src);

	//�������������� ���������� � ������� ������
	cv::cvtColor(src, src_grey, CV_BGR2GRAY); // � opencv �� RGB, � BGR

	cv::imshow("Result", src_grey);

	//�������� ���������
	cv::createTrackbar("Blur", "Result", &karnelsize,51, GaussinBlurTrackbar);
	cv::createTrackbar("Second Blur", "Result", &sec_karnelsize, 51, SecondGaussinBlurTrackbar);
	cv::createTrackbar("Canny min", "Result", &canny_theshold_min, 100, CannyThreshold);
	cv::createTrackbar("Canny max", "Result", &canny_theshold_max, 300, CannyThreshold);
	cv::createTrackbar("Print", "Result",&x, 2 , printmatrix);

	//�������� ������� �������
	cv::waitKey(0);
	

	return 0;
}