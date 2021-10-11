// edgeDetect.cpp : Defines the entry point for the console application. 
// Environment: Visual studio 2019, Windows 10
// Assumptions: Opecv is installed configured in the visual studio project
// Opencv version: OpenCV 4.5.3

#include "edgeMethods.h"
#include <opencv2/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<string>


int main()
{
    cv::Mat imgFinal;
    cv::Mat vert1;
    cv::Mat vert2;

    cv::Mat imgOriginal;        // input image
    cv::Mat imgGrayscale;        // grayscale of input image
    cv::Mat imgBlurred;            // intermediate blured image
    cv::Mat imgCanny;            // Canny edge image
    cv::Mat imgSob;             // Sobel edge image
    cv::Mat imgLan2;            // Laplacian edge image in consideration to the diagonal pixels

    // Declare window
    // Note: you can use CV_WINDOW_NORMAL which allows resizing the window
    // or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
    // CV_WINDOW_AUTOSIZE is the default
    cv::namedWindow("EdgeImg", cv::WINDOW_NORMAL);


    cv::VideoCapture cap;
    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation.
    if (!cap.open(0))
    {
        return 0;
    }
    for (;;)
    {
        cv::Mat frame;
        cap >> frame;

        if (frame.empty())
        {
            break;
        }
        else
        {
            imgOriginal = frame;
        }

        if (!imgOriginal.empty())
        {
            cv::cvtColor(imgOriginal, imgGrayscale, cv::COLOR_BGR2GRAY);        // convert to grayscale

            cv::GaussianBlur(imgGrayscale,
                imgBlurred,
                cv::Size(5, 5),                        // smoothing window width and height in pixels
                1.5);                                // sigma value, determines how much the image will be blurred

            cv::Canny(imgBlurred,
                imgCanny,
                100,                        // low threshold
                200);                        // high threshold


            edgeDetectSob(imgBlurred, imgSob);

            edgeDetectLan2(imgBlurred, imgLan2);

            cv::cvtColor(imgCanny,imgCanny, cv::COLOR_GRAY2BGR);
            cv::cvtColor(imgSob, imgSob, cv::COLOR_GRAY2BGR);
            cv::cvtColor(imgLan2, imgLan2, cv::COLOR_GRAY2BGR);

            cv::hconcat(imgOriginal,imgCanny,vert1);
            cv::hconcat(imgSob, imgLan2, vert2);
            cv::vconcat(vert1,vert2,imgFinal);

            //Show window
            cv::imshow("EdgeImg", imgFinal);

        }
        if(cv::waitKey(30)==27) break;  // hold windows open until user presses a key
    }

    return 0;
}