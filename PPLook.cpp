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

// PPLook.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "PPLook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPLook property page

IMPLEMENT_DYNCREATE(CPPLook, CPropertyPage)

CPPLook::CPPLook() : CPropertyPage(CPPLook::IDD)
, m_bAnimate(FALSE)
{
	//{{AFX_DATA_INIT(CPPLook)
	m_bAlpha = FALSE;
	m_iAlpha = 0;
	//}}AFX_DATA_INIT
}

CPPLook::~CPPLook()
{
}

void CPPLook::DoDataExchange(CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPPLook)
  DDX_Control(pDX, IDC_PS_25, m_cP25);
  DDX_Control(pDX, IDC_WNDALPHA, m_cAlpha);
  DDX_Control(pDX, IDC_S_COLUMNS, m_cColumns);
  DDX_Control(pDX, IDC_PS_100, m_cP100);
  DDX_Check(pDX, IDC_S_TRANSP, m_bAlpha);
  DDX_Slider(pDX, IDC_WNDALPHA, m_iAlpha);
  //}}AFX_DATA_MAP
  DDX_Check(pDX, IDC_S_ANIMICON, m_bAnimate);
}


BEGIN_MESSAGE_MAP(CPPLook, CPropertyPage)
	//{{AFX_MSG_MAP(CPPLook)
	ON_BN_CLICKED(IDC_S_TRANSP, OnSTransp)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_WNDALPHA, OnReleasedcaptureWndalpha)
	ON_BN_CLICKED(IDC_S_UP2, OnSUp2)
	ON_BN_CLICKED(IDC_S_DOWN2, OnSDown2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPLook message handlers

void CPPLook::OnSTransp() 
{
	UpdateData();

	m_cAlpha.EnableWindow(m_bAlpha);
	m_cP100.EnableWindow(m_bAlpha);
	m_cP25.EnableWindow(m_bAlpha);

	if (m_bAlpha) {
		m_cAlpha.SetPos(AfxGetApp()->GetProfileInt(L"CFG", L"MainWndAlpha", 255));
		pMain->ModifyStyleEx(0, WS_EX_LAYERED);
		::SetLayeredWindowAttributes(pMain->m_hWnd, RGB(0,0,0), m_iAlpha, LWA_ALPHA);
	}
	else {
		::SetLayeredWindowAttributes(pMain->m_hWnd, RGB(0,0,0), 255, LWA_ALPHA);
		pMain->ModifyStyleEx(WS_EX_LAYERED, 0);
	}
	
	pMain->RedrawWindow();
}

void CPPLook::OnOK() 
{

	UpdateData(TRUE);

	AfxGetApp()->WriteProfileInt(L"CFG", L"MainWndAlpha", m_iAlpha);
	AfxGetApp()->WriteProfileInt(L"CFG", L"MainWndAlphaOn", m_bAlpha);
	AfxGetApp()->WriteProfileInt(L"CFG", L"DaphneAnimatedIcon", m_bAnimate);

	if (pProcList) {

		int * array = (int*)malloc(m_cColumns.GetItemCount()*sizeof(int));

		if (array != NULL) {

			for(int i = 0; i < m_cColumns.GetItemCount(); i++) {
				array[i] = (int)m_cColumns.GetItemData(i);
			}

			pProcList->SetColumnOrderArray(m_cColumns.GetItemCount(), array);
			pProcList->RedrawWindow();

			AfxGetApp()->WriteProfileBinary(L"CFG", L"ColumnOrder", (PBYTE)array, m_cColumns.GetItemCount()*sizeof(int));

			free(array);
		}
	}
	

	CPropertyPage::OnOK();
}

BOOL CPPLook::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// Language
	SetWindowText(_("Look and feel")); // Ver CDaphneDlg::OnSettings()
	SetDlgItemText(IDC_STT_MAINW, _("Main window and tray icon"));
	SetDlgItemText(IDC_STG_PROCLC, _("Process list colum order"));
	SetDlgItemText(IDC_S_UP2, _("Up"));
	SetDlgItemText(IDC_S_DOWN2, _("Down"));
	SetDlgItemText(IDC_S_TRANSP, _("Window transparence"));
  SetDlgItemText(IDC_S_ANIMICON, _("Animate tray icon with CPU usage."));


	pMain = GetParent()->GetParent();

  m_bAnimate = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneAnimatedIcon", 1);
	
	m_bAlpha = AfxGetApp()->GetProfileInt(L"CFG", L"MainWndAlphaOn", 0);
	UpdateData(FALSE);

	m_cAlpha.EnableWindow(m_bAlpha);
	m_cP100.EnableWindow(m_bAlpha);
	m_cP25.EnableWindow(m_bAlpha);


	m_cAlpha.SetRange(64, 255);
	m_cAlpha.SetPos(AfxGetApp()->GetProfileInt(L"CFG", L"MainWndAlpha", 255));

	m_cColumns.InsertColumn(0, L"Name", LVCFMT_LEFT, 200, -1);
	m_cColumns.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	if (pProcList) {

		int i = 0;
		LVCOLUMN data;
		CString str;

		data.mask = LVCF_TEXT | LVCF_ORDER;
		data.pszText = str.GetBufferSetLength(256);
		data.cchTextMax = 256;

		while(pProcList->GetColumn(i, &data)) {

			str.ReleaseBuffer();

			int index = m_cColumns.InsertItem(data.iOrder, str);
			m_cColumns.SetItemData(index, (DWORD)i);

			data.mask = LVCF_TEXT | LVCF_ORDER;
			data.pszText = str.GetBufferSetLength(256);
			data.cchTextMax = 256;

			i++;
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPPLook::OnReleasedcaptureWndalpha(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);

	::SetLayeredWindowAttributes(pMain->m_hWnd, RGB(0,0,0), m_iAlpha, LWA_ALPHA);
	AfxGetApp()->WriteProfileInt(L"CFG", L"MainWndAlpha", m_iAlpha);
	
	*pResult = 0;
}

void CPPLook::OnSUp2() 
{
	POSITION pos = m_cColumns.GetFirstSelectedItemPosition();
	int index = 0;

	if (pos != NULL) {

		index = ((int)pos) - 1;

		if (index > 0) {

			DWORD_PTR id = m_cColumns.GetItemData(index);
			CString text = m_cColumns.GetItemText(index, 0);
			m_cColumns.DeleteItem(index);
			m_cColumns.InsertItem(index-1, text);
			m_cColumns.SetItemData(index-1, id);

			m_cColumns.SetItemState(index-1, LVIS_SELECTED | LVIS_FOCUSED, 0xFF);
		}
	}
}

void CPPLook::OnSDown2() 
{
	POSITION pos = m_cColumns.GetFirstSelectedItemPosition();
	int index = 0;

	if (pos != NULL) {

		index = ((int)pos) - 1;

		if (index < (m_cColumns.GetItemCount()-1)) {

			DWORD_PTR id = m_cColumns.GetItemData(index);
			CString text = m_cColumns.GetItemText(index, 0);
			m_cColumns.DeleteItem(index);
			m_cColumns.InsertItem(index+1, text);
			m_cColumns.SetItemData(index+1, id);

			m_cColumns.SetItemState(index+1, LVIS_SELECTED | LVIS_FOCUSED, 0xFF);
		}
	}
}
