#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

// Generic class implementing a filter with the input and output image data and the parameters
class Filter{

// Methods

public:

	// constructor 
	// input_img: image to be filtered
	// filter_size : size of the kernel/window of the filter
	Filter(cv::Mat input_img, int filter_size);

	// perform filtering (in base class do nothing, to be reimplemented in the derived filters)
	void doFilter();

	// get the output of the filter
	cv::Mat getResult();

	//set the window size (square window of dimensions size x size)
	void setSize(int size);
	
	//get the Window Size
	int getSize();

// Data

protected:

	// input image
	cv::Mat input_image;

	// output image (filter result)
	cv::Mat result_image;

	// window size
	int filter_size;



};

// Gaussian Filter
class GaussianFilter : public Filter  {
public:
	// constructor 
	// input_img: image to be filtered
	// filter_size : size of the kernel/window of the filter
	GaussianFilter(cv::Mat input_img, int filter_size, double std);

	// perform filtering (in base class do nothing, to be reimplemented in the derived filters)
	void doFilter();

// additional parameter: standard deviation (sigma)

	//standard deviation in x and y 
	double std;

};

class MedianFilter : public Filter {
public:
	//MedianBlur()
// place constructor
	MedianFilter(cv::Mat input_img, int filter_size);


// re-implement  doFilter()
	void doFilter();

// no additional parameters

};

class BilateralFilter : public Filter {
public:
// place constructor
	BilateralFilter(cv::Mat input_img, int filter_size, double sigma_range, double sigma_space);
// re-implement  doFilter()
	void doFilter();

// additional parameters: sigma_range, sigma_space
	//sigmas
	double sigma_range;
	double sigma_space;

};