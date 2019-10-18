// lssoftDlg.cpp : implementation file
//

#include "stdafx.h"
#include "daphne.h"
#include "lssoftDlg.h"
#include "Software.h"
#include "SoftwarePropertiesDlg.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// ClssoftDlg dialog
ClssoftDlg::ClssoftDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ClssoftDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ClssoftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTLS, listControl);
}

BEGIN_MESSAGE_MAP(ClssoftDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &ClssoftDlg::OnBnClickedCancel)
	ON_NOTIFY(NM_RCLICK, IDC_LISTLS, &ClssoftDlg::OnNMRclickList)
//	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &ClssoftDlg::OnHdnItemdblclickListls)
ON_NOTIFY(NM_DBLCLK, IDC_LISTLS, &ClssoftDlg::OnNMDblclkListls)
ON_BN_CLICKED(IDC_LSSOFT_EXPORT, &ClssoftDlg::OnBnClickedLssoftExport)
END_MESSAGE_MAP()


// ClssoftDlg message handlers

BOOL ClssoftDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

  // Statistics
  CDaphneApp::hitStatisticalRecord(L"InstalledSoftware");

	// Language
	SetWindowText(_(" Installed software list")); // Ver CDaphneDlg::OnSettings()
	SetDlgItemText(IDCANCEL, _("Close"));

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// SETUP
	listControl.InsertColumn(0, _("Software"), 0, 250, -1);
	listControl.InsertColumn(1, _("Version"), 0, 75, -1);
	listControl.InsertColumn(2, _("Publisher"), 0, 100, -1);
	listControl.InsertColumn(3, _("Publisher WEB"), 0, 200, -1);

	listControl.SetExtendedStyle(LVS_EX_FULLROWSELECT);


	// HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall
	HKEY hkResult;
	if (RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", &hkResult) == ERROR_SUCCESS) {

		TCHAR name[256]; // Límite de 255 para un nombre
		DWORD nameSize = 255;
		FILETIME ftLastWriteTime;
		LONG index = 0;
		while(RegEnumKeyEx(hkResult, index, name, &nameSize, NULL, NULL, NULL, &ftLastWriteTime) == ERROR_SUCCESS) {

			try {
				Software * s = new Software(hkResult, name);
				software.push_back(s);
			}
			catch(int error) {
				if (error == 1) {
					// Empty name
					continue;
				}
			}

			index++;
			nameSize = 255;
		}

		RegCloseKey(hkResult);
	}

  ClssoftDlg::column = AfxGetApp()->GetProfileInt(L"CFG", L"lssoftOrderColumn", 0);
	ClssoftDlg::inverted = AfxGetApp()->GetProfileInt(L"CFG", L"lssoftOrderMode", 0) > 0;
  
  reloadList();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

int ClssoftDlg::column = 0;
bool ClssoftDlg::inverted = false;

bool ClssoftDlg::whichFirst(const Software * left, const Software * right)
{
  switch(column) {
  case 0:
    if (!inverted)
      return left->name < right->name;
    else
      return left->name > right->name;
  case 1:
    if (!inverted)
      return left->version < right->version;
    else
      return left->version > right->version;
  case 2:
    if (!inverted)
      return left->publisher < right->publisher;
    else
      return left->publisher > right->publisher;
  case 3:
    if (!inverted)
      return left->publisherURL < right->publisherURL;
    else
      return left->publisherURL > right->publisherURL;
  }

  return left->name < right->name;
}
void ClssoftDlg::reloadList()
{
  std::sort(software.begin(), software.end(), ClssoftDlg::whichFirst);
  AfxGetApp()->WriteProfileInt(L"CFG", L"lssoftOrderColumn", ClssoftDlg::column);
  AfxGetApp()->WriteProfileInt(L"CFG", L"lssoftOrderMode", (ClssoftDlg::inverted)?1:0);

	int nCount = listControl.GetItemCount();
	for (int k=0; k < nCount; k++)
		listControl.DeleteItem(0);

	int item;
	for (SoftwareListPtrIt i = software.begin(); i != software.end(); i++) {
		item = listControl.InsertItem(listControl.GetItemCount(), (*i)->getName());
		listControl.SetItem(item, 1, LVIF_TEXT, (*i)->getVersion(), 0, 0, 0, 0);
		listControl.SetItem(item, 2, LVIF_TEXT, (*i)->getPublisher(), 0, 0, 0, 0);
		listControl.SetItem(item, 3, LVIF_TEXT, (*i)->getPublisherURL(), 0, 0, 0, 0);
	}
}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void ClssoftDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR ClssoftDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void ClssoftDlg::OnBnClickedCancel()
{
	OnCancel();
}

void ClssoftDlg::OnCancel()
{
	int nCount = listControl.GetItemCount();
	for (int k=0; k < nCount; k++)
		listControl.DeleteItem(0);

	for (SoftwareListPtrIt i = software.begin(); i != software.end(); i++) {
		delete (*i);
	}
	software.clear();

	CDialog::OnCancel();
}

