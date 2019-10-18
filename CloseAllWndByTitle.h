#pragma once


// CCloseAllWndByTitle dialog

class CCloseAllWndByTitle : public CDialog
{
	DECLARE_DYNAMIC(CCloseAllWndByTitle)

public:
	CCloseAllWndByTitle(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCloseAllWndByTitle();

// Dialog Data
	enum { IDD = IDD_WNDCLOSEALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
public:
	BOOL m_bCase;
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
};
