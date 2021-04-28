#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "filter.h"
#include <iostream>

	// constructor
	Filter::Filter(cv::Mat input_img, int size) {

		input_image = input_img;
		if (size % 2 == 0)
			size++;
		filter_size = size;
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

	//get window size 
	int Filter::getSize() {

		return filter_size;
	}

	//CONSTRUCTORS
	GaussianFilter::GaussianFilter(cv::Mat input_img, int size, double standard_deviation) 
		: Filter(input_img, size) 
	{
		std = standard_deviation;
	}

	MedianFilter::MedianFilter(cv::Mat input_img, int size)
		: Filter(input_img, size)
	{
	}

	BilateralFilter::BilateralFilter(cv::Mat input_img, int size, double sigma_r, double sigma_s)
		: Filter(input_img, size)
	{
		sigma_range = sigma_r;
		sigma_space = sigma_s;
	}

	//DOFILTER() METHOIDS IMPLEMENTATION

	void GaussianFilter::doFilter() {
		cv::GaussianBlur(input_image, result_image, cv::Size(filter_size,filter_size), std, std);
	}

	void MedianFilter::doFilter() {
		if (filter_size == 0) {
			result_image = input_image.clone();
			return;
		}
		if (filter_size <= 1)
			filter_size = 3;
		cv::medianBlur(input_image, result_image, filter_size);
	}

	void BilateralFilter::doFilter() {
		cv::Mat dst;
		cv::Mat src = input_image;
		cv::bilateralFilter(src, dst , filter_size, sigma_range, sigma_space);
		result_image = dst;
	}
	
