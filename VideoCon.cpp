#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>

// ���������� ���������� 
cv::Mat src;// ������� ��� ��������� ����������� 
cv::Mat src_grey;// ������� ��� ����������� � ��������� ������
cv::Mat dst;//������� ����� �������� 
cv::Mat grany;//������� ����� ��������� ������ 
int karnelsize;// �������� ��� ��������
int canny_theshold_min;// ������ ������� ��� ��������� ����� 
int canny_theshold_max;// ������� ������� ��� ��������� �����


void CannyThreshold(int, void*)
{   //�������� ������
	cv::GaussianBlur(src_grey, dst, cv::Size(abs(2 * karnelsize - 1), abs(2 * karnelsize - 1)),0,0);

	// �������� ������ ����� 
	cv::Canny(dst, grany, canny_theshold_min, canny_theshold_max);

	//dst1 = cv::Scalar::all(0); //�������� ������� �� ����� 
	//src_grey.copyTo(dst1, grany); //����������� � ��������� ����������� � ������ ������� �� �������� ����� 

	cv::imshow("Result", grany);
}

//������� ��� �������� �������� ������� ������ 
void GaussinBlurTrackbar(int, void*) {

	// �������� ������
	cv::GaussianBlur(src_grey, dst, cv::Size(abs(2 * karnelsize - 1), abs(2 * karnelsize - 1)),0,0);

	//�������� ������
	cv::Canny(dst, grany, canny_theshold_min, canny_theshold_max);

	//����� ����������� ����� �������� � ��������� ������
	cv::imshow("Result", grany);

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
	int x=0;//���������� ��� ������� ������ ������� � �������, �� ���� �������� 

	//std::cout << "write image name" << std::endl;

	// �������� �����������, ������� � ������� �����������
	//char filename[128];//�������� ������ ��� ����� ����� 
	//std::cin >> filename; //���������� ����� �����
	//src = cv::imread(filename, 1);// �������� � ������� �����������
								  //>0 - �������������, <0 - �������� ���-�� �������, =0 - ������� ������


	//if (!src.data)
	//{
		//return -1;
	//}

	cv::VideoCapture cap(0); // ����������� � ������ 
	if (!cap.isOpened())  // �������� 
		return -1;


	//�������� ���� ��� ������
	cv::namedWindow("Before");
	cv::namedWindow("Result");
	cv::namedWindow("Tools");

	for (;;) {

		cap >> src;

		// ����� � ���� �� ������� � �������� ������������
		cv::imshow("Before", src);

		//�������������� ���������� � ������� ������
		cv::cvtColor(src, src_grey, CV_BGR2GRAY); // � opencv �� RGB, � BGR

		//�������� ���������
		cv::createTrackbar("Blur", "Tools", &karnelsize, 51, GaussinBlurTrackbar);
		cv::createTrackbar("Canny min", "Tools", &canny_theshold_min, 256, CannyThreshold);
		cv::createTrackbar("Canny max", "Tools", &canny_theshold_max, 256, CannyThreshold);
		cv::createTrackbar("Print", "Tools", &x, 2, printmatrix);

		cv::GaussianBlur(src_grey, dst, cv::Size(abs(2 * karnelsize - 1), abs(2 * karnelsize - 1)),0,0);
		cv::Canny(dst, grany, canny_theshold_min, canny_theshold_max);
		cv::imshow("Result", grany);

		//�������� ������� �������
		if (cv::waitKey(1) >= 0) break;

	}

	return 0;
}