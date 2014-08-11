#pragma once
#include <opencv2/opencv.hpp>
class IsMove
{
private:
	IplImage* m_glpBkImage;
	IplImage *m_glpPreImage;
	IplImage *m_glpGrayImage ;
	IplImage *m_glpDstDiff ;
	IplImage *m_glpMhi ;
	IplImage *m_glpMask ;
	int m_mark ;
	int m_threshold ;
public:
	IsMove(void);
	~IsMove(void);
	void Init(CvSize size,int depth,int nChannels);
	int IsSame(IplImage *lpImage,IplImage *pbImage);//�ж���֡ͼƬ�Ƿ���ͬ����ͬ����0����ͬ����1.
};

