#pragma once


// bdDlg �Ի���

class bdDlg : public CDialog
{
	DECLARE_DYNAMIC(bdDlg)

public:
	bdDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~bdDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMBD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
