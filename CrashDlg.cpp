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

// CrashDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "CrashDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCrashDlg dialog


CCrashDlg::CCrashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCrashDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCrashDlg)
	m_strError = _T("");
	//}}AFX_DATA_INIT
	courier = NULL;
}


void CCrashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCrashDlg)
	DDX_Control(pDX, IDC_CR_ERROR, m_cError);
	DDX_Text(pDX, IDC_CR_ERROR, m_strError);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCrashDlg, CDialog)
	//{{AFX_MSG_MAP(CCrashDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCrashDlg message handlers

void CCrashDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

BOOL CCrashDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Language
	SetWindowText(_(" Daphne - Crash"));
	SetDlgItemText(IDOK, _("SEND"));
	SetDlgItemText(IDCANCEL, _("Quit"));
	SetDlgItemText(IDC_STT_EXCEP, _("An unhandled exception has crashed Daphne. Please, press SEND to report this problem by mail."));
	SetDlgItemText(IDC_STG_ERRINFO, _("Error information"));
	SetDlgItemText(IDC_STT_STAR, _("* You will see the mail before sending."));


	courier = new CFont();
	if (courier) {
		courier->CreatePointFont(90, L"Courier New");

		m_cError.SetFont(courier);
	}
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
