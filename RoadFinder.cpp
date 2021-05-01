#include "RoadFinder.h"


using namespace cv;
using namespace std;

// constructor
RoadFinder::RoadFinder(Mat input_img, int low_thrsh_canny, const int max_low_thrsh_canny, const int canny_ratio, const int canny_ker,
	int thrsh_hough, double rho_hough, double theta_hough, String window_name)
{
	this->input_img = input_img;

	this->low_thrsh_canny = low_thrsh_canny;
	this->max_low_thrsh_canny = max_low_thrsh_canny;
	this->canny_ratio = canny_ratio;
	this->canny_ker = canny_ker;

	this->thrsh_hough = thrsh_hough;
	this->rho_hough = rho_hough;
	this->theta_hough = theta_hough;

	this->window_name = window_name;

	this->input_gray.create(input_img.size(), input_img.type());
	this->edge_img.create(input_img.size(), input_img.type());
	this->lines_img.create(input_img.size(), input_img.type());
	this->circles_img.create(input_img.size(), input_img.type());
}

//methods
Mat RoadFinder::generate_Canny() {
	//convert to gray level
	cvtColor(input_img, input_gray, COLOR_BGR2GRAY);
	//apply a gaussian kernel (useless, set size and std at 0)
	GaussianBlur(input_gray, input_gray, Size(2*gauss_size+1, 2*gauss_size+1), gauss_std, gauss_std);
	//canny
	Canny(input_gray, edge_img, low_thrsh_canny, low_thrsh_canny * canny_ratio, canny_ker);
	
	//return
	return edge_img.clone();
}

Mat RoadFinder::generate_Hough(bool find_lines, bool find_circles) {

	Mat out = input_img.clone();

	int max_pixels = out.rows * out.cols;
	// Copy edges to the images that will display the results in BGR
	//cvtColor(edge_img, out, COLOR_GRAY2BGR);
	//lines detection
	if (find_lines) {
		HoughLines(edge_img, lines, rho_hough, theta_hough, thrsh_hough);
		///*
		// Draw the lines,
		//I think this section is quiet interesting although it makes the image different from the reference
		//comment this for cycle to get the image without the lines
		for (size_t i = 0; i < lines.size(); i++)
		{
			float rho = lines[i][0], theta = lines[i][1];
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a * rho, y0 = b * rho;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			cv::line(out, pt1, pt2, Scalar(0, 0, 255), 1, LINE_AA);
		}
		//*/

		//draw triangle
		if (lines.size()>1) {
			//note that the 2 strongest lines are always the first 2 
			float rhoA = lines[0][0], thetaA = lines[0][1];
			float rhoB = lines[1][0], thetaB = lines[1][1];
			double aA = cos(thetaA), bA = sin(thetaA);
			double aB = cos(thetaB), bB = sin(thetaB);
			Point ptA, ptB, ptC;
			ptA.y = cvRound(out.rows - 1); //y is on the first row for A and B
			ptB.y = ptA.y;
			ptA.x = cvRound((rhoA - ptA.y * bA) / aA);
			ptB.x = cvRound((rhoB - ptB.y * bB) / aB);

			//find intersection of the 2 lines at point ptC
			ptC.y = cvRound((rhoA/aA - rhoB/aB) / (bA / aA - bB / aB));
			ptC.x = cvRound((rhoB - ptC.y * bB) / aB);

			vector<Point> triangle = { ptA,ptB,ptC };
			fillConvexPoly(out, triangle, Scalar(100, 100, 255), LINE_AA);
		}

	}
	//circles detection
	if (find_circles) {
		HoughCircles(input_gray, circles, HOUGH_GRADIENT, 1.5, 1, 3*low_thrsh_canny, circles_thrsh);
		//draw the circles
		for (size_t i = 0; i < circles.size(); i++)
		{
			Vec3i c = circles[i];
			Point center = Point(c[0], c[1]);
			int radius = c[2];
			circle(out, center, radius, Scalar(255, 0, 255), FILLED, LINE_AA);
		}
	}
	return out;
}




