#pragma once
#include "afxcmn.h"


// CAffinityDlg dialog

class CAffinityDlg : public CDialog
{
	DECLARE_DYNAMIC(CAffinityDlg)

public:
	CAffinityDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAffinityDlg();

// Dialog Data
	enum { IDD = IDD_AFFINITY };
	CProcess * m_pProcess;
  TrapList * m_traps;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
  afx_msg void OnLvnItemchangedCpulist(NMHDR *pNMHDR, LRESULT *pResult);
  CListCtrl m_CPUList;
  afx_msg void OnBnClickedCpuall();
  afx_msg void OnBnClickedCpunone();

private:
  ULONG64 m_process, m_system;
public:
  afx_msg void OnBnClickedOk();
  BOOL trap_it;
};
