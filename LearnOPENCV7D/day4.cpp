#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include<iostream>
#include<stdio.h>
#include<string>

using namespace std;
using namespace cv;

String faceCascadePath,smileCascadePath;
CascadeClassifier faceCascade,smileCascade;

int  ___main(int argc, const char argv)
{
	int faceNeighborsMax = 100;
	int neighborStep = 2;
	faceCascadePath = "C:/Users/liu/Documents/gitforVS/LearnOPENCV7D/LearnOPENCV7D/models/haarcascade_frontalface_default.xml";
	smileCascadePath = "C:/Users/liu/Documents/gitforVS/LearnOPENCV7D/LearnOPENCV7D/models/haarcascade_smile.xml";

	if (!faceCascade.load(faceCascadePath))
	{
		printf("--(!)Error loading face cascade\n");
		return -1;
	}
	if (!smileCascade.load(smileCascadePath))
	{

		printf("--(!) Error loading smile cascade\n");
		return -1;

	}
	std::vector<Rect> faces;
	
	Mat frame = imread("hillary_clinton.jpg");
	Mat frameGray, frameClone;
	cvtColor(frame, frameGray, COLOR_BGR2GRAY);

	// Perform multi scale detection of faces.
	//-- Dectect faces
	faceCascade.detectMultiScale(frameGray, faces, 1.4, 5);
	for (size_t i = 0; i < faces.size(); i++)
	{ 
		

		int x = faces[i].x;;
		int y = faces[i].y;
		int w = faces[i].width;
		int h = faces[i].height;
		rectangle(frame, Point(x, y), Point(x + w, y + h), Scalar(255, 0, 0), 2, 4);

		Mat faceGrayROI = frameGray(faces[i]);
		

		for (int neigh = 0; neigh < faceNeighborsMax; neigh = neigh + neighborStep)
		{
			
			vector<Rect> smile;
			Mat frameClone = frame.clone();
			Mat faceROIClone = frameClone(faces[i]);

			
			smileCascade.detectMultiScale(faceGrayROI, smile, 1.5, neigh);
			for (size_t j = 0; j < smile.size(); j++)
			{
				int smileX = smile[j].x;
				int smileY = smile[j].y;
				int smileW = smile[j].width;
				int smileH = smile[j].height;

				rectangle(faceROIClone, Point(smileX, smileY), Point(smileX + smileW, smileY + smileH), Scalar(0, 255, 0), 2, 4);
				
			}
			putText(frameClone, format("# Neighbors =%d", neigh), Point(10, 50), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 255), 2);
			imshow("Face Detection and Smile Demo ", frameClone);

			int k = waitKey(500);
			if (k == 27)
			{

				destroyAllWindows();
				break;
			}

	
		}
	}
	system("pause");
	return 0;
}

