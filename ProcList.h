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

#if !defined(AFX_PROCLIST_H__95A4D9D7_0D26_4371_AE73_D1D49E9CD826__INCLUDED_)
#define AFX_PROCLIST_H__95A4D9D7_0D26_4371_AE73_D1D49E9CD826__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProcList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProcList window

class CProcList : public CListCtrl
{
// Construction
public:
	CProcList();

// Attributes
private:
  CFont font;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcList)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CProcList();
  void clearSearchPattern(void);

  // Generated message map functions
protected:
	//{{AFX_MSG(CProcList)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

private:
	CRect m_rectClient;
	CString m_strSearch;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCLIST_H__95A4D9D7_0D26_4371_AE73_D1D49E9CD826__INCLUDED_)
