#pragma once


// CPPMultidesktop dialog

class CPPMultidesktop : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPMultidesktop)

public:
	CPPMultidesktop();   // standard constructor
	~CPPMultidesktop();

// Dialog Data
	enum { IDD = IDD_PP_MULTIDESK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bEnableMD;
  CToolTipCtrl tooltip;
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnDeltaposSpinEd(NMHDR *pNMHDR, LRESULT *pResult);
public:
	UINT m_iExtraDesktops;
public:
  afx_msg void OnBnClickedScmdOn();
public:
  BOOL m_bContextMenu;
public:
  afx_msg void OnBnClickedContextmenuOn();
  CString m_strRS1;
  CString m_strRS2;
  CString m_strRS3;
  CString m_strRS4;
  virtual BOOL PreTranslateMessage(MSG* pMsg);
};
