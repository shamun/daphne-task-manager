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

#if !defined(AFX_WNDSPOPUP_H__62B0C98F_6FAF_4B3D_88C8_FE83BEFED2D6__INCLUDED_)
#define AFX_WNDSPOPUP_H__62B0C98F_6FAF_4B3D_88C8_FE83BEFED2D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WndsPopUp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWndsPopUp dialog

class CWndsPopUp : public CDialog
{
// Construction
public:
	CWndsPopUp(CWnd* pParent = NULL);   // standard constructor
	MapWindows * hiddenWindows;

// Dialog Data
	//{{AFX_DATA(CWndsPopUp)
	enum { IDD = IDD_WNDLIST };
	CListCtrl	m_cWnds;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndsPopUp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void saveState();
	HWND GetSelectedHandler();

	// Generated message map functions
	//{{AFX_MSG(CWndsPopUp)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkWnds(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CImageList images;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDSPOPUP_H__62B0C98F_6FAF_4B3D_88C8_FE83BEFED2D6__INCLUDED_)
