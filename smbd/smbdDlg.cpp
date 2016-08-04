// smbdDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "smbd.h"
#include "smbdDlg.h"
#include "afxdialogex.h"
# include <string>
#include <cstdlib>
#include "CameraCalibration.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
using namespace std;

CameraCalibration *pCalib = new CameraCalibration;
// smbdDlg 对话框

IMPLEMENT_DYNAMIC(smbdDlg, CDialogEx)

smbdDlg::smbdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(smbd, pParent)
	, picNumber(0)
	, picHig(0)
	, picWid(0)
	, squareWidth(0)
{

}

smbdDlg::~smbdDlg()
{
}

void smbdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, picNum, picNumber);
	DDX_Text(pDX, picHigh, picHig);
	DDX_Text(pDX, picWidth, picWid);
	DDX_Control(pDX, outRes, lOutResult);
	DDX_Control(pDX, rOutRes, rOutResult);
	//DDX_Control(pDX, IDC_tupian, m_outImg);
	DDX_Text(pDX, squerWidth, squareWidth);
	DDX_Control(pDX, bdout, bdOut);
}


BEGIN_MESSAGE_MAP(smbdDlg, CDialogEx)
	ON_BN_CLICKED(strBd, &smbdDlg::OnBnClickedstrbd)
	ON_BN_CLICKED(selectPic, &smbdDlg::OnBnClickedselectpic)
	ON_BN_CLICKED(IDC_BUTTON4, &smbdDlg::OnBnClickedButton4)
	ON_BN_CLICKED(selectRpic, &smbdDlg::OnBnClickedselectrpic)
	ON_BN_CLICKED(leftBd, &smbdDlg::OnBnClickedleftbd)
	ON_BN_CLICKED(smbdok, &smbdDlg::OnBnClickedsmbdok)
	ON_BN_CLICKED(openImg, &smbdDlg::OnBnClickedopenimg)
END_MESSAGE_MAP()


// smbdDlg 消息处理程序

//左相机标定
void smbdDlg::OnBnClickedstrbd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	lOutResult.ResetContent();
	
	if (picWid > 0&&  picHig>0 && picNumber>0 && squareWidth>0) {

		pCalib->calibration(picWid, picHig, picNumber, squareWidth);
		CString res;
		ifstream fin("Lbdresult.txt");
		string  s;
		while (getline(fin, s))
		{
			res=s.c_str();
			lOutResult.AddString(res);
		}	
	}
	else {
		AfxMessageBox("数据输入错误，请重新输入");
	}

	/*
	CStdioFile m_file("bdresult.txt", CFile::modeRead);
	CString contents;
	while (m_file.ReadString(contents))
	{
		outResult.AddString(contents);
	}
	
	std::ifstream fin("end.txt", std::ios::in);
	char line[100] = { 0 };
	while (fin.getline(line, sizeof(line)))
	{
		std::stringstream word(line);
		res = line;
		outResult.AddString(res);
	}
	fin.clear();
	fin.close();
	
	HBITMAP phBmp = (HBITMAP)LoadImage(NULL, _T("角点绘图.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	
		if (phBmp)
		{
			m_outImg.SetBitmap(phBmp);
		}
		
		UpdateData(FALSE);
		*/
	//outResult.SetWindowTextW(buffer);

	
}

//选择左相机图片
void smbdDlg::OnBnClickedselectpic()
{
	// TODO: 在此添加控件通知处理程序代码


	UpdateData(true);
	CFileDialog filedlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | 
						OFN_OVERWRITEPROMPT | 
						OFN_ALLOWMULTISELECT, _T("所有文件(*.*)|*.*"));

	TCHAR *pBuffer = new TCHAR[MAX_PATH * 20];//最多允许同时打开20个文件
	filedlg.m_ofn.lpstrFile = pBuffer;
	filedlg.m_ofn.nMaxFile = MAX_PATH * 20;
	filedlg.m_ofn.lpstrFile[0] = '\0';

	if (filedlg.DoModal() == IDOK)
	{
		CString cstrfilepath=_T("");
		POSITION pos = filedlg.GetStartPosition();
		HANDLE hFile = CreateFile("Limages.txt",
						GENERIC_WRITE, 
						FILE_SHARE_WRITE,
						NULL, 
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL, 
						NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			AfxMessageBox("could not create file Rimages.txt");
		}
		CloseHandle(hFile);
		ofstream out;
		out.open("Limages.txt");
		
		while (pos != NULL)
		{
			CString cstrfilepath =filedlg.GetNextPathName(pos);//取得文件路径
			out << cstrfilepath << endl;	//将文件路径写入文件
		}
		out.close();
	}
}

//选择右相机图片
void smbdDlg::OnBnClickedselectrpic()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CFileDialog filedlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | 
						OFN_OVERWRITEPROMPT | 
						OFN_ALLOWMULTISELECT, _T("所有文件(*.*)|*.*"));

	TCHAR *pBuffer = new TCHAR[MAX_PATH * 20];//最多允许同时打开20个文件
	filedlg.m_ofn.lpstrFile = pBuffer;
	filedlg.m_ofn.nMaxFile = MAX_PATH * 20;
	filedlg.m_ofn.lpstrFile[0] = '\0';

	if (filedlg.DoModal() == IDOK)
	{
		CString cstrfilepath = _T("");
		POSITION pos = filedlg.GetStartPosition();
		HANDLE hFile = CreateFile("Rimages.txt",
			GENERIC_WRITE, 
			FILE_SHARE_WRITE, 
			NULL, 
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, 
			NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			AfxMessageBox("could not create file Rimages.txt");
		}
		CloseHandle(hFile);
		ofstream out;
		out.open("Rimages.txt");

		while (pos != NULL)
		{
			CString cstrfilepath = filedlg.GetNextPathName(pos);//取得文件路径
			out << cstrfilepath << endl;	//将文件路径写入文件
		}
		out.close();
	}
}

//右相机标定
void smbdDlg::OnBnClickedleftbd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	rOutResult.ResetContent();
	if (picWid > 0 && picHig>0 && picNumber>0 && squareWidth>0) {
	 pCalib->calibration2(picWid, picHig, picNumber, squareWidth);

	CString res;
	ifstream fin("Rbdresult.txt");
	string  s;
	while (getline(fin, s))
	{
		res = s.c_str();
		rOutResult.AddString(res);
	}
	}
	else {
		AfxMessageBox("数据输入错误，请重新输入");
	}
	UpdateData(FALSE);
}
//双目标定
void smbdDlg::OnBnClickedsmbdok()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	bdOut.ResetContent();
	pCalib->smcalib();
	CString res;
	ifstream fin("result.txt");
	string  s;
	while (getline(fin, s))
	{
		res = s.c_str();
		bdOut.AddString(res);
	}
	UpdateData(FALSE);
}

void smbdDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::EndDialog(smbd);
}




void smbdDlg::OnBnClickedopenimg()
{
	// TODO: 在此添加控件通知处理程序代码
}
