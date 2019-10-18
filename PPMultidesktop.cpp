// PPMultidesktop.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "PPMultidesktop.h"


// CPPMultidesktop dialog

IMPLEMENT_DYNCREATE(CPPMultidesktop, CPropertyPage)

CPPMultidesktop::CPPMultidesktop()
	 : CPropertyPage(CPPMultidesktop::IDD)
	, m_bEnableMD(FALSE)
	, m_iExtraDesktops(0)
  , m_bContextMenu(FALSE)
  , m_strRS1(_T(""))
  , m_strRS2(_T(""))
  , m_strRS3(_T(""))
  , m_strRS4(_T(""))
{

}

CPPMultidesktop::~CPPMultidesktop()
{
}

void CPPMultidesktop::DoDataExchange(CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange(pDX);
  DDX_Check(pDX, IDC_SCMD_ON, m_bEnableMD);
  DDX_Text(pDX, IDC_CTRL_EDN, m_iExtraDesktops);
  DDX_Check(pDX, IDC_CONTEXTMENU_ON, m_bContextMenu);
  DDX_Text(pDX, IDC_RS_F1, m_strRS1);
  DDX_Text(pDX, IDC_RS_F2, m_strRS2);
  DDX_Text(pDX, IDC_RS_F3, m_strRS3);
  DDX_Text(pDX, IDC_RS_F4, m_strRS4);
}


BEGIN_MESSAGE_MAP(CPPMultidesktop, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ED, &CPPMultidesktop::OnDeltaposSpinEd)
  ON_BN_CLICKED(IDC_SCMD_ON, &CPPMultidesktop::OnBnClickedScmdOn)
  ON_BN_CLICKED(IDC_CONTEXTMENU_ON, &CPPMultidesktop::OnBnClickedContextmenuOn)
END_MESSAGE_MAP()


// CPPMultidesktop message handlers

