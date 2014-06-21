#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "cvaux.h"

#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

static void printPrompt( const string& applName )
{
    cout << "/*\n"
         << " * Sandbox for doing LBP and and Casscade Classifier\n"
         << " */\n" << endl;

    cout << endl << "Format:\n" << endl;
    cout << "./" << applName << " [detectorType] [descriptorType] [matcherType] [queryImage] [fileWithTrainImages] [dirToSaveResImages]" << endl;
    cout << endl;
}
  
#define T double

T getMagnitude(const int leftx, const int uppery, const int rightx, const int lowery, const Mat& img)
{
    T one = img.at<T>(uppery, leftx);//(leftx, uppery);
    T two = img.at<T>(uppery, rightx);//(rightx, uppery);
    T three = img.at<T>(lowery, leftx);//(leftx, lowery);
    T four = img.at<T>(lowery,rightx);//(rightx, lowery);
    // 1 -- 2
    // |    |
    // 3 -- 4
    //cout << one << "\t" << two << "\t" << three << "\t" << four;

    return four + one - two - three;
}

Mat getLBP(const Mat& image, const int divisions)
{
    Mat results(divisions, divisions, CV_64F);

    cout << "Rows :" << image.rows << endl;
    cout << "Cols :" << image.cols << endl;

    int h_division = (int)image.rows/divisions;
    int w_division = (int)image.cols/divisions;

    int uppery, lowery, leftx, rightx;
    double mag;

    for (int r = 0; r < divisions; r++)
    {
        uppery = r*h_division;
        lowery = uppery + h_division;
        for (int c = 0; c < divisions; c++)
        {
            leftx = c*w_division;
            rightx = leftx + w_division - 1;
            // cout << "r: " << r << "\t" << "c: " << c << "\t" << leftx << "\t" << rightx << "\t" << uppery << "\t" << lowery << "\n";// << endl;

            mag = getMagnitude(leftx, uppery, rightx, lowery, image);
            // cout << "\tmag: " << mag << endl;

            results.at<double>(r, c) = mag;
        }
    }

    normalize(results, results, 0, 255, NORM_MINMAX, CV_64F);
    
    Mat results2(results.size(), CV_32S);

    for (int r = 0; r < results.rows; r++)
    {
        for (int c = 0; c < results.cols; c++)
        {
            results2.at<int>(r,c) = (int)results.at<double>(r,c);
        }
    }

    return results2;
}


int main(int argc, char** argv)
{
    if( argc != 2 && argc != 1 )
    {
        printPrompt( argv[0] );
        return -1;
    }

    cv::initModule_nonfree();

    string ImageName;

    if( argc != 1 )
    {
        ImageName = argv[1];
    }

    Mat Image;
    Image = imread(ImageName, CV_LOAD_IMAGE_GRAYSCALE);

  //  normalize(Image, Image, 1, 10, NORM_MINMAX);


    Mat iImage;
    integral(Image, iImage, CV_64F);

    Mat niImage(iImage.size(), CV_64F);
    //normalize(iImage, niImage, 1, 50, NORM_MINMAX, CV_32S);
    cv::pow(iImage, 0.5, niImage);
 //   cv::log(niImage, niImage);

    iImage.row(0) = Image.row(0);
    iImage.col(0) = Image.col(0);

    imwrite("integralimage.jpg", niImage);

    cout << Image.row(0);
//    waitKey();

    //cout << getMagnitude(100, 100, 1000, 1000, iImage) << endl;
    cout << "LBP for image:" << endl;
    Mat lbp = getLBP(niImage, 40);

    //normalize(lbp, lbp, 1, 4000, NORM_MINMAX, CV_32S);

    cout << lbp << endl;

    #define WN2 "LBP Image"
  //  namedWindow(WN2, WINDOW_NORMAL);
  //  imshow(WN2, lbp);
    

    imwrite("LBP.jpg", lbp);
    waitKey();
    return 0;
}
