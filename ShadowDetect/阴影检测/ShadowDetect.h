#pragma once

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<stdio.h>
#include <iostream>
#include<algorithm>
using namespace std;
using namespace cv;
#define Pi 3.1415926

#define T1  80  //类间最大误差法调高的阈值 用作HSV去噪
#define T2  40  //类间最大误差法调高的阈值 用作C1C2C3去噪
//类别
#define HSV 0            
#define C1C2C3 1
void RGBToHSV(Mat r, Mat g, Mat b,Mat* h, Mat* s, Mat* v);//HSV空间转换
Mat Shadow(Mat h, Mat s, Mat v);//HSV阴影识别
Mat Segment(Mat img, int type);//二值化处理
Mat Merge(Mat img, int type);//合并
Mat RGBToC1C2C3(Mat r, Mat g, Mat b);//C1C2C3空间转换

