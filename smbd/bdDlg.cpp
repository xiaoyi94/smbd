// bdDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "smbd.h"
#include "bdDlg.h"
#include "afxdialogex.h"


// bdDlg 对话框

IMPLEMENT_DYNAMIC(bdDlg, CDialog)

bdDlg::bdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SMBD_DIALOG, pParent)
{

}

bdDlg::~bdDlg()
{
}

void bdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(bdDlg, CDialog)
END_MESSAGE_MAP()


// bdDlg 消息处理程序
