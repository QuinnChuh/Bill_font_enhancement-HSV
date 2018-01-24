#include <cv.h>  
#include <highgui.h>   
#include <stdlib.h>
#include <stdio.h>
int main( int argc, char* argv[] )  
{
	//读入图片
	IplImage* src  = cvLoadImage("picture\\1.png");
	//显示原图
	cvNamedWindow("原图");
	cvShowImage("原图",src);

	//颜色空间转换
	IplImage* hsv = cvCloneImage(src);
    cvCvtColor(src,hsv,CV_BGR2HSV);//得到HSV图
	//显示HSV图
	cvNamedWindow("HSV");
	cvShowImage("HSV",hsv);
	//获取图像基本信息
	int height     = hsv->height;
	int width      = hsv->width;
	int step       = hsv->widthStep/sizeof(uchar);
	int channels   = hsv->nChannels;
	uchar* data    = (uchar *)hsv->imageData;

	//创建二值模板
	IplImage* bw = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
	uchar* data_bw    = (uchar *)bw -> imageData;
	int step_bw       = bw->widthStep/sizeof(uchar);

	//遍历图片
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			//提取H通道
			int H = data[i*step+j*channels+0];
			//字体区域分割
		    if((H>110)&&(H<150))
			{
				data_bw[i*step_bw+j] = 255;    //前景点灰度值置为255（白）
			 }
			else
			{
				data_bw[i*step_bw+j] = 0;     //其余位置灰度值设为0（黑）
			}
		}
	}
	//显示二值图
    cvNamedWindow("二值图");
	cvShowImage("二值图",bw);

	//建立IplImage类型的dst
    IplImage* dst = cvCloneImage(hsv);
	uchar* data_dst    = (uchar *)dst -> imageData;

	//遍历图像，前景点对应的S增加100
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			if(data_bw[i*step_bw+j] == 255)
			{
				int temp = data_dst[i*step+j*channels+1]+100;
				if (temp>255)
				{
					temp = 255;
				}
				data_dst[i*step+j*channels+1] = temp;  //设置中间变量避免超过范围
				
			}
		}
	}

	//颜色空间转换
	IplImage* dstdst = cvCloneImage(hsv);
	cvCvtColor(dst,dstdst,CV_HSV2BGR);//得到HSV图
	//显示增强S通道的结果
	cvNamedWindow("结果");
	cvShowImage("结果",dstdst);

	//对二值图进行中值滤波
	IplImage* bw_smooth = cvCloneImage(bw);
	uchar* data_bw_smooth    = (uchar *)bw_smooth -> imageData;
	cvSmooth(bw,bw_smooth,CV_MEDIAN,3,3);

	
	cvNamedWindow("滤波");
	cvShowImage("滤波",bw_smooth);
	 
	

    //对二值图进行膨胀操作
	IplConvKernel* element = cvCreateStructuringElementEx(3,3,2,2,CV_SHAPE_CROSS);
	IplImage* bw_dilate = cvCloneImage(bw);
	uchar* data_bw_dilate    = (uchar *)bw_dilate -> imageData;
	cvDilate(bw_smooth,bw_dilate,element,1);

	cvNamedWindow("膨胀");
	cvShowImage("膨胀",bw_dilate);

	

	//建立IplImage类型的dst
    IplImage* dst_rgb = cvCloneImage(hsv);
	uchar* data_dst_rgb    = (uchar *)dst_rgb -> imageData;

	//遍历图像，前景点对应的S增加150
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			if(data_bw_smooth[i*step_bw+j] == 255)
			{
				int temp1 = data_dst_rgb[i*step+j*channels+1]+100;
				if (temp1>255)
				{
					temp1 = 255;
				}
				data_dst_rgb[i*step+j*channels+1] = temp1;  //前景点的B值设为255，G和R设为0
				
			}
		}
	}

	//颜色空间转换
	IplImage* dstdst_rgb = cvCloneImage(hsv);
	cvCvtColor(dst_rgb,dstdst_rgb,CV_HSV2BGR);//得到HSV图
	//显示增强S通道的结果
	cvNamedWindow("最终结果");
	cvShowImage("最终结果",dstdst_rgb);


	cvWaitKey(0);

	cvReleaseImage(&src);
	cvDestroyWindow("原图");
	
	cvReleaseImage(&bw);
	cvDestroyWindow("二值图");

	cvReleaseImage(&hsv);
	cvDestroyWindow("HSV");

	cvReleaseImage(&bw_smooth);
	cvDestroyWindow("滤波");

	cvReleaseImage(&dst);
	cvDestroyWindow("结果");

	cvReleaseImage(&bw_dilate);
	cvDestroyWindow("膨胀");

	

	return 0;
	
}