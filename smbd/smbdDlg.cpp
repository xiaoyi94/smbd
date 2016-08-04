// smbdDlg.cpp : ʵ���ļ�
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
// smbdDlg �Ի���

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


// smbdDlg ��Ϣ�������

//������궨
void smbdDlg::OnBnClickedstrbd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		AfxMessageBox("���������������������");
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
	
	HBITMAP phBmp = (HBITMAP)LoadImage(NULL, _T("�ǵ��ͼ.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	
		if (phBmp)
		{
			m_outImg.SetBitmap(phBmp);
		}
		
		UpdateData(FALSE);
		*/
	//outResult.SetWindowTextW(buffer);

	
}

//ѡ�������ͼƬ
void smbdDlg::OnBnClickedselectpic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	UpdateData(true);
	CFileDialog filedlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | 
						OFN_OVERWRITEPROMPT | 
						OFN_ALLOWMULTISELECT, _T("�����ļ�(*.*)|*.*"));

	TCHAR *pBuffer = new TCHAR[MAX_PATH * 20];//�������ͬʱ��20���ļ�
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
			CString cstrfilepath =filedlg.GetNextPathName(pos);//ȡ���ļ�·��
			out << cstrfilepath << endl;	//���ļ�·��д���ļ�
		}
		out.close();
	}
}

//ѡ�������ͼƬ
void smbdDlg::OnBnClickedselectrpic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CFileDialog filedlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | 
						OFN_OVERWRITEPROMPT | 
						OFN_ALLOWMULTISELECT, _T("�����ļ�(*.*)|*.*"));

	TCHAR *pBuffer = new TCHAR[MAX_PATH * 20];//�������ͬʱ��20���ļ�
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
			CString cstrfilepath = filedlg.GetNextPathName(pos);//ȡ���ļ�·��
			out << cstrfilepath << endl;	//���ļ�·��д���ļ�
		}
		out.close();
	}
}

//������궨
void smbdDlg::OnBnClickedleftbd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		AfxMessageBox("���������������������");
	}
	UpdateData(FALSE);
}
//˫Ŀ�궨
void smbdDlg::OnBnClickedsmbdok()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::EndDialog(smbd);
}




void smbdDlg::OnBnClickedopenimg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
