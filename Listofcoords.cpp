#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>
#include <stdio.h> 
#include <stdlib.h>

// глобальные переменные 
cv::Mat src;// матрица для исходного изображения 
cv::Mat src_grey;// матрица для изображения в градациях серого
cv::Mat dst;//матрица после размытия 
cv::Mat grany;//матрица после детектора границ 
cv::Mat grany_copy;// копия матрицы grany
int karnelsize = 1;// параметр для размытия
int canny_theshold_min = 0;// нижняя граница для детектора канни 
int canny_theshold_max = 0;// верхняя граница для детектора канни 



struct Node //Структура являющаяся звеном списка
{
	int x;//Значение x будет передаваться в список
	int y; //Значение y будет передаваться в список
	Node *Next, *Prev; //Указатели на адреса следующего и предыдущего элементов списка
};


class List   //Создаем тип данных Список
{
	Node *Head, *Tail; //Указатели на адреса начала списка и его конца
public:
	List() :Head(NULL), Tail(NULL) {}; //Инициализируем адреса как пустые
	~List(); //Деструктор
	void Show(); //Функция отображения списка на экране
	void Add(int x, int y); //Функция добавления элементов в список
};


List::~List() //Деструктор
{
	while (Head) //Пока по адресу на начало списка что-то есть
	{
		Tail = Head->Next; //Резервная копия адреса следующего звена списка
						   // head.next - значение указателя, а head->next переход по указателю к следующему элементу 
		delete Head; //Очистка памяти от первого звена
		Head = Tail; //Смена адреса начала на адрес следующего элемента
	}
}


void List::Add(int x, int y)
{
	Node *temp = new Node; //Выделение памяти под новый элемент структуры
	temp->Next = NULL;  //Указываем, что изначально по следующему адресу пусто
	temp->x = x;//Записываем значение в структуру
	temp->y = y;//Записываем значение в структуру

	if (Head != NULL) //Если список не пуст
	{
		temp->Prev = Tail; //Указываем адрес на предыдущий элемент в соотв. поле
		Tail->Next = temp; //Указываем адрес следующего за хвостом элемента
		Tail = temp; //Меняем адрес хвоста
	}
	else //Если список пустой
	{
		temp->Prev = NULL; //Предыдущий элемент указывает в пустоту
		Head = Tail = temp; //Голова=Хвост=тот элемент, что сейчас добавили
	}
}


void List::Show()
{
	//ВЫВОДИМ СПИСОК
	Node *temp = Head; //Временно указываем на адрес первого элемента
	while (temp != NULL) //Пока не встретим пустое значение
	{
		std::cout <<"["<< temp->x << " "; //Выводим каждое считанное значение на экран
		std::cout << temp->y << " "<<"]"; //Выводим каждое считанное значение на экран
		temp = temp->Next; //Смена адреса на адрес следующего элемента
	}
	std::cout << std::endl; // Переход на новую строку 
}


void CannyThreshold(int, void*)
{   //размытие гаусса
	cv::GaussianBlur(src_grey, dst, cv::Size(abs(2 * karnelsize - 1), abs(2 * karnelsize - 1)), 0, 0);

	// Детектор границ канни 
	cv::Canny(dst, grany, canny_theshold_min, canny_theshold_max);

	//dst1 = cv::Scalar::all(0); //Создание массива из нулей 
	//src_grey.copyTo(dst1, grany); //копирование с исходного изображения в пустую матрицу по границам канни 

	cv::imshow("Result", grany);
}


//функция для ползунка размытия методом Гаусса 
void GaussinBlurTrackbar(int, void*) {

	// размытие Гаусса
	cv::GaussianBlur(src_grey, dst, cv::Size(abs(2 * karnelsize - 1), abs(2 * karnelsize - 1)), 0, 0);

	//детектор границ
	cv::Canny(dst, grany, canny_theshold_min, canny_theshold_max);

	//вывод изображения после размытия и детектора границ
	cv::imshow("Result", grany);

}


//функция для вывода созданного нами изображения в консоль в виде бинарной матрицы
void printmatrix(int, void*) {

	// создаем ещё одну матрицу в которую копируем матрицу после детектора границ
	grany_copy = grany.clone();

	List cords; //Объявляем переменную, тип которой есть список

	for (int i = 0; i < grany_copy.rows; ++i)
		for (int j = 0; j <grany_copy.cols; ++j)
			if (grany_copy.at<uchar>(i, j) > 0)
				grany_copy.at<uchar>(i, j) = 255;

	for (int i = 0; i < grany_copy.rows; ++i)
		for (int j = 0; j < grany_copy.cols; ++j)
			if (grany_copy.at<uchar>(i, j) == 255)
				cords.Add(j, i);

	//вывод матрицы после преобразования  на экран 
	cv::imshow("Before", grany_copy);

	//вывод матрицы в консоль
	cords.Show();
	cords.~List();
}


int main(int argc, char** argv)
{
	int x=0;//переменная для функции вывода матрицы в консоль, не несёт ценности 

	std::cout << "write image name" << std::endl;

	// Загрузка изображение, перенос в матрицу изображения
	char filename[128];//создание строки для имени файла 
	std::cin >> filename; //считывание имени файла
	src = cv::imread(filename, 1);// загрузка в матрицу изображения
								  //>0 - трехканальное, <0 - исходное кол-во каналов, =0 - оттенки серого


	if (!src.data)
	{
		return -1;
	}

	//Создание окон для вывода
	cv::namedWindow("Before", CV_WINDOW_NORMAL);
	cv::namedWindow("Result", CV_WINDOW_NORMAL);
	cv::namedWindow("Tools", 0);

	// Вывод в окне ДО матрицы с исходным изображением
	cv::imshow("Before", src);

	//преобразование избражения в оттенки серого
	cv::cvtColor(src, src_grey, CV_BGR2GRAY); // в opencv не RGB, а BGR

	cv::imshow("Result", src_grey);

	cv::GaussianBlur(src_grey, dst, cv::Size(1,1), 0, 0);
	cv::Canny(dst, grany, 0, 0);

	//создание ползунков
	cv::createTrackbar("Blur", "Tools", &karnelsize, 51, GaussinBlurTrackbar);
	cv::createTrackbar("Canny min", "Tools", &canny_theshold_min, 100, CannyThreshold);
	cv::createTrackbar("Canny max", "Tools", &canny_theshold_max, 300, CannyThreshold);
	cv::createTrackbar("Print", "Tools", &x, 2, printmatrix);

	//ожидание нажатия клавиши
	cv::waitKey(0);


	return 0;
}