#include <iostream>
#include <opencv2/opencv.hpp>
#include "filter.h"

using namespace cv;
using namespace std;

#define GAUSS_STD_FILTER_SCALE_FACTOR 10
#define BILATERAL_SIGMA_RANGE_SCALE_FACTOR 1
#define BILATERAL_SIGMA_SPACE_SCALE_FACTOR 1
#define GAUSS_STRING "GAUSSIAN FILTER"
#define MED_STRING "MEDIAN FILTER"
#define BIL_STRING "BILATER FILTER"

#define MAX_GAUSS_STD 100
#define MAX_GAUSS_SIZE 50
#define MAX_MED_SIZE 50
#define MAX_BIL_SIZE 50
#define MAX_BIL_RANGE 250
#define MAX_BIL_SPACE 250

void showHistogram(std::vector<cv::Mat>& hists);

vector<Mat> calc_BGR_hist(Mat input_image);
Mat convert_and_equalize_channel(Mat img_in_BGR, int channel_index);

//callbacks for trackbars
void onGaussTrackbar1(int pos, void* userdata);
void onGaussTrackbar0(int pos, void* userdata);
void onMedTrackbar(int pos, void* userdata);
static void onBilTrackbar0(int pos, void* userdata);
static void onBilTrackbar1(int pos, void* userdata);
static void onBilTrackbar2(int pos, void* userdata);

    //COMMENTS

//todo:
//redo better the class definition, add the window string as parameter so you can regroup all the callbacks for changing the size param

/* Part 1 and 2 quiet easy, I had understood the important methods split and merge in lab0, 
* as well as the conversion between color spaces, and the importance of HSV, which is evident also in this lab
* 
* The filtering part was much harder, I did a bit of C++ for Arduino in my spare time, but the programs I wrote 
* rarerely needed classes definition with constructors, implementations and "advanced" cpp. I had never seen a 
* sublcass constructor, took me some tries to get the code compile,I guess that for someone who has never seen cpp 
* this section requiring the right edits at the .h file, constructors, subclasses and implementations is extremely difficoult
* to get it right in one step, I think a more gradual step was important.
* 
* Pointers are a pain in the assssssss
* 
*/  






