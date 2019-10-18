// ScheduleShutdownDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "ScheduleShutdownDlg.h"


// CScheduleShutdownDlg dialog

IMPLEMENT_DYNAMIC(CScheduleShutdownDlg, CDialog)

CScheduleShutdownDlg::CScheduleShutdownDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScheduleShutdownDlg::IDD, pParent)
  , date(0)
  , time(0)
  , type(0)
{

}

CScheduleShutdownDlg::~CScheduleShutdownDlg()
{
}

void CScheduleShutdownDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_DateTimeCtrl(pDX, IDC_DATE, date);
  DDX_DateTimeCtrl(pDX, IDC_TIME, time);
  DDX_Control(pDX, IDC_TYPE, type_ctrl);
  DDX_Control(pDX, IDC_TIME, ctrl_time);
}


BEGIN_MESSAGE_MAP(CScheduleShutdownDlg, CDialog)
  ON_BN_CLICKED(IDOK, &CScheduleShutdownDlg::OnBnClickedOk)
  ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TIME, &CScheduleShutdownDlg::OnDtnDatetimechangeTime)
END_MESSAGE_MAP()


// CScheduleShutdownDlg message handlers

void CScheduleShutdownDlg::OnBnClickedOk()
{
  UpdateData(TRUE);
  fulltime = CTime(date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute(), 0);
  switch(type_ctrl.GetCurSel()) {
    case 0:
      type = 8;
      break;
    case 1:
      type = 2;
      break;
    case 2:
      type = 0;
      break;
  }
  OnOK();
}

BOOL CScheduleShutdownDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

 	// Language
	SetWindowText(_(" Shutdown at..."));
	SetDlgItemText(IDOK, _("OK"));
	SetDlgItemText(IDCANCEL, _("Cancel"));
	SetDlgItemText(IDC_STT_SELECTKA, _("Schedule system shutdown at:"));

  type_ctrl.AddString(_("Shutdown"));
  type_ctrl.AddString(_("Restart"));
  type_ctrl.AddString(_("Log off"));
  type_ctrl.SetCurSel(0);

  date = CTime::GetCurrentTime();
  time = CTime::GetCurrentTime();
  time = CTime(time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), 0);
  UpdateData(FALSE);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}


void CScheduleShutdownDlg::OnDtnDatetimechangeTime(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
  
  pDTChange->st.wSecond = 0;
  ctrl_time.SetTime(pDTChange->st);

  *pResult = 0;
}
