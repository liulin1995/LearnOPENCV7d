#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include<math.h>

using namespace std;
using namespace cv;

int affineMain()
{

	Mat source = imread("boy.jpg", 1);
	//CREATE  TWO WARP MATRICES FOR DEFFERNET TRANSFORMATIONS.
	//first two column control the rotation ,scale ,shear 
	//the third colum control the translation (shift).
	//	[xt]=[a b][x] +[tx]
	//	[yt] [c d][y]  [ty].
	//
	Mat warpMat = (Mat_<double>(2, 3) << 1.2, 0.2, 2, -0.3, 1.3, 1);
	Mat warpMat2 = (Mat_<double>(2, 3) << 1.2, 0.3, 2, 0.2, 1.3, 1);
	
	Mat result, result2;
	//USE WARP AFFINE	
	// BORDER_REFLECT_101 makes the border reflect the part of the image adjacent to it and hence you get a slightly modified image
	cv::warpAffine(source, result, warpMat, Size(1.5*source.rows, 1.4*source.cols), INTER_LINEAR, BORDER_REFLECT_101);

	cv::warpAffine(source, result2, warpMat2, Size(1.5*source.rows, 1.4*source.cols), INTER_LINEAR, BORDER_REFLECT_101);

	imshow("Original", source);
	imshow("Result", result);
	imshow("Result2",result2);

	waitKey(0);
	destroyAllWindows();




	return EXIT_SUCCESS;
}
int inverseAffineMain()
{
	//the points shoule be collinear.
	vector<Point2f> tri1;
	tri1.push_back(Point2f(50, 50));
	tri1.push_back(Point2f(180, 140));
	tri1.push_back(Point2f(150, 200));

	vector<Point2f> tri2;
	tri2.push_back(Point2f(72, 51));
	tri2.push_back(Point2f(246, 129));
	tri2.push_back(Point2f(222, 216));

	vector<Point2f> tri3;
	tri3.push_back(Point2f(77, 76));
	tri3.push_back(Point2f(260, 219));
	tri3.push_back(Point2f(242, 291));
	//The getAffineTransform function requires the two sets of points, to calculate the transformation matrix.
	Mat warp = cv::getAffineTransform(tri1, tri2);
	Mat warp2 = cv::getAffineTransform(tri1, tri3);

	cout << "Warp Matrix 1 \n\n" << warp << "\n\n" << "Warp Matrix 2:\n" << warp2 << endl;



	return EXIT_SUCCESS;
}
// pts_src and pts_dst are vectors of points in source 
// and destination images. They are of type vector<Point2f>. 
// We need at least 4 corresponding points. 

// Mat h = findHomography(pts_src, pts_dst);

// The calculated homography can be used to warp 
// the source image to destination. im_src and im_dst are
// of type Mat. Size is the size (width,height) of im_dst. 
// warpPerspective(im_src, im_dst, h, size);
int momography()
{
	Mat img_src = imread("book1.jpg");

	vector<Point2f> pts_src;
	pts_src.push_back(Point2f(318, 256));
	pts_src.push_back(Point2f(534, 372));
	pts_src.push_back(Point2f(316, 670));
	pts_src.push_back(Point2f(73, 437));

	Mat img_dst = imread("book2.jpg");

	vector<Point2f> pts_dst;
	pts_dst.push_back(Point2f(141, 131));
	pts_dst.push_back(Point2f(480, 159));
	pts_dst.push_back(Point2f(493, 630));
	pts_dst.push_back(Point2f(64, 601));

	//calculate the homograghy.
	Mat h = findHomography(pts_src, pts_dst);

	Mat img_out;
	// Warp source image to destination based on homography
	// we can see that both books look in same positon and posture.
	warpPerspective(img_src, img_out, h, img_dst.size());

	imshow("Source Image", img_src);
	imshow("Destination Image", img_dst);
	imshow("Warpped Source Image", img_out);

	waitKey(0);
	return EXIT_SUCCESS;
}