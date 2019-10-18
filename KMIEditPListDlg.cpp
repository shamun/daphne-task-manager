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

// KMIEditPListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "KMIEditPListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKMIEditPListDlg dialog


CKMIEditPListDlg::CKMIEditPListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKMIEditPListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKMIEditPListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CKMIEditPListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKMIEditPListDlg)
	DDX_Control(pDX, IDC_KMI_PLIST, m_cList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKMIEditPListDlg, CDialog)
	//{{AFX_MSG_MAP(CKMIEditPListDlg)
	ON_LBN_DBLCLK(IDC_KMI_PLIST, OnDblclkKmiPlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKMIEditPListDlg message handlers

BOOL CKMIEditPListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	std::vector<std::wstring>::iterator i;

	for(i = cprocs.begin(); i != cprocs.end(); i++)
		m_cList.AddString((*i).c_str());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKMIEditPListDlg::OnOK() 
{
	CString str;
	int * indices;

	indices = (int*)malloc(sizeof(int)*m_cList.GetCount());

	int items = m_cList.GetSelItems(m_cList.GetCount(), indices);

	for(int i=0; i<items; i++) {

		m_cList.GetText(*(indices+i), str);

		(*kpList) += str;
		(*kpList) += "\r\n";
	}

	free(indices);
	
	CDialog::OnOK();
}

void CKMIEditPListDlg::OnDblclkKmiPlist() 
{
	OnOK();	
}
