/***********************************************************************************
	
	Daphne -- GPL task manager replacement for Windows
	Copyright (C) 2005  Leandro H. Fern�ndez

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

#if !defined(AFX_SENDMSGDLG_H__5B8C9E63_6461_4878_9687_CBE53EB420F4__INCLUDED_)
#define AFX_SENDMSGDLG_H__5B8C9E63_6461_4878_9687_CBE53EB420F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SendMsgDlg.h : header file
//

extern MsgMap commands;

/////////////////////////////////////////////////////////////////////////////
// CSendMsgDlg dialog

class CSendMsgDlg : public CDialog
{
// Construction
public:
	HWND hTarget;
	CSendMsgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSendMsgDlg)
	enum { IDD = IDD_SENDMSG };
	CComboBox	m_cMsg;
	DWORD	m_lParam;
	DWORD	m_wParam;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendMsgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSendMsgDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnGetval1();
	afx_msg void OnGetval2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDMSGDLG_H__5B8C9E63_6461_4878_9687_CBE53EB420F4__INCLUDED_)
