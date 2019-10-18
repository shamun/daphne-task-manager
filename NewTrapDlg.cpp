// NewTrapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "NewTrapDlg.h"
#include "Trap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewTrapDlg dialog


CNewTrapDlg::CNewTrapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewTrapDlg::IDD, pParent)
  , m_stri64(_T(""))
{
	//{{AFX_DATA_INIT(CNewTrapDlg)
	m_strName = _T("");
	m_iType = -1;
	m_strWContains = _T("");
	//}}AFX_DATA_INIT
	trap = NULL;
}


void CNewTrapDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CNewTrapDlg)
  DDX_Control(pDX, IDC_STT_WCONT, m_lblWContains);
  DDX_Control(pDX, IDC_TR_WCONTAINS, m_cWContains);
  DDX_Control(pDX, IDC_TRAP, m_cTrap);
  DDX_Text(pDX, IDC_NAME, m_strName);
  DDX_CBIndex(pDX, IDC_TRAP, m_iType);
  DDX_Text(pDX, IDC_TR_WCONTAINS, m_strWContains);
  //}}AFX_DATA_MAP
  DDX_Text(pDX, IDC_GEN64INT, m_stri64);
}


BEGIN_MESSAGE_MAP(CNewTrapDlg, CDialog)
	//{{AFX_MSG_MAP(CNewTrapDlg)
	ON_CBN_SELCHANGE(IDC_TRAP, OnSelchangeTrap)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewTrapDlg message handlers

BOOL CNewTrapDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	// Language
	SetWindowText(_(" Trap..."));
	SetDlgItemText(IDOK, _("OK"));
	SetDlgItemText(IDCANCEL, _("Cancel"));
	SetDlgItemText(IDC_STT_PNAME, _("Process name:"));
	SetDlgItemText(IDC_STT_TYPE, _("Action:"));
	SetDlgItemText(IDC_STT_WCONT, _("Window title contains:"));
  SetDlgItemText(IDC_STT_GEN64INT, _("Generic 64 bits argument:"));

  tooltip.Create(this);
  tooltip.AddTool(GetDlgItem(IDC_GEN64INT), _("Use a 16 digit hexadecimal number here."));

	for(UINT i=0; i<= CTrap::getMaxType(); i++)
    if (CTrap::canEdit(i))
		  m_cTrap.AddString(CTrap::getTypeName(i));

	if (trap != NULL) {
		m_strName = trap->getProcessName();
		m_iType = trap->getType();
		m_strWContains = trap->getWindowContains();
    m_stri64.Format(L"%016llx", trap->geti64DWORD());
	}
	else {
		m_iType = 0;
	}
	
	UpdateData(FALSE);

	updateGUI();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewTrapDlg::OnOK() 
{
	UpdateData();

	if (trap == NULL) {

		trap = new CTrap(m_strName, m_iType, m_strWContains);
	}
	else {
		trap->setProcessName(m_strName);
		trap->setType(m_iType);
		trap->setWindowContains(m_strWContains);
    trap->seti64DWORD(_tcstoui64(m_stri64, NULL, 16));
	}

	CDialog::OnOK();
}

void CNewTrapDlg::updateGUI()
{
	UpdateData();

	m_lblWContains.EnableWindow(CTrap::appliesToWindow(m_iType));
	m_cWContains.EnableWindow(CTrap::appliesToWindow(m_iType));
}

void CNewTrapDlg::OnSelchangeTrap() 
{
	updateGUI();
}


BOOL CNewTrapDlg::PreTranslateMessage(MSG* pMsg)
{
  tooltip.RelayEvent(pMsg);

  return CDialog::PreTranslateMessage(pMsg);
}
