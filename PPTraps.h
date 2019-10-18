#if !defined(AFX_PPTRAPS_H__548B06EA_69D3_41D7_A73A_FFF9A7D3192E__INCLUDED_)
#define AFX_PPTRAPS_H__548B06EA_69D3_41D7_A73A_FFF9A7D3192E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPTraps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPPTraps dialog

class CPPTraps : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPTraps)

// Construction
public:
	CPPTraps();
	~CPPTraps();
	TrapList * traps;

// Dialog Data
	//{{AFX_DATA(CPPTraps)
	enum { IDD = IDD_PP_TRAPS };
	CListCtrl	m_cTraps;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPTraps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPTraps)
	virtual BOOL OnInitDialog();
	afx_msg void OnTrAdd();
	afx_msg void OnTrEdit();
	afx_msg void OnTrRemove();
	afx_msg void OnDblclkTrList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void refreshList();
	int getSelectedTrapPosition();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPTRAPS_H__548B06EA_69D3_41D7_A73A_FFF9A7D3192E__INCLUDED_)
