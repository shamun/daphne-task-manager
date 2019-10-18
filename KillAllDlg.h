/***********************************************************************************
	
	Daphne -- GPL task manager replacement for Windows
	Copyright (C) 2012  Leandro H. Fernández

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

#if !defined(AFX_KILLALLDLG_H__C8D4D8C4_91BC_4E99_864E_9DBE034F402F__INCLUDED_)
#define AFX_KILLALLDLG_H__C8D4D8C4_91BC_4E99_864E_9DBE034F402F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KillAllDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKillAllDlg dialog

class CKillAllDlg : public CDialog
{
// Construction
public:
	CKillAllDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL m_bPolite;

// Dialog Data
	//{{AFX_DATA(CKillAllDlg)
	enum { IDD = IDD_KILLALL };
	CString	m_strName;
	BOOL	m_bCase;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKillAllDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKillAllDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KILLALLDLG_H__C8D4D8C4_91BC_4E99_864E_9DBE034F402F__INCLUDED_)
