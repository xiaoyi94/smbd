#pragma once
#include "afxwin.h"


// smbdDlg �Ի���

class smbdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(smbdDlg)

public:
	smbdDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~smbdDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = smbd };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ͼƬ����
	int picNumber;
	// �궨ͼƬ�߶Ƚǵ�
	int picHig;
	// �궨ͼƬ��Ƚǵ�
	int picWid;
	// �궨���̸��ӵı߳�
	float squareWidth;
	// ���������궨���
	CListBox lOutResult;
	// ���������ı궨���
	CListBox rOutResult;
	// ��ʾ�ǵ��ͼ
	//CStatic m_outImg;
	
	// ���˫Ŀ�궨���
	CListBox bdOut;
	afx_msg void OnBnClickedstrbd();
	afx_msg void OnBnClickedselectpic();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedselectrpic();
	afx_msg void OnBnClickedleftbd();
	afx_msg void OnBnClickedsmbdok();
	afx_msg void OnBnClickedopenimg();
};
