#pragma once
#include <opencv2/opencv.hpp>
#include "IsMove.h"
#include "IsMove.h"
class InitBackground
{
	int m_sign;
	IplImage * m_preImg;
	IsMove isMove;
	int m_count;
public:
	InitBackground(void);
	~InitBackground(void);
	int prepareBG(IplImage*);
};

