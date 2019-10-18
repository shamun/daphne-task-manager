#if !defined(AFX_HIDDENOWNER_H__D2CB031D_FE87_4C29_BE86_F6A971152F8C__INCLUDED_)
#define AFX_HIDDENOWNER_H__D2CB031D_FE87_4C29_BE86_F6A971152F8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HiddenOwner.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHiddenOwner dialog

class CHiddenOwner : public CDialog
{
// Construction
public:
	CHiddenOwner(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHiddenOwner)
	enum { IDD = IDD_HIDDEN_OWNER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHiddenOwner)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHiddenOwner)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HIDDENOWNER_H__D2CB031D_FE87_4C29_BE86_F6A971152F8C__INCLUDED_)
