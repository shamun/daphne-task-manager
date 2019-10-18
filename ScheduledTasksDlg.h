#pragma once
#include "afxcmn.h"


// CScheduledTasksDlg dialog

class CScheduledTasksDlg : public CDialog
{
	DECLARE_DYNAMIC(CScheduledTasksDlg)

public:
	CScheduledTasksDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CScheduledTasksDlg();

// Dialog Data
	enum { IDD = IDD_SCHEDULED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
  void reload();
	afx_msg void OnRclickList(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
public:
  static int CALLBACK compare(LPARAM left, LPARAM right, LPARAM sort);
  Task * GetSelectedTask();
  virtual BOOL OnInitDialog();
  CListCtrl tasks;
  TaskMap * schedule;
};
