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

#if !defined(AFX_PPLOOK_H__A437DB91_2B59_4ED5_B7C6_654E850F3EE9__INCLUDED_)
#define AFX_PPLOOK_H__A437DB91_2B59_4ED5_B7C6_654E850F3EE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPLook.h : header file
//

#include "ProcList.h"

/////////////////////////////////////////////////////////////////////////////
// CPPLook dialog

class CPPLook : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPLook)

// Construction
public:
	CPPLook();
	~CPPLook();
	CProcList * pProcList;
	CWnd * pMain;


// Dialog Data
	//{{AFX_DATA(CPPLook)
	enum { IDD = IDD_PP_LOOK };
	CStatic	m_cP25;
	CSliderCtrl	m_cAlpha;
	CListCtrl	m_cColumns;
	CStatic	m_cP100;
	BOOL	m_bAlpha;
	int		m_iAlpha;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPLook)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPLook)
	afx_msg void OnSTransp();
	virtual BOOL OnInitDialog();
	afx_msg void OnReleasedcaptureWndalpha(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSUp2();
	afx_msg void OnSDown2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
  BOOL m_bAnimate;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPLOOK_H__A437DB91_2B59_4ED5_B7C6_654E850F3EE9__INCLUDED_)
