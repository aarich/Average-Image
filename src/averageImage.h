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

namespace averageImage {

    // Get the sum of the pixel values within a square of an image
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
    cout << "Values: " << one << "\t" << two << "\t" << three << "\t" << four << endl;
    // cout << img.at<T>(uppery, leftx);
    return four + one - two - three;
    }

    // Get the pixel sum image of a picture.
    Mat getPixSum(const Mat& image, const int divisions)
    {
        cout << image.size() << endl;
        Mat results(divisions, divisions, CV_64F);

        int h_division = (float)(image.rows-1)/ (float)divisions;
        int w_division = (float)(image.cols-1)/ (float)divisions;
        cout << h_division << endl;

        int uppery, lowery, leftx, rightx;
        double mag;

        for (int r = 0; r < divisions; r++)
        {
            uppery = (int) r*h_division;
            lowery = (int) (r+1)*h_division;
            for (int c = 0; c < divisions; c++)
            {
                leftx = (int) c*w_division;
                rightx = (int) (c + 1) * w_division;
                cout << "r: " << r << "\t" << "c: " << c << "\t" << leftx << "\t" << rightx << "\t" << uppery << "\t" << lowery << endl;
                mag = getMagnitude(leftx, uppery, rightx, lowery, image);
                cout << "\tmag: " << mag << endl;

                results.at<double>(r, c) = abs(mag);
            }
        }
        // cout << "Results:" << endl << results << endl;
        normalize(results, results, 0, 255, NORM_MINMAX, CV_64F);

        // cout << results << endl;
        
        Mat results2(results.size(), CV_32S);

        for (int r = 0; r < results.rows; r++)
        {
            for (int c = 0; c < results.cols; c++)
            {
                results2.at<int>(r,c) = (int)results.at<double>(r,c);
            }
        }

        cout << results2.size();

        return results2;
    }

    // Get the bw above and below image of a picture.
    // Input is the average pixel image
    // Output is the bw image with above average colored white
    // and below painted black.
    Mat aboveBelow(const Mat& image)
    {
        Mat results(image.size(), CV_32S);
        int avg = (int) (mean(image)).val[0];

        for (int r = 0; r < image.rows; r++)
        {
            for (int c = 0; c < image.cols; c++)
            {
                int i = image.at<int>(r,c);
                if (i > avg)
                    results.at<int>(r,c) = 255;
                else
                    results.at<int>(r,c) = 0;
            }
        }

        return results;
    }

    // Determine Similarity of two images. Only works for matrices of the same size, for now.
    float determineSimilarity (const Mat& mat1, const Mat& mat2)
    {
        if (mat1.size() != mat2.size())
        {
            cout << "Matrices are not the same size";
            return -1;
        } 

        int difference = 0;

        for (int r = 0; r < mat1.rows; r++)
        {
            for (int c = 0; c < mat1.cols; c++)
            {
                difference += abs(mat1.at<int>(r,c) - mat2.at<int>(r,c));
            }
        }

        // Calculate mean difference
        return difference/(mat1.rows * mat1.cols);
    }

}