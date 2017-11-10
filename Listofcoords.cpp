#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>
#include <stdio.h> 
#include <stdlib.h>

// ���������� ���������� 
cv::Mat src;// ������� ��� ��������� ����������� 
cv::Mat src_grey;// ������� ��� ����������� � ��������� ������
cv::Mat dst;//������� ����� �������� 
cv::Mat grany;//������� ����� ��������� ������ 
cv::Mat grany_copy;// ����� ������� grany
int karnelsize = 1;// �������� ��� ��������
int canny_theshold_min = 0;// ������ ������� ��� ��������� ����� 
int canny_theshold_max = 0;// ������� ������� ��� ��������� ����� 



struct Node //��������� ���������� ������ ������
{
	int x;//�������� x ����� ������������ � ������
	int y; //�������� y ����� ������������ � ������
	Node *Next, *Prev; //��������� �� ������ ���������� � ����������� ��������� ������
};


class List   //������� ��� ������ ������
{
	Node *Head, *Tail; //��������� �� ������ ������ ������ � ��� �����
public:
	List() :Head(NULL), Tail(NULL) {}; //�������������� ������ ��� ������
	~List(); //����������
	void Show(); //������� ����������� ������ �� ������
	void Add(int x, int y); //������� ���������� ��������� � ������
};


List::~List() //����������
{
	while (Head) //���� �� ������ �� ������ ������ ���-�� ����
	{
		Tail = Head->Next; //��������� ����� ������ ���������� ����� ������
						   // head.next - �������� ���������, � head->next ������� �� ��������� � ���������� �������� 
		delete Head; //������� ������ �� ������� �����
		Head = Tail; //����� ������ ������ �� ����� ���������� ��������
	}
}


void List::Add(int x, int y)
{
	Node *temp = new Node; //��������� ������ ��� ����� ������� ���������
	temp->Next = NULL;  //���������, ��� ���������� �� ���������� ������ �����
	temp->x = x;//���������� �������� � ���������
	temp->y = y;//���������� �������� � ���������

	if (Head != NULL) //���� ������ �� ����
	{
		temp->Prev = Tail; //��������� ����� �� ���������� ������� � �����. ����
		Tail->Next = temp; //��������� ����� ���������� �� ������� ��������
		Tail = temp; //������ ����� ������
	}
	else //���� ������ ������
	{
		temp->Prev = NULL; //���������� ������� ��������� � �������
		Head = Tail = temp; //������=�����=��� �������, ��� ������ ��������
	}
}


void List::Show()
{
	//������� ������
	Node *temp = Head; //�������� ��������� �� ����� ������� ��������
	while (temp != NULL) //���� �� �������� ������ ��������
	{
		std::cout <<"["<< temp->x << " "; //������� ������ ��������� �������� �� �����
		std::cout << temp->y << " "<<"]"; //������� ������ ��������� �������� �� �����
		temp = temp->Next; //����� ������ �� ����� ���������� ��������
	}
	std::cout << std::endl; // ������� �� ����� ������ 
}


void CannyThreshold(int, void*)
{   //�������� ������
	cv::GaussianBlur(src_grey, dst, cv::Size(abs(2 * karnelsize - 1), abs(2 * karnelsize - 1)), 0, 0);

	// �������� ������ ����� 
	cv::Canny(dst, grany, canny_theshold_min, canny_theshold_max);

	//dst1 = cv::Scalar::all(0); //�������� ������� �� ����� 
	//src_grey.copyTo(dst1, grany); //����������� � ��������� ����������� � ������ ������� �� �������� ����� 

	cv::imshow("Result", grany);
}


//������� ��� �������� �������� ������� ������ 
void GaussinBlurTrackbar(int, void*) {

	// �������� ������
	cv::GaussianBlur(src_grey, dst, cv::Size(abs(2 * karnelsize - 1), abs(2 * karnelsize - 1)), 0, 0);

	//�������� ������
	cv::Canny(dst, grany, canny_theshold_min, canny_theshold_max);

	//����� ����������� ����� �������� � ��������� ������
	cv::imshow("Result", grany);

}


//������� ��� ������ ���������� ���� ����������� � ������� � ���� �������� �������
void printmatrix(int, void*) {

	// ������� ��� ���� ������� � ������� �������� ������� ����� ��������� ������
	grany_copy = grany.clone();

	List cords; //��������� ����������, ��� ������� ���� ������

	for (int i = 0; i < grany_copy.rows; ++i)
		for (int j = 0; j <grany_copy.cols; ++j)
			if (grany_copy.at<uchar>(i, j) > 0)
				grany_copy.at<uchar>(i, j) = 255;

	for (int i = 0; i < grany_copy.rows; ++i)
		for (int j = 0; j < grany_copy.cols; ++j)
			if (grany_copy.at<uchar>(i, j) == 255)
				cords.Add(j, i);

	//����� ������� ����� ��������������  �� ����� 
	cv::imshow("Before", grany_copy);

	//����� ������� � �������
	cords.Show();
	cords.~List();
}


int main(int argc, char** argv)
{
	int x=0;//���������� ��� ������� ������ ������� � �������, �� ���� �������� 

	std::cout << "write image name" << std::endl;

	// �������� �����������, ������� � ������� �����������
	char filename[128];//�������� ������ ��� ����� ����� 
	std::cin >> filename; //���������� ����� �����
	src = cv::imread(filename, 1);// �������� � ������� �����������
								  //>0 - �������������, <0 - �������� ���-�� �������, =0 - ������� ������


	if (!src.data)
	{
		return -1;
	}

	//�������� ���� ��� ������
	cv::namedWindow("Before", CV_WINDOW_NORMAL);
	cv::namedWindow("Result", CV_WINDOW_NORMAL);
	cv::namedWindow("Tools", 0);

	// ����� � ���� �� ������� � �������� ������������
	cv::imshow("Before", src);

	//�������������� ���������� � ������� ������
	cv::cvtColor(src, src_grey, CV_BGR2GRAY); // � opencv �� RGB, � BGR

	cv::imshow("Result", src_grey);

	cv::GaussianBlur(src_grey, dst, cv::Size(1,1), 0, 0);
	cv::Canny(dst, grany, 0, 0);

	//�������� ���������
	cv::createTrackbar("Blur", "Tools", &karnelsize, 51, GaussinBlurTrackbar);
	cv::createTrackbar("Canny min", "Tools", &canny_theshold_min, 100, CannyThreshold);
	cv::createTrackbar("Canny max", "Tools", &canny_theshold_max, 300, CannyThreshold);
	cv::createTrackbar("Print", "Tools", &x, 2, printmatrix);

	//�������� ������� �������
	cv::waitKey(0);


	return 0;
}