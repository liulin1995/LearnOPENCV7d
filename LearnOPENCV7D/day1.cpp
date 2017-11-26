#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;
int MatMain()
{

	Mat A = imread("C:\\Users\\liu\\Documents\\selfFiles\\opencv_road\\day-1-basic-operations\\sample.jpg", IMREAD_COLOR);
	//Mat B(A);
	//Mat C = A;
	//cout << A << endl;
	//cout << B << endl;
	//cout << C << endl;

	Mat B(A, Rect(15, 15, 50, 50));

	/*
	or Mat B = A(Range(2,4),Range(4,6));

	clone the header and data
	B = A.clone();

	clone with a mask .
	A.copyto(B,mask);


	*/
	cout << A.rows << endl;
	cout << A.rows << endl;
	cout << A.channels() << endl;
	/*
	or A.size() return object of type size(cols,rows),but specifying size using height and width,wo have to use Size(width,height).


	*/
	Size sz = A.size();
	cout << sz.height << endl;
	cout << sz.width << endl;

	/*
	create mat using constructor .
	*/
	Mat M(3, 3, CV_8UC3, Scalar(0, 255, 180));
	cout << M << endl;

	/*
	using c++ array .
	*/
	int size[] = { 4,4 };
	// two dimension matrix ,with above size filled with 20.
	Mat L(2, size, CV_8U, Scalar::all(20));
	cout << L << endl;

	/*
	using creat method of the Mat class c
	*/
	Mat C;
	// 
	C.create(4, 4, CV_8UC(2));
	cout << C << endl;;
	//ones ,zeros,identity matrices
	Mat M1 = Mat::ones(3, 3, CV_64F);
	cout << M1 << endl;
	Mat M2 = Mat::zeros(3, 3, CV_64F);
	cout << M2 << endl;
	Mat M3 = Mat::eye(3, 3, CV_64F);
	cout << M3 << endl;

	//initialize small matrices
	Mat M4 = (Mat_<double>(3, 3) << 0, -1, 0, -1, 5, 6, 0, -3, 3);
	cout << M4 << endl;

	system("pause");
	return 0;
}
int basicOperationsMain()
{
	Mat image = imread("C:\\Users\\liu\\Documents\\selfFiles\\opencv_road\\day-1-basic-operations\\sample.jpg", IMREAD_COLOR);
	if (image.empty())
	{
		cout << "could not open or find the image" << endl;
		return EXIT_FAILURE;

	}
	//convert color image to gray
	Mat grayImage;
	cvtColor(image, grayImage, COLOR_BGR2GRAY);

	//save result 
	imwrite("imageGray.jpg", grayImage);

	//create a window for display.
	namedWindow("image", WINDOW_AUTOSIZE);
	namedWindow("gray image", WINDOW_NORMAL);

	//display image.
	imshow("image", image);
	imshow("gray image", grayImage);
	waitKey(0);

	return EXIT_SUCCESS;

}
int basicOperationsMain2()
{
	Mat source, scaleDown, scaleUp;

	source = imread("C:\\Users\\liu\\Documents\\selfFiles\\opencv_road\\day-1-basic-operations\\sample.jpg", 1);
	
	double scaleX = 0.6;
	double scaleY = 0.6;

	//The resize has the following prototype:
	// resize(sourceImage, destinationImage, Size(,), scale factor in x 
	// direction, scale factor in y direction, interpolation method) 
	// We can either specify the Size(,) of the output to determine the height
	// and width of the output or we can add the scaling factors and the Size 
	// will be calculated automatically. 

	cv::resize(source, scaleDown, Size(), scaleX, scaleY, INTER_LINEAR);
	cv::resize(source, scaleUp, Size(), scaleX * 3, scaleY * 3, INTER_LINEAR);

	//crop image 
	Mat crop = source(cv::Range(50, 150), cv::Range(20, 200));

	namedWindow("Original", WINDOW_AUTOSIZE);
	namedWindow("Scaled Down", WINDOW_AUTOSIZE);
	namedWindow("Scaled Up", WINDOW_AUTOSIZE);
	namedWindow("cropped image", WINDOW_AUTOSIZE);

	//show image 
	imshow("Original", source);
	imshow("Scaled Down", scaleDown);
	imshow("Scaled Up", scaleUp);
	imshow("cropped image", crop);

	waitKey(0);

	return EXIT_SUCCESS;


}

int rotation()
{
	Mat source, M, result;
	source = cv::imread("boy.jpg", 1);
	Point2f center(source.cols / 2, source.rows / 2);
	double rotationAngle = -30;
	double scale = 1;

	// The getRotationMatrix2D function takes the following parameters:
	// Center: point about which rotation will occur
	// rotationAngle: angle by which rotation is occurring
	// Scale : an optional scaling factor
	// Getting the matrix which will define the rotation

	M = cv::getRotationMatrix2D(center, rotationAngle, scale);
	cout << M << endl;
	//rotate the source and store in the matrix.
	cv::warpAffine(source, result, M, Size(source.cols, source.rows));

	namedWindow("Original image", WINDOW_AUTOSIZE);
	namedWindow("rotated image", WINDOW_AUTOSIZE);

	imshow("Original image", source);
	imshow("rotated image", result);

	waitKey(0);
	return EXIT_SUCCESS;
}