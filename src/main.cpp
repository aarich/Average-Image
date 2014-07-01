#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "cvaux.h"
#include "averageImage.h"

#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;
using namespace averageImage;

// Print usage
static void printPrompt( const string& applName )
{
    cout << "/*\n"
         << " * Sandbox for doing LBP and and Casscade Classifier\n"
         << " */\n" << endl;

    cout << endl << "Format:\n" << endl;
    cout << "./" << applName << " [ImageName] [FileWithMoreImages(.txt)] [Divisions]" << endl;
    cout << endl;
}

int main(int argc, char** argv)
{
    if( argc < 3 )
    {
        printPrompt( argv[0] );
        return -1;
    }

    cv::initModule_nonfree();

    Mat image;

    image = imread(argv[1]);

    int divs = atoi(argv[2]);

    cout << "< Computing integral Image ... \n";

    Mat iImage;
    integral(image, iImage, CV_64F);
    
    cout << "> \n< Computing Pixel Sum, after cropping ...\n";

    Rect ROI (1, 1, iImage.cols-1, iImage.rows-1);
    Mat cropped = iImage(ROI);
    Mat pixsum = getPixSum(cropped, divs);

    imwrite("pixsum.jpg", pixsum);

    cout << "> \n< Computing Black and White Image ...\n";

    Mat ab = aboveBelow(pixsum);

    imwrite("aboveBelow.jpg", ab);

    cout << "> \nDone!" << endl;

    return 0;
}