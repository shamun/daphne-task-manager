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

// KMIEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "KMIEditDlg.h"
#include "KMIEditPListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKMIEditDlg dialog


CKMIEditDlg::CKMIEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKMIEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKMIEditDlg)
	m_strName = _T("");
	m_strProcessList = _T("");
	//}}AFX_DATA_INIT
	pKiller = NULL;
}


void CKMIEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKMIEditDlg)
	DDX_Text(pDX, IDC_KMI_NAME, m_strName);
	DDX_Text(pDX, IDC_KMI_PLIST, m_strProcessList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKMIEditDlg, CDialog)
	//{{AFX_MSG_MAP(CKMIEditDlg)
	ON_BN_CLICKED(IDC_KMI_PROCS, OnKmiProcs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKMIEditDlg message handlers

void CKMIEditDlg::OnOK() 
{
	UpdateData(TRUE);

	pKiller->strName = m_strName;
	pKiller->strProcesses = m_strProcessList;
	
	CDialog::OnOK();
}

BOOL CKMIEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Language
	SetWindowText(_(" Kill menu item"));
	SetDlgItemText(IDOK, _("OK"));
	SetDlgItemText(IDCANCEL, _("Cancel"));
	SetDlgItemText(IDC_STT_MENU, _("Menu name:"));
	SetDlgItemText(IDC_STG_PROCL, _("Process list (case sensitive)"));

	
	if (pKiller == NULL) {
		pKiller = new CKiller();
		bWasNew = TRUE;
	}
	else
		bWasNew = FALSE;

	m_strName = pKiller->strName;
	m_strProcessList = pKiller->strProcesses;

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKMIEditDlg::OnKmiProcs() 
{
	CKMIEditPListDlg dlg;

	UpdateData();

	dlg.cprocs = cprocs;
	dlg.kpList = &m_strProcessList;

	if (dlg.DoModal() == IDOK)
		UpdateData(FALSE);
}

void CKMIEditDlg::OnCancel() 
{

	if (bWasNew)
		delete pKiller;

	CDialog::OnCancel();
}
