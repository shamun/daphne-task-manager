// CPUUsageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "CPUUsageDlg.h"


// CCPUUsageDlg dialog

IMPLEMENT_DYNAMIC(CCPUUsageDlg, CDialog)

CCPUUsageDlg::CCPUUsageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCPUUsageDlg::IDD, pParent)
  , info(_T(""))
  , used_phy(0)
  , total_phy(0)
  , sys_cache(0)
  , kernel_mem(0)
  , threads(0)
  , process(0)
  , handles(0)
  , pagesize(0)
{

}

CCPUUsageDlg::~CCPUUsageDlg()
{
}

void CCPUUsageDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_GRAPH, graph);
  DDX_Text(pDX, IDC_STINFO, info);
  DDX_Text(pDX, IDC_CU_UPHY, used_phy);
  DDX_Text(pDX, IDC_CU_TPHY, total_phy);
  DDX_Text(pDX, IDC_CU_SYSC, sys_cache);
  DDX_Text(pDX, IDC_CU_MEMK, kernel_mem);
  DDX_Text(pDX, IDC_CU_THR, threads);
  DDX_Text(pDX, IDC_CU_PRO, process);
  DDX_Text(pDX, IDC_CU_HND, handles);
  DDX_Text(pDX, IDC_CU_PAGE, pagesize);
}


BEGIN_MESSAGE_MAP(CCPUUsageDlg, CDialog)
  ON_WM_TIMER()
  ON_WM_MOUSEWHEEL()
  ON_NOTIFY(UDN_DELTAPOS, IDC_TFACTOR, &CCPUUsageDlg::OnDeltaposTfactor)
END_MESSAGE_MAP()


// CCPUUsageDlg message handlers

BOOL CCPUUsageDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Statistics
  CDaphneApp::hitStatisticalRecord(L"CPUUsage");

	// Language
	SetWindowText(_(" CPU Usage"));
	SetDlgItemText(IDCANCEL, _("Close"));

  SetDlgItemText(IDC_CU_STA, _("System information"));
  SetDlgItemText(IDC_CU_UPHY_T, _("Used physical memory [MB]"));
  SetDlgItemText(IDC_CU_TPHY_T, _("Total physical memory [MB]"));
  SetDlgItemText(IDC_CU_SYSC_T, _("System cache [MB]"));
  SetDlgItemText(IDC_CU_MEMK_T, _("Kernel memory [MB]"));
  SetDlgItemText(IDC_CU_THR_T, _("Threads"));
  SetDlgItemText(IDC_CU_PRO_T, _("Process"));
  SetDlgItemText(IDC_CU_HND_T, _("Handles"));


	if (AfxGetApp()->GetProfileInt(L"CFG", L"MainFastRefresh", 1)) {
		SetTimer(0, 1000, NULL);
		SetTimer(1, 1500, NULL);
	}
	else {
		SetTimer(0, 2000, NULL);
		SetTimer(1, 2250, NULL);
	}

  showTimeData();

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CCPUUsageDlg::OnTimer(UINT_PTR nIDEvent)
{
  PERFORMANCE_INFORMATION info;

  if (nIDEvent == 0) {
    Invalidate(0);
  }
  else if (nIDEvent == 1) {
    // Info
    info.cb = sizeof(PERFORMANCE_INFORMATION);
    if (GetPerformanceInfo(&info, info.cb)) {
      pagesize = info.PageSize;
      threads = info.ThreadCount;
      process = info.ProcessCount;
      handles = info.HandleCount;
      used_phy = (info.PhysicalTotal - info.PhysicalAvailable) * info.PageSize / 1048576;
      total_phy = info.PhysicalTotal * info.PageSize / 1048576;
      sys_cache = info.SystemCache * info.PageSize / 1048576;
      kernel_mem = info.KernelTotal * info.PageSize / 1048576;
      UpdateData(FALSE);
    }
  }

  CDialog::OnTimer(nIDEvent);
}

BOOL CCPUUsageDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
  if (zDelta >= 0) {
    graph.expandTime();
  }
  else {
    graph.collapseTime();
  }
  showTimeData();

  return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CCPUUsageDlg::showTimeData(void)
{
  int time;
	if (AfxGetApp()->GetProfileInt(L"CFG", L"MainFastRefresh", 1)) {
    time = DAPHNE_MAX_QUEUE_CPUDATA/graph.getTFactor();
	}
	else {
    time = DAPHNE_MAX_QUEUE_CPUDATA/(graph.getTFactor()*2);
	}

  info.Format(_("Showing last %d:%02d minutes"), time/60, time%60);
  UpdateData(FALSE);
}

void CCPUUsageDlg::OnDeltaposTfactor(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
  // TODO: Add your control notification handler code here
  *pResult = 0;

  if (pNMUpDown->iDelta < 0) {
    graph.expandTime();
  }
  else {
    graph.collapseTime();
  }
  showTimeData();

}
