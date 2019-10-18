#pragma once
#include "afxcmn.h"


// CProcessTreeDlg dialog

class CProcessTreeDlg : public CDialog
{
	DECLARE_DYNAMIC(CProcessTreeDlg)

public:
	CProcessTreeDlg(ProcInfoMap *p, CWnd* pParent = NULL);   // standard constructor
	virtual ~CProcessTreeDlg();

// Dialog Data
	enum { IDD = IDD_PTREE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl treeControl;
public:
	ProcInfoMap processes;
public:
	virtual BOOL OnInitDialog();
public:
  afx_msg void OnNMDblclkTree(NMHDR *pNMHDR, LRESULT *pResult);
public:
  CProcess* selectedProcess;
};
