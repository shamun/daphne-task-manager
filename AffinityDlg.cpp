// AffinityDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "AffinityDlg.h"
#include "afxdialogex.h"


// CAffinityDlg dialog

IMPLEMENT_DYNAMIC(CAffinityDlg, CDialog)

CAffinityDlg::CAffinityDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAffinityDlg::IDD, pParent)
  , trap_it(FALSE)
{

}

CAffinityDlg::~CAffinityDlg()
{
}

void CAffinityDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_CPULIST, m_CPUList);
  DDX_Check(pDX, IDC_TRAP_AFFINITY, trap_it);
}


BEGIN_MESSAGE_MAP(CAffinityDlg, CDialog)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_CPULIST, &CAffinityDlg::OnLvnItemchangedCpulist)
  ON_BN_CLICKED(IDC_CPUALL, &CAffinityDlg::OnBnClickedCpuall)
  ON_BN_CLICKED(IDC_CPUNONE, &CAffinityDlg::OnBnClickedCpunone)
  ON_BN_CLICKED(IDOK, &CAffinityDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAffinityDlg message handlers


BOOL CAffinityDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  
	LVITEM lvItem;
  TCHAR tmp[255];
  ULONG64 process, system;


	SetWindowText(_(" Affinity"));
	SetDlgItemText(IDCANCEL, _("Close"));
	SetDlgItemText(IDOK, _("OK"));
	SetDlgItemText(IDC_CPUALL, _("Select all"));
	SetDlgItemText(IDC_CPUNONE, _("Select none"));
	SetDlgItemText(IDC_TRAP_AFFINITY, _("Create or modify trap"));

  m_CPUList.SetExtendedStyle(LVS_EX_CHECKBOXES);
  m_CPUList.SetColumnWidth(0, 200);

  m_pProcess->GetAffinityMask(&m_process, &m_system);
  process = m_process;
  system = m_system;

  for(int i=0; i < sizeof(DWORD_PTR)*8; ++i) {
    if (system & 1) {
      wsprintf(tmp, L"CPU%d", i);
      lvItem.pszText = tmp;
	    lvItem.mask = LVIF_TEXT;
	    lvItem.iItem = i;
	    lvItem.iSubItem = 0;
	    m_CPUList.InsertItem(&lvItem);
    }
    m_CPUList.SetCheck(i, process & 1);
    system = system >> 1;
    process = process >> 1;
  }

  // TODO:  Add extra initialization here

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}


void CAffinityDlg::OnLvnItemchangedCpulist(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  // TODO: Add your control notification handler code here
  *pResult = 0;
}


void CAffinityDlg::OnBnClickedCpuall()
{
  ULONG64 system;
  system = m_system;

  for(int i=0; i < sizeof(DWORD_PTR)*8; ++i) {
    m_CPUList.SetCheck(i, system & 1);
    system = system >> 1;
  }
}


void CAffinityDlg::OnBnClickedCpunone()
{
  ULONG64 system;
  system = m_system;

  for(int i=0; i < sizeof(DWORD_PTR)*8; ++i) {
    if (system & 1)
      m_CPUList.SetCheck(i, 0);
    system = system >> 1;
  }
}


void CAffinityDlg::OnBnClickedOk()
{
  UpdateData();

  ULONG64 process, system;
  system = m_system;

  for(int i=0; i < sizeof(DWORD_PTR)*8; ++i) {
    if ((system & 1) && m_CPUList.GetCheck(i))
      process |= (1i64 << i);
    system = system >> 1;
  }

  m_pProcess->SetAffinityMask(process);

  BOOL found = FALSE;
  if (trap_it) {

    if (WaitForSingleObject(hTrapsMutex, INFINITE) == WAIT_OBJECT_0) {

      TrapList::iterator i;
  	  for (i = m_traps->begin(); i != m_traps->end(); i++) {
	  	  CTrap * trap = (CTrap *)(*i);
        if (trap->getType() == 12 && m_pProcess->strName == trap->getProcessName()) {
          found = TRUE;
          // Modify
          trap->seti64DWORD(process);
        }
      }
      if (!found) {
        // Add
        CTrap * trap = new CTrap(m_pProcess->strName, 12, L"", process);
        m_traps->push_back(trap);
      }

      ReleaseMutex(hTrapsMutex);
    }
  }

  CDialog::OnOK();
}
