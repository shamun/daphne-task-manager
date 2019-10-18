#pragma once

#include "Software.h"
#include "afxwin.h"

// CSoftwarePropertiesDlg dialog

class CSoftwarePropertiesDlg : public CDialog
{
	DECLARE_DYNAMIC(CSoftwarePropertiesDlg)

public:
	CSoftwarePropertiesDlg(Software * soft, CWnd * pParent = NULL);   // standard constructor
	virtual ~CSoftwarePropertiesDlg();

// Dialog Data
	enum { IDD = IDD_SOFTPROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CString title;
//	afx_msg void OnStnClickedVersion();
private:
	Software * soft;
	CString version;
private:
	CString publisher;
private:
	CString website;
private:
	CString raw;
	virtual BOOL OnInitDialog();
private:
	CFont* courier;
	afx_msg void OnBnClickedCancel();
private:
	CEdit rawControl;
  virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
};
