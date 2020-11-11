#include "Matcher.h"
#include <opencv2/opencv.hpp>
#include <opencv2\imgproc\types_c.h>
#include <opencv2\highgui\highgui_c.h>

Matcher::Matcher()
{
	cv::Mat a;

}
using namespace cv;
using namespace std;

Mat g_srcImage, g_tempalteImage, g_resultImage;
int g_nMatchMethod;
int g_nMaxTrackbarNum = 5;


void on_matching(int, void*)
{
	Mat srcImage;
	g_srcImage.copyTo(srcImage);
	int resultImage_cols = g_srcImage.cols - g_tempalteImage.cols + 1;
	int resultImage_rows = g_srcImage.rows - g_tempalteImage.rows + 1;
	g_resultImage.create(resultImage_cols, resultImage_rows, CV_32FC1);

	matchTemplate(g_srcImage, g_tempalteImage, g_resultImage, g_nMatchMethod);
	normalize(g_resultImage, g_resultImage, 0, 1, NORM_MINMAX, -1, Mat());
	double minValue, maxValue;
	Point minLocation, maxLocation, matchLocation;
	minMaxLoc(g_resultImage, &minValue, &maxValue, &minLocation, &maxLocation);

	if (g_nMatchMethod == TM_SQDIFF || g_nMatchMethod == CV_TM_SQDIFF_NORMED)
	{
		matchLocation = minLocation;
	}
	else
	{
		matchLocation = maxLocation;
	}

	rectangle(srcImage, matchLocation, Point(matchLocation.x + g_tempalteImage.cols, matchLocation.y + g_tempalteImage.rows), Scalar(0, 0, 255), 2, 8, 0);
	rectangle(g_resultImage, matchLocation, Point(matchLocation.x + g_tempalteImage.cols, matchLocation.y + g_tempalteImage.rows), Scalar(0, 0, 255), 2, 8, 0);

	imshow("原始图", srcImage);
	imshow("效果图", g_resultImage);

}

int main()
{
	g_srcImage = imread("curry.jpg");
	if (!g_srcImage.data)
	{
		cout << "原始图读取失败" << endl;
		return -1;
	}
	g_tempalteImage = imread("curry2.jpg");
	if (!g_tempalteImage.data)
	{
		cout << "模板图读取失败" << endl;
		return -1;
	}

	imshow("g_srcImage", g_srcImage);
	imshow("g_tempalteImage", g_tempalteImage);

	namedWindow("原始图", CV_WINDOW_AUTOSIZE);
	namedWindow("效果图", CV_WINDOW_AUTOSIZE);
	createTrackbar("方法", "原始图", &g_nMatchMethod, g_nMaxTrackbarNum, on_matching);

	on_matching(0, NULL);


	waitKey(0);

	return 0;
}
