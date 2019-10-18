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

#if !defined(AFX_KMIEDITDLG_H__763A44B2_4C61_431E_99BD_EC757BCEAB30__INCLUDED_)
#define AFX_KMIEDITDLG_H__763A44B2_4C61_431E_99BD_EC757BCEAB30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KMIEditDlg.h : header file
//

#include "Killer.h"

/////////////////////////////////////////////////////////////////////////////
// CKMIEditDlg dialog

class CKMIEditDlg : public CDialog
{
// Construction
public:
	CKiller * pKiller;
	CKMIEditDlg(CWnd* pParent = NULL);   // standard constructor
	std::vector<std::wstring> cprocs;
	BOOL bWasNew;


// Dialog Data
	//{{AFX_DATA(CKMIEditDlg)
	enum { IDD = IDD_KMI_EDIT };
	CString	m_strName;
	CString	m_strProcessList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKMIEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKMIEditDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnKmiProcs();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KMIEDITDLG_H__763A44B2_4C61_431E_99BD_EC757BCEAB30__INCLUDED_)
