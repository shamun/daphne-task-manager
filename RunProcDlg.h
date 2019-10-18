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

#if !defined(AFX_RUNPROCDLG_H__28BE047A_F41E_4591_B093_74E9A3D615EB__INCLUDED_)
#define AFX_RUNPROCDLG_H__28BE047A_F41E_4591_B093_74E9A3D615EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RunProcDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRunProcDlg dialog
class CAuthUserDlg;

class CRunProcDlg : public CDialog
{
// Construction
public:
	CRunProcDlg(CWnd* pParent = NULL);   // standard constructor
  CAuthUserDlg * pInfo;

// Dialog Data
	//{{AFX_DATA(CRunProcDlg)
	enum { IDD = IDD_RUNP };
	CString	m_strArg;
	CString	m_strDir;
	CString	m_strPrg;
	BOOL	m_bSuspended;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRunProcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRunProcDlg)
	afx_msg void OnExplore();
	virtual void OnOK();
	afx_msg void OnExploreDir();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNPROCDLG_H__28BE047A_F41E_4591_B093_74E9A3D615EB__INCLUDED_)
