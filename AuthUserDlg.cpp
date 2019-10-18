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

// AuthUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "AuthUserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAuthUserDlg dialog


CAuthUserDlg::CAuthUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAuthUserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAuthUserDlg)
	m_strPasswd = _T("");
	m_strUser = _T("");
	//}}AFX_DATA_INIT
	hToken = NULL;
}


void CAuthUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAuthUserDlg)
	DDX_Text(pDX, IDC_A_PASSWD, m_strPasswd);
	DDX_Text(pDX, IDC_A_USER, m_strUser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAuthUserDlg, CDialog)
	//{{AFX_MSG_MAP(CAuthUserDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAuthUserDlg message handlers

void CAuthUserDlg::OnOK() 
{
  WCHAR tmp[30];
	UpdateData();

  if (LogonUser(m_strUser, NULL, m_strPasswd, LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &hToken)) {
    CloseHandle(hToken);
		CDialog::OnOK();
  }
	else
		MessageBox(_("Invalid username or password"), __("ERROR", tmp, sizeof(tmp)), MB_OK|MB_ICONSTOP);
}

BOOL CAuthUserDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Language
	SetWindowText(_(" Authenticate user"));
	SetDlgItemText(IDOK, _("OK"));
	SetDlgItemText(IDCANCEL, _("Cancel"));
	SetDlgItemText(IDC_STT_USERN, _("Username:"));
	SetDlgItemText(IDC_STT_PASSW, _("Password:"));

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
