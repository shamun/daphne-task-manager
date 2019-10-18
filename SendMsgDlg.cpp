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

// SendMsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "SendMsgDlg.h"
#include "W32Values.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSendMsgDlg dialog


CSendMsgDlg::CSendMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSendMsgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSendMsgDlg)
	m_lParam = 0;
	m_wParam = 0;
	//}}AFX_DATA_INIT
	hTarget = NULL;
}


void CSendMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSendMsgDlg)
	DDX_Control(pDX, IDC_M_MSG, m_cMsg);
	DDX_Text(pDX, IDC_M_LPARAM, m_lParam);
	DDX_Text(pDX, IDC_M_WPARAM, m_wParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSendMsgDlg, CDialog)
	//{{AFX_MSG_MAP(CSendMsgDlg)
	ON_BN_CLICKED(IDC_GETVAL1, OnGetval1)
	ON_BN_CLICKED(IDC_GETVAL2, OnGetval2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSendMsgDlg message handlers

BOOL CSendMsgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Language
	SetWindowText(_(" Send message..."));
	SetDlgItemText(IDOK, _("OK"));
	SetDlgItemText(IDCANCEL, _("Cancel"));
	SetDlgItemText(IDC_STT_MSG, _("Message:"));
	SetDlgItemText(IDC_STT_LP, _("lParam:"));
	SetDlgItemText(IDC_STT_WP, _("wParam:"));

	
	MsgMap::iterator i;
	for(i = commands.begin(); i != commands.end(); i++) {
		m_cMsg.AddString(i->first.c_str());
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSendMsgDlg::OnOK() 
{
	CString cmd; 

	UpdateData(TRUE);
	m_cMsg.GetWindowText(cmd);

	::PostMessage(hTarget, commands[(LPCWSTR)cmd], m_wParam, m_lParam);
	
	CDialog::OnOK();
}

void CSendMsgDlg::OnGetval1() 
{
	CW32Values dlg;

	UpdateData();
	if (dlg.DoModal() == IDOK) {
		m_lParam = dlg.value;
		UpdateData(FALSE);
	}
}

void CSendMsgDlg::OnGetval2() 
{
	CW32Values dlg;

	UpdateData();
	if (dlg.DoModal() == IDOK) {
		m_wParam = dlg.value;
		UpdateData(FALSE);
	}
}
