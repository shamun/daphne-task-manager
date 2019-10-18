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

// W32Values.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "W32Values.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MsgMap values;

/////////////////////////////////////////////////////////////////////////////
// CW32Values dialog


CW32Values::CW32Values(CWnd* pParent /*=NULL*/)
	: CDialog(CW32Values::IDD, pParent)
{
	//{{AFX_DATA_INIT(CW32Values)
	m_strValue = _T("");
	m_strFilter = _T("");
	//}}AFX_DATA_INIT
}


void CW32Values::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CW32Values)
	DDX_Control(pDX, IDC_V_FILTER, m_cFilter);
	DDX_Control(pDX, IDC_V_LIST, m_cValues);
	DDX_LBString(pDX, IDC_V_LIST, m_strValue);
	DDX_Text(pDX, IDC_V_FILTER, m_strFilter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CW32Values, CDialog)
	//{{AFX_MSG_MAP(CW32Values)
	ON_EN_UPDATE(IDC_V_FILTER, OnUpdateVFilter)
	ON_LBN_DBLCLK(IDC_V_LIST, OnDblclkVList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CW32Values message handlers

void CW32Values::OnOK() 
{
	UpdateData();

	value = values[(LPCWSTR)m_strValue];
	
	CDialog::OnOK();
}

BOOL CW32Values::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Language
	SetWindowText(_(" Win32 values"));
	SetDlgItemText(IDOK, _("OK"));
	SetDlgItemText(IDCANCEL, _("Cancel"));
	SetDlgItemText(IDC_STT_FILTER, _("Filter:"));

		
	value = 0x00000000;

	MsgMap::iterator i;
	for(i = values.begin(); i != values.end(); i++) {
		m_cValues.AddString(i->first.c_str());
	}

	m_cValues.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CW32Values::OnUpdateVFilter() 
{
	UpdateData();

	m_cValues.SetCurSel(m_cValues.FindString(-1, m_strFilter));
}

void CW32Values::OnDblclkVList() 
{
	OnOK();	
}
