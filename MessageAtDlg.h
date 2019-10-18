#pragma once
#include "atltime.h"
#include "afxdtctl.h"


// CMessageAtDlg dialog

class CMessageAtDlg : public CDialog
{
	DECLARE_DYNAMIC(CMessageAtDlg)

public:
	CMessageAtDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMessageAtDlg();

// Dialog Data
	enum { IDD = IDD_MESSAGEAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  CTime date;
  CTime time;
  CTime fulltime;
  CString message;
  afx_msg void OnDtnDatetimechangeTime(NMHDR *pNMHDR, LRESULT *pResult);
  CDateTimeCtrl ctrl_time;
};
