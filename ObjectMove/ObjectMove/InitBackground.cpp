#include "InitBackground.h"


InitBackground::InitBackground(void)
{
	m_sign = 0;
	m_preImg = NULL;
	m_count = 0;
}


InitBackground::~InitBackground(void)
{
	if(m_preImg!=NULL)
	{
		cvReleaseImage(&m_preImg);
		m_preImg = NULL;
	}
}

int InitBackground::prepareBG(IplImage* pImg)
{
	if(m_sign==0){
		m_sign = 1;
		m_preImg = cvCreateImage(cvSize(pImg->width,pImg->height),pImg->depth,pImg->nChannels);
		return 0;
	}
	int result = isMove.IsSame(pImg,m_preImg);

	if(result==0){
		m_count++;
	}else
		m_count = 0;

	if(m_count==50)
	{
		m_count = 0;
		m_sign = 0;
		if(m_preImg!=NULL)
		{
			cvReleaseImage(&m_preImg);
			m_preImg = NULL;
		}
		return 1;
	}
	cvCopy(pImg,m_preImg);
	return 0;
}