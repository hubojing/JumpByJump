#include "stdafx.h"
#include <iostream>
#include "JumpByJump.h"
#include "include\opencv\highgui.h"
#include "ADBHelper.h"
using namespace  std;

#define CAPTION	"微信自动跳一跳 V1.0"

double m_parameter = 1.18;

void onClick(int event, int x, int y, int flags, void* param)
{
	if (event != CV_EVENT_LBUTTONUP)
	{
		return;
	}
	cout << "目标点坐标：" << "(" << x << "," << y << ")" << endl;
	JumpByJump* jumper = (JumpByJump*)param;
	jumper->m_targetPosX = x;
	jumper->m_targetPosY = y;
	CvPoint chessLoc = jumper->LocateChess();
	double distance = sqrt(powf(jumper->m_targetPosX - chessLoc.x, 2) + powf(jumper->m_targetPosY - chessLoc.y, 2));
	ADBHelper::jump(distance * m_parameter);
	Sleep(1000);//等待截图
	ADBHelper::CapScreen();
	jumper->ReadImage();
	jumper->ShowScreen();
}

JumpByJump::JumpByJump()
{
	m_image = NULL;
	m_imgChessModel = NULL;
	m_matchResult = NULL;
	//载入棋子模型
	m_imgChessModel = cvLoadImage("chess.png");
	cvNamedWindow(CAPTION, 0);
	//设置鼠标点击回调函数
	cvSetMouseCallback(CAPTION, onClick, (void*)this);
}

JumpByJump::~JumpByJump()
{
	cvReleaseImage(&m_image);
	cvReleaseImage(&m_imgChessModel);
	cvReleaseImage(&m_matchResult);
}

void JumpByJump::CarryOut()
{
	ADBHelper::CapScreen();
	ReadImage();
// 	LocateChess();
	ShowScreen();
	cvWaitKey(0);
}

void JumpByJump::ReadImage()
{
	const char* filename = "JumpByJump.png";
	if (m_image)
	{
		cvReleaseImage(&m_image);
	}

	m_image = cvLoadImage(filename);
	if (!m_matchResult)
	{
		cvResizeWindow(CAPTION, m_image->width / 3, m_image->height / 3);
		m_matchResult = cvCreateImage(cvSize(m_image->width - m_imgChessModel->width + 1, m_image->height - m_imgChessModel->height + 1), IPL_DEPTH_32F, 1);
	}
}


CvPoint JumpByJump::LocateChess()
{
	cvMatchTemplate(m_image, m_imgChessModel, m_matchResult, CV_TM_SQDIFF);
	double min_val = 0, max_val = 0;
	CvPoint min_loc, max_loc, chessPt;
	cvMinMaxLoc(m_matchResult, &min_val, &max_val, &min_loc, &max_loc);
// 	cvRectangle(m_image, min_loc, cvPoint(min_loc.x + m_imgChessModel->width, min_loc.y + m_imgChessModel->height), cvScalar(0, 0, 255), 2);
	cout << "棋子当前坐标：" << "(" << min_loc.x << "," << min_loc.y<<")" << endl;

	chessPt = cvPoint(min_loc.x + m_imgChessModel->width / 2, min_loc.y + m_imgChessModel->height);
	return chessPt;
}

void JumpByJump::ShowScreen()
{
	cvShowImage(CAPTION, m_image);
	cvResizeWindow(CAPTION, m_image->width / 3, m_image->height / 3);
}

int main(int argc, char* argv[])
{
	JumpByJump jumper;
	jumper.CarryOut();
	return 0;
}
