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
IplImage * pImg=NULL;//当前图像
Mat frame;
int mark =0 ;
int mark1 = 0;
void onMouse(int event,int x,int y,int,void*)
{
	//Point origin;//不能在这个地方进行定义，因为这是基于消息响应的函数，执行完后origin就释放了，所以达不到效果。
	if(select_flag)
	{
		select1.x=MIN(origin.x,x);//不一定要等鼠标弹起才计算矩形框，而应该在鼠标按下开始到弹起这段时间实时计算所选矩形框
		select1.y=MIN(origin.y,y);
		select1.width=abs(x-origin.x);//算矩形宽度和高度
		select1.height=abs(y-origin.y);
		select1&=Rect(0,0,frame.cols,frame.rows);//保证所选矩形框在视频显示区域之内
	}
	if(event==CV_EVENT_LBUTTONDOWN)
	{
		select_flag=true;//鼠标按下的标志赋真值
		origin=Point(x,y);//保存下来单击是捕捉到的点
		select1=Rect(x,y,0,0);//这里一定要初始化，宽和高为(0,0)是因为在opencv中Rect矩形框类内的点是包含左上角那个点的，但是不含右下角那个点
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
		cout<<"无法打开摄像机"<<endl;
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
			printf("读取数据出错\r\n");
			break;
		}
		frameNum++;
		frame = pImg;
		if(mark==0){
			//捕捉鼠标
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
					cout<<"背景初始化完毕,开始检测"<<endl;
				}
			}
		
			int result =objExc.process(pImg,select1);
			if(result==1){
				BackOk = 0;
				cout<<frameNum<<"物体异常"<<endl;
				cout<<frameNum<<"背景更新中..."<<endl;
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



