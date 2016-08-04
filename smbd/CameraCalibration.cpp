#include "stdafx.h"
#include "CameraCalibration.h"
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <Windows.h>
#include <cxcore.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
using namespace std;

CameraCalibration::CameraCalibration()
{

}

CameraCalibration::~CameraCalibration()
{

}
double lR[3] = { 1.9575, 1.7735, -0.1596 };
double lT[3] = { -60.8771, -82.6107, 676.0082 };
double rR[3] = { 1.9617, 1.7684, -0.1508 };
double rT[3] = { -192.1040, -85.1017, 673.5092 };
void CameraCalibration::calibration(int ChessBoardSize_w, int ChessBoardSize_h,  
											int NImages,float    SquareWidth)
{
	int image_width = 640;
	int image_height = 480;//���궨ͼƬ�Ĵ�С
	//float    SquareWidth = 30; //���̸��ӵı߳����������趨����Ӱ���ڲ���
	CvMat *intrinsics;
	CvMat *distortion_coeff;

	CvMat *rotation_vectors;
	CvMat *translation_vectors;
	CvMat *object_points;
	CvMat *point_counts;
	CvMat *image_points;
	CvSize  ChessBoardSize = cvSize(ChessBoardSize_w, ChessBoardSize_h);
    int  NPoints = ChessBoardSize_w*ChessBoardSize_h;//�ǵ����
	int corner_count[8] = { 0 };; //int corner_count[NImages] = { 0 };
	
	IplImage     *current_frame_rgb;
	IplImage     *current_frame_gray;
	IplImage     *chessBoard_Img;
	CvPoint2D32f corners[48*8];		//CvPoint2D32f corners[NPoints*NImages];

	chessBoard_Img = cvCreateImage(cvSize(image_width, image_height), IPL_DEPTH_8U, 3);
	current_frame_gray = cvCreateImage(cvSize(image_width, image_height), IPL_DEPTH_8U, 1);
	current_frame_rgb = cvCreateImage(cvSize(image_width, image_height), IPL_DEPTH_8U, 3);

	int captured_frames = 0;
	std::ifstream fin("Limages.txt", std::ios::in);
	char filename[100] = { 0 };
	while (fin.getline(filename, sizeof(filename)))
	{
		std::stringstream word(filename);
		chessBoard_Img = cvLoadImage(filename, 1);
		cvCvtColor(chessBoard_Img, current_frame_gray, CV_BGR2GRAY);
		cvCopy(chessBoard_Img, current_frame_rgb);

		int find_corners_result;
		find_corners_result =
			cvFindChessboardCorners(current_frame_gray,
				ChessBoardSize,
				&corners[captured_frames*NPoints],
				&corner_count[captured_frames],
				CV_CALIB_CB_ADAPTIVE_THRESH);
		cvFindCornerSubPix(current_frame_gray,
			&corners[captured_frames*NPoints],
			NPoints, cvSize(11, 11), cvSize(-1, -1),
			cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.01));
		cvDrawChessboardCorners(current_frame_rgb, ChessBoardSize, //���Ƽ�⵽�����̽ǵ�
			&corners[captured_frames*NPoints],
			NPoints,
			find_corners_result);

		cvShowImage("Window 0", chessBoard_Img);
		cvSaveImage("��ǵ��ͼ.bmp", current_frame_rgb);
		cvNamedWindow("result", 1);
		cvShowImage("result", current_frame_rgb);
		captured_frames++;
		cvWaitKey(100);
	
	}
	fin.clear();
	fin.close();
	//��������
	intrinsics = cvCreateMat(3, 3, CV_32FC1);
	distortion_coeff = cvCreateMat(1, 4, CV_32FC1);
	rotation_vectors = cvCreateMat(NImages, 3, CV_32FC1);
	translation_vectors = cvCreateMat(NImages, 3, CV_32FC1);
	point_counts = cvCreateMat(NImages, 1, CV_32SC1);
	object_points = cvCreateMat(NImages*NPoints, 3, CV_32FC1);
	image_points = cvCreateMat(NImages*NPoints, 2, CV_32FC1);

	//��2ά��ת������ά�㣨object_points���������

	InitCorners3D(object_points, ChessBoardSize, NImages, SquareWidth);
	cvSetData(image_points, corners, sizeof(CvPoint2D32f));
	cvSetData(point_counts, &corner_count, sizeof(int));



	//�������
	cvCalibrateCamera2(object_points,
		image_points,
		point_counts,
		cvSize(image_width, image_height),
		intrinsics,
		distortion_coeff,
		rotation_vectors,
		translation_vectors,
		0);

	float intr[3][3] = { 0.0 };
	float dist[4] = { 0.0 };
	float tranv[3] = { 0.0 };
	float rotv[3] = { 0.0 };


	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			intr[i][j] = ((float*)(intrinsics->data.ptr + intrinsics->step*i))[j];
		
		}
		dist[i] = ((float*)(distortion_coeff->data.ptr))[i];
		tranv[i] = ((float*)(translation_vectors->data.ptr))[i];
		lT[i] = tranv[i];
		rotv[i] = ((float*)(rotation_vectors->data.ptr))[i];
		lR[i] = rotv[i];
	}
	dist[3] = ((float*)(distortion_coeff->data.ptr))[3];
	 
	HANDLE hFile = CreateFile("Lbdresult.txt",
		GENERIC_WRITE, 
		FILE_SHARE_WRITE,
		NULL, 
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("could not create file Lbdresult.txt ");
	}
		CloseHandle(hFile);
		ofstream out;
		out.open("Lbdresult.txt");
		out << "-------------------------------------------------- \n";
		out << "�ڲ�������\n"; //"INTRINSIC MATRIX:  \n";
		out.precision(4);
		out.setf(ios::fixed);
		out<<"[ "<< intr[0][0]<<", "<<intr[0][1] << ", " <<intr[0][2]<< " ]\n";
		out<<"[ "<< intr[1][0]<<", "<<intr[1][1] << ", " <<intr[1][2]<< " ]\n";
		out<<"[ "<< intr[2][0]<<", "<<intr[2][1] << ", " <<intr[2][2]<< " ]\n";
		out<<"---------------------------------------------------- \n";
		out<<"����ϵ��:  \n";  //"DISTORTION VECTOR:  \n";
		out<< "[ "<< dist[0] << ", " << dist[1] << ", " <<dist[2] << ", " <<dist[3] << " ]\n";
		out<<"---------------------------------------------------- \n";
		out<<"��һ��ͼ����ת����:  \n";  //"ROTATION VECTOR:  \n";
		out<< "[ " << rotv[0] << ", " <<rotv[1] << ", " <<rotv[2] << " ]\n";
		out<<"��һ��ͼ��ƽ������:  \n";  //"TRANSLATION VECTOR:  \n";
		out<< "[ " << tranv[0] << ", " <<tranv[1] << ", " <<tranv[2] << " ]\n";
		out<<"---------------------------------------------------- \n";
		out.close();
	cvReleaseMat(&intrinsics);
	cvReleaseMat(&distortion_coeff);
	cvReleaseMat(&rotation_vectors);
	cvReleaseMat(&translation_vectors);
	cvReleaseMat(&point_counts);
	cvReleaseMat(&object_points);
	cvReleaseMat(&image_points);
	cvDestroyAllWindows();
	

}
void CameraCalibration::calibration2(int ChessBoardSize_w, int ChessBoardSize_h, int NImages, float    SquareWidth)
{
	int image_width = 640;
	int image_height = 480;//���궨ͼƬ�Ĵ�С
						   //float    SquareWidth = 30; //���̸��ӵı߳����������趨����Ӱ���ڲ���
	CvMat *intrinsics;
	CvMat *distortion_coeff;

	CvMat *rotation_vectors;
	CvMat *translation_vectors;
	CvMat *object_points;
	CvMat *point_counts;
	CvMat *image_points;
	CvSize  ChessBoardSize = cvSize(ChessBoardSize_w, ChessBoardSize_h);
	int  NPoints = ChessBoardSize_w*ChessBoardSize_h;//�ǵ����
	int corner_count[8] = { 0 };; 

	IplImage     *current_frame_rgb;
	IplImage     *current_frame_gray;
	IplImage     *chessBoard_Img;
	CvPoint2D32f corners[48 * 8];		

	chessBoard_Img = cvCreateImage(cvSize(image_width, image_height), IPL_DEPTH_8U, 3);
	current_frame_gray = cvCreateImage(cvSize(image_width, image_height), IPL_DEPTH_8U, 1);
	current_frame_rgb = cvCreateImage(cvSize(image_width, image_height), IPL_DEPTH_8U, 3);

	int captured_frames = 0;
	std::ifstream fin("Rimages.txt", std::ios::in);
	char filename[100] = { 0 };
	while (fin.getline(filename, sizeof(filename)))
	{
		std::stringstream word(filename);
		chessBoard_Img = cvLoadImage(filename, 1);
		cvCvtColor(chessBoard_Img, current_frame_gray, CV_BGR2GRAY);
		cvCopy(chessBoard_Img, current_frame_rgb);

		int find_corners_result;
		find_corners_result =
			cvFindChessboardCorners(current_frame_gray,
				ChessBoardSize,
				&corners[captured_frames*NPoints],
				&corner_count[captured_frames],//int corner_count[NImages] = {0};
				CV_CALIB_CB_ADAPTIVE_THRESH);
		cvFindCornerSubPix(current_frame_gray,
			&corners[captured_frames*NPoints],
			NPoints, cvSize(11, 11), cvSize(-1, -1),
			cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.01));
		cvDrawChessboardCorners(current_frame_rgb, ChessBoardSize, //���Ƽ�⵽�����̽ǵ�
			&corners[captured_frames*NPoints],
			NPoints,
			find_corners_result);

		cvShowImage("Window 0", chessBoard_Img);
		cvSaveImage("�ҽǵ��ͼ.bmp", current_frame_rgb);
		cvNamedWindow("result", 1);
		cvShowImage("result", current_frame_rgb);
		captured_frames++;
		cvWaitKey(100);

	}
	fin.clear();
	fin.close();
	
	intrinsics = cvCreateMat(3, 3, CV_32FC1);
	distortion_coeff = cvCreateMat(1, 4, CV_32FC1);
	rotation_vectors = cvCreateMat(NImages, 3, CV_32FC1);
	translation_vectors = cvCreateMat(NImages, 3, CV_32FC1);
	point_counts = cvCreateMat(NImages, 1, CV_32SC1);
	object_points = cvCreateMat(NImages*NPoints, 3, CV_32FC1);
	image_points = cvCreateMat(NImages*NPoints, 2, CV_32FC1);
	
	//��2ά��ת������ά�㣨object_points���������
	InitCorners3D(object_points, ChessBoardSize, NImages, SquareWidth);
	cvSetData(image_points, corners, sizeof(CvPoint2D32f));
	cvSetData(point_counts, &corner_count, sizeof(int));
	
	
	//�������
	cvCalibrateCamera2(object_points,
		image_points,
		point_counts,
		cvSize(image_width, image_height),
		intrinsics,
		distortion_coeff,
		rotation_vectors,
		translation_vectors,
		0);

	float intr[3][3] = { 0.0 };
	float dist[4] = { 0.0 };
	float tranv[3] = { 0.0 };
	float rotv[3] = { 0.0 };


	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			intr[i][j] = ((float*)(intrinsics->data.ptr + intrinsics->step*i))[j];

		}
		dist[i] = ((float*)(distortion_coeff->data.ptr))[i];
		tranv[i] = ((float*)(translation_vectors->data.ptr))[i];
		rotv[i] = ((float*)(rotation_vectors->data.ptr))[i];
		rT[i] = tranv[i];
		rR[i] = rotv[i];
	}
	dist[3] = ((float*)(distortion_coeff->data.ptr))[3];

	HANDLE hFile = CreateFile("Rbdresult.txt",
		GENERIC_WRITE, 
		FILE_SHARE_WRITE,
		NULL, 
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("could not create file Rbdresult.txt ");
	}
	CloseHandle(hFile);
	ofstream out;
	out.open("Rbdresult.txt");
	out << "-------------------------------------------------- \n";
	out << "�ڲ�������\n"; //"INTRINSIC MATRIX:  \n";
	out.precision(4);
	out.setf(ios::fixed);
	out << "[ " << intr[0][0] << ", " << intr[0][1] << ", " << intr[0][2] << " ]\n";
	out << "[ " << intr[1][0] << ", " << intr[1][1] << ", " << intr[1][2] << " ]\n";
	out << "[ " << intr[2][0] << ", " << intr[2][1] << ", " << intr[2][2] << " ]\n";
	out << "---------------------------------------------------- \n";
	out << "����ϵ��:  \n";  //"DISTORTION VECTOR:  \n";
	out << "[ " << dist[0] << ", " << dist[1] << ", " << dist[2] << ", " << dist[3] << " ]\n";
	out << "---------------------------------------------------- \n";
	out << "��һ��ͼ����ת����:  \n";  //"ROTATION VECTOR:  \n";
	out << "[ " << rotv[0] << ", " << rotv[1] << ", " << rotv[2] << " ]\n";
	out << "��һ��ͼ��ƽ������:  \n";  //"TRANSLATION VECTOR:  \n";
	out << "[ " << tranv[0] << ", " << tranv[1] << ", " << tranv[2] << " ]\n";
	out << "---------------------------------------------------- \n";
	out.close();
	cvReleaseMat(&intrinsics);
	cvReleaseMat(&distortion_coeff);
	cvReleaseMat(&rotation_vectors);
	cvReleaseMat(&translation_vectors);
	cvReleaseMat(&point_counts);
	cvReleaseMat(&object_points);
	cvReleaseMat(&image_points);
	cvDestroyAllWindows();


}
void CameraCalibration::InitCorners3D(CvMat *Corners3D, CvSize ChessBoardSize, int NImages, float SquareSize)
{
	int CurrentImage = 0;
	int CurrentRow = 0;
	int CurrentColumn = 0;
	int NPoints = ChessBoardSize.height*ChessBoardSize.width;
	float * temppoints = new float[NImages*NPoints * 3];

	// for now, assuming we're row-scanning
	for (CurrentImage = 0; CurrentImage < NImages; CurrentImage++)
	{
		for (CurrentRow = 0; CurrentRow < ChessBoardSize.height; CurrentRow++)
		{
			for (CurrentColumn = 0; CurrentColumn < ChessBoardSize.width; CurrentColumn++)
			{
				temppoints[(CurrentImage*NPoints * 3) + (CurrentRow*ChessBoardSize.width +
					CurrentColumn) * 3] = (float)CurrentRow*SquareSize;
				temppoints[(CurrentImage*NPoints * 3) + (CurrentRow*ChessBoardSize.width +
					CurrentColumn) * 3 + 1] = (float)CurrentColumn*SquareSize;
				temppoints[(CurrentImage*NPoints * 3) + (CurrentRow*ChessBoardSize.width +
					CurrentColumn) * 3 + 2] = 0.f;
			}
		}
	}
	(*Corners3D) = cvMat(NImages*NPoints, 3, CV_32FC1, temppoints);
}
//˫Ŀ�궨
void CameraCalibration::smcalib() 
{
	double lr[3][3];
	double rr[3][3];
	double rrni[3][3];
	double R[3][3];
	double TR[3];
	double T[3];

	//ת������ת����
	double L_matrix[9];
	CvMat pl_vec;
	CvMat pL_matrix;
	cvInitMatHeader(&pl_vec, 1, 3, CV_64FC1, lR, CV_AUTOSTEP);
	cvInitMatHeader(&pL_matrix, 3, 3, CV_64FC1, L_matrix, CV_AUTOSTEP);
	cvRodrigues2(&pl_vec, &pL_matrix, 0);
	for (int i = 0; i<3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int z = i * 3 + j;
			lr[i][j] = L_matrix[z];
		}
	}
	//ת������ת����
	double R_matrix[9];
	CvMat pr_vec;
	CvMat pR_matrix;
	cvInitMatHeader(&pr_vec, 1, 3, CV_64FC1, rR, CV_AUTOSTEP);
	cvInitMatHeader(&pR_matrix, 3, 3, CV_64FC1, R_matrix, CV_AUTOSTEP);
	cvRodrigues2(&pr_vec, &pR_matrix, 0);
	for (int i = 0; i<3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int z = i * 3 + j;
			rr[i][j] = R_matrix[z];
			cout << rr[i][j] << " ,";
		}
		cout << endl;
	}
	double result[3][3];
	double b[3][6];
	for (int i = 0; i<3; i++)
	{
		for (int j = 0; j<3; j++)
		{
			b[i][j] = rr[i][j];
		}
	}
	for (int i = 0; i<3; i++)
	{
		for (int j = 3; j<6; j++)
		{
			if (i == (j - 3))
			{
				b[i][j] = 1;
			}
			else
			{
				b[i][j] = 0;
			}
		}
	} for (int i = 0; i<3; i++)
	{
		if (b[i][i] == 0)
		{
			for (int k = i; k<3; k++)
			{
				if (b[k][k] != 0)
				{
					for (int j = 0; j<6; j++)
					{
						double temp;
						temp = b[i][j];
						b[i][j] = b[k][j];
						b[k][j] = temp;
					}
					break;
				}
			}

		}
		for (int j = 5; j >= i; j--)
		{
			b[i][j] /= b[i][i];
		}
		for (int k = 0; k<3; k++)
		{
			if (k != i)
			{
				double temp = b[k][i];
				for (int j = 0; j<6; j++)
				{
					b[k][j] -= temp*b[i][j];
				}
			}
		}
	} for (int i = 0; i<3; i++)
	{
		for (int j = 3; j<6; j++)
		{
			result[i][j - 3] = b[i][j];
		}
	}
	for (int i = 0; i<3; i++)
	{
		for (int j = 0; j<3; j++)
		{
			rrni[i][j] = result[i][j];
		}
		
	}
	//	����ת����
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++) {
			R[i][j] = 0;
			for (int k = 0; k < 3; k++)
				R[i][j] = R[i][j] + lr[i][k] * rrni[k][j];
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 1; j++) {
			TR[i] = 0;
			for (int k = 0; k < 3; k++)
				TR[i] = TR[i] + R[i][k] * rT[k];
		}
	}
	//��ƽ�ƾ���
	for (int i = 0; i < 3; i++)
	{
		T[i] = lT[i] - TR[i];
	}
	HANDLE hFile = CreateFile("result.txt",
		GENERIC_WRITE, FILE_SHARE_WRITE,
		NULL, 
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("could not create file result.txt ");
	}
	CloseHandle(hFile);
	ofstream out;
	out.open("result.txt");
	out << "-------------------------------------------------- \n";
	out << "��������ת����\n"; //"INTRINSIC MATRIX:  \n";
	out.precision(4);
	out.setf(ios::fixed);
	out << "[ " << R[0][0] << ", " << R[0][1] << ", " << R[0][2] << " ]\n";
	out << "[ " << R[1][0] << ", " << R[1][1] << ", " << R[1][2] << " ]\n";
	out << "[ " << R[2][0] << ", " << R[2][1] << ", " << R[2][2] << " ]\n";
	out << "---------------------------------------------------- \n";
	out << "������ƽ�ƾ���:  \n";  //"DISTORTION VECTOR:  \n";
	out << "[ " << T[0] << ", " << T[1] << ", " << T[2]   << " ]\n";
	out << "---------------------------------------------------- \n";
	out.close();
}