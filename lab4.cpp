#include <opencv2/highgui.hpp>

using namespace cv;
int main(void)
{
	//adding a comment for git
	Mat img(200, 200, CV_8U);
	for (int i = 0; i < 200; ++i)
		for (int j = 0; j < 200; ++j)
			img.at<unsigned char>(i, j) = std::min(i + j, 255);
	namedWindow("Example 2");
	imshow("Example 2", img);
	waitKey(0);
	return 0;
}
