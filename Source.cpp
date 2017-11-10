#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

cv::Mat src, src_gray;
cv::Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";

/**
* @function CannyThreshold
* @brief Trackbar callback - Canny thresholds input with a ratio 1:3
*/
void CannyThreshold(int, void*)
{
	/// Reduce noise with a kernel 3x3
	GaussianBlur(src_gray, detected_edges, cv::Size(5, 5), 0, 0);

	/// Canny detector
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);

	/// Using Canny's output as a mask, we display our result
	dst = cv::Scalar::all(0);

	src.copyTo(dst, detected_edges);
	imshow(window_name, dst);
}

int main()
{
	/// Load an image
	src = cv::imread("0pUPfjDV_W0.jpg");

	if (!src.data)
	{
		return -1;
	}

	/// Create a matrix of the same type and size as src (for dst)
	dst.create(src.size(), src.type());

	/// Convert the image to grayscale
	cvtColor(src, src_gray, CV_BGR2GRAY);

	//GaussianBlur(src_gray, src_gray, cv::Size(5, 5), 0, 0);

	/// Create a window
	cv::namedWindow(window_name, 0);

	imshow(window_name, src_gray);

	/// Create a Trackbar for user to enter threshold
	cv::createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);

	/// Show the image
	CannyThreshold(0, 0);

	std::cout << src_gray<< std::endl;

	/// Wait until user exit program by pressing a key
	cv::waitKey(0);

	return 0;
}
