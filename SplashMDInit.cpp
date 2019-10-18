// SplashMDInit.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "SplashMDInit.h"


// CSplashMDInit dialog

IMPLEMENT_DYNAMIC(CSplashMDInit, CDialog)

CSplashMDInit::CSplashMDInit(CWnd* pParent /*=NULL*/)
	: CDialog(CSplashMDInit::IDD, pParent)
	, strText(_T(""))
{

}

CSplashMDInit::~CSplashMDInit()
{
}

void CSplashMDInit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SPL_TXT, strText);
}


BEGIN_MESSAGE_MAP(CSplashMDInit, CDialog)
END_MESSAGE_MAP()


// CSplashMDInit message handlers

BOOL CSplashMDInit::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Language
	SetWindowText(_(" Daphne multidesktop")); // Ver CDaphneDlg::OnSettings()
	strText = _("Initializing...");
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSplashMDInit::SetMessage(CString text)
{
	strText = text;
	UpdateData(FALSE);
}
