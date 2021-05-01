#include <opencv2/opencv.hpp>
#include "RoadFinder.h"

/*
comments:
-I manage the imgshow outside of the class to have more control
-some variables inn the class are probably useless, but I added them anyway

-I used the same slider for the canny to tune the hough circle threshold (3* is the value that make things work)

*/


using namespace cv;

//parameters
//canny
const int max_low_thrsh_canny = 1000;
const int canny_ratio = 3;
const int canny_ker = 3;

const bool find_lines = true;
const bool find_circles = true;

const int max_thrsh_hough = 1000;


String window_name = "Road Finder";
String canny_string = "Canny";
String hough_string = "Hough";

void onCannyTrackbarThrsh(int pos, void* userdata);
void onCannyTrackbarSize(int pos, void* userdata);
void onCannyTrackbarStd(int pos, void* userdata);
void onHoughTrackbarThrsh(int pos, void* userdata);
void onHoughTrackbarRho(int pos, void* userdata);
void onHoughTrackbarTheta(int pos, void* userdata);
void onHoughTrackbarCircle(int pos, void* userdata);



int main(void)
{
	
	//load the image input.png
	Mat input_img = imread("input.png");
	
	namedWindow(canny_string, WINDOW_NORMAL);
	namedWindow(hough_string, WINDOW_NORMAL);

	//variable params
	int low_thrsh_canny = 280;
	int thrsh_hough = 100;
	int rho_hough = 1;
	int theta_hough = 5;
	int gauss_size = 0;
	int gauss_std = 0;
	int circles_thrsh = 40;
	
	RoadFinder road_finder = RoadFinder(input_img, low_thrsh_canny, max_low_thrsh_canny, canny_ratio, canny_ker, 
		thrsh_hough, rho_hough, theta_hough*(CV_PI/180), window_name);

	createTrackbar("Canny threshold", canny_string, &low_thrsh_canny, max_low_thrsh_canny, onCannyTrackbarThrsh,
		static_cast<void*>(&road_finder));
	createTrackbar("gauss_size", canny_string, &gauss_size, 100, onCannyTrackbarSize,
		static_cast<void*>(&road_finder));
	createTrackbar("gauss_std", canny_string, &gauss_std, 150, onCannyTrackbarStd,
		static_cast<void*>(&road_finder));

	createTrackbar("Hough threshold", hough_string, &thrsh_hough, max_thrsh_hough, onHoughTrackbarThrsh,
		static_cast<void*>(&road_finder));
	createTrackbar("rho_hough", hough_string, &rho_hough, 20, onHoughTrackbarRho,
		static_cast<void*>(&road_finder));
	createTrackbar("theta_hough", hough_string, &theta_hough, 100, onHoughTrackbarTheta,
		static_cast<void*>(&road_finder));
	createTrackbar("circles_thrsh", hough_string, &circles_thrsh, 100, onHoughTrackbarCircle,
		static_cast<void*>(&road_finder));


	road_finder.gauss_size = gauss_size;
	road_finder.gauss_std = gauss_std / 10;
	road_finder.circles_thrsh = circles_thrsh;

	imshow(canny_string, road_finder.generate_Canny());
	imshow(hough_string, road_finder.generate_Hough(find_lines, find_circles));

	waitKey(0);
	return 0;
}


void onCannyTrackbarThrsh(int thrsh, void* userdata) {
	RoadFinder* road_finder = static_cast<RoadFinder*>(userdata);
	road_finder->low_thrsh_canny = thrsh;
	Mat out = road_finder->generate_Canny();

	imshow(canny_string, out);
}

void onCannyTrackbarSize(int gauss_size, void* userdata) {
	RoadFinder* road_finder = static_cast<RoadFinder*>(userdata);
	cout << road_finder->gauss_size << endl;
	road_finder->gauss_size = gauss_size;
	cout << road_finder->gauss_size << endl;

	Mat out = road_finder->generate_Canny();

	imshow(canny_string, out);
}

void onCannyTrackbarStd(int gauss_std, void* userdata) {
	RoadFinder* road_finder = static_cast<RoadFinder*>(userdata);
	road_finder->gauss_std = gauss_std;
	Mat out = road_finder->generate_Canny();

	imshow(canny_string, out);
}

void onHoughTrackbarThrsh(int thrsh, void* userdata) {
	RoadFinder* road_finder = static_cast<RoadFinder*>(userdata);
	road_finder->thrsh_hough = thrsh;
	Mat out = road_finder->generate_Hough(true, false);
	imshow(hough_string, out);
}

void onHoughTrackbarRho(int rho, void* userdata) {
	RoadFinder* road_finder = static_cast<RoadFinder*>(userdata);
	if (rho < 1) return;
	road_finder->rho_hough = rho;
	Mat out = road_finder->generate_Hough(true, false);
	imshow(hough_string, out);
}

void onHoughTrackbarTheta(int theta, void* userdata) {
	RoadFinder* road_finder = static_cast<RoadFinder*>(userdata);
	if (theta < 1) return;
	road_finder->theta_hough = theta*(CV_PI/180);
	Mat out = road_finder->generate_Hough(true, false);
	imshow(hough_string, out);
}

void onHoughTrackbarCircle(int circles_thrsh, void* userdata) {
	RoadFinder* road_finder = static_cast<RoadFinder*>(userdata);
	if (circles_thrsh < 1) return;
	road_finder->circles_thrsh = circles_thrsh;
	Mat out = road_finder->generate_Hough(false, true);
	imshow(hough_string, out);
}



