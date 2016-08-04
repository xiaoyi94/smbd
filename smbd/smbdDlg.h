#pragma once
#include "afxwin.h"


// smbdDlg 对话框

class smbdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(smbdDlg)

public:
	smbdDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~smbdDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = smbd };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 图片数量
	int picNumber;
	// 标定图片高度角点
	int picHig;
	// 标定图片宽度角点
	int picWid;
	// 标定棋盘格子的边长
	float squareWidth;
	// 输出左相机标定结果
	CListBox lOutResult;
	// 输出右相机的标定结果
	CListBox rOutResult;
	// 显示角点绘图
	//CStatic m_outImg;
	
	// 输出双目标定结果
	CListBox bdOut;
	afx_msg void OnBnClickedstrbd();
	afx_msg void OnBnClickedselectpic();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedselectrpic();
	afx_msg void OnBnClickedleftbd();
	afx_msg void OnBnClickedsmbdok();
	afx_msg void OnBnClickedopenimg();
};
