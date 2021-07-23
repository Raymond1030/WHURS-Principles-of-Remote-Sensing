// 阴影检测.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<stdio.h>
#include <iostream>
#include<algorithm>
#include"ShadowDetect.h"
using namespace std;
using namespace cv;

//遥感影像的阴影检测
#define Pi 3.1415926

#define T1  80  //类间最大误差法调高的阈值 用作HSV去噪
#define T2  40  //类间最大误差法调高的阈值 用作C1C2C3去噪
//类别
#define HSV 0            
#define C1C2C3 1

int main()
{
    Mat img = imread("Color.bmp");

    vector<Mat>channel;
    split(img, channel);    //分成B G R三频道

    Mat H(img.size(), CV_64FC1), S(img.size(), CV_64FC1), V(img.size(), CV_64FC1);
   
    //基于彩色模型的图像阴影检测
    printf_s("运行基于HSV空间的阴影检测……\n");
    RGBToHSV(channel[2], channel[1], channel[0], &H, &S, &V);//HSV空间转换
    Mat HSV_img=Shadow(H,S,V);  //HSV阴影识别
    Mat HSV_img_binary = Segment(HSV_img, HSV);
    imshow("合并前基于HSV空间的阴影检测", HSV_img_binary);
    waitKey(0);
    imwrite("./Output/合并前基于HSV空间的阴影检测.jpg", HSV_img_binary);
    Mat HSV_merge = Merge(HSV_img_binary, HSV);
    imshow("合并后基于HSV空间的阴影检测", HSV_merge);
    waitKey(0);
    imwrite("./Output/合并后基于HSV空间的阴影检测.jpg", HSV_merge);

    //基于C1C2C3彩色空间的阴影检测
    printf_s("运行基于C1C2C3彩色空间的阴影检测……\n");
    Mat C1C2C3_img = RGBToC1C2C3(channel[2], channel[1], channel[0]); // C1C2C3空间转换
    Mat C1C2C3_img_binary = Segment(C1C2C3_img,C1C2C3);
    imshow("合并前基于C1C2C3彩色空间的阴影检测",C1C2C3_img_binary);
    waitKey(0);
    imwrite("./Output/合并前基于C1C2C3彩色空间的阴影检测.jpg",C1C2C3_img_binary);
    Mat C1C2C3_Merge = Merge(C1C2C3_img_binary, C1C2C3);
    imshow("合并后基于C1C2C3彩色空间的阴影检测", C1C2C3_Merge);
    waitKey(0);
    imwrite("./Output/合并后基于C1C2C3彩色空间的阴影检测.jpg", C1C2C3_Merge);

  
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
