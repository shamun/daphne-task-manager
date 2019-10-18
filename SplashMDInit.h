#pragma once


// CSplashMDInit dialog

class CSplashMDInit : public CDialog
{
	DECLARE_DYNAMIC(CSplashMDInit)

public:
	CSplashMDInit(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSplashMDInit();

// Dialog Data
	enum { IDD = IDD_SPLASH_DESKTOP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	CString strText;
public:
	void SetMessage(CString text);
};
