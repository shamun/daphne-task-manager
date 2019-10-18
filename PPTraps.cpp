// PPTraps.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "PPTraps.h"
#include "NewTrapDlg.h"
#include "Trap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPTraps property page

IMPLEMENT_DYNCREATE(CPPTraps, CPropertyPage)

CPPTraps::CPPTraps() : CPropertyPage(CPPTraps::IDD)
{
	//{{AFX_DATA_INIT(CPPTraps)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPPTraps::~CPPTraps()
{
}

void CPPTraps::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPTraps)
	DDX_Control(pDX, IDC_TR_LIST, m_cTraps);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPPTraps, CPropertyPage)
	//{{AFX_MSG_MAP(CPPTraps)
	ON_BN_CLICKED(IDC_TR_ADD, OnTrAdd)
	ON_BN_CLICKED(IDC_TR_EDIT, OnTrEdit)
	ON_BN_CLICKED(IDC_TR_REMOVE, OnTrRemove)
	ON_NOTIFY(NM_DBLCLK, IDC_TR_LIST, OnDblclkTrList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPTraps message handlers

BOOL CPPTraps::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// Language
	SetWindowText(_("Traps")); // Ver CDaphneDlg::OnSettings()
	SetDlgItemText(IDC_TR_ADD, _("Add"));
	SetDlgItemText(IDC_TR_EDIT, _("Edit"));
	SetDlgItemText(IDC_TR_REMOVE, _("Remove"));
	SetDlgItemText(IDC_STG_TRAPS, _("Traps"));


	m_cTraps.InsertColumn(0, _("Process name"), LVCFMT_LEFT, 100, -1);
	m_cTraps.InsertColumn(1, _("Action"), LVCFMT_LEFT, 160, -1);
	m_cTraps.InsertColumn(2, _("Window title contains"), LVCFMT_LEFT, 160, -1);
	m_cTraps.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	refreshList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPPTraps::refreshList() {

	m_cTraps.DeleteAllItems();

	int p = 0;
	TrapList::iterator i;
	for(i = traps->begin(); i != traps->end(); i++) {
		CTrap * trap = (CTrap *)(*i);

		int index = m_cTraps.InsertItem(p, trap->getProcessName());
		m_cTraps.SetItemText(index, 1, CTrap::getTypeName(trap->getType()));
		m_cTraps.SetItemText(index, 2, trap->getWindowContains());
		m_cTraps.SetItemData(index, p++);
	}

}

void CPPTraps::OnTrAdd() 
{
	CNewTrapDlg trapdlg;

	if (trapdlg.DoModal() == IDOK) {

		traps->push_back(trapdlg.trap);
		refreshList();
	}
}

void CPPTraps::OnTrEdit() 
{
	int index = getSelectedTrapPosition();

	if ((index >= 0) && (index < m_cTraps.GetItemCount())) {

		CTrap * p = (CTrap *)traps->at(index);

    if (p->canEdit()) {
		  CNewTrapDlg trapdlg;
		  trapdlg.trap = p;
	
		  if (trapdlg.DoModal() == IDOK) {
			  refreshList();
		  }
    }
    else {
      MessageBox(_("Can't edit this trap"), _("Sorry"), MB_OK);
    }
	}
}

void CPPTraps::OnTrRemove() 
{
	int index = getSelectedTrapPosition();

	if ((index >= 0) && (index < m_cTraps.GetItemCount())) {

		CTrap * p = (CTrap *)traps->at(index);

		CString text, title;
		text.Format(_("Are you sure you want to remove the selected trap on %s?"), p->getProcessName());
		title.Format(_("Removing \"%s\""), CTrap::getTypeName(p->getType()));

		if (MessageBox(text, title, MB_YESNO | MB_ICONQUESTION) == IDYES) {
			m_cTraps.DeleteItem(index);
			traps->erase(traps->begin() + index);
			delete p;

			refreshList();
		}
	}
	
}

int CPPTraps::getSelectedTrapPosition() {

	POSITION pos = m_cTraps.GetFirstSelectedItemPosition();

	if (pos != NULL) {

		return (int)m_cTraps.GetItemData(((int)pos) - 1);
	}

	return -1;
}

void CPPTraps::OnDblclkTrList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnTrEdit();
	
	*pResult = 0;
}
