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

// WndsPopUp.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "WndsPopUp.h"
#include "DaphneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWndsPopUp dialog


CWndsPopUp::CWndsPopUp(CWnd* pParent /*=NULL*/)
	: CDialog(CWndsPopUp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWndsPopUp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWndsPopUp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWndsPopUp)
	DDX_Control(pDX, IDC_WNDS, m_cWnds);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWndsPopUp, CDialog)
	//{{AFX_MSG_MAP(CWndsPopUp)
	ON_NOTIFY(NM_DBLCLK, IDC_WNDS, OnDblclkWnds)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWndsPopUp message handlers

BOOL CWndsPopUp::OnInitDialog() 
{
	CString str;
	CDialog::OnInitDialog();
	
	SetWindowText(_(" Hidden applications"));

	m_cWnds.InsertColumn(0, L"Text", LVCFMT_LEFT, 170, -1);

	m_cWnds.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	images.Create(16, 16, ILC_COLOR8|ILC_MASK, 1, 1);
	images.Add(AfxGetApp()->LoadIcon(IDI_NULL));
	int imgidx;
	HICON icon = NULL;
	m_cWnds.SetImageList(&images, LVSIL_SMALL);
	m_cWnds.SetImageList(&images, LVSIL_NORMAL);
	
	MapWindows::iterator i;
	for(i = hiddenWindows->begin(); i != hiddenWindows->end(); i++) {
		CWnd * p = i->second;
	
		// Verifico si sigue viva la aplicación
		if (GetWindowThreadProcessId(p->GetSafeHwnd(), NULL) != 0) {
			p->GetWindowText(str);
			icon = getIconFromWindow(p->GetSafeHwnd());
			if (icon)
				imgidx = images.Add(icon);
			else
				imgidx = 0;

			int index = m_cWnds.InsertItem(0, str, imgidx);
			m_cWnds.SetItemData(index, (DWORD)p->GetSafeHwnd());
		}
	}
	
	int x = AfxGetApp()->GetProfileInt(L"CFG", L"HAppsX", -1);
	int y = AfxGetApp()->GetProfileInt(L"CFG", L"HAppsY", -1);

	UINT nFlags = SWP_NOMOVE | SWP_NOSIZE;

	if ((x>=0) && (y>=0))
		nFlags &= ~SWP_NOMOVE;

	SetWindowPos(&CWnd::wndTopMost, x, y, 0, 0, nFlags);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWndsPopUp::OnDblclkWnds(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CDaphneDlg * parent = (CDaphneDlg*)GetParent();

	parent->showWindow(GetSelectedHandler());
	OnCancel();
	
	*pResult = 0;
}

HWND CWndsPopUp::GetSelectedHandler()
{
	POSITION pos = m_cWnds.GetFirstSelectedItemPosition();

	return (HWND)m_cWnds.GetItemData((int)(pos-1));
}

void CWndsPopUp::saveState()
{
	WINDOWPLACEMENT wndinfo;
	GetWindowPlacement(&wndinfo);
	AfxGetApp()->WriteProfileInt(L"CFG", L"HAppsX", wndinfo.rcNormalPosition.left);
	AfxGetApp()->WriteProfileInt(L"CFG", L"HAppsY", wndinfo.rcNormalPosition.top);
}

void CWndsPopUp::OnCancel() 
{
	saveState();
	
	CDialog::OnCancel();
}
