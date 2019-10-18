#pragma once
#include "afxwin.h"


// CWindowSize dialog

class CWindowSizeDlg : public CDialog
{
	DECLARE_DYNAMIC(CWindowSizeDlg)

public:
	CWindowSizeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWindowSizeDlg();

// Dialog Data
	enum { IDD = IDD_WINDOWSIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  CString m_strSize;
  CComboBox m_cPre;
  afx_msg void OnCbnSelchangePresize();
  CString m_strPre;
};
