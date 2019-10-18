#if !defined(AFX_PPHIGHLIGHT_H__2989C005_8C01_4BA9_B0F6_6F18A401A0A7__INCLUDED_)
#define AFX_PPHIGHLIGHT_H__2989C005_8C01_4BA9_B0F6_6F18A401A0A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPHighlight.h : header file
//

class CProcList;

/////////////////////////////////////////////////////////////////////////////
// CPPHighlight dialog

class CPPHighlight : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPHighlight)

// Construction
public:
	CPPHighlight();
	~CPPHighlight();
	CProcList * pProcList;

// Dialog Data
	//{{AFX_DATA(CPPHighlight)
	enum { IDD = IDD_PP_HIGHLIGHTS };
	CString	m_strList;
	BOOL	m_bHLTop;
	BOOL	m_bHLSys;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPHighlight)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPHighlight)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnHlProcs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	std::vector<std::wstring> cprocs;

private:
	BOOL m_bHLTopMem;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPHIGHLIGHT_H__2989C005_8C01_4BA9_B0F6_6F18A401A0A7__INCLUDED_)
