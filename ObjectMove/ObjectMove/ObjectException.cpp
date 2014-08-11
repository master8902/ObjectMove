#include "ObjectException.h"

ObjectException::ObjectException(void)
{
	m_pImg=NULL;//当前图像
	m_bImg = NULL;//背景图像
	m_preImg = NULL;//当前图像的前一帧
	m_temp = NULL;
	isException =0;
	m_sign = 0;
	m_mark = 0;
	m_sum=0;
	m_wait = 0;
	m_begin=0;
	m_end =0;
	m_noObjectNum = 0;
	m_setRoi=0;
	m_count = 0;
	m_result = 0;
	m_detect = 0;
	m_detect1 = 0;
}

ObjectException::~ObjectException(void)
{
	if(m_pImg!=NULL){
		cvReleaseImage(&m_pImg);
	}
	if(m_bImg!=NULL){
		cvReleaseImage(&m_bImg);
	}
	if(m_preImg!=NULL){
		cvReleaseImage(&m_preImg);
	}
	if(m_temp!=NULL){
		cvReleaseImage(&m_temp);
	}
}

void ObjectException::Init(IplImage* pImg,CvRect rect)
{
	cvSetImageROI(pImg,rect);
	if(m_setRoi==0){
		m_setRoi = 1;
		m_pImg = cvCreateImage(cvSize(rect.width,rect.height),pImg->depth,pImg->nChannels);
	}
	cvCopy(pImg,m_pImg);
	cvResetImageROI(pImg);
	if(m_sign==0){
		m_bImg = cvCreateImage(cvSize(m_pImg->width,m_pImg->height),m_pImg->depth,m_pImg->nChannels);
		m_preImg = cvCreateImage(cvSize(m_pImg->width,m_pImg->height),m_pImg->depth,m_pImg->nChannels);
		m_temp = cvCreateImage(cvSize(m_pImg->width,m_pImg->height),m_pImg->depth,m_pImg->nChannels);
		cvCopy(m_pImg,m_bImg);
		cvCopy(m_pImg,m_preImg);
		m_sign++;
	}
	cvCopy(m_pImg,m_temp);
}

void ObjectException::updateBackground()
{
	//当连续500帧视频没有变化时更新背景
	if(m_noObjectNum>500){
	//	std::cout<<"无运动物体情况下定期检测"<<std::endl;
		if(m_noObjectNum == 506)
		{			
			Reset();					
			cvCopy(m_pImg,m_bImg);
			return;
		}		
		int result1 = isMove.IsSame(m_pImg,m_bImg);

		if(result1==1){		
			m_detect ++;
			if(m_detect==3){				
				isException = 1;		
		//		cvSaveImage("current.jpg",m_pImg);
		//		cvSaveImage("background.jpg",m_bImg);
			}		
		}
	}
}

void ObjectException::MonitorObject()
{
	//判断当前图像和前一帧图像是否相同，即判断是否有物体运动，连续三次就判定有物体运动
	m_result = isMove.IsSame(m_pImg,m_preImg);
	if(m_mark == 0){
		if(m_result==1){
			m_sum++;
			if(m_sum==5){
		//		std::cout<<"有运动物体出现"<<std::endl;
				m_mark=1;	
				m_sum=0;	
				m_end = m_begin + 200;
				m_noObjectNum=0;
			}		
		}else
		{
			m_noObjectNum++;
		}
	}

	//当判定为有运动物体后，即mark==1，且当前帧和前一帧相同，即可能运动物体离开。
	if(m_mark == 1){
		if(m_begin<m_end){
			m_begin++;	
			if(m_result == 0){
				m_wait++;
				if(m_wait>150){
					if(m_wait==160){
						Reset();
						cvCopy(m_pImg,m_bImg);
					}

					//判断当前帧与背景是否相同，不同即返回1
					int result1 = isMove.IsSame(m_pImg,m_bImg);
					if(result1==1){		
				//		std::cout<<"常规检测"<<std::endl;
						m_detect1++;
				///		std::cout<<"m_detect1"<<m_detect1<<std::endl;
						if(m_detect1==6){
							isException = 1;	
					//		cvSaveImage("current.jpg",m_pImg);
					//		cvSaveImage("background.jpg",m_bImg);
						}					
					}
				}
			}
		}
		else{
			Reset();
			cvCopy(m_pImg,m_bImg);
		}
	}
	cvCopy(m_temp,m_preImg);

	m_result = 0;
}

void ObjectException::Reset()
{
	m_mark = 0;
	m_begin = 0;
	m_end = 0;
	m_count = 0;
	m_wait=0;
	m_detect = 0;
	m_noObjectNum = 0;
	m_detect1 = 0;
}

int  ObjectException::process(IplImage* pImg,CvRect rect)
{
	//初始化
	Init(pImg,rect);
	//物体监控
	MonitorObject();
	if(isException==1){
		isException = 0;
		Reset();
		return 1;//检测到物体异常
	}
	//定期更新背景
	updateBackground();
	return 0;//未检测到物体异常
}
