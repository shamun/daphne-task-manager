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

#if !defined(AFX_INSPECTOR_H__5654B960_3C11_4F67_AF6B_C61F55DF8FF9__INCLUDED_)
#define AFX_INSPECTOR_H__5654B960_3C11_4F67_AF6B_C61F55DF8FF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Inspector.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInspector dialog

class CInspector : public CDialog
{
// Construction
public:
	CInspector(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInspector)
	enum { IDD = IDD_INSPECTOR };
	CStatic	m_cLupa;
	CString	m_strTitle;
	CString	m_strClass;
	CString	m_strText;
	CString	m_strLocation;
	CString	m_strSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInspector)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInspector)
	afx_msg void OnLupactrl();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	HCURSOR m_hTarget;
	HCURSOR m_hNormal;
	BOOL bLupa;
	BOOL bHighlight;
	HWND hLast;
	HWND hWndLastHL;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSPECTOR_H__5654B960_3C11_4F67_AF6B_C61F55DF8FF9__INCLUDED_)
