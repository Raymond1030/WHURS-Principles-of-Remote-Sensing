#include "RS_Image_Index.h"

Mat Color(Mat Index,int type)//伪彩色化
{
	Mat Img(Index.size(), CV_8UC3);
	if (type == Plant)
	{
		for (int i = 0; i < Index.rows; i++)
		{
			for (int j = 0; j < Index.cols; j++)
			{
				if (Index.at<double>(i, j) > 0)
				{
					Img.at<Vec3b>(i, j)[0] = 0;
					Img.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(Index.at<double>(i, j) * 255);   //添加绿色;
					Img.at<Vec3b>(i, j)[2] = 0;
				}
				else
				{

					Img.at<Vec3b>(i, j)[0] = 0;
					Img.at<Vec3b>(i, j)[1] = 0;
					Img.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}
	}
	else if (type == Water)
	{
		for (int i = 0; i < Index.rows; i++)
			for (int j = 0; j < Index.cols; j++)
			{
				if (Index.at<double>(i, j) > 0)
				{
					Img.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(Index.at<double>(i, j) * 255);  //添加蓝色
					Img.at<Vec3b>(i, j)[1] = 0;
					Img.at<Vec3b>(i, j)[2] = 0;
				}
				else
				{
					Img.at<Vec3b>(i, j)[0] = 0;
					Img.at<Vec3b>(i, j)[1] = 0;
					Img.at<Vec3b>(i, j)[2] = 0;
				}
			}
	}
	else if (type == Buliding)
	{
		for (int i = 0; i < Index.rows; i++)
			for (int j = 0; j < Index.cols; j++)
			{
				if (Index.at<double>(i, j) > 0)
				{
					Img.at<Vec3b>(i, j)[0] = 0;
					Img.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(Index.at<double>(i, j) * 255);  //添加黄色
					Img.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(Index.at<double>(i, j) * 255);
				}
				else
				{
					Img.at<Vec3b>(i, j)[0] = 0;
					Img.at<Vec3b>(i, j)[1] = 0;
					Img.at<Vec3b>(i, j)[2] = 0;
				}
			}
	}
	return Img;
}

Mat ColorRGB(Mat b, Mat g, Mat r)
{

	Mat img(b.size(), CV_8UC3);

	/*for(int i=0;i<b.rows;i++)
		for (int j = 0; j < b.cols; j++)
		{
			img.at<Vec3b>(i, j)[0] = r.at<uchar>(i, j);
			img.at<Vec3b>(i, j)[1] = b.at<uchar>(i, j);
			img.at<Vec3b>(i, j)[2] = g.at<uchar>(i, j);
		}*/
	Mat channel[3] = { b,g,r };

	merge(channel, 3, img);

	
	//彩色化
	//CV_64FC1
	//CV_8UC3

	return img;
}

Mat Binary(Mat Index)
{
	Mat out(Index.size(), CV_8UC1);

	double temp = 0;
	double max = 0;
	double min = 255;

	//最大类间误差法

	//灰度拉伸 从0-255
	for (int i = 0; i < Index.rows; i++)
		for (int j = 0; j < Index.cols; j++)
		{
			temp = Index.at<double>(i, j);
			if (temp > max)
			{
				max = temp;
			}
			if (temp < min)
			{
				min = temp;
			}
		}
	double scale = 255 / (max - min);

	for (int i = 0; i <Index.rows; i++)
		for (int j = 0; j < Index.cols; j++)
		{
			out.at<uchar>(i, j) = (unsigned char)((int)((Index.at<double>(i, j) - min) * scale));
		}

	//动态计算阈值
	int thre = 0;
	double f1 = 0; double f2 = 0;         //前后景灰度频数
	double s_temp = 0; double s = 0;     //方差
	double avg1 = 0; double avg2 = 0;    //前后景分别均值

	for (int t = 0; t < 256; t++)          //找每一个阈值
	{
		f1 = 0;
		f2 = 0;
		avg1 = 0;
		avg2 = 0;

		for (int i = 0; i < out.rows; i++)
			for (int j = 0; j < out.cols; j++)
			{
				int val = (int)(out.at<uchar>(i, j));
				if (val < t)
				{
					f1++;
					avg1 += val;
				}
				else
				{
					f2++;
					avg2 += val;
				}
			}
		avg1 /= f1;
		avg2 /= f2;
		s_temp = f1 * f2 * pow((avg1 - avg2), 2) / pow((out.rows - out.cols), 2);

		if (s_temp > s)    //找到动态规划后的阈值
		{
			thre = t;
			s = s_temp;
		}
	}

	Mat res(out.size(), out.type());

	//二值化
	for (int i = 0; i < res.rows; i++)
		for (int j = 0; j < res.cols; j++)
		{
			int val = (int)(out.at<uchar>(i, j));
			if (val > thre)
			{
				res.at<uchar>(i, j) = 255;
			}
			else
			{
				res.at<uchar>(i, j) = 0;
			}
		}

	return res;
}

Mat RVI(Mat Rn, Mat Rr)
{
	//Rn 近红外波段
	//Rr 红波段
	//RVI=Rn/Rr
	Mat Ratio(Rn.size(), CV_64FC1);
	divide(Rn, Rr, Ratio);//相除
	Ratio.convertTo(Ratio, CV_64FC1);//转换格式 防止double类型丢失
	return Ratio;
}



Mat Normalize(Mat s1, Mat s2)//NDVI植被归一化指数、NDWI归一化水体指数、NDBI归一化裸地指数
{
	//NDVI
	//NIR 近红外波段
	//R 红光波段
	//NDVI=(NIR-R)/(NIR+R)

	//NDWI 
	//G 绿波段
	//nR 近红外波段
	//NDWI=(G-nR)/(G+nR)

	//NDBI 
	//mR 中红外波段 
	//nR 近红外波段
	//NDBI=(mR-nR)/(mR+nR)
	
	

	Mat m(s1.size(), CV_64FC1);   //分子
	Mat n(s2.size(), CV_64FC1);   //分母

	subtract(s1, s2, m);
	add(s1, s2, n);

	Mat NDVI(s1.size(), CV_64FC1);    //计算得到的指数 用double存储

	m.convertTo(m, CV_64FC1);   //运算会导致类型不为double
	n.convertTo(n, CV_64FC1);   // 此处进行强制转换

	divide(m, n, NDVI);


	return NDVI;
}

#define L 0.5
Mat SAVI(Mat Rn, Mat Rr)//土壤调节植被指数
{
	//Rn近红外波段
	//Rr红光波段
	//SAVI=(Rn-Rr)(1+L)/(Rn+Rr+L)  (取 L=0.5)

	Mat SAVI(Rn.size(), CV_64FC1);
	Mat m(Rn.size(), CV_64FC1);   //分子
	Mat n(Rn.size(), CV_64FC1);   //分母
	subtract(Rn , Rr,m);
	m.convertTo(m, CV_64FC1);
	multiply(m, 1 + L, m);

	add(Rn, Rr, n);
	add(n, L, n);
	n.convertTo(n, CV_64FC1);

	divide(m, n, SAVI);
	SAVI.convertTo(SAVI, CV_64FC1);
	return SAVI;
}

Mat AWEI(Mat G, Mat nR, Mat mR, Mat dR) {
	//自动水体提取指数
	//G 绿光
	//nR 近红外波段
	//mR 中红外波段
	//dR 远红外波段
	//AWEI=4(G-mR)/(0.25nR+2.75dR)

	Mat m(G.size(), CV_64FC1);//分子
	Mat n(G.size(), CV_64FC1);//分母
	Mat t(G.size(), CV_64FC1);

	nR.convertTo(nR, CV_64FC1);
	dR.convertTo(nR, CV_64FC1);

	subtract(G, mR, m);
	multiply(m, 4, m);
	m.convertTo(m, CV_64FC1);

	multiply(nR, 0.25, n);
	multiply(dR, 2.75, t);
	n.convertTo(n, CV_64FC1);
	t.convertTo(t, CV_64FC1);
	add(n, t, n);
	n.convertTo(n, CV_64FC1);

	Mat img(G.size(), CV_64FC1);
	divide(m, n, img);
	img.convertTo(img, CV_64FC1);
	return img;
}
Mat IBI(Mat NDBI, Mat SAVI, Mat NDWI) {
	//建筑用地指数
	//NDBI归一化建筑指数
	//SAVI土壤调节植被指数
	//NDWI归一化水体指数
	//IBI=(NDBI-(SAVI+NDWI)/2)/(NDBI+(SAVI+NDWI)/2）
	Mat m(NDBI.size(), CV_64FC1);
	add(SAVI, NDWI, m);
	divide(m, 2, m);
	m.convertTo(m, CV_64FC1);

	Mat img = Normalize(NDBI, m);

	return img;
}