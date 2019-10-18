// PPLogging.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "PPLogging.h"


// CPPLogging dialog

IMPLEMENT_DYNAMIC(CPPLogging, CPropertyPage)

CPPLogging::CPPLogging()
	: CPropertyPage(CPPLogging::IDD)
  , logProcess(FALSE)
  , logProcessFile(_T(""))
{

}

CPPLogging::~CPPLogging()
{
}

void CPPLogging::DoDataExchange(CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange(pDX);
  DDX_Check(pDX, IDC_PPL_LOGP, logProcess);
  DDX_Text(pDX, IDC_PPL_LOGPF, logProcessFile);
  DDX_Control(pDX, IDC_PPL_LOGPF, cLogProcessFile);
  DDX_Control(pDX, IDC_PPL_LOGPS, cLogProcessFileSelector);
}


BEGIN_MESSAGE_MAP(CPPLogging, CPropertyPage)
  ON_BN_CLICKED(IDC_PPL_LOGPS, &CPPLogging::OnBnClickedPplLogps)
  ON_BN_CLICKED(IDC_PPL_LOGP, &CPPLogging::OnBnClickedPplLogp)
END_MESSAGE_MAP()


// CPPLogging message handlers

BOOL CPPLogging::OnInitDialog()
{
  CPropertyPage::OnInitDialog();

  // Language
  SetWindowText(_("Logging")); // Ver CDaphneDlg::OnSettings()
  SetDlgItemText(IDC_STG_LOG, _("Logging options"));
  SetDlgItemText(IDC_PPL_LOGP, _("Log started and ended process"));

  logProcess = AfxGetApp()->GetProfileInt(L"LOG", L"LogProcess", 0);
  logProcessFile = AfxGetApp()->GetProfileString(L"LOG", L"LogProcessFile", L"");

  refreshGUI();

  UpdateData(FALSE);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}


void CPPLogging::OnBnClickedPplLogps()
{
  UpdateData(TRUE);
  OPENFILENAME data;
  SecureZeroMemory(&data, sizeof(data));
  data.hwndOwner = this->GetSafeHwnd();
  data.lpstrFilter = L"Log files (*.log)\0*.LOG\0All Files\0*.*\0\0";
  data.lpstrDefExt = L"log";
  data.lpstrFile = logProcessFile.GetBufferSetLength(MAX_PATH);
  data.nMaxFile = MAX_PATH;
  data.lpstrTitle = _("Log process activity to...");
  data.lStructSize = sizeof (OPENFILENAME);
  data.Flags = OFN_DONTADDTORECENT|OFN_EXPLORER|OFN_NONETWORKBUTTON|OFN_NOREADONLYRETURN|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST;
  if (GetSaveFileName(&data)) {
    logProcessFile.ReleaseBuffer();
    UpdateData(FALSE);
  }
}

void CPPLogging::OnOK()
{
	UpdateData();

  AfxGetApp()->WriteProfileInt(L"LOG", L"LogProcess", logProcess);
  AfxGetApp()->WriteProfileString(L"LOG", L"LogProcessFile", logProcessFile);

  CPropertyPage::OnOK();
}

void CPPLogging::refreshGUI(void)
{
  cLogProcessFile.EnableWindow(logProcess);
  cLogProcessFileSelector.EnableWindow(logProcess);
}

void CPPLogging::OnBnClickedPplLogp()
{
  UpdateData();
  refreshGUI();
}
