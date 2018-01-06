#pragma once
#include "include/opencv/cv.h"
class JumpByJump
{
public:
	JumpByJump();
	~JumpByJump();

	double m_parameter;

	void CarryOut();//执行操作
	void ReadImage();//读取截图
	CvPoint	LocateChess();//棋子定位
	void ShowScreen();//刷新屏幕
	double m_targetPosX;
	double m_targetPosY;
private:
	IplImage*	m_image;
	IplImage*	m_imgChessModel;
	IplImage*	m_matchResult;
};

