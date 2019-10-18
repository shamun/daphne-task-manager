#pragma once

#include "resource.h"
#include "afxwin.h"

// CPPLogging dialog

class CPPLogging : public CPropertyPage
{
	DECLARE_DYNAMIC(CPPLogging)

public:
	CPPLogging();
	virtual ~CPPLogging();

// Dialog Data
	enum { IDD = IDD_PP_LOGGING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  BOOL logProcess;
public:
  CString logProcessFile;
public:
  virtual BOOL OnInitDialog();
public:
  afx_msg void OnBnClickedPplLogps();
public:
  virtual void OnOK();
protected:
  void refreshGUI(void);
public:
  CEdit cLogProcessFile;
public:
  CButton cLogProcessFileSelector;
public:
  afx_msg void OnBnClickedPplLogp();
};
