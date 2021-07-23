// Index.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。


#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<stdio.h>
#include <iostream>
#include<string>
#include"RS_Image_Index.h"

using namespace std;
using namespace cv;

//求遥感图像地物指数程序
#define Plant 0
#define Water 1
#define Buliding 2

int main()
{
	Mat img_b, img_g, img_r, img_nR, img_mR, img_dR;//波段
	//读取图像
	printf_s("正在读取图像……\n");
	img_b = imread("./tm/tm1.tif", 0);	//蓝波段
	img_g = imread("./tm/tm2.tif", 0);//绿波段
	img_r = imread("./tm/tm3.tif", 0);//红波段
	img_nR = imread("./tm/tm4.tif", 0);//近红外波段
	img_mR = imread("./tm/tm5.tif", 0);//中红外波段
	img_dR = imread("./tm/tm6.tif", 0);//远红外波段


	//植被
	//RVI比值植被指数
	printf_s("等待RVI比值植被指数处理……\n");
	Mat mRVI =RVI(img_nR,img_r );
	Mat color_RVI = Color(mRVI,Plant);//伪彩色化(绿色）
	Mat Binary_RVI = Binary(mRVI);//二值化
	/*imshow("比值植被指数_伪彩色", color_RVI);*/
	imshow("比值植被指数_二值化", Binary_RVI);
	waitKey(0);
	imwrite("./Output_Img/比值植被指数_二值化.jpg", Binary_RVI);
	/*imwrite("./Output_Img/比值植被指数_伪彩色.jpg", color_RVI);*/

	//NDVI归一化植被指数
	printf_s("等待NDVI归一化植被指数处理……\n");
	Mat mNDVI = Normalize(img_nR, img_r);
	Mat color_NDVI = Color(mNDVI,Plant);//伪彩色化(绿色）
	Mat Binary_NDVI = Binary(mNDVI);//二值化
	imshow("归一化植被指数_伪彩色", color_NDVI);
	imshow("归一化植被指数_二值化", Binary_NDVI);
	waitKey(0);
	imwrite("./Output_Img/归一化植被指数_伪彩色.jpg", color_NDVI);
	imwrite("./Output_Img/归一化植被指数_二值化.jpg", Binary_NDVI);

	//SAVI土壤调节植被指数
	printf_s("等待SAVI土壤调节植被指数处理……\n");
	Mat mSAVI = SAVI(img_nR, img_r);
	Mat color_SAVI = Color(mSAVI,Plant);//伪彩色化(绿色）
	Mat Binary_SAVI = Binary(mSAVI);//二值化
	imshow("土壤调节植被指数_伪彩色", color_SAVI);
	imshow("土壤调节植被指数_二值化", Binary_SAVI);
	waitKey(0);
	imwrite("./Output_Img/土壤调节植被指数_伪彩色.jpg", color_SAVI);
	imwrite("./Output_Img/土壤调节植被指数_二值化.jpg", Binary_SAVI);


	//水体
	//NDWI归一化水体指数
	printf_s("等待NDWI归一化水体指数处理……\n");
	Mat mNDWI =Normalize(img_g, img_nR);
	Mat color_NDWI = Color(mNDWI,Water);//伪彩色化(绿色）
	Mat Binary_NDWI = Binary(mNDWI);//二值化
	imshow("归一化水体指数_伪彩色", color_NDWI);
	imshow("归一化水体指数_二值化", Binary_NDWI);
	waitKey(0);
	imwrite("./Output_Img/归一化水体指数_伪彩色.jpg", color_NDWI);
	imwrite("./Output_Img/归一化水体指数_二值化.jpg", Binary_NDWI);

	//AWEI自动水体提取指数
	printf_s("等待AWEI自动水体提取指数处理……\n");
	Mat mAWEI = AWEI(img_g,img_nR,img_mR,img_dR);
	Mat color_AWEI = Color(mAWEI,Water);//伪彩色化(绿色）
	Mat Binary_AWEI = Binary(mAWEI);//二值化
	imshow("自动水体提取指数_伪彩色", color_AWEI);
	imshow("自动水体提取指数_二值化", Binary_AWEI);
	waitKey(0);
	imwrite("./Output_Img/自动水体提取指数_伪彩色.jpg", color_AWEI);
	imwrite("./Output_Img/自动水体提取指数_二值化.jpg", Binary_AWEI);

	//建筑
	//NDBI归一化裸地指数
	printf_s("等待NDBI归一化裸地指数处理……\n");
	Mat mNDBI =Normalize(img_mR,img_nR);
	Mat color_NDBI = Color(mNDBI, Buliding);//伪彩色化(绿色）
	Mat Binary_NDBI = Binary(mNDBI);//二值化
	imshow("归一化裸地指数_伪彩色", color_NDBI);
	imshow("归一化裸地指数_二值化", Binary_NDBI);
	waitKey(0);
	imwrite("./Output_Img/归一化裸地指数_伪彩色.jpg", color_NDBI);
	imwrite("./Output_Img/归一化裸地指数_二值化.jpg", Binary_NDBI);

	//IBI建筑用地指数
	printf_s("等待IBI建筑用地指数处理……\n");
	Mat mIBI = IBI(mNDBI,mSAVI,mNDWI);
	Mat color_IBI = Color(mIBI, Buliding);//伪彩色化(绿色）
	Mat Binary_IBI = Binary(mIBI);//二值化
	imshow("建筑用地指数_伪彩色", color_IBI);
	imshow("建筑用地指数_二值化", Binary_IBI);
	waitKey(0);
	imwrite("./Output_Img/建筑用地指数_伪彩色.jpg", color_IBI);
	imwrite("./Output_Img/建筑用地指数_二值化.jpg", Binary_IBI);

	//真彩色合成
	printf_s("等待真彩色合成处理……\n");
	Mat Color=ColorRGB(img_b, img_g, img_r);
	imshow("真彩图", Color);
	waitKey(0);
	imwrite("./Output_Img/真彩图.jpg", Color);
	printf_s("运行结束！");
	return 0;
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
