#pragma once


// bdDlg 对话框

class bdDlg : public CDialog
{
	DECLARE_DYNAMIC(bdDlg)

public:
	bdDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~bdDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMBD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
