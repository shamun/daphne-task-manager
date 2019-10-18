// WindowSize.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "WindowSize.h"
#include "afxdialogex.h"


// CWindowSize dialog

IMPLEMENT_DYNAMIC(CWindowSizeDlg, CDialog)

CWindowSizeDlg::CWindowSizeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWindowSizeDlg::IDD, pParent)
  , m_strSize(_T(""))
  , m_strPre(_T(""))
{

}

CWindowSizeDlg::~CWindowSizeDlg()
{
}

void CWindowSizeDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_SETSIZE, m_strSize);
  DDX_Control(pDX, IDC_PRESIZE, m_cPre);
  DDX_CBString(pDX, IDC_PRESIZE, m_strPre);
}


BEGIN_MESSAGE_MAP(CWindowSizeDlg, CDialog)
  ON_CBN_SELCHANGE(IDC_PRESIZE, &CWindowSizeDlg::OnCbnSelchangePresize)
END_MESSAGE_MAP()


// CWindowSize message handlers


BOOL CWindowSizeDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

	SetWindowText(_(" Set window size"));
	SetDlgItemText(IDCANCEL, _("Close"));
	SetDlgItemText(IDOK, _("OK"));
	SetDlgItemText(IDC_STT_WS_NEWSIZE, _("New window size:"));
	SetDlgItemText(IDC_STT_WS_PREDEF, _("Load predefined size:"));

  m_cPre.AddString(L"");
  m_cPre.AddString(L"640x480 (4:3)");
  m_cPre.AddString(L"800x600 (4:3)");
  m_cPre.AddString(L"1024x768 (4:3)");
  m_cPre.AddString(L"1280x1024 (4:3)");
  m_cPre.AddString(L"1152×864 (4:3)");
  m_cPre.AddString(L"1280×960 (4:3)");
  m_cPre.AddString(L"1600×1200 (4:3)");
  m_cPre.AddString(L"854x480 (427:240)");
  m_cPre.AddString(L"960×540 (16:9)");
  m_cPre.AddString(L"1024×576 (16:9)");
  m_cPre.AddString(L"1280×720 (16:9)");
  m_cPre.AddString(L"1600×900 (16:9)");
  m_cPre.AddString(L"1920×1080 (16:9)");

  UpdateData(FALSE);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}


void CWindowSizeDlg::OnOK()
{
  UpdateData(TRUE);
  CDialog::OnOK();
}


void CWindowSizeDlg::OnCbnSelchangePresize()
{
  UpdateData(TRUE);
  if (m_strPre != L"") {
    m_strSize = m_strPre.Left(m_strPre.Find(L" "));
  }
  UpdateData(FALSE);
}
