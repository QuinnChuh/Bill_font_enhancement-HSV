#include <cv.h>  
#include <highgui.h>   
#include <stdlib.h>
#include <stdio.h>
int main( int argc, char* argv[] )  
{
	//����ͼƬ
	IplImage* src  = cvLoadImage("picture\\1.png");
	//��ʾԭͼ
	cvNamedWindow("ԭͼ");
	cvShowImage("ԭͼ",src);

	//��ɫ�ռ�ת��
	IplImage* hsv = cvCloneImage(src);
    cvCvtColor(src,hsv,CV_BGR2HSV);//�õ�HSVͼ
	//��ʾHSVͼ
	cvNamedWindow("HSV");
	cvShowImage("HSV",hsv);
	//��ȡͼ�������Ϣ
	int height     = hsv->height;
	int width      = hsv->width;
	int step       = hsv->widthStep/sizeof(uchar);
	int channels   = hsv->nChannels;
	uchar* data    = (uchar *)hsv->imageData;

	//������ֵģ��
	IplImage* bw = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
	uchar* data_bw    = (uchar *)bw -> imageData;
	int step_bw       = bw->widthStep/sizeof(uchar);

	//����ͼƬ
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			//��ȡHͨ��
			int H = data[i*step+j*channels+0];
			//��������ָ�
		    if((H>110)&&(H<150))
			{
				data_bw[i*step_bw+j] = 255;    //ǰ����Ҷ�ֵ��Ϊ255���ף�
			 }
			else
			{
				data_bw[i*step_bw+j] = 0;     //����λ�ûҶ�ֵ��Ϊ0���ڣ�
			}
		}
	}
	//��ʾ��ֵͼ
    cvNamedWindow("��ֵͼ");
	cvShowImage("��ֵͼ",bw);

	//����IplImage���͵�dst
    IplImage* dst = cvCloneImage(hsv);
	uchar* data_dst    = (uchar *)dst -> imageData;

	//����ͼ��ǰ�����Ӧ��S����100
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
				data_dst[i*step+j*channels+1] = temp;  //�����м�������ⳬ����Χ
				
			}
		}
	}

	//��ɫ�ռ�ת��
	IplImage* dstdst = cvCloneImage(hsv);
	cvCvtColor(dst,dstdst,CV_HSV2BGR);//�õ�HSVͼ
	//��ʾ��ǿSͨ���Ľ��
	cvNamedWindow("���");
	cvShowImage("���",dstdst);

	//�Զ�ֵͼ������ֵ�˲�
	IplImage* bw_smooth = cvCloneImage(bw);
	uchar* data_bw_smooth    = (uchar *)bw_smooth -> imageData;
	cvSmooth(bw,bw_smooth,CV_MEDIAN,3,3);

	
	cvNamedWindow("�˲�");
	cvShowImage("�˲�",bw_smooth);
	 
	

    //�Զ�ֵͼ�������Ͳ���
	IplConvKernel* element = cvCreateStructuringElementEx(3,3,2,2,CV_SHAPE_CROSS);
	IplImage* bw_dilate = cvCloneImage(bw);
	uchar* data_bw_dilate    = (uchar *)bw_dilate -> imageData;
	cvDilate(bw_smooth,bw_dilate,element,1);

	cvNamedWindow("����");
	cvShowImage("����",bw_dilate);

	

	//����IplImage���͵�dst
    IplImage* dst_rgb = cvCloneImage(hsv);
	uchar* data_dst_rgb    = (uchar *)dst_rgb -> imageData;

	//����ͼ��ǰ�����Ӧ��S����150
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
				data_dst_rgb[i*step+j*channels+1] = temp1;  //ǰ�����Bֵ��Ϊ255��G��R��Ϊ0
				
			}
		}
	}

	//��ɫ�ռ�ת��
	IplImage* dstdst_rgb = cvCloneImage(hsv);
	cvCvtColor(dst_rgb,dstdst_rgb,CV_HSV2BGR);//�õ�HSVͼ
	//��ʾ��ǿSͨ���Ľ��
	cvNamedWindow("���ս��");
	cvShowImage("���ս��",dstdst_rgb);


	cvWaitKey(0);

	cvReleaseImage(&src);
	cvDestroyWindow("ԭͼ");
	
	cvReleaseImage(&bw);
	cvDestroyWindow("��ֵͼ");

	cvReleaseImage(&hsv);
	cvDestroyWindow("HSV");

	cvReleaseImage(&bw_smooth);
	cvDestroyWindow("�˲�");

	cvReleaseImage(&dst);
	cvDestroyWindow("���");

	cvReleaseImage(&bw_dilate);
	cvDestroyWindow("����");

	

	return 0;
	
}