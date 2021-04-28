#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "filter.h"

//using namespace cv;

	// constructor
	Filter::Filter(cv::Mat input_img, int size, cv::String window) {
		input_image = input_img;
		if (size % 2 == 0)
			size++;
		filter_size = size;
		winname = window;
		result_image = input_image.clone();
	}

	// for base class do nothing (in derived classes it performs the corresponding filter)
	void Filter::doFilter() {
		// it just returns a copy of the input image
		result_image = input_image.clone();
	}

	// get output of the filter
	cv::Mat Filter::getResult() {
		return result_image;
	}

	//set window size (it needs to be odd)
	void Filter::setSize(int size) {
		if (size % 2 == 0)
			size++;
		filter_size = size;
	}

	//get window name 
	cv::String Filter::getWinname() {
		return winname;
	}


//constructors

	MedianFilter::MedianFilter(cv::Mat input_img, int size, cv::String window)
		: Filter(input_img, size, window)
	{}

	GaussianFilter::GaussianFilter(cv::Mat input_img, int size, cv::String window, double sigmaXY_input)
		: Filter(input_img, size, window) {
		sigmaXY = sigmaXY_input;
	}

	BilateralFilter::BilateralFilter(cv::Mat input_img, int size, cv::String window, double sigmaColor_input, double sigmaSpace_input)
		: Filter(input_img, size, window) {
		sigmaColor = static_cast<double>(sigmaColor_input);
		sigmaSpace = static_cast<double>(sigmaSpace_input);
	}


//doFilter reimplementations

	void MedianFilter::doFilter() {
		//kernel size must be either 0 (do nothing) or odd and >= 3
		if (filter_size == 0) {
			result_image = input_image.clone();
			return;
		}
		if (filter_size <= 1)
			filter_size = 3;

		/*cv::Mat* dsd = &(input_image);
		cv::Mat* uuu = &(result_image);*/
		/*std::cout << filter_size << "\n";
		std::cout << input_image.size << "\n";
		std::cout << result_image.size << "\n";*/
		
		cv::medianBlur(input_image, result_image, filter_size);
	}
	
	void GaussianFilter::doFilter() {
		cv::Size ksize = cv::Size_<int>(filter_size, filter_size);
		cv::GaussianBlur(input_image, result_image, ksize, sigmaXY);
	}
	
	void BilateralFilter::doFilter(){
		cv::bilateralFilter(input_image, result_image, -1, sigmaColor, sigmaSpace);
	}

//parameter access and update

	void GaussianFilter::setSigmaXY(double sigma){
		sigmaXY = sigma;
	}

	void BilateralFilter::setSigmaColor(double sigma) {
		sigmaColor = sigma;
	}

	void BilateralFilter::setSigmaSpace(double sigma) {
		sigmaSpace = sigma;
	}
