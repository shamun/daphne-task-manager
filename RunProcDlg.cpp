/***********************************************************************************
	
	Daphne -- GPL task manager replacement for Windows
	Copyright (C) 2005  Leandro H. Fernández

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

***********************************************************************************/

// RunProcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "RunProcDlg.h"
#include "AuthUserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static WCHAR BASED_CODE szFilter[] = L"Program files|*.exe;*.com;*.bat;*.cmd;*.pif|All files (*.*)|*.*||\0";

/////////////////////////////////////////////////////////////////////////////
// CRunProcDlg dialog


CRunProcDlg::CRunProcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRunProcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRunProcDlg)
	m_strArg = _T("");
	m_strDir = _T("");
	m_strPrg = _T("");
	m_bSuspended = FALSE;
	//}}AFX_DATA_INIT
	pInfo = NULL;
}


void CRunProcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRunProcDlg)
	DDX_Text(pDX, IDC_R_ARG, m_strArg);
	DDX_Text(pDX, IDC_R_DIR, m_strDir);
	DDX_Text(pDX, IDC_R_PRG, m_strPrg);
	DDX_Check(pDX, IDC_RP_SUSP, m_bSuspended);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRunProcDlg, CDialog)
	//{{AFX_MSG_MAP(CRunProcDlg)
	ON_BN_CLICKED(IDC_EXPLORE, OnExplore)
	ON_BN_CLICKED(IDC_EXPLORE2, OnExploreDir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRunProcDlg message handlers

void CRunProcDlg::OnExplore() 
{
	UpdateData();

	LPCTSTR file = NULL;
	if (m_strPrg.GetLength())
		file = (LPCTSTR)m_strPrg;

	CFileDialog * dlg = new CFileDialog(TRUE, L"EXE", file, 0, szFilter, this);
	if (dlg->DoModal() == IDOK) {
		m_strPrg = dlg->GetPathName();

		if (m_strDir.GetLength() == 0)
			if (m_strPrg.ReverseFind('\\') != -1)
				m_strDir = m_strPrg.Left(m_strPrg.ReverseFind('\\'));
		UpdateData(FALSE);
	}

	delete dlg;
}

void CRunProcDlg::OnOK()
{
	UpdateData();

	if (m_strPrg.GetLength()) {

		STARTUPINFO startup;
		PROCESS_INFORMATION pinfo;
		DWORD nFlags = 0;

		if (m_bSuspended)
			nFlags |= CREATE_SUSPENDED;

    ZeroMemory( &pinfo, sizeof(pinfo) );
    ZeroMemory( &startup, sizeof(startup) );
		startup.cb = sizeof(STARTUPINFO);
		startup.dwFlags = STARTF_USESHOWWINDOW;
		startup.wShowWindow = SW_SHOWDEFAULT;
		startup.lpDesktop = NULL;
		startup.lpTitle = NULL;
		startup.cbReserved2 = 0;
		startup.lpReserved2 = NULL;

    TCHAR args[32*1024];
    memset(args, 0, 32*1024);
    if (!m_strArg.IsEmpty())
      wcscpy_s(args, 32*1024, (LPCTSTR)m_strArg);

		BOOL result = FALSE;
		if (pInfo == NULL)
			result = CreateProcess((LPCTSTR)m_strPrg, args, NULL, NULL, FALSE, nFlags, NULL, (LPCTSTR)m_strDir, &startup, &pinfo);
		else
			result = CreateProcessWithLogonW(pInfo->m_strUser, NULL, pInfo->m_strPasswd, LOGON_WITH_PROFILE, (LPCTSTR)m_strPrg, args, nFlags, NULL, (LPCTSTR)m_strDir, &startup, &pinfo);

		if (!result) {
			ShowMsgLastError(m_hWnd);
		}
		else {
			//OK
			AfxGetApp()->WriteProfileString(L"CFG", L"RPProgram", m_strPrg);
			AfxGetApp()->WriteProfileString(L"CFG", L"RPArguments", m_strArg);
			AfxGetApp()->WriteProfileString(L"CFG", L"RPDirectory", m_strDir);
			AfxGetApp()->WriteProfileInt(L"CFG", L"RPSuspended", m_bSuspended);
		}
		m_strArg.ReleaseBuffer();
	}
	
	CDialog::OnOK();
}

void CRunProcDlg::OnExploreDir() 
{
	BROWSEINFO info;
	STRRET retval;
	WCHAR directory[MAX_PATH];

	UpdateData();

	LPCWSTR dir = NULL;
	if (m_strDir.GetLength())
		dir = (LPCTSTR)m_strDir;

	if (CoInitializeEx(NULL, COINIT_APARTMENTTHREADED) == S_OK) {

		info.hwndOwner = m_hWnd;
		info.pidlRoot = NULL;
		info.pszDisplayName = directory;
		info.lpszTitle = L"Select initial directory";
		info.ulFlags = BIF_NEWDIALOGSTYLE|BIF_RETURNONLYFSDIRS;
		info.lpfn = NULL;
		info.lParam = 0;
		info.iImage = 0;

		ITEMIDLIST * result = (ITEMIDLIST*)SHBrowseForFolder(&info);
		if (result != NULL) {

			IShellFolder * desktop;
			SHGetDesktopFolder(&desktop);

			retval.uType = STRRET_CSTR;
			desktop->GetDisplayNameOf(result, SHGDN_FORPARSING, &retval);
			StrRetToBuf(&retval, result, directory, sizeof(directory));

			desktop->Release();

			m_strDir = directory;

			UpdateData(FALSE);
		}

		CoUninitialize();
	}
}

BOOL CRunProcDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Language
	SetWindowText(_(" Run process"));
	SetDlgItemText(IDC_RP_SUSP, _("Start process suspended"));
	SetDlgItemText(IDOK, _("Run"));
	SetDlgItemText(IDCANCEL, _("Cancel"));
	SetDlgItemText(IDC_STT_PRG, _("Program:"));
	SetDlgItemText(IDC_STT_ARG, _("Arguments:"));
	SetDlgItemText(IDC_STT_DIR, _("Directory:"));


	m_bSuspended = AfxGetApp()->GetProfileInt(L"CFG", L"RPSuspended", 1);
	m_strPrg = AfxGetApp()->GetProfileString(L"CFG", L"RPProgram", L"");
	m_strArg = AfxGetApp()->GetProfileString(L"CFG", L"RPArguments", L"");
	m_strDir = AfxGetApp()->GetProfileString(L"CFG", L"RPDirectory", L"");

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
