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

// PPKill.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "PPKill.h"
#include "KMIEditDlg.h"
#include "ProcList.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPKill property page

IMPLEMENT_DYNCREATE(CPPKill, CPropertyPage)

CPPKill::CPPKill() : CPropertyPage(CPPKill::IDD)
{
	//{{AFX_DATA_INIT(CPPKill)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPPKill::~CPPKill()
{
}

void CPPKill::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPKill)
	DDX_Control(pDX, IDC_KILLERS, m_cKillers);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPPKill, CPropertyPage)
	//{{AFX_MSG_MAP(CPPKill)
	ON_BN_CLICKED(IDC_S_ADD, OnSAdd)
	ON_BN_CLICKED(IDC_S_DOWN, OnSDown)
	ON_BN_CLICKED(IDC_S_EDIT, OnSEdit)
	ON_BN_CLICKED(IDC_S_REMOVE, OnSRemove)
	ON_BN_CLICKED(IDC_S_UP, OnSUp)
	ON_NOTIFY(NM_DBLCLK, IDC_KILLERS, OnDblclkKillers)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPKill message handlers

BOOL CPPKill::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// Language
	SetWindowText(_("Kill menu")); // Ver CDaphneDlg::OnSettings()
	SetDlgItemText(IDC_S_ADD, _("Add"));
	SetDlgItemText(IDC_S_EDIT, _("Edit"));
	SetDlgItemText(IDC_S_REMOVE, _("Remove"));
	SetDlgItemText(IDC_S_UP, _("Up"));
	SetDlgItemText(IDC_S_DOWN, _("Down"));
	SetDlgItemText(IDC_STG_DMENU, _("Kill menu"));

	
	CString str;

	// Killers
	m_cKillers.InsertColumn(0, L"Name", LVCFMT_LEFT, 200, -1);
	m_cKillers.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	int items = AfxGetApp()->GetProfileInt(L"CFG", L"KillMenuItems", 0);
	for(int i = 0; i < items; i++) {

		CKiller * p = new CKiller();

		str.Format(L"Menu_%u", i);
		p->strName = AfxGetApp()->GetProfileString(L"KMI", str, L"");
		str.Format(L"PList_%u", i);
		p->strProcesses = AfxGetApp()->GetProfileString(L"KMI", str, L"");

		int index = m_cKillers.InsertItem(m_cKillers.GetItemCount(), p->strName);
		m_cKillers.SetItemData(index, (DWORD)p);
	}

	// Creo una lista de nombres de procesos
	for(int j=0; j < pProcList->GetItemCount(); j++)
		cprocs.push_back((LPCWSTR)pProcList->GetItemText(j, 1));

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPPKill::OnOK() 
{
	CString str;

	// Killers
	int items = AfxGetApp()->GetProfileInt(L"CFG", L"KillMenuItems", 0);
	for(int i = 0; i < items; i++) {
		str.Format(L"Menu_%u", i);
		AfxGetApp()->WriteProfileString(L"KMI", str, NULL);
		str.Format(L"PList_%u", i);
		AfxGetApp()->WriteProfileString(L"KMI", str, NULL);
	}

	AfxGetApp()->WriteProfileInt(L"CFG", L"KillMenuItems", m_cKillers.GetItemCount());
	for(int i = 0; i < m_cKillers.GetItemCount(); i++) {
		CKiller * p = (CKiller*)m_cKillers.GetItemData(i);

		str.Format(L"Menu_%u", i);
		AfxGetApp()->WriteProfileString(L"KMI", str, p->strName);
		str.Format(L"PList_%u", i);
		AfxGetApp()->WriteProfileString(L"KMI", str, p->strProcesses);

		delete p;
	}
	
	CPropertyPage::OnOK();
}

void CPPKill::OnSAdd() 
{
	CKMIEditDlg dlg;
	dlg.cprocs = cprocs;

	if (dlg.DoModal() == IDOK) {

		if ((dlg.pKiller->strName.GetLength() > 0) && (dlg.pKiller->strProcesses.GetLength() > 0)) {

			int index = m_cKillers.InsertItem(m_cKillers.GetItemCount(), dlg.pKiller->strName);
			m_cKillers.SetItemData(index, (DWORD)dlg.pKiller);
		}
		else {
			delete dlg.pKiller;
		}
	}
}

void CPPKill::OnSDown() 
{
	POSITION pos = m_cKillers.GetFirstSelectedItemPosition();
	int index = 0;

	if (pos != NULL) {

		index = ((int)pos) - 1;

		if (index < (m_cKillers.GetItemCount()-1)) {

			CKiller * p = (CKiller *)m_cKillers.GetItemData(index);
			m_cKillers.DeleteItem(index);
			m_cKillers.InsertItem(index+1, p->strName);
			m_cKillers.SetItemData(index+1, (DWORD)p);

			m_cKillers.SetItemState(index+1, LVIS_SELECTED | LVIS_FOCUSED, 0xFF);
		}
	}
}

void CPPKill::OnSEdit() 
{
	CKMIEditDlg dlg;
	POSITION pos = m_cKillers.GetFirstSelectedItemPosition();
	int index = 0;

	dlg.cprocs = cprocs;

	if (pos != NULL) {

		index = ((int)pos) - 1;
	
		dlg.pKiller = (CKiller *)m_cKillers.GetItemData(index);

		if (dlg.DoModal() == IDOK) {

			if ((dlg.pKiller->strName.GetLength() > 0) && (dlg.pKiller->strProcesses.GetLength() > 0)) {

				m_cKillers.SetItemText(index, 0, dlg.pKiller->strName);
			}
		}
	}
}

void CPPKill::OnSRemove() 
{
	POSITION pos = m_cKillers.GetFirstSelectedItemPosition();
	int index = 0;

	if (pos != NULL) {

		index = ((int)pos) - 1;

		CString str;
		str.Format(_("Removing %s"), m_cKillers.GetItemText(index, 0));
		if (MessageBox(_("You are about to remove a kill menu item. This action can't be undone.\n\nAre you sure?"), str, MB_YESNO | MB_ICONQUESTION) == IDYES) {

			delete (CKiller *)m_cKillers.GetItemData(index);
			m_cKillers.DeleteItem(index);
		}
	}
}

void CPPKill::OnSUp() 
{
	POSITION pos = m_cKillers.GetFirstSelectedItemPosition();
	int index = 0;

	if (pos != NULL) {

		index = ((int)pos) - 1;

		if (index > 0) {

			CKiller * p = (CKiller *)m_cKillers.GetItemData(index);
			m_cKillers.DeleteItem(index);
			m_cKillers.InsertItem(index-1, p->strName);
			m_cKillers.SetItemData(index-1, (DWORD)p);

			m_cKillers.SetItemState(index-1, LVIS_SELECTED | LVIS_FOCUSED, 0xFF);
		}
	}
}

void CPPKill::OnCancel() 
{
	int i;

	for(i = 0; i < m_cKillers.GetItemCount(); i++) {
		CKiller * p = (CKiller*)m_cKillers.GetItemData(i);

		delete p;
	}
	
	CPropertyPage::OnCancel();
}

void CPPKill::OnDblclkKillers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnSEdit();
	
	*pResult = 0;
}
