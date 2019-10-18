// ProcessTreeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "ProcessTreeDlg.h"


// CProcessTreeDlg dialog

IMPLEMENT_DYNAMIC(CProcessTreeDlg, CDialog)

CProcessTreeDlg::CProcessTreeDlg(ProcInfoMap *p, CWnd* pParent /*=NULL*/)
	: CDialog(CProcessTreeDlg::IDD, pParent)
	, processes(*p)
  , selectedProcess(NULL)
{

}

CProcessTreeDlg::~CProcessTreeDlg()
{
}

void CProcessTreeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, treeControl);
}


BEGIN_MESSAGE_MAP(CProcessTreeDlg, CDialog)
  ON_NOTIFY(NM_DBLCLK, IDC_TREE, &CProcessTreeDlg::OnNMDblclkTree)
END_MESSAGE_MAP()


// CProcessTreeDlg message handlers

BOOL CProcessTreeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

  // Statistics
  CDaphneApp::hitStatisticalRecord(L"ProcessTree");

	// Language
	SetWindowText(_(" Process tree"));
	SetDlgItemText(IDCANCEL, _("Close"));
	SetDlgItemText(IDC_STT_NOUPDATE, _("This tree is a snapshot and won't be updated until the dialog is closed and opened again."));

	// Info
	std::map<DWORD, HTREEITEM> tmap;
	std::vector<CProcess*> left, lost;
	HTREEITEM root = treeControl.InsertItem(L"ROOT");
	tmap[0] = root;

	ProcInfoMap::iterator j;
	for(j = processes.begin(); j != processes.end(); j++) {
		if ((j->second)->dwPID != 0) {
      if ((j->second)->dwParentPID == 0) {
				tmap[(j->second)->dwPID] = treeControl.InsertItem((j->second)->strName, root);
        treeControl.SetItemData(tmap[(j->second)->dwPID], (DWORD_PTR)(j->second));
      }
			else
				left.push_back(j->second);
		}
	}

	std::vector<CProcess*>::iterator k;
	bool inserted;
	do {
		inserted = false;
		for(k = left.begin(); k != left.end(); k++) {
			if (tmap.find((*k)->dwParentPID) != tmap.end()) {
				tmap[(*k)->dwPID] = treeControl.InsertItem((*k)->strName, tmap[(*k)->dwParentPID]);
        treeControl.SetItemData(tmap[(*k)->dwPID], (DWORD_PTR)(*k));
				inserted = true;
			}
			else {
				if (processes.find((*k)->dwParentPID) != processes.end())
					lost.push_back(*k);
        else {
					tmap[(*k)->dwPID] = treeControl.InsertItem((*k)->strName, root);
          treeControl.SetItemData(tmap[(*k)->dwPID], (DWORD_PTR)(*k));
        }
			}
		}

		left = lost;
		lost.clear();
	}
	while(!left.empty() && inserted);

	for(k = left.begin(); k != left.end(); k++) {
		tmap[(*k)->dwPID] = treeControl.InsertItem((*k)->strName, root);
    treeControl.SetItemData(tmap[(*k)->dwPID], (DWORD_PTR)(*k));
	}

	treeControl.Expand(root, TVE_EXPAND);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CProcessTreeDlg::OnNMDblclkTree(NMHDR *pNMHDR, LRESULT *pResult)
{
  selectedProcess = (CProcess*)treeControl.GetItemData(treeControl.GetSelectedItem());
  OnOK();
  *pResult = 0;
}
