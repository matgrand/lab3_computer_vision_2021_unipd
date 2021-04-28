#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <filter.h>

using namespace cv;
using namespace std;

void size_TrackbarCallback(int, void*);
void sigmaXY_TrackbarCallback(int, void*);
void sigmaColor_TrackbarCallback(int, void*);
void sigmaSpace_TrackbarCallback(int, void*);

int main(void)
{
	cv::String path = "data/image.jpg";
	cv::Mat image = cv::imread(path, IMREAD_COLOR);
	if (image.empty()) { std::cout << "Error loading image \n"; return -1; }

	cv::String gaussian_string = "Gaussian filter";
	cv::String median_string = "Median filter";
	cv::String bilateral_string = "Bilateral filter";

	cv::namedWindow(gaussian_string, WINDOW_NORMAL);
	cv::namedWindow(median_string, WINDOW_NORMAL);
	cv::namedWindow(bilateral_string, WINDOW_NORMAL);
	cv::resizeWindow(gaussian_string, (int) image.cols/2, (int) image.rows/2);
	cv::resizeWindow(median_string, (int)image.cols/2, (int)image.rows/2);
	cv::resizeWindow(bilateral_string, (int)image.cols/2, (int)image.rows/2);
	cv::imshow(gaussian_string, image);
	cv::imshow(median_string, image);
	cv::imshow(bilateral_string, image);
		
	int starting_sigma = 0;
	int starting_filter_size = 0;
	int sigmaXY_max = 20;
	int sigmaColor_max = 20;
	int sigmaSpace_max = 20;
	int filter_size_max = 40;
	
	GaussianFilter gaus_fil = GaussianFilter(image, starting_filter_size, gaussian_string, starting_sigma);
	cv::createTrackbar("kernel size", gaussian_string, 0, filter_size_max, size_TrackbarCallback, static_cast<void*> (&gaus_fil));
	cv::createTrackbar("sigmaXY", gaussian_string, 0, sigmaXY_max, sigmaXY_TrackbarCallback, static_cast<void*> (&gaus_fil));

	MedianFilter med_fil = MedianFilter(image, starting_filter_size, median_string);
	cv::createTrackbar("kernel size", median_string, 0, filter_size_max, size_TrackbarCallback, static_cast<void*>(&med_fil));

	BilateralFilter bil_fil = BilateralFilter(image, starting_filter_size, bilateral_string, starting_sigma, starting_sigma);
	cv::createTrackbar("sigmaColor", bilateral_string, 0, sigmaColor_max, sigmaColor_TrackbarCallback, static_cast<void*> (&bil_fil));
	cv::createTrackbar("sigmaSpace", bilateral_string, 0, sigmaSpace_max, sigmaSpace_TrackbarCallback, static_cast<void*> (&bil_fil));

	cv::waitKey(0);
	return 0;
}


//Callback functions

void size_TrackbarCallback(int position, void* userdata)
{
	Filter* FIL = reinterpret_cast<Filter*>(userdata);
	FIL->setSize(position);
	FIL->doFilter();
	cv::Mat result = FIL->getResult();
	cv::imshow(FIL->getWinname(), result);
}

void sigmaXY_TrackbarCallback(int position, void* userdata)
{
	GaussianFilter* FIL = reinterpret_cast<GaussianFilter*>(userdata);
	FIL->setSigmaXY(static_cast<double>(position));
	FIL->doFilter();
	cv::Mat result = FIL->getResult();
	cv::imshow(FIL->getWinname(), result);
}

void sigmaSpace_TrackbarCallback(int position, void* userdata)
{
	BilateralFilter* FIL = reinterpret_cast<BilateralFilter*>(userdata);
	FIL->setSigmaSpace(static_cast<double>(position));
	FIL->doFilter();
	cv::Mat result = FIL->getResult();
	cv::imshow(FIL->getWinname(), result);
}

void sigmaColor_TrackbarCallback(int position, void* userdata)
{
	BilateralFilter* FIL = reinterpret_cast<BilateralFilter*>(userdata);
	FIL->setSigmaColor(static_cast<double>(position));
	FIL->doFilter();
	cv::Mat result = FIL->getResult();
	cv::imshow(FIL->getWinname(), result);
}
