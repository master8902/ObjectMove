#pragma once
#include <opencv2/opencv.hpp>
#include "IsMove.h"
class ObjectException
{
	IsMove isMove;//用于判断两幅图像是否相同
	IplImage * m_pImg;//当前图像
	IplImage * m_bImg ;//背景图像
	IplImage * m_preImg ;//当前图像的前一帧
	IplImage * m_temp;
	int m_sign ;//标志位，初始化。
	int m_mark ;//当检测到运动物体后，设为1.
	int m_sum ;//计数，当m_sum==10，即判断有运动物体
	int m_wait ;//当m_wait==200时，认为监控物体异常
	int m_begin ;//m_begin和m_end，定义一个视频片段
	int m_count;
	int m_wait1;
	int m_wait2; 
	int m_end ;
	unsigned long m_noObjectNum ;//设定5000帧还没检测到运动物体就，判断一次是否有物体移动，更新背景。
	int m_setRoi ;//roi初始化标志位
	int isException;//返回结果
	int m_result ;
	int m_detect;
	int m_detect1;
	void Init(IplImage* pImg,CvRect rect);
	void updateBackground();
	void MonitorObject();
	void Reset();
public:
	ObjectException(void);
	~ObjectException(void);
	int process(IplImage*,CvRect);
	
};

