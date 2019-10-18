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

// Inspector.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "Inspector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace DRKSpy;

/////////////////////////////////////////////////////////////////////////////
// CInspector dialog

CInspector::CInspector(CWnd* pParent /*=NULL*/)
	: CDialog(CInspector::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInspector)
	m_strTitle = _T("");
	m_strClass = _T("");
	m_strText = _T("");
	m_strLocation = _T("");
	m_strSize = _T("");
	//}}AFX_DATA_INIT
	m_hTarget = AfxGetApp()->LoadCursor(IDC_LUPA);
	bLupa = FALSE;
	hLast = NULL;
	hWndLastHL = NULL;
}


void CInspector::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInspector)
	DDX_Control(pDX, IDC_LUPACTRL, m_cLupa);
	DDX_Text(pDX, IDC_I_TITLE, m_strTitle);
	DDX_Text(pDX, IDC_I_CLASS, m_strClass);
	DDX_Text(pDX, IDC_I_TEXT, m_strText);
	DDX_Text(pDX, IDC_I_LOCATION, m_strLocation);
	DDX_Text(pDX, IDC_I_SIZE, m_strSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInspector, CDialog)
	//{{AFX_MSG_MAP(CInspector)
	ON_BN_CLICKED(IDC_LUPACTRL, OnLupactrl)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInspector message handlers

void CInspector::OnLupactrl() 
{
	bLupa = TRUE;
	m_hNormal = SetCursor(m_hTarget); 
	SetCapture();
	m_cLupa.SetIcon(NULL);
}

void CInspector::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (bLupa) {

		ReleaseCapture();
		SetCursor(m_hNormal);
		m_cLupa.SetIcon(m_hTarget);

		bLupa = FALSE;
		hLast = NULL;

		if (bHighlight)
			RemoveHighlight(hWndLastHL);
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CInspector::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (bLupa) {

		ClientToScreen(&point);
		HWND hWnd = ::WindowFromPoint(point);

		if (hWnd != hWndLastHL) {
			if (bHighlight)
				RemoveHighlight(hWndLastHL);
			hWndLastHL = hWnd;
		}

		CWnd * p = CWnd::FromHandle(hWnd);

		if (p) {


			// Title
			p->GetWindowText(m_strTitle);

			// Class
			::GetClassName(hWnd, m_strClass.GetBufferSetLength(256), 256);
			m_strClass.ReleaseBuffer();

			if ((!m_strClass.CompareNoCase(L"EDIT") || 
				!m_strClass.CompareNoCase(L"TEDIT") || 
				!m_strClass.CompareNoCase(L"ThunderTextBox") ||
				!m_strClass.CompareNoCase(L"ThunderRT6TextBox") ||
				!m_strClass.CompareNoCase(L"Iris.Password")) &&			
				p->GetStyle() & ES_PASSWORD) {

				// Password
				DWORD dwPID;
				if (GetWindowThreadProcessId(hWnd, &dwPID)) {

					if (dwPID != GetCurrentProcessId()) {
						CProcess * proc = new CProcess(dwPID, NULL);

						g_iAction = 0;
						g_hWndCtrl = hWnd;
						proc->InjectDLL(strSpyAgentDLL);
						proc->RemoveDLL();
						g_hWndCtrl = 0;

						m_strText = g_strMsg;
						delete proc;
					}
				}

			}
			else {
				::SendMessage(hWnd, WM_GETTEXT, 256, (LPARAM)m_strText.GetBufferSetLength(256));
				m_strText.ReleaseBuffer();
			}

			// Location & Size
			CRect rect;
			p->GetWindowRect(&rect);

			m_strLocation.Format(L"(%u, %u)", rect.left, rect.top);
			m_strSize.Format(L"(%u, %u)", rect.right - rect.left, rect.bottom - rect.top);


			// Highlight
			if (bHighlight)
				HighlightFoundWindow(hWnd);

			UpdateData(FALSE);
		}
	}
	
	CDialog::OnMouseMove(nFlags, point);
}


BOOL CInspector::OnInitDialog() 
{
	CDialog::OnInitDialog();

  // Statistics
  CDaphneApp::hitStatisticalRecord(L"ControlInspector");

	// Language
	SetWindowText(_(" Inspector"));
	SetDlgItemText(IDC_STT_DRAG, _("Drag the magnifying glass over windows and controls to inspect its properties."));
	SetDlgItemText(IDC_STT_TITLE, _("Title:"));
	SetDlgItemText(IDC_STT_CLASS, _("Class:"));
	SetDlgItemText(IDC_STT_TEXT, _("Text:"));
	SetDlgItemText(IDC_STT_LOC, _("Location:"));
	SetDlgItemText(IDC_STT_SZ, _("Size:"));

	bHighlight = TRUE;

	m_cLupa.SetIcon(m_hTarget);

	int x = AfxGetApp()->GetProfileInt(L"CFG", L"InspX", -1);
	int y = AfxGetApp()->GetProfileInt(L"CFG", L"InspY", -1);

	UINT nFlags = SWP_NOMOVE | SWP_NOSIZE;

	if ((x>=0) && (y>=0))
		nFlags &= ~SWP_NOMOVE;

	SetWindowPos(&CWnd::wndTopMost, x, y, 0, 0, nFlags);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInspector::OnCancel() 
{
	WINDOWPLACEMENT wndinfo;
	GetWindowPlacement(&wndinfo);
	AfxGetApp()->WriteProfileInt(L"CFG", L"InspX", wndinfo.rcNormalPosition.left);
	AfxGetApp()->WriteProfileInt(L"CFG", L"InspY", wndinfo.rcNormalPosition.top);
	
	CDialog::OnCancel();
}
