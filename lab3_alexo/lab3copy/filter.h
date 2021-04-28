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
	Filter(cv::Mat input_img, int filter_size, cv::String window);

	// perform filtering (in base class do nothing, to be reimplemented in the derived filters)
	virtual void doFilter();

	// get the output of the filter
	cv::Mat getResult();

	//set the window size (square window of dimensions size x size)
	void setSize(int size);
	
	//get the Window Name
	cv::String getWinname();

// Data

//protected:

	// input image
	cv::Mat input_image;

	// output image (filter result)
	cv::Mat result_image;

	// window size
	int filter_size;

	// window name
	cv::String winname;
};

// Gaussian Filter
class GaussianFilter : public Filter {

public:
	GaussianFilter(cv::Mat input_img, int filter_size, cv::String window, double sigma);
	void doFilter();
	void setSigmaXY(double sigma);

private:
	// additional parameter: standard deviation along X and Y(sigma)
	double sigmaXY;
};

class MedianFilter : public Filter {

public:
	MedianFilter(cv::Mat input_img, int filter_size, cv::String window);
	void doFilter();
};

class BilateralFilter : public Filter {

public:
	BilateralFilter(cv::Mat input_img, int filter_size, cv::String window, double sigmaColor_input, double sigmaSpace_input);
	void doFilter();
	void setSigmaColor(double sigmaColor_input);
	void setSigmaSpace(double sigmaSpace_input);

private:
	// additional parameters: standard deviations
	double sigmaColor;
	double sigmaSpace;
};