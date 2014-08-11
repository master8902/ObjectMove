#include "IsMove.h"

IsMove::IsMove(void)
{
	m_mark = 0;
	m_threshold = 60;
}

IsMove::~IsMove(void)
{
	if(m_glpBkImage!=NULL){
		cvReleaseImage(&m_glpBkImage);
	}
	if(m_glpPreImage!=NULL){
		cvReleaseImage(&m_glpPreImage);
	}
	if(m_glpGrayImage!=NULL){
		cvReleaseImage(&m_glpGrayImage);
	}
	if(m_glpDstDiff!=NULL){
		cvReleaseImage(&m_glpDstDiff);
	}
	if(m_glpMhi!=NULL){
		cvReleaseImage(&m_glpMhi);
	}
	if(m_glpMask!=NULL){
		cvReleaseImage(&m_glpMask);
	}

}

void IsMove::Init(CvSize size,int depth,int nChannels)
{		
	m_glpBkImage = cvCreateImage(size,IPL_DEPTH_8U,1);
	m_glpPreImage = cvCreateImage(size,IPL_DEPTH_8U,1);
	m_glpGrayImage = cvCreateImage(size,IPL_DEPTH_8U,1);
	m_glpDstDiff = cvCreateImage(size,IPL_DEPTH_8U,1);
	m_glpMask = cvCreateImage(size,IPL_DEPTH_8U,1);
	m_glpMhi = cvCreateImage( size, IPL_DEPTH_32F, 1 );
	cvZero(m_glpMhi); // clear MHI at the beginning
}

int IsMove::IsSame(IplImage *lpImage,IplImage *pbImage)
{
	//cvNamedWindow("lpImage",1);
//	cvShowImage("lpImage",lpImage);

	if(m_mark==0){
		Init(cvSize(lpImage->width,lpImage->height),lpImage->depth,lpImage->nChannels);
		m_mark = 1;
	}
	cvCvtColor(lpImage,m_glpGrayImage,CV_BGR2GRAY);
	cvCvtColor(pbImage,m_glpPreImage,CV_BGR2GRAY);
	cvSmooth(m_glpGrayImage,m_glpGrayImage,CV_GAUSSIAN,3,3);
	cvSmooth(m_glpPreImage,m_glpPreImage,CV_GAUSSIAN,3,3);
	cvAbsDiff(m_glpGrayImage,m_glpPreImage,m_glpDstDiff);

	cvThreshold(m_glpDstDiff, m_glpDstDiff, m_threshold, 255, CV_THRESH_BINARY); //
	cvSmooth(m_glpDstDiff,m_glpDstDiff,CV_GAUSSIAN,3,3);
	cvSmooth(m_glpDstDiff,m_glpDstDiff,CV_GAUSSIAN,3,3);

	cvErode(m_glpDstDiff,m_glpDstDiff,NULL,1);
	cvErode(m_glpDstDiff,m_glpDstDiff,NULL,1);


	CvMemStorage *lpMemStotage = cvCreateMemStorage(0);
	CvSeq* first_contour = NULL;
	cvFindContours(m_glpDstDiff,lpMemStotage,&first_contour,sizeof(CvContour),
		CV_RETR_EXTERNAL);
	int temp =0;

	for(;first_contour;first_contour = first_contour->h_next)
	{
		CvRect r = ((CvContour*)first_contour)->rect;
		if(r.height * r.width > 200) // 面积小的方形抛弃掉
		{					
			temp =1;
		}				
	}	
	cvReleaseMemStorage(&lpMemStotage);
	return temp;
}