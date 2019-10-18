#if !defined(AFX_MBWEBAGREEDLG_H__0D1C98B8_171D_4D44_9863_5E9CCB0C6835__INCLUDED_)
#define AFX_MBWEBAGREEDLG_H__0D1C98B8_171D_4D44_9863_5E9CCB0C6835__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MBWebAgreeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMBWebAgreeDlg dialog

class CMBWebAgreeDlg : public CDialog
{
// Construction
public:
	CMBWebAgreeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMBWebAgreeDlg)
	enum { IDD = IDD_MB_WEBAGREE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMBWebAgreeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMBWebAgreeDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MBWEBAGREEDLG_H__0D1C98B8_171D_4D44_9863_5E9CCB0C6835__INCLUDED_)
