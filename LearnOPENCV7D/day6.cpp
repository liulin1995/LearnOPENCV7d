#include<iostream>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include"opencv2/objdetect.hpp"
#include<opencv2/ml.hpp>


using namespace cv::ml;
using namespace cv;
using namespace std;

int main() {
	//the param should be noticed!!! 
	// I wasted 10  hours because it!
	HOGDescriptor hogDefault(
		Size(64, 128),
		Size(16, 16),
		Size(8, 8),
		Size(8, 8),
		9, 0, -1, 0,0.2, 1, 64, 0
	);
	
	vector<float> svmDetectorDefault = hogDefault.getDefaultPeopleDetector();
	hogDefault.setSVMDetector(svmDetectorDefault);
	
	float finalHeight = 800.0;

	String imagePath = "race.jpg";
	Mat im = imread(imagePath, IMREAD_COLOR);

	if (im.empty())
	{
		cout << "Open Image error!\n";
		return 1;
	}
	float finalWidth = (finalHeight  *  im.cols) / im.rows;
	
	resize(im, im, Size(finalWidth, finalHeight));




	vector<Rect> bboxes;
	vector<double> weights;

	float hitThresthond = 1.0;
	Size winStride = Size(8, 8);
	Size padding = Size(32, 32);
	float scale = 1.05;
	float finalThresthold = 2;
	bool useMeanshifGrouping = 0;
	
	hogDefault.detectMultiScale(im, bboxes, weights, 0, winStride, padding, scale, finalThresthold, useMeanshifGrouping);
	
	if (!bboxes.empty())
	{
		cout << "Default Detector :: pedestrians detected:" << bboxes.size() << endl;
		vector<Rect>::const_iterator loc = bboxes.begin();
		vector<Rect>::const_iterator end = bboxes.end();
		for (; loc != end; ++loc)
		{
			rectangle(im, *loc, Scalar(0, 255, 0), 2);
		}
	}

	imshow("pedestrians", im);
	imwrite("result_pedestrians.jpg", im);
	waitKey(0);

	return 0;
}