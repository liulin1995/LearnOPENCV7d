#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include<math.h>

using namespace std;
using namespace cv;

Point center, circumference;
Mat source;
//fucntion which will be called on mouse input.
void drawCircle(int action, int x, int y, int flags, void *userdata)
{
	if (action == EVENT_LBUTTONDOWN)
	{
		center = Point(x, y);
		circle(source, center, 1, Scalar(255, 25, 0), 2, CV_AA);
	}
	else if(action == EVENT_LBUTTONUP)
	{
		circumference = Point(x, y);
		float radius = sqrt(pow(center.x - circumference.x, 2) + pow(center.y - circumference.y, 2));
		circle(source, center, radius, Scalar(255, 255, 0), 2, CV_AA);
		imshow("window", source);
	}
}
/*This program shows how highgui enables us to take mouse inputs.
In this code we use mouse input to draw a circle on an image.
The mouse is dragged from the center to one of the points on the
circumference. ¡®c¡¯ can be pressed to remove the drawn circles. */
int mouseHandlingMain()
{
	source = imread("boy.jpg", 1);
	// Make a dummy image, will be useful to clear the drawing
	Mat dummy = source.clone();
	namedWindow("window");
	// highgui function called when mouse events occur
	//the mousehandle should be ued to a window which show a picture .
	setMouseCallback("window", drawCircle);
	int k = 0;
	// loop until escape character is pressed
	while (k != 27) {
		imshow("window", source);
		putText(source, "choose center,and drag,Press ESC to exit  and c to clear .", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 255), 2);
		//waitKey will return the ascii code of the pressed key.
		//why should &0xFF ,because the ascii code only 8 bit!
		k = waitKey(20)&0xFF;
		if (k == 99) {
			//clear all the action .
			dummy.copyTo(source);
		}
	}

	return EXIT_SUCCESS;
}
int drawOverImage()
{
	Mat image = imread("mark.jpg", 1);

	//draw a line .
	Mat imageLine = image.clone();
	//image,p_begin,p_end,line_color,line_thickness,line_type,CV_AA is anti-aliased lines.
	line(imageLine, Point(322, 179), Point(400, 183), Scalar(0, 255, 0), 1, CV_AA);

	imshow("imageLine", imageLine);
	imwrite("imageLine.jpg", imageLine);

	//draw a circle .
	Mat imageCircle = image.clone();
	circle(imageCircle, Point(350, 200), 150, Scalar(0, 233, 0), 1, CV_AA);
	imshow("imageCircle", imageCircle);
	imwrite("imageCircle.jpg", imageCircle);

	Mat imageEllipse = image.clone();
	ellipse(imageEllipse, Point(360, 200), Size(100, 170), 45, 0, 360, Scalar(255, 0, 0), 1, 8);
	ellipse(imageEllipse, Point(360, 200), Size(100, 170), 135, 0, 360, Scalar(0, 0, 255), 1, 8);
	imshow("ellipse", imageEllipse);
	imwrite("imageEllipse.jpg", imageEllipse);

	//draw a rectangle.
	Mat imageRectangle = image.clone();
	rectangle(imageRectangle, Point(208, 55), Point(450, 355), Scalar(0, 255, 0), 1, 8);
	imshow("rectangle", imageRectangle);
	imwrite("imageRectangle.jpg", imageRectangle);

	//put a text into image .
	Mat imageTex = image.clone();
	putText(imageTex, "Mark Zuckerberg", Point(205, 50),FONT_HERSHEY_SIMPLEX,1, Scalar(0, 255, 0), 1, 8);
	imshow("imageText", imageTex);
	imwrite("imageText.jpg", imageTex);

	waitKey(0);




	return EXIT_SUCCESS;
}
int videoMan()
{
	//create  a VideoCapter object and open the input file.
	//if the input is the web camera ,pass o instead of the video file name.
	VideoCapture cap("chaplin.mp4");
	if (!cap.isOpened()) {
		cout << "Error opening video stream of file" << endl;
		return EXIT_FAILURE;
	}
	while (1) {
		Mat frame;
		//capture frame by frame.
		cap >> frame;
		//if frame is empty,break immediately.
		if (frame.empty())
		{
			break;
		}
		imshow("Frame", frame);
		char c = char(waitKey(25));
		if (c == 27)
			break;
	}
	//when everything is done,release the video capture object.
	cap.release();
	//close all the frames.
	destroyAllWindows();
	return EXIT_SUCCESS;
}
int videoMain()
{
	VideoCapture cap(0);
	if (!cap.isOpened()) {
		cout << "Error opening video stream or file" << endl;
		return EXIT_FAILURE;
	}
	//In this block of code, we obtain the system dependent 
	//frame width and height.
	// Default resolutions of the frame are obtained. 
	// The default resolutions are system dependent.
	int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	//We create the videoWriter object ¡®video¡¯, name the output file as 
	//¡®outcpp.avi¡¯ and define the 4 FOURCC code. We also specify the ¡®fps¡¯ 
	//value and the dimensions of the system dependent frame.
	// Define the codec and create VideoWriter object.The output is stored 
	// in 'outcpp.avi' file.
	VideoWriter video("outcpp.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(frame_width, frame_height));

	while (1) {
		Mat frame;
		cap >> frame;
		if (frame.empty())
		{

			break;
		}
		video.write(frame);
		imshow("frame", frame);
		char c = (char)waitKey(25);
		if (c == 27)
			break;

	}
	cap.release();
	video.release();

	destroyAllWindows();


	return EXIT_SUCCESS;
}

