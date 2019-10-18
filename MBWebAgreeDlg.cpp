// MBWebAgreeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "MBWebAgreeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMBWebAgreeDlg dialog


CMBWebAgreeDlg::CMBWebAgreeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMBWebAgreeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMBWebAgreeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMBWebAgreeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMBWebAgreeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMBWebAgreeDlg, CDialog)
	//{{AFX_MSG_MAP(CMBWebAgreeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMBWebAgreeDlg message handlers

BOOL CMBWebAgreeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Language
	SetWindowText(_(" Web updates"));
	SetDlgItemText(IDC_STT_NOTE, _("Daphne is about to check the web for updates. This is done without sending any personally identifiable information to the web site."));
	SetDlgItemText(IDC_STT_Q, _("Do you want to check for updates at internet?"));
	SetDlgItemText(IDOK, _("Yes"));
	SetDlgItemText(IDCANCEL, _("No"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
