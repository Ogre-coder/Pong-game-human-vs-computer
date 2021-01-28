#include "stdafx.h"
#include "CBase4618.h"

#define CV_8UC3 CV_MAKETYPE(CV_8U,3)


CBase4618::CBase4618()
{

}

CBase4618::~CBase4618()
{


}

void CBase4618::update()
{


}

void CBase4618::draw()
{

    //_canvas = Mat(CV_8UC3, Scalar::all(0));

    //_canvas = cv::imread("BCIT.jpg");


    //srand(time(0));

    /*
    for (int i = 0; i < 500; i++)
    {
        float radius = 50 * rand() / RAND_MAX;
        cv::Point center = cv::Point(im.size().width * rand() / RAND_MAX, im.size().height * rand() / RAND_MAX);

        cv::circle(im, center, radius, cv::Scalar(200, 200, 200), 1, cv::LINE_AA);

        im.at<char>(i, i) = 255;

        cv::imshow("Image", im);
        cv::waitKey(1);
        */
}
