#if !defined(AFX_NEWTRAPDLG_H__ED669C2C_2653_4B22_808A_6A08BEE17815__INCLUDED_)
#define AFX_NEWTRAPDLG_H__ED669C2C_2653_4B22_808A_6A08BEE17815__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewTrapDlg.h : header file
//
class CTrap;

/////////////////////////////////////////////////////////////////////////////
// CNewTrapDlg dialog

class CNewTrapDlg : public CDialog
{
// Construction
public:
	CNewTrapDlg(CWnd* pParent = NULL);   // standard constructor
	CTrap * trap;

// Dialog Data
	//{{AFX_DATA(CNewTrapDlg)
	enum { IDD = IDD_NEW_TRAP };
	CStatic	m_lblWContains;
	CEdit	m_cWContains;
	CComboBox	m_cTrap;
	CString	m_strName;
	int		m_iType;
	CString	m_strWContains;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewTrapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

  CToolTipCtrl tooltip;

// Implementation
protected:
	void updateGUI();

	// Generated message map functions
	//{{AFX_MSG(CNewTrapDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeTrap();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
  CString m_stri64;
  virtual BOOL PreTranslateMessage(MSG* pMsg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWTRAPDLG_H__ED669C2C_2653_4B22_808A_6A08BEE17815__INCLUDED_)
