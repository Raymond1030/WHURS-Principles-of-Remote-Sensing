#include<opencv2/highgui.hpp>
#include<opencv2/core.hpp>

#include<stdio.h>
#include <iostream>
#include<string>

#define Plant 0
#define Water 1
#define Buliding 2
using namespace std;
using namespace cv;

Mat Color(Mat Index,int type);//伪彩色化
Mat ColorRGB(Mat b, Mat g, Mat r);//真彩色合成
Mat Binary(Mat Index);//二值化
Mat RVI(Mat Rn, Mat Rr);//比值植被指数
Mat Normalize(Mat s1, Mat s2);//NDVI植被归一化指数、NDWI归一化水体指数、NDBI归一化裸地指数
Mat SAVI(Mat Rn, Mat Rr);//土壤调节植被指数、NDWI归一化水体指数
Mat AWEI(Mat G, Mat nR, Mat mR, Mat dR);//自动水体提取指数
Mat IBI(Mat NDBI, Mat SAVI, Mat NDWI);//建筑用地指数

