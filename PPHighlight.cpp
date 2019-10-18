// PPHighlight.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "PPHighlight.h"
#include "KMIEditPListDlg.h"
#include "ProcList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPHighlight property page

IMPLEMENT_DYNCREATE(CPPHighlight, CPropertyPage)

CPPHighlight::CPPHighlight() : CPropertyPage(CPPHighlight::IDD)
, m_bHLTopMem(FALSE)
{
	//{{AFX_DATA_INIT(CPPHighlight)
	m_strList = _T("");
	m_bHLTop = FALSE;
	m_bHLSys = FALSE;
	//}}AFX_DATA_INIT
}

CPPHighlight::~CPPHighlight()
{
}

void CPPHighlight::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPHighlight)
	DDX_Text(pDX, IDC_HL_LIST, m_strList);
	DDX_Check(pDX, IDC_HL_HLTOP, m_bHLTop);
	DDX_Check(pDX, IDC_HL_HLSYSTEM, m_bHLSys);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_HL_HLTOPMEM, m_bHLTopMem);
}


BEGIN_MESSAGE_MAP(CPPHighlight, CPropertyPage)
	//{{AFX_MSG_MAP(CPPHighlight)
	ON_BN_CLICKED(IDC_HL_PROCS, OnHlProcs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPHighlight message handlers

BOOL CPPHighlight::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();


	// Language
	SetWindowText(_("Highlights")); // Ver CDaphneDlg::OnSettings()
	SetDlgItemText(IDC_STT_HLLIST, _("Add process names to highlight in the main list:"));
	SetDlgItemText(IDC_HL_HLSYSTEM, _("Highlight system process with blue"));
	SetDlgItemText(IDC_HL_HLTOP, _("Hightlight process over 95% CPU in red"));
	SetDlgItemText(IDC_HL_HLTOPMEM, _("Highlight orange, process with more than 20% of memory used"));

	m_strList = AfxGetApp()->GetProfileString(L"CFG", L"Highlights", L"");
	m_bHLSys = AfxGetApp()->GetProfileInt(L"CFG", L"HighlightSystem", 1);
	m_bHLTop = AfxGetApp()->GetProfileInt(L"CFG", L"HighlightTop", 1);
	m_bHLTopMem = AfxGetApp()->GetProfileInt(L"CFG", L"HighlightTopMem", 1);

	// Creo una lista de nombres de procesos
	for(int j=0; j < pProcList->GetItemCount(); j++)
		cprocs.push_back((LPCWSTR)pProcList->GetItemText(j, 1));

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPPHighlight::OnOK() 
{
	UpdateData(TRUE);

	AfxGetApp()->WriteProfileString(L"CFG", L"Highlights", m_strList);
	AfxGetApp()->WriteProfileInt(L"CFG", L"HighlightSystem", m_bHLSys);
	AfxGetApp()->WriteProfileInt(L"CFG", L"HighlightTop", m_bHLTop);
	AfxGetApp()->WriteProfileInt(L"CFG", L"HighlightTopMem", m_bHLTopMem);

	CPropertyPage::OnOK();
}

void CPPHighlight::OnHlProcs() 
{
	CKMIEditPListDlg dlg;

	UpdateData();

	dlg.cprocs = cprocs;
	dlg.kpList = &m_strList;

	if (dlg.DoModal() == IDOK)
	UpdateData(FALSE);	
}
