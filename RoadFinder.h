#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class RoadFinder {
public:

	// constructor
	RoadFinder(Mat input_img, int low_thrsh_canny, const int max_low_thrsh_canny, const int canny_ratio, const int canny_ker, 
		int thrsh_hough, double rho_hough, double theta_hough, String window_name);
	
	//methods
	Mat generate_Canny();
	Mat generate_Hough(bool find_lines, bool find_circles);

	//variables

	Mat input_img;
	String window_name;
	
	//canny
	int low_thrsh_canny;
	int max_low_thrsh_canny;
	int canny_ratio;
	int canny_ker;

	int gauss_size;
	double gauss_std;

	Mat input_gray;
	Mat edge_img;

	//hough
	int thrsh_hough;
	double rho_hough;
	double theta_hough;
	int circles_thrsh;

	Mat lines_img;
	Mat circles_img;
	vector<Vec3f> lines;
	vector<Vec3f> circles;

};


