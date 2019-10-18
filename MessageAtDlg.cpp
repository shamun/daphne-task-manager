// MessageAtDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "MessageAtDlg.h"
#include "afxdialogex.h"


// CMessageAtDlg dialog

IMPLEMENT_DYNAMIC(CMessageAtDlg, CDialog)

CMessageAtDlg::CMessageAtDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMessageAtDlg::IDD, pParent)
  , date(0)
  , time(0)
  , message(_T(""))
{

}

CMessageAtDlg::~CMessageAtDlg()
{
}

void CMessageAtDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_DateTimeCtrl(pDX, IDC_DATE, date);
  DDX_DateTimeCtrl(pDX, IDC_TIME, time);
  DDX_Text(pDX, IDC_MSGATMSG, message);
  DDX_Control(pDX, IDC_TIME, ctrl_time);
}


BEGIN_MESSAGE_MAP(CMessageAtDlg, CDialog)
  ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TIME, &CMessageAtDlg::OnDtnDatetimechangeTime)
END_MESSAGE_MAP()


// CMessageAtDlg message handlers


BOOL CMessageAtDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

 	// Language
	SetWindowText(_(" Message at..."));
	SetDlgItemText(IDOK, _("OK"));
	SetDlgItemText(IDCANCEL, _("Cancel"));
  SetDlgItemText(IDC_STT_MESSAGE, _("Message:"));
	SetDlgItemText(IDC_STT_SELECTMA, _("Select date and time to display message at:"));

  date = CTime::GetCurrentTime();
  time = CTime::GetCurrentTime();
  time = CTime(time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), 0);
  
  UpdateData(FALSE);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}


void CMessageAtDlg::OnOK()
{
  UpdateData(TRUE);
  fulltime = CTime(date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute(), 0);
  CDialog::OnOK();
}


void CMessageAtDlg::OnDtnDatetimechangeTime(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
  
  pDTChange->st.wSecond = 0;
  ctrl_time.SetTime(pDTChange->st);

  *pResult = 0;
}
