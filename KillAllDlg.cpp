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

// KillAllDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "KillAllDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKillAllDlg dialog


CKillAllDlg::CKillAllDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKillAllDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKillAllDlg)
	m_strName = _T("");
	m_bCase = FALSE;
	//}}AFX_DATA_INIT
	m_bPolite = FALSE;
}


void CKillAllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKillAllDlg)
	DDX_Text(pDX, IDC_KA_NAME, m_strName);
	DDX_Check(pDX, IDC_KA_CASE, m_bCase);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKillAllDlg, CDialog)
	//{{AFX_MSG_MAP(CKillAllDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKillAllDlg message handlers

void CKillAllDlg::OnOK() 
{
	UpdateData(TRUE);
	
	CDialog::OnOK();
}

BOOL CKillAllDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

  // Statistics
  CDaphneApp::hitStatisticalRecord(L"KillAll");
  
	// Language
	SetWindowText(_(" Kill all by name"));
	SetDlgItemText(IDOK, _("OK"));
	SetDlgItemText(IDCANCEL, _("Cancel"));
	SetDlgItemText(IDC_STT_PROC, _("Process name:"));
	SetDlgItemText(IDC_KA_CASE, _("case-sensitive"));


	m_bCase = TRUE;
	UpdateData(FALSE);
	
	CString name;
	if (m_bPolite) {

		GetWindowText(name);
		name += _(" (Polite)");
		SetWindowText(name);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
