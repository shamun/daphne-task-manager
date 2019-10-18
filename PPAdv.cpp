// PPAdv.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "PPAdv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPAdv property page

IMPLEMENT_DYNCREATE(CPPAdv, CPropertyPage)

CPPAdv::CPPAdv() : CPropertyPage(CPPAdv::IDD)
, m_strStoreCfgText(_T(""))
{
	//{{AFX_DATA_INIT(CPPAdv)
	m_bAlt = FALSE;
	m_bShift = FALSE;
	m_bSC = FALSE;
	m_bShowHidden = FALSE;
	m_bCtrl = FALSE;
	m_bJavaHide = FALSE;
	m_bDontCheckDebug = FALSE;
	//}}AFX_DATA_INIT
}

CPPAdv::~CPPAdv()
{
}

void CPPAdv::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPAdv)
	DDX_Control(pDX, IDC_SC_CTRL, m_cCtrl);
	DDX_Control(pDX, IDC_SC_SHIFT, m_cShift);
	DDX_Control(pDX, IDC_SC_ALT, m_cAlt);
	DDX_Control(pDX, IDC_SC_VKEY, m_cVK);
	DDX_Check(pDX, IDC_SC_ALT, m_bAlt);
	DDX_Check(pDX, IDC_SC_SHIFT, m_bShift);
	DDX_Check(pDX, IDC_SC_ON, m_bSC);
	DDX_Check(pDX, IDC_ADV_UNHIDE, m_bShowHidden);
	DDX_Check(pDX, IDC_SC_CTRL, m_bCtrl);
	DDX_Check(pDX, IDC_ADV_JAVAHIDE, m_bJavaHide);
	DDX_Check(pDX, IDC_CHECKDEBUG, m_bDontCheckDebug);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPPAdv, CPropertyPage)
	//{{AFX_MSG_MAP(CPPAdv)
	ON_BN_CLICKED(IDC_SC_ON, OnScOn)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_STORE_INI, &CPPAdv::OnBnClickedStoreIni)
	ON_BN_CLICKED(IDC_STORE_REG, &CPPAdv::OnBnClickedStoreReg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPAdv message handlers

BOOL CPPAdv::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();


	// Language
	SetWindowText(_("Advanced")); // Ver CDaphneDlg::OnSettings()
	SetDlgItemText(IDC_SC_ON, _("Install system wide Daphne activation shortcut"));
	SetDlgItemText(IDC_SC_SHIFT, _("Shift"));
	SetDlgItemText(IDC_SC_ALT, _("Alt"));
	SetDlgItemText(IDC_SC_CTRL, _("Ctrl"));
	SetDlgItemText(IDC_STG_SC, _("Advanced options"));
	SetDlgItemText(IDC_ADV_UNHIDE, _("Show all hidden applications on exit"));
	SetDlgItemText(IDC_ADV_JAVAHIDE, _("Try to hide Java VM from command line"));
	SetDlgItemText(IDC_CHECKDEBUG, _("Don't check Debug programs privilege on startup"));
	SetDlgItemText(IDC_STG_STRCFG, _("Store configuration"));
	SetDlgItemText(IDC_STORE_INI, _("Using Daphne.INI file"));
	SetDlgItemText(IDC_STORE_REG, _("Using Windows registry"));

	m_cVK.AddString(L"F12");
	m_cVK.AddString(L"F11");
	m_cVK.AddString(L"F10");
	m_cVK.AddString(L"F9");

	int iVK = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneSCVirtualKey", VK_F12);

	switch(iVK) {
	case VK_F12:
		m_cVK.SetCurSel(0);
		break;
	case VK_F11:
		m_cVK.SetCurSel(1);
		break;
	case VK_F10:
		m_cVK.SetCurSel(2);
		break;
	case VK_F9:
		m_cVK.SetCurSel(3);
		break;
	}

	if (((CDaphneApp*)AfxGetApp())->SettingsAtRegistry())
		CheckRadioButton(IDC_STORE_INI, IDC_STORE_REG, IDC_STORE_REG);
	else
		CheckRadioButton(IDC_STORE_INI, IDC_STORE_REG, IDC_STORE_INI);

	m_bSC = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneSC", 1);
	m_bAlt = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneSCAlt", 1);
	m_bShift = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneSCShift", 1);
	m_bCtrl = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneSCCtrl", 0);
	m_bShowHidden = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneAdvShowHidden", 0);
	m_bJavaHide = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneJavaVMHide", 0);
	m_bDontCheckDebug = AfxGetApp()->GetProfileInt(L"CFG", L"DontShowDebugWarning", 0);

	m_cAlt.EnableWindow(m_bSC);
	m_cShift.EnableWindow(m_bSC);
	m_cVK.EnableWindow(m_bSC);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPPAdv::OnOK() 
{
	UpdateData();

	ChangeSettingLocation();

	AfxGetApp()->WriteProfileInt(L"CFG", L"DaphneSC", m_bSC);
	AfxGetApp()->WriteProfileInt(L"CFG", L"DaphneSCAlt", m_bAlt);
	AfxGetApp()->WriteProfileInt(L"CFG", L"DaphneSCShift", m_bShift);
	AfxGetApp()->WriteProfileInt(L"CFG", L"DaphneSCCtrl", m_bCtrl);
	AfxGetApp()->WriteProfileInt(L"CFG", L"DaphneAdvShowHidden", m_bShowHidden);
	AfxGetApp()->WriteProfileInt(L"CFG", L"DaphneJavaVMHide", m_bJavaHide);
	AfxGetApp()->WriteProfileInt(L"CFG", L"DontShowDebugWarning", m_bDontCheckDebug);

	switch(m_cVK.GetCurSel()) {
	case 0:
		AfxGetApp()->WriteProfileInt(L"CFG", L"DaphneSCVirtualKey", VK_F12);
		break;
	case 1:
		AfxGetApp()->WriteProfileInt(L"CFG", L"DaphneSCVirtualKey", VK_F11);
		break;
	case 2:
		AfxGetApp()->WriteProfileInt(L"CFG", L"DaphneSCVirtualKey", VK_F10);
		break;
	case 3:
		AfxGetApp()->WriteProfileInt(L"CFG", L"DaphneSCVirtualKey", VK_F9);
		break;
	}
	
	CPropertyPage::OnOK();
}

void CPPAdv::OnScOn() 
{
	UpdateData();
	
	m_cAlt.EnableWindow(m_bSC);
	m_cShift.EnableWindow(m_bSC);
	m_cCtrl.EnableWindow(m_bSC);
	m_cVK.EnableWindow(m_bSC);
}

void CPPAdv::OnBnClickedStoreIni()
{
}

void CPPAdv::OnBnClickedStoreReg()
{
}

void CPPAdv::ChangeSettingLocation(void)
{
	if (GetCheckedRadioButton(IDC_STORE_INI, IDC_STORE_REG) == IDC_STORE_INI) {
		((CDaphneApp*)AfxGetApp())->ChangeSettingsLocation(TRUE);
	}
	else {
		((CDaphneApp*)AfxGetApp())->ChangeSettingsLocation(FALSE);
	}
}
