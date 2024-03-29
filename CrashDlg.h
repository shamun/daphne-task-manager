/***********************************************************************************
	
	Daphne -- GPL task manager replacement for Windows
	Copyright (C) 2005  Leandro H. Fernández

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

***********************************************************************************/

#if !defined(AFX_CRASHDLG_H__8CFC5FE1_BCEB_4A19_8287_72BEFF95C86E__INCLUDED_)
#define AFX_CRASHDLG_H__8CFC5FE1_BCEB_4A19_8287_72BEFF95C86E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CrashDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCrashDlg dialog

class CCrashDlg : public CDialog
{
// Construction
public:
	CCrashDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCrashDlg)
	enum { IDD = IDD_CRASH };
	CEdit	m_cError;
	CString	m_strError;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCrashDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCrashDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CFont * courier;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRASHDLG_H__8CFC5FE1_BCEB_4A19_8287_72BEFF95C86E__INCLUDED_)
