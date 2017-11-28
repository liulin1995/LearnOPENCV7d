/*
	A very happy day! Digit classification use HOG and SVM!
	
*/

#include<iostream>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/ml.hpp>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace cv::ml;
using namespace std;


string digitPath = "digits.png";
int SZ = 20;
float affineFlags = WARP_INVERSE_MAP | INTER_LINEAR;

//remove slant 
Mat deskew(Mat& img)
{
	Moments m = moments(img);
	if (abs(m.mu02) < 1e-2)
	{
		return img.clone();
	}
	float skew = m.mu11 / m.mu02;
	Mat warpMat = (Mat_<float>(2, 3) << 1, skew, -0.5*SZ*skew, 0, 1, 0);
	Mat imgOut = Mat::zeros(img.rows, img.cols, img.type());
	warpAffine(img, imgOut, warpMat, imgOut.size(), affineFlags);

	return imgOut;
}

void loadTrainTestLabel(string &pathName, vector<Mat> &trainCells, vector<Mat> &testCells, vector<int> &trainLabels, vector<int> &testLabels)
{
	Mat img = imread(pathName, CV_LOAD_IMAGE_GRAYSCALE);
	int ImgCount = 0;
	for(int i=0;i<img.rows;i=i+SZ)
		for (int j = 0; j < img.cols; j = j + SZ)
		{
			Mat digitImg = (img.colRange(j, j + SZ).rowRange(i, i + SZ)).clone();
			if (j<int(0.9*img.cols))
			{
				trainCells.push_back(digitImg);
			}
			else {

				testCells.push_back(digitImg);
			}
			ImgCount++;
		}
	cout << "Image Count :" << ImgCount << endl;
	float digitClassNumber = 0;
	for (int z = 0; z<int(0.9*ImgCount); z++)
	{
		if (z % 450 == 0 && z != 0)
		{
			digitClassNumber++;
		}
		trainLabels.push_back(digitClassNumber);
	}
	digitClassNumber = 0;
	for (int z = 0; z<int(0.1*ImgCount); z++)
	{
		if (z % 50 == 0 && z != 0)
		{
			digitClassNumber++;
		}
		testLabels.push_back(digitClassNumber);
	}


}

void createDeskewedTrainTest(vector<Mat> &deskewedTrainCells, vector<Mat> &deskewedTestCells, vector<Mat> &trainCells, vector<Mat> &testCells)
{
	for (int i = 0; i < trainCells.size(); i++)
	{
		Mat deskewedImg = deskew(trainCells[i]);
		deskewedTrainCells.push_back(deskewedImg);
	}
	for (int j = 0; j < testCells.size(); j++)
	{
		Mat deskewedImg = deskew(testCells[j]);
		deskewedTestCells.push_back(deskewedImg);
	}

}
/*
	NOTE:THERE IS CONSTRAIN :
	1.		(winSize - blockSize) % blockStride == 0.
	2.		It only accepts n that is a power of two.
*/
HOGDescriptor hog(
	Size(20, 20),  //winSize
	Size(8, 8), //blockSize
	Size(4,4),//blockStride
	Size(8,8),//cellSize
	9,//nbins
	1,//derivAer
	-1,//winSigma
	0,//higtograNormtype
	64,//nLevel=64
	1);

void createTrianTestHOG(vector<vector<float>> &trainHOG, vector<vector<float>> &testHOG, vector<Mat> &deskewedtrainCells, vector <Mat> &deskewedtestCells)
{
	for (int i = 0; i < deskewedtrainCells.size(); i++)
	{
		vector<float> descriptors;
		hog.compute(deskewedtrainCells[i], descriptors);
		trainHOG.push_back(descriptors);
	}
	for (int j = 0; j < deskewedtestCells.size(); j++)
	{
		vector<float> descriptors;
		hog.compute(deskewedtestCells[j], descriptors);
		testHOG.push_back(descriptors);
	}

}
void convertVectortoMatrix(vector<vector<float>> &trainHOG, vector<vector<float>> &testHOG, Mat &trainMat, Mat &testMat)
{
	int descriptorSize = trainHOG[0].size();
	for(int i=0;i<trainHOG.size();i++)
	{
		for (int j = 0; j < trainHOG[i].size(); j++)
		{
			trainMat.at<float>(i, j) = trainHOG[i][j];
		}
	}
	for(int i=0;i<testHOG.size();i++)
	{ 
		for (int j = 0; j < testHOG[i].size(); j++)
		{

			testMat.at<float>(i, j) = testHOG[i][j];
		}
	}
}

void getSVMParams(SVM *svm)
{
	cout << "Kernel type     :" << svm->getKernelType() << endl;
	cout << "Type            :" << svm->getType() << endl;
	cout << "C               :" << svm->getC() << endl;
	cout << "Degree          :" << svm->getDegree() << endl;
	cout << "Nu              :" << svm->getNu() << endl;
	cout << "Gamma           :" << svm->getGamma() << endl;

}
Ptr<SVM> svmInit(float C, float gamma)
{
	Ptr<SVM> svm = SVM::create();
	svm->setGamma(gamma);
	svm->setC(C);
	svm->setKernel(SVM::RBF);
	svm->setType(SVM::C_SVC);

	return svm;
}

void svmTrain(Ptr<SVM> svm, Mat &trainMat, vector<int> &trainLabels)
{
	Ptr<TrainData> td = TrainData::create(trainMat, ml::ROW_SAMPLE, trainLabels);
	svm->train(td);
	svm->save("digitClassifierModel.xml");

}
void svmPredict(Ptr<SVM> svm, Mat &testResponse, Mat &testMat)
{
	svm->predict(testMat, testResponse);
}
void SVMevaluate(Mat &testResponse, float &count, float &accuracy, vector<int> &testLabels, vector<int> &wrongDigit,vector<float> &wrongAns)
{
	for (int i = 0; i < testResponse.rows; i++)
	{
		if (testResponse.at<float>(i, 0) == testLabels[i])
		{
			count++;
		}
		else
		{
			wrongDigit.push_back(i);
			wrongAns.push_back(testResponse.at<float>(i, 0));
		}
	}
	accuracy = (count / testResponse.rows) * 100;
}
void showWrongAns(vector<Mat> &testCells, vector<int> &testLabels, vector<int> &wrongDigit, vector<float> &wrongAns)
{

	int digitNumber = wrongDigit.size();
	float ratio = testCells[0].rows / testCells[0].cols;
	int digitW = 100;
	int digitH = digitW*ratio;
	
	Mat newDigit(digitH, digitW * digitNumber, CV_32FC1,Scalar(0));
	for (int i = 0; i < digitNumber; i++)
	{
		int wrongNumber = wrongDigit[i];
		string wrong = to_string(wrongAns[i]);
		string right = to_string(testLabels[wrongNumber]);
		Mat img;
		resize(testCells[wrongDigit[i]].clone(),img,Size( digitH, digitW));
		
		putText(img, wrong, Point(82, 82), FONT_HERSHEY_SCRIPT_SIMPLEX, 1, Scalar(255, 255, 255), 1);
		putText(img, right, Point(82, 22), FONT_HERSHEY_SCRIPT_SIMPLEX, 1, Scalar(255, 255, 255), 1);
		
		img.copyTo(newDigit(Range(0, digitW), Range(i * digitW, (i + 1) * digitW )));
	}
	imshow("All Wrong Digits", newDigit);
	waitKey(0);
}
int SVMmain()
{
	vector<Mat> trainCells;
	vector<Mat> testCells;
	vector<int> trainLabels;
	vector<int> testLabels;
	loadTrainTestLabel(digitPath, trainCells, testCells, trainLabels, testLabels);

	vector<Mat> deskewedTrainCells;
	vector<Mat> deskewedTestCells;
	createDeskewedTrainTest(deskewedTrainCells, deskewedTestCells, trainCells, testCells);

	vector<vector<float>> trainHOG;
	vector<vector<float>> testHOG;
	createTrianTestHOG(trainHOG, testHOG, deskewedTrainCells, deskewedTestCells);

	int descriptor_size = trainHOG[0].size();
	cout << "descriptor size  :" << descriptor_size << endl;

	Mat trainMat(trainHOG.size(), descriptor_size, CV_32FC1);
	Mat testMat(testHOG.size(), descriptor_size, CV_32FC1);

	convertVectortoMatrix(trainHOG, testHOG, trainMat, testMat);

	float C = 15.5, gamma = 0.3;

	Mat testResponse;
	Ptr<SVM> model = svmInit(C, gamma);
	//training
	svmTrain(model, trainMat, trainLabels);
	//testing 
	svmPredict(model, testResponse, testMat);

	//accuracy 
	float count = 0;
	float accuracy = 0;
	vector<int> wrongDigit;
	vector<float> wrongAns;
	getSVMParams(model);
	SVMevaluate(testResponse, count, accuracy, testLabels,wrongDigit,wrongAns);
	
	cout << "the accuracy is :" << accuracy << endl;

	showWrongAns(testCells, testLabels, wrongDigit, wrongAns);

	return 0;


}