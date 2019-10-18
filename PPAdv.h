#if !defined(AFX_PPADV_H__5B12B8C1_1A4D_4A37_B654_6C95250F0AD3__INCLUDED_)
#define AFX_PPADV_H__5B12B8C1_1A4D_4A37_B654_6C95250F0AD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPAdv.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPPAdv dialog

class CPPAdv : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPAdv)

// Construction
public:
	CPPAdv();
	~CPPAdv();

// Dialog Data
	//{{AFX_DATA(CPPAdv)
	enum { IDD = IDD_PP_ADVANCED };
	CButton	m_cCtrl;
	CButton	m_cShift;
	CButton	m_cAlt;
	CComboBox	m_cVK;
	BOOL	m_bAlt;
	BOOL	m_bShift;
	BOOL	m_bSC;
	BOOL	m_bShowHidden;
	BOOL	m_bCtrl;
	BOOL	m_bJavaHide;
	BOOL	m_bDontCheckDebug;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPAdv)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPAdv)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnScOn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedStoreIni();
public:
	afx_msg void OnBnClickedStoreReg();
public:
	CString m_strStoreCfgText;
public:
	void ChangeSettingLocation(void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPADV_H__5B12B8C1_1A4D_4A37_B654_6C95250F0AD3__INCLUDED_)
