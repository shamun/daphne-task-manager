// CloswAllWndByTitle.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "CloseAllWndByTitle.h"


// CCloswAllWndByTitle dialog

IMPLEMENT_DYNAMIC(CCloseAllWndByTitle, CDialog)

CCloseAllWndByTitle::CCloseAllWndByTitle(CWnd* pParent /*=NULL*/)
	: CDialog(CCloseAllWndByTitle::IDD, pParent)
	, m_strName(_T(""))
	, m_bCase(FALSE)
{

}

CCloseAllWndByTitle::~CCloseAllWndByTitle()
{
}

void CCloseAllWndByTitle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WCA_NAME, m_strName);
	DDX_Check(pDX, IDC_WCA_CASE, m_bCase);
}


BEGIN_MESSAGE_MAP(CCloseAllWndByTitle, CDialog)
	ON_BN_CLICKED(IDOK, &CCloseAllWndByTitle::OnBnClickedOk)
END_MESSAGE_MAP()


// CCloswAllWndByTitle message handlers

void CCloseAllWndByTitle::OnBnClickedOk()
{
	UpdateData(TRUE);
	OnOK();
}

BOOL CCloseAllWndByTitle::OnInitDialog()
{
	CDialog::OnInitDialog();

  // Statistics
  CDaphneApp::hitStatisticalRecord(L"CloseWndByName");

	// Language
	SetWindowText(_(" Close all windows by title"));
	SetDlgItemText(IDOK, _("OK"));
	SetDlgItemText(IDCANCEL, _("Cancel"));
	SetDlgItemText(IDC_STT_WND, _("Window title contains:"));
	SetDlgItemText(IDC_WCA_CASE, _("case-sensitive"));


	m_bCase = TRUE;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
