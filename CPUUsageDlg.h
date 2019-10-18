#pragma once
#include "linegraph.h"


// CCPUUsageDlg dialog

class CCPUUsageDlg : public CDialog
{
	DECLARE_DYNAMIC(CCPUUsageDlg)

public:
	CCPUUsageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCPUUsageDlg();

// Dialog Data
	enum { IDD = IDD_CPUUSAGEH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CLineGraph graph;
  virtual BOOL OnInitDialog();
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  CString info;
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  void showTimeData(void);
  afx_msg void OnDeltaposTfactor(NMHDR *pNMHDR, LRESULT *pResult);
  SIZE_T used_phy;
  SIZE_T total_phy;
  SIZE_T sys_cache;
  SIZE_T kernel_mem;
  DWORD threads;
  DWORD process;
  DWORD handles;
  SIZE_T pagesize;
};
