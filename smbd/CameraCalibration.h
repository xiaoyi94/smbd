#pragma once

#include "cv.h"
#include "highgui.h"

class CameraCalibration 
{
public:
	CameraCalibration();
	virtual ~CameraCalibration();

private:


public:
	void InitCorners3D(CvMat *Corners3D, CvSize ChessBoardSize, int Nimages, float SquareSize);
	void calibration(int ChessBoardSize_w, int ChessBoardSize_h, const int NImages,float    SquareWidth);
	void calibration2(int ChessBoardSize_w, int ChessBoardSize_h, const int NImages, float    SquareWidth);
	void smcalib();
};