BOOL CPPMultidesktop::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// Language
	SetWindowText(_("More...")); // Ver CDaphneDlg::OnSettings()
	SetDlgItemText(IDC_STG_MD, _("Multidesktop options"));
	SetDlgItemText(IDC_SCMD_ON, _("Enable Multidesktop feature (at Daphne's startup)"));
	SetDlgItemText(IDC_STED_NUM, _("Extra desktops"));
	SetDlgItemText(IDC_STG_IN, _("Integration options"));
  SetDlgItemText(IDC_CONTEXTMENU_ON, _("Enable Windows Explorer integration"));
  SetDlgItemText(IDC_STG_RUNSLOTS, _("Run slots"));

  tooltip.Create(this);
  tooltip.AddTool(GetDlgItem(IDC_STG_RUNSLOTS), _("Run these commands with Ctrl-Shift-F1 to F4"));
  tooltip.AddTool(GetDlgItem(IDC_RS_F1), _("Run this command with Ctrl-Shift-F1"));
  tooltip.AddTool(GetDlgItem(IDC_RS_F2), _("Run this command with Ctrl-Shift-F2"));
  tooltip.AddTool(GetDlgItem(IDC_RS_F3), _("Run this command with Ctrl-Shift-F3"));
  tooltip.AddTool(GetDlgItem(IDC_RS_F4), _("Run this command with Ctrl-Shift-F4"));

	m_bEnableMD = AfxGetApp()->GetProfileInt(L"CFG", L"UseMultidesktop", 0);
	m_iExtraDesktops = AfxGetApp()->GetProfileInt(L"CFG", L"MDExtraDesktops", 1);
  m_bContextMenu = AfxGetApp()->GetProfileInt(L"CFG", L"IntegrateWindowsExplorer", 1);

  m_strRS1 = AfxGetApp()->GetProfileString(L"CFG", L"RunSlotPath1", L"");
  m_strRS2 = AfxGetApp()->GetProfileString(L"CFG", L"RunSlotPath2", L"");
  m_strRS3 = AfxGetApp()->GetProfileString(L"CFG", L"RunSlotPath3", L"");
  m_strRS4 = AfxGetApp()->GetProfileString(L"CFG", L"RunSlotPath4", L"");

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPPMultidesktop::OnOK()
{
	UpdateData();

	AfxGetApp()->WriteProfileInt(L"CFG", L"UseMultidesktop", m_bEnableMD);
	AfxGetApp()->WriteProfileInt(L"CFG", L"MDExtraDesktops", m_iExtraDesktops);
	AfxGetApp()->WriteProfileInt(L"CFG", L"IntegrateWindowsExplorer", m_bContextMenu);

	AfxGetApp()->WriteProfileString(L"CFG", L"RunSlotPath1", m_strRS1);
	AfxGetApp()->WriteProfileString(L"CFG", L"RunSlotPath2", m_strRS2);
	AfxGetApp()->WriteProfileString(L"CFG", L"RunSlotPath3", m_strRS3);
	AfxGetApp()->WriteProfileString(L"CFG", L"RunSlotPath4", m_strRS4);

 	CRegKey key;

	if (key.Open(HKEY_CLASSES_ROOT, L"*\\shellex\\ContextMenuHandlers") == ERROR_SUCCESS) {

		if (m_bContextMenu) {
      key.SetKeyValue(L"DRKShell", L"{E18C8C1B-FD35-455E-BB21-4BA26B1E751D}");
		}
		else {
      key.DeleteSubKey(L"DRKShell");
		}

		key.Close();
	}
	if (key.Open(HKEY_CLASSES_ROOT, L"Folder\\shellex\\ContextMenuHandlers") == ERROR_SUCCESS) {

		if (m_bContextMenu) {
      key.SetKeyValue(L"DRKShell", L"{E18C8C1B-FD35-455E-BB21-4BA26B1E751D}");
		}
		else {
      key.DeleteSubKey(L"DRKShell");
		}

		key.Close();
	}
	if (key.Open(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\services\\eventlog\\Application") == ERROR_SUCCESS) {

		if (m_bContextMenu) {
      key.SetKeyValue(L"Daphne.DRKShell", L"");
      CRegKey subkey;
      if (subkey.Open(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\services\\eventlog\\Application\\Daphne.DRKShell") == ERROR_SUCCESS) {
        subkey.SetDWORDValue(L"CategoryCount", 3);
        subkey.SetDWORDValue(L"TypesSupported", 7);
        subkey.SetStringValue(L"CategoryMessageFile", strDRKShellDLL);
        subkey.SetStringValue(L"EventMessageFile", strDRKShellDLL);
        subkey.SetStringValue(L"ParameterMessageFile", strDRKShellDLL);
      }
		}
		else {
      key.DeleteSubKey(L"Daphne.DRKShell");
		}

		key.Close();
	}

  // Extras
  if (key.Open(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced") == ERROR_SUCCESS) {

		if (m_bContextMenu) {
      DWORD prev;
      if (key.QueryDWORDValue(L"EncryptionContextMenu", prev) == ERROR_SUCCESS)
        AfxGetApp()->WriteProfileInt(L"CFG", L"ExtrasPreviousEncrypt", prev);
      else
        AfxGetApp()->WriteProfileInt(L"CFG", L"ExtrasPreviousEncrypt", 0);
      key.SetDWORDValue(L"EncryptionContextMenu", 1);
		}
		else {
      key.SetDWORDValue(L"EncryptionContextMenu", AfxGetApp()->GetProfileInt(L"CFG", L"ExtrasPreviousEncrypt", 0));
		}

		key.Close();
	}


	CPropertyPage::OnOK();
}

void CPPMultidesktop::OnDeltaposSpinEd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here

	m_iExtraDesktops -= pNMUpDown->iDelta;
	if (m_iExtraDesktops < 1)
		m_iExtraDesktops = 1;
	if (m_iExtraDesktops > 3)
		m_iExtraDesktops = 3;

	UpdateData(FALSE);

	*pResult = 0;
}

void CPPMultidesktop::OnBnClickedScmdOn()
{
  WCHAR tmp[30];
  UpdateData();
  if (m_bEnableMD) {
    MessageBox(_("This feature is experimental. It won't damage your computer but,\nif you notice strange behavior after enabling it. Turn it off\nand report the situation."), __("Daphne multidesktop feature", tmp, sizeof(tmp)), MB_OK | MB_ICONWARNING);
  }
}

void CPPMultidesktop::OnBnClickedContextmenuOn()
{
}

BOOL CPPMultidesktop::PreTranslateMessage(MSG* pMsg)
{
  tooltip.RelayEvent(pMsg);

  return CPropertyPage::PreTranslateMessage(pMsg);
}
