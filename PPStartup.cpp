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

// PPStartup.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "PPStartup.h"
#include "DaphneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPStartup property page

IMPLEMENT_DYNCREATE(CPPStartup, CPropertyPage)

CPPStartup::CPPStartup() : CPropertyPage(CPPStartup::IDD)
{
	//{{AFX_DATA_INIT(CPPStartup)
	m_bStartHidden = FALSE;
	m_bAutoStart = FALSE;
	m_bAlternateBackground = FALSE;
	m_bHideZombies = FALSE;
	m_bUpdates = FALSE;
	m_strProxyAddress = _T("");
	m_iProxyPort = 0;
	m_bUseProxy = FALSE;
	m_bFastr = FALSE;
	//}}AFX_DATA_INIT
	pDaphne = NULL;
}

CPPStartup::~CPPStartup()
{
}

void CPPStartup::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPStartup)
	DDX_Control(pDX, IDC_STS_PROXY, m_cPText);
	DDX_Control(pDX, IDC_S_PPORT, m_cPPort);
	DDX_Control(pDX, IDC_S_PADDR, m_cPAddr);
	DDX_Check(pDX, IDC_S_START_HIDE, m_bStartHidden);
	DDX_Check(pDX, IDC_S_START_WIN, m_bAutoStart);
  DDX_Check(pDX, IDC_S_ALT_BKGND, m_bAlternateBackground);
	DDX_Check(pDX, IDC_S_HIDEZOMBIES, m_bHideZombies);
	DDX_Check(pDX, IDC_S_CHECKUP, m_bUpdates);
	DDX_Text(pDX, IDC_S_PADDR, m_strProxyAddress);
	DDX_Text(pDX, IDC_S_PPORT, m_iProxyPort);
	DDX_Check(pDX, IDC_S_PROXY, m_bUseProxy);
	DDX_Check(pDX, IDC_S_FASTR, m_bFastr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPPStartup, CPropertyPage)
	//{{AFX_MSG_MAP(CPPStartup)
	ON_BN_CLICKED(IDC_S_PROXY, OnSProxy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPStartup message handlers

void CPPStartup::OnOK() 
{
	UpdateData();

	AfxGetApp()->WriteProfileInt(L"CFG", L"MainWndStartHidden", m_bStartHidden);
	AfxGetApp()->WriteProfileInt(L"CFG", L"DaphneAutoStart", m_bAutoStart);
	AfxGetApp()->WriteProfileInt(L"CFG", L"DaphneCheckUpdates", m_bUpdates);
	AfxGetApp()->WriteProfileInt(L"CFG", L"DaphneAlternateBackground", m_bAlternateBackground);
	AfxGetApp()->WriteProfileInt(L"CFG", L"DaphneHideZombies", m_bHideZombies);
	AfxGetApp()->WriteProfileInt(L"CFG", L"MainFastRefresh", m_bFastr);

	AfxGetApp()->WriteProfileInt(L"CFG", L"DaphneProxy", m_bUseProxy);
	AfxGetApp()->WriteProfileString(L"CFG", L"DaphneProxyAddress", m_strProxyAddress);
	AfxGetApp()->WriteProfileInt(L"CFG", L"DaphneProxyPort", m_iProxyPort);

	pDaphne->m_bAlternateBackground = m_bAlternateBackground;

	CRegKey start_key;

	if (start_key.Open(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run") == ERROR_SUCCESS) {

		if (m_bAutoStart) {
			start_key.SetStringValue(L"LHFDaphne", strDaphneExe);
		}
		else {
			start_key.DeleteValue(L"LHFDaphne");
		}

		start_key.Close();
	}

	CPropertyPage::OnOK();
}

BOOL CPPStartup::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// Language
	SetWindowText(_("Main")); // Ver CDaphneDlg::OnSettings()
	SetDlgItemText(IDC_STG_STARTU, _("Daphne startup"));
	SetDlgItemText(IDC_STG_BEHAVE, _("Behavior"));
	SetDlgItemText(IDC_STS_PROXY, _("Proxy server:"));
	SetDlgItemText(IDC_S_PROXY, _("Use HTTP proxy"));
	SetDlgItemText(IDC_S_START_WIN, _("Start Daphne automatically when starting Windows"));
	SetDlgItemText(IDC_S_START_HIDE, _("Always hide Daphne on startup"));
	SetDlgItemText(IDC_S_CHECKUP, _("Check internet for Daphne updates on startup"));
  SetDlgItemText(IDC_S_ALT_BKGND, _("Alternate list item background color"));
	SetDlgItemText(IDC_S_HIDEZOMBIES, _("Hide zombie processes"));
	SetDlgItemText(IDC_S_FASTR, _("Fast refresh"));

	
	m_bStartHidden = AfxGetApp()->GetProfileInt(L"CFG", L"MainWndStartHidden", 0);
	m_bAutoStart = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneAutoStart", 0);
	m_bUpdates = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneCheckUpdates", 1);
	m_bAlternateBackground = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneAlternateBackground", 0);
	m_bHideZombies = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneHideZombies", 0);
	m_bFastr = AfxGetApp()->GetProfileInt(L"CFG", L"MainFastRefresh", 1);
	m_bUseProxy = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneProxy", 0);
	m_strProxyAddress = AfxGetApp()->GetProfileString(L"CFG", L"DaphneProxyAddress", L"");
	m_iProxyPort = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneProxyPort", 8080);
	UpdateData(FALSE);

	m_cPAddr.EnableWindow(m_bUseProxy);
	m_cPPort.EnableWindow(m_bUseProxy);
	m_cPText.EnableWindow(m_bUseProxy);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPPStartup::OnSProxy() 
{
	UpdateData();
	
	m_cPAddr.EnableWindow(m_bUseProxy);
	m_cPPort.EnableWindow(m_bUseProxy);
	m_cPText.EnableWindow(m_bUseProxy);
}
