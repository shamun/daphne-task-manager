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

#if !defined(AFX_W32VALUES_H__5F64EB0C_B07F_4D13_92D4_B5BB408FF981__INCLUDED_)
#define AFX_W32VALUES_H__5F64EB0C_B07F_4D13_92D4_B5BB408FF981__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// W32Values.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CW32Values dialog

class CW32Values : public CDialog
{
// Construction
public:
	DWORD value;
	CW32Values(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CW32Values)
	enum { IDD = IDD_GETVALUE };
	CEdit	m_cFilter;
	CListBox	m_cValues;
	CString	m_strValue;
	CString	m_strFilter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CW32Values)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CW32Values)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateVFilter();
	afx_msg void OnDblclkVList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_W32VALUES_H__5F64EB0C_B07F_4D13_92D4_B5BB408FF981__INCLUDED_)
