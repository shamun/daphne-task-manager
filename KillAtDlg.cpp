// KillAtDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "KillAtDlg.h"


// CKillAtDlg dialog

IMPLEMENT_DYNAMIC(CKillAtDlg, CDialog)

CKillAtDlg::CKillAtDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKillAtDlg::IDD, pParent)
  , date(0)
  , time(0)
  , fulltime(0)
  , polite(false)
  , pname(_T(""))
{

}

CKillAtDlg::~CKillAtDlg()
{
}

void CKillAtDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_DateTimeCtrl(pDX, IDC_DATE, date);
  DDX_DateTimeCtrl(pDX, IDC_TIME, time);
  DDX_Check(pDX, IDC_BE_POLITE, polite);
  DDX_Text(pDX, IDC_PNAME, pname);
  DDX_Control(pDX, IDC_TIME, ctrl_time);
}


BEGIN_MESSAGE_MAP(CKillAtDlg, CDialog)
  ON_BN_CLICKED(IDOK, &CKillAtDlg::OnBnClickedOk)
  ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TIME, &CKillAtDlg::OnDtnDatetimechangeTime)
END_MESSAGE_MAP()


// CKillAtDlg message handlers

BOOL CKillAtDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

 	// Language
	SetWindowText(_(" Kill at..."));
	SetDlgItemText(IDOK, _("OK"));
	SetDlgItemText(IDCANCEL, _("Cancel"));
  SetDlgItemText(IDC_STT_PTK, _("Process to kill:"));
	SetDlgItemText(IDC_STT_SELECTKA, _("Select date and time to kill the process at:"));
	SetDlgItemText(IDC_BE_POLITE, _("Be polite"));

  date = CTime::GetCurrentTime();
  time = CTime::GetCurrentTime();
  time = CTime(time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), 0);
  pname = processname;
  UpdateData(FALSE);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CKillAtDlg::OnBnClickedOk()
{
  UpdateData(TRUE);
  fulltime = CTime(date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute(), 0);
  OnOK();
}


void CKillAtDlg::OnDtnDatetimechangeTime(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

  pDTChange->st.wSecond = 0;
  ctrl_time.SetTime(pDTChange->st);

  *pResult = 0;
}