int main(void)
{
	
    /////////////////////////////////////////////////////////////////////////////////////////////////
    // PART 1 : Histogram Equalization

    // 1 load the image

    Mat input_img = imread("data/image.jpg");
    //Mat input_img = imread("data/countryside.jpg");

    resize(input_img, input_img, Size(720, 480));

    // 2 print the histograms
    //show original image
    imshow("ORIGINAL IMAGE", input_img);
    vector<Mat> hists = calc_BGR_hist(input_img);
    showHistogram(hists);

    // 3 equalize te B G R channels
    Mat img_B_out, img_G_out, img_R_out;
    Mat channels_BGR[3];
    Mat img_to_merge[3];
    split(input_img,channels_BGR);

    equalizeHist(channels_BGR[0], img_to_merge[0]);
    equalizeHist(channels_BGR[1], img_to_merge[1]);
    equalizeHist(channels_BGR[2], img_to_merge[2]);

    Mat output_img;
    merge(img_to_merge, 3, output_img);

    // 4 show the equalized image and the histograms
    imshow("Equalized Histograms Image BGR", output_img);
    vector<Mat> eq_hists = calc_BGR_hist(output_img);
    showHistogram(eq_hists);

    // 5 experiments with HSV color space
    Mat input_img_HSV, out_img_HSV, out_img_BGR;
    
    out_img_BGR = convert_and_equalize_channel(input_img, 0); //H
    imshow("H CHANNEL EQUALIZED", out_img_BGR);
    out_img_BGR = convert_and_equalize_channel(input_img, 1); //S
    imshow("S CHANNEL EQUALIZED", out_img_BGR);
    out_img_BGR = convert_and_equalize_channel(input_img, 2); //V
    imshow("V CHANNEL EQUALIZED", out_img_BGR); //V equlized image is much better

    /////////////////////////////////////////////////////////////////////////////////////////////////
    // PART 2 : Image Filtering

    cout << "Press any key to go to the filtering section..." << endl;

    waitKey(0); 
    destroyAllWindows(); //then remove all unnecessary windows for clarity

    //load the image 
    Mat img_in = imread("data/lena.png");

    //one image for each filter
    Mat img_gauss, img_med, img_bil;
    String gauss_string = GAUSS_STRING;
    String med_string = MED_STRING;
    String bil_string = BIL_STRING;
    namedWindow(gauss_string);
    namedWindow(med_string);
    namedWindow(bil_string);
    imshow(GAUSS_STRING, img_in);
    imshow(MED_STRING, img_in);
    imshow(BIL_STRING, img_in);

    //paramters
    int filter_size = 3;
    int gauss_track1 = 1;
    int gauss_track0 = 1;
    int med_track = 3;
    int bil_track0 = 3;
    int bil_track1 = 3;
    int bil_track2 = 3;
    double std_gauss = 5;
    double sigma_range = 5;
    double sigma_space = 5;

    //create filters
    GaussianFilter gauss_filter = GaussianFilter(img_in, filter_size, std_gauss);
    MedianFilter med_filter = MedianFilter(img_in, filter_size);
    BilateralFilter bil_filter = BilateralFilter(img_in, filter_size, sigma_range, sigma_space);

    //create trackbars 
    createTrackbar("STD /10", GAUSS_STRING, &gauss_track1, MAX_GAUSS_STD, onGaussTrackbar1, static_cast<void*>(&gauss_filter));
    createTrackbar("size", GAUSS_STRING, &gauss_track0, MAX_GAUSS_SIZE, onGaussTrackbar0, static_cast<void*>(&gauss_filter));
    createTrackbar("size", MED_STRING, &med_track, MAX_MED_SIZE, onMedTrackbar, static_cast<void*>(&med_filter));
    createTrackbar("size", BIL_STRING, &bil_track0, MAX_BIL_SIZE, onBilTrackbar0, static_cast<void*>(&bil_filter));
    createTrackbar("Range", BIL_STRING, &bil_track0, MAX_BIL_RANGE, onBilTrackbar1, static_cast<void*>(&bil_filter));
    createTrackbar("Space", BIL_STRING, &bil_track0, MAX_BIL_SPACE, onBilTrackbar2, static_cast<void*>(&bil_filter));

	waitKey(0);
	return 0;
}


//callbacks for trackbars
//GAUSS TRACKBAR
//STD 
void onGaussTrackbar1(int pos, void* userdata) {
    GaussianFilter* gauss_filter = static_cast<GaussianFilter*>(userdata);
    //set the new std
    gauss_filter->std = static_cast<double>(pos) / GAUSS_STD_FILTER_SCALE_FACTOR;
    //apply filter 
    gauss_filter->doFilter();
    //show result
    Mat out_img = gauss_filter->getResult();
    imshow(GAUSS_STRING, out_img);
}
//size 
void onGaussTrackbar0(int pos, void* userdata) {
    GaussianFilter* filter = static_cast<GaussianFilter*>(userdata);
    filter->setSize(pos);
    filter->doFilter();
    Mat out_img = filter->getResult();
    imshow(GAUSS_STRING, out_img);
}

//size 
void onMedTrackbar(int pos, void* userdata) {
    MedianFilter* filter = static_cast<MedianFilter*>(userdata);
    filter->setSize(pos);
    filter->doFilter();
    Mat out_img = filter->getResult();
    imshow(MED_STRING, out_img);
}

