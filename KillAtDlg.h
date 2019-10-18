#pragma once
#include "afxdtctl.h"


// CKillAtDlg dialog

class CKillAtDlg : public CDialog
{
	DECLARE_DYNAMIC(CKillAtDlg)

public:
	CKillAtDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CKillAtDlg();

// Dialog Data
	enum { IDD = IDD_KILLAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
  virtual BOOL OnInitDialog();
  CString processname;
  CTime date;
  CTime time;
  CTime fulltime;
  BOOL polite;
  afx_msg void OnBnClickedOk();
public:
  CString pname;
  afx_msg void OnDtnDatetimechangeTime(NMHDR *pNMHDR, LRESULT *pResult);
  CDateTimeCtrl ctrl_time;
};
