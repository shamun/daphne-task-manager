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

#if !defined(AFX_PPSTARTUP_H__936257C1_F647_4B62_9C2E_0A13271B59DC__INCLUDED_)
#define AFX_PPSTARTUP_H__936257C1_F647_4B62_9C2E_0A13271B59DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPStartup.h : header file
//

class CDaphneDlg;

/////////////////////////////////////////////////////////////////////////////
// CPPStartup dialog

class CPPStartup : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPStartup)

// Construction
public:
	CPPStartup();
	~CPPStartup();
	CDaphneDlg * pDaphne;

// Dialog Data
	//{{AFX_DATA(CPPStartup)
	enum { IDD = IDD_PP_STARTUP };
	CStatic	m_cPText;
	CEdit	m_cPPort;
	CEdit	m_cPAddr;
	BOOL	m_bStartHidden;
	BOOL	m_bAutoStart;
	BOOL	m_bAlternateBackground;
	BOOL	m_bHideZombies;
	BOOL	m_bUpdates;
	CString	m_strProxyAddress;
	UINT	m_iProxyPort;
	BOOL	m_bUseProxy;
	BOOL	m_bFastr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPStartup)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPStartup)
	virtual BOOL OnInitDialog();
	afx_msg void OnSProxy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPSTARTUP_H__936257C1_F647_4B62_9C2E_0A13271B59DC__INCLUDED_)
