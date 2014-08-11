#include <iostream>
#include <windows.h>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "IsMove.h"
#include "ObjectException.h"
#include "InitBackground.h"
using namespace std;
using namespace cv;

//////////////////////////////////////////////

#ifdef _DEBUG
#pragma comment(lib,"opencv_core246d.lib")
#pragma comment(lib,"opencv_highgui246d.lib")
#pragma comment(lib,"opencv_imgproc246d.lib")
#pragma comment(lib,"opencv_objdetect246d.lib")
#pragma comment(lib,"opencv_video246d.lib")
#pragma comment(lib,"opencv_legacy246d.lib")
#else
#pragma comment(lib,"opencv_core246.lib")
#pragma comment(lib,"opencv_highgui246.lib")
#pragma comment(lib,"opencv_imgproc246.lib")
#pragma comment(lib,"opencv_objdetect246.lib")
#pragma comment(lib,"opencv_video246.lib")
#pragma comment(lib,"opencv_legacy246.lib")
#endif

Rect select1;
bool select_flag=false;
Point origin;
IplImage * pImg=NULL;//��ǰͼ��
Mat frame;
int mark =0 ;
int mark1 = 0;
void onMouse(int event,int x,int y,int,void*)
{
	//Point origin;//����������ط����ж��壬��Ϊ���ǻ�����Ϣ��Ӧ�ĺ�����ִ�����origin���ͷ��ˣ����Դﲻ��Ч����
	if(select_flag)
	{
		select1.x=MIN(origin.x,x);//��һ��Ҫ����굯��ż�����ο򣬶�Ӧ������갴�¿�ʼ���������ʱ��ʵʱ������ѡ���ο�
		select1.y=MIN(origin.y,y);
		select1.width=abs(x-origin.x);//����ο�Ⱥ͸߶�
		select1.height=abs(y-origin.y);
		select1&=Rect(0,0,frame.cols,frame.rows);//��֤��ѡ���ο�����Ƶ��ʾ����֮��
	}
	if(event==CV_EVENT_LBUTTONDOWN)
	{
		select_flag=true;//��갴�µı�־����ֵ
		origin=Point(x,y);//�������������ǲ�׽���ĵ�
		select1=Rect(x,y,0,0);//����һ��Ҫ��ʼ������͸�Ϊ(0,0)����Ϊ��opencv��Rect���ο����ڵĵ��ǰ������Ͻ��Ǹ���ģ����ǲ������½��Ǹ���
	}
	else if(event==CV_EVENT_LBUTTONUP)
	{
		select_flag=false;
		if(select1.x>0&&select1.y>0&&select1.width>0&&select1.height>0){
			mark =1;
			mark1 =1;
		}
	}
}

int main()
{
	select1.x=0;
	select1.y=0;
	select1.width=0;
	select1.height=0;

	CvCapture* pCap = NULL;
	pCap = cvCreateCameraCapture(0);
	if (pCap == NULL){
		cout<<"�޷��������"<<endl;
		return 0;
	}
	ObjectException objExc;
	InitBackground initBackground;
	CvRect rect = cvRect(50,50,300,300);
	unsigned long frameNum = 0;

	int BackOk = 0;
	cvNamedWindow( "SRC", 1);
	while(pCap)
	{		
		pImg = cvQueryFrame(pCap);
		if(pImg==NULL){
			printf("��ȡ���ݳ���\r\n");
			break;
		}
		frameNum++;
		frame = pImg;
		if(mark==0){
			//��׽���
			setMouseCallback("SRC",onMouse,0);	

			CvScalar color = CV_RGB(255,0,0);
			if(!(select1.x==0&&select1.y==0&&select1.width==0&&select1.height==0)){
				//cvRectangle(pImg,cvPoint(select1.x,select1.y),cvPoint(select1.width,select1.height),color,3,8,0);
				rectangle(frame,select1,Scalar(255,0,0),3,8,0);
			}
			imshow( "SRC",frame );
			cvWaitKey(5);
		}
		if(mark==1&&mark1==1){
			cvDestroyWindow("SRC"); 
			mark1 = 0;
		}
	
		if(mark == 1){
			if(BackOk==0){
				int isBackOk = initBackground.prepareBG(pImg);	
				if(isBackOk == 0)
					continue;
				else
				{
					BackOk = 1;
					cout<<"������ʼ�����,��ʼ���"<<endl;
				}
			}
		
			int result =objExc.process(pImg,select1);
			if(result==1){
				BackOk = 0;
				cout<<frameNum<<"�����쳣"<<endl;
				cout<<frameNum<<"����������..."<<endl;
			}

			cvNamedWindow( "SRC", 1);
			CvScalar color = CV_RGB(255,0,0);
			cvRectangle(pImg,cvPoint(select1.x,select1.y),cvPoint(select1.width+select1.x,select1.height+select1.y),color,3,8,0);
			cvShowImage( "SRC",pImg );
			cvWaitKey(5);
		}
		
	}
	return 0;
}



