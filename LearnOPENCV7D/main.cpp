#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
int redEyeRemoverMain(int argc, char** argv);
int MatMain();
int basicOperationsMain();
int basicOperationsMain2();
int rotation();
int drawOverImage();
int mouseHandlingMain();
int videoMan();
int affineMain();
int inverseAffineMain();
int main()
{
	inverseAffineMain();
	system("pause");
	return EXIT_SUCCESS;
}