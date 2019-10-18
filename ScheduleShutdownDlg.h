#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CScheduleShutdownDlg dialog

class CScheduleShutdownDlg : public CDialog
{
	DECLARE_DYNAMIC(CScheduleShutdownDlg)

public:
	CScheduleShutdownDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CScheduleShutdownDlg();

// Dialog Data
	enum { IDD = IDD_SHUTDOWN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedOk();
  CTime date;
  CTime time;
  CTime fulltime;
  UINT type;
  virtual BOOL OnInitDialog();
  CComboBox type_ctrl;
  afx_msg void OnDtnDatetimechangeTime(NMHDR *pNMHDR, LRESULT *pResult);
  CDateTimeCtrl ctrl_time;
};