void ClssoftDlg::showContextMenu(POINT & point)
{
	CMenu menu;

	menu.CreatePopupMenu();
	menu.InsertMenu(0, 0, IDM_UNINSTALL, _("Uninstall"));
	menu.InsertMenu(-1, MF_BYPOSITION, MF_SEPARATOR);
	menu.InsertMenu(-1, MF_BYPOSITION, IDM_PROPERTIES, _("Properties"));

	menu.SetDefaultItem(IDM_PROPERTIES);

	menu.TrackPopupMenu(0, point.x, point.y, this);

}

void ClssoftDlg::OnNMRclickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	POINT point;

	GetCursorPos(&point);
	showContextMenu(point);

	*pResult = 0;
}

BOOL ClssoftDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	WORD item = LOWORD(wParam);


	switch(item) {
	case IDM_UNINSTALL:
			uninstall();
		break;
	case IDM_PROPERTIES: {
			CSoftwarePropertiesDlg dlg(getSelectedSoftware());
			dlg.DoModal();
		 }
		break;
	}

	return CDialog::OnCommand(wParam, lParam);
}

Software* ClssoftDlg::getSelectedSoftware(void)
{
	int pos = listControl.GetSelectionMark();
	if (pos == -1)
		return NULL;
	return software[pos];
}

void ClssoftDlg::uninstall(void)
{
  WCHAR tmp[30];
	Software * s = getSelectedSoftware();

	if (s != NULL) {
		if (s->getUninstallString().IsEmpty()) {
			MessageBox(_("Uninstall information is not available"), __("ERROR", tmp, sizeof(tmp)), MB_OK | MB_ICONSTOP);
			return;
		}

		CString question;
		question.Format(_("Are you sure you want to remove %s from this computer?"), s->getName());
		if (MessageBox(question, _("Warning: this can not be undone"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
      // Primero lo suponemos un sólo argumento
      int r = (INT)::ShellExecute(NULL, L"OPEN", s->getUninstallString(), NULL, L"C:\\", SW_SHOWNORMAL);

      // Si falla, probamos separar por .exe
      if (r < 32) {
        int p = s->getUninstallString().MakeLower().Find(L".exe");
        r = (INT)::ShellExecute(NULL, 
                                L"OPEN", 
                                s->getUninstallString().Left(p+4),
                                s->getUninstallString().Right(s->getUninstallString().GetLength() - p - 5),
                                L"C:\\",
                                SW_SHOWNORMAL);
      }

      // Si falla, probamos separar por primer espacio
      if (r < 32) {
        int p = s->getUninstallString().Find(L" ",1);
        r = (INT)::ShellExecute(NULL, 
                                L"OPEN", 
                                s->getUninstallString().Left(p),
                                s->getUninstallString().Right(s->getUninstallString().GetLength() - p - 1),
                                L"C:\\",
                                SW_SHOWNORMAL);
      }

			if (r < 32)
				MessageBox(_("Unable to run uninstaller"), __("ERROR", tmp, sizeof(tmp)), MB_OK | MB_ICONSTOP);
      else
        PostMessage(WM_CLOSE, 0, 0);
		}
	}
}

void ClssoftDlg::OnNMDblclkListls(NMHDR *pNMHDR, LRESULT *pResult)
{
	CSoftwarePropertiesDlg dlg(getSelectedSoftware());
	dlg.DoModal();
	*pResult = 0;
}

void ClssoftDlg::WinHelp(DWORD dwData, UINT nCmd)
{
  AfxGetApp()->WinHelp(dwData, nCmd);
}

void ClssoftDlg::OnBnClickedLssoftExport()
{
  OPENFILENAME data;
  CString exportfile;

  SecureZeroMemory(&data, sizeof(data));
  data.hwndOwner = this->GetSafeHwnd();
  data.lpstrFilter = L"Comma separated values (*.csv)\0*.CSV\0Text files (*.txt)\0*.TXT\0All Files\0*.*\0\0";
  data.lpstrDefExt = L"csv";
  data.lpstrFile = exportfile.GetBufferSetLength(MAX_PATH);
  data.nMaxFile = MAX_PATH;
  data.lpstrTitle = _("Export installed software list to...");
  data.lStructSize = sizeof (OPENFILENAME);
  data.Flags = OFN_DONTADDTORECENT|OFN_EXPLORER|OFN_NONETWORKBUTTON|OFN_NOREADONLYRETURN|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST;
  if (GetSaveFileName(&data)) {
    exportfile.ReleaseBuffer();

    // Export
    CStdioFile f;
    CFileException fileException;
    CString line, format;

    if (exportfile.Right(3) == "CSV")
      format = L"%s,%s,%s,%s\n";
    else if (exportfile.Right(3) == "TXT")
      format = L"'%s' %s '%s' %s\n";
    else
      format = L"%s,%s,%s,%s\n";

    if (f.Open( exportfile, CFile::modeCreate | CFile::modeWrite, &fileException ) )
    {
	    for (SoftwareListPtrIt i = software.begin(); i != software.end(); i++) {
        line.Format(format, (*i)->getName(), (*i)->getVersion(), (*i)->getPublisher(), (*i)->getPublisherURL());
        f.WriteString(line);
	    }
      f.Close();
    }

  }
}
