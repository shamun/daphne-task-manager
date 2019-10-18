/***********************************************************************************
	
	Daphne -- GPL task manager replacement for Windows
	Copyright (C) 2010  Leandro H. Fernández

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

#if !defined(AFX_PROPERTIESDLG_H__19482826_4CFB_4D72_B6DE_95C535B02EC9__INCLUDED_)
#define AFX_PROPERTIESDLG_H__19482826_4CFB_4D72_B6DE_95C535B02EC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertiesDlg.h : header file
//

#include "Process.h"

/////////////////////////////////////////////////////////////////////////////
// CPropertiesDlg dialog

class CPropertiesDlg : public CDialog
{
// Construction
public:
	DWORD GetSelectedThreadID();
	HWND GetSelectHandle();
	CPropertiesDlg(CProcess * process, CWnd* pParent = NULL);   // standard constructor
  afx_msg void OnBnClickedOk();

// Dialog Data
	//{{AFX_DATA(CPropertiesDlg)
	enum { IDD = IDD_PROPERTIES };
	CListCtrl	m_cThreadList;
	CTreeCtrl	m_cWnds;
	CEdit	m_cInfo;
	CString	m_strName;
	CString	m_strPath;
	CString	m_strPriority;
	CString	m_strClass;
	CString	m_strCreated;
	CString	m_strKernel;
	CString	m_strUser;
	CString	m_strInfo;
  CString	m_strOwner;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void RefreshThreads();

	// Generated message map functions
	//{{AFX_MSG(CPropertiesDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnRclickTWnds(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGoogle();
	afx_msg void OnDblclkTWnds(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPInfom();
	afx_msg void OnPInfot();
	afx_msg void OnPInfoe();
	afx_msg void OnRclickPThreadlist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CProcess * m_pProcess;
	CFont * courier;
	CImageList images;
	CString m_strParent;
  virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
  afx_msg void OnBnClickedPInfoio();
public:
  ULONG_PTR minWS;
  ULONG_PTR maxWS;
  HWND hPreselectWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTIESDLG_H__19482826_4CFB_4D72_B6DE_95C535B02EC9__INCLUDED_)
