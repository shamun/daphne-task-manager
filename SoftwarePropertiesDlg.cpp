// SoftwarePropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "daphne.h"
#include "SoftwarePropertiesDlg.h"


// CSoftwarePropertiesDlg dialog

IMPLEMENT_DYNAMIC(CSoftwarePropertiesDlg, CDialog)

CSoftwarePropertiesDlg::CSoftwarePropertiesDlg(Software * soft, CWnd* pParent /*=NULL*/)
	: CDialog(CSoftwarePropertiesDlg::IDD, pParent)
	, title(_T(""))
	, version(_T(""))
	, publisher(_T(""))
	, website(_T(""))
	, raw(_T(""))
	, soft(soft)
	, courier(NULL)
{

}

CSoftwarePropertiesDlg::~CSoftwarePropertiesDlg()
{
}

void CSoftwarePropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LSTITLE, title);
	DDX_Text(pDX, IDC_LSVERSION, version);
	DDX_Text(pDX, IDC_LSPUBLISHER, publisher);
	DDX_Text(pDX, IDC_LSWEB, website);
	DDX_Text(pDX, IDC_LSRAW, raw);
	DDX_Control(pDX, IDC_LSRAW, rawControl);
}


BEGIN_MESSAGE_MAP(CSoftwarePropertiesDlg, CDialog)
ON_BN_CLICKED(IDCANCEL, &CSoftwarePropertiesDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSoftwarePropertiesDlg message handlers

BOOL CSoftwarePropertiesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Language
	SetWindowText(_(" Properties")); // Ver CDaphneDlg::OnSettings()
	SetDlgItemText(IDCANCEL, _("Close"));
	SetDlgItemText(IDC_ST_LSVER, _("Version:"));
	SetDlgItemText(IDC_ST_LSWEB, _("Website:"));
	SetDlgItemText(IDC_ST_LSPUB, _("Publisher:"));
	SetDlgItemText(IDC_ST_LSRAWT, _("Raw information view"));
	
	courier = new CFont();
	if (courier) {
		courier->CreatePointFont(90, L"Courier New");

		rawControl.SetFont(courier);
	}

	title = soft->getName();
	version = soft->getVersion();
	publisher = soft->getPublisher();
	website = soft->getPublisherURL();
	raw = soft->getRAWInformation();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSoftwarePropertiesDlg::OnBnClickedCancel()
{
	if (courier)
		delete courier;

	OnCancel();
}

void CSoftwarePropertiesDlg::WinHelp(DWORD dwData, UINT nCmd)
{
  AfxGetApp()->WinHelp(dwData, nCmd);
}
