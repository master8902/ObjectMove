#pragma once
#include <opencv2/opencv.hpp>
#include "IsMove.h"
class ObjectException
{
	IsMove isMove;//�����ж�����ͼ���Ƿ���ͬ
	IplImage * m_pImg;//��ǰͼ��
	IplImage * m_bImg ;//����ͼ��
	IplImage * m_preImg ;//��ǰͼ���ǰһ֡
	IplImage * m_temp;
	int m_sign ;//��־λ����ʼ����
	int m_mark ;//����⵽�˶��������Ϊ1.
	int m_sum ;//��������m_sum==10�����ж����˶�����
	int m_wait ;//��m_wait==200ʱ����Ϊ��������쳣
	int m_begin ;//m_begin��m_end������һ����ƵƬ��
	int m_count;
	int m_wait1;
	int m_wait2; 
	int m_end ;
	unsigned long m_noObjectNum ;//�趨5000֡��û��⵽�˶�����ͣ��ж�һ���Ƿ��������ƶ������±�����
	int m_setRoi ;//roi��ʼ����־λ
	int isException;//���ؽ��
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