// BILATERAL
//size
static void onBilTrackbar0(int pos, void* userdata) {
    BilateralFilter* filter = static_cast<BilateralFilter*>(userdata);
    filter->setSize(pos);
    filter->doFilter();
    Mat out_img = filter->getResult();
    imshow(BIL_STRING, out_img);
}
//sigma r
static void onBilTrackbar1(int pos, void* userdata) {
    BilateralFilter* bil_filter = static_cast<BilateralFilter*>(userdata);
    bil_filter->sigma_range = (static_cast<double>(pos) / BILATERAL_SIGMA_RANGE_SCALE_FACTOR);
    bil_filter->doFilter();
    Mat out_img = bil_filter->getResult();
    imshow(BIL_STRING, out_img);
}
//sigma s
static void onBilTrackbar2(int pos, void* userdata) {
    BilateralFilter* bil_filter = static_cast<BilateralFilter*>(userdata);
    bil_filter->sigma_space = (static_cast<double>(pos) / BILATERAL_SIGMA_SPACE_SCALE_FACTOR);
    bil_filter->doFilter();
    Mat out_img = bil_filter->getResult();
    imshow(BIL_STRING, out_img);
}





Mat convert_and_equalize_channel(Mat img_in_BGR, int channel_index) {
    //convert to HSV
    int i = channel_index;
    Mat img_in_HSV, img_out_BGR;
    cvtColor(img_in_BGR, img_in_HSV, COLOR_BGR2HSV);
    Mat channels[3];
    split(img_in_HSV, channels);
    equalizeHist(channels[i], channels[i]);
    merge(channels, 3, img_out_BGR);
    cvtColor(img_out_BGR, img_out_BGR, COLOR_HSV2BGR);
    return img_out_BGR;
}


vector<Mat> calc_BGR_hist(Mat input_image) {
    
    Mat image_splitted_RGB[3];
    split(input_image, image_splitted_RGB);

    Mat img_B = image_splitted_RGB[0];
    Mat img_G = image_splitted_RGB[1];
    Mat img_R = image_splitted_RGB[2];
    Mat hist_B, hist_G, hist_R;

    int channel[] = { 0 };
    int bins_BGR = 256;
    int histSize[]{ bins_BGR };
    float range_BGR[] = { 0, 256 };
    const float* ranges[] = { range_BGR };

    calcHist(&img_B, 1, channel, Mat(), hist_B, 1, histSize, ranges, true, false);
    calcHist(&img_G, 1, channel, Mat(), hist_G, 1, histSize, ranges, true, false);
    calcHist(&img_R, 1, channel, Mat(), hist_R, 1, histSize, ranges, true, false);

    vector<Mat> hists = { hist_B , hist_G, hist_R };

    return hists;
}

// hists = vector of 3 cv::mat of size nbins=256 with the 3 histograms
// e.g.: hists[0] = cv:mat of size 256 with the red histogram
//       hists[1] = cv:mat of size 256 with the green histogram
//       hists[2] = cv:mat of size 256 with the blue histogram
void showHistogram(std::vector<cv::Mat>& hists)
{
    // Min/Max computation
    double hmax[3] = { 0,0,0 };
    double min;
    cv::minMaxLoc(hists[0], &min, &hmax[0]);
    cv::minMaxLoc(hists[1], &min, &hmax[1]);
    cv::minMaxLoc(hists[2], &min, &hmax[2]);

    std::string wname[3] = { "blue", "green", "red" };
    cv::Scalar colors[3] = { cv::Scalar(255,0,0), cv::Scalar(0,255,0),
                             cv::Scalar(0,0,255) };

    std::vector<cv::Mat> canvas(hists.size());

    // Display each histogram in a canvas
    for (int i = 0, end = hists.size(); i < end; i++)
    {
        canvas[i] = cv::Mat::ones(125, hists[0].rows, CV_8UC3);

        for (int j = 0, rows = canvas[i].rows; j < hists[0].rows - 1; j++)
        {
            cv::line(
                canvas[i],
                cv::Point(j, rows),
                cv::Point(j, rows - (hists[i].at<float>(j) * rows / hmax[i])),
                hists.size() == 1 ? cv::Scalar(200, 200, 200) : colors[i],
                1, 8, 0
            );
        }

        cv::imshow(hists.size() == 1 ? "value" : wname[i], canvas[i]);
    }
}