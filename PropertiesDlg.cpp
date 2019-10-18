/***********************************************************************************
	
	Daphne -- GPL task manager replacement for Windows
	Copyright (C) 2010  Leandro H. Fernández

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

// PropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "DaphneDlg.h"
#include "PropertiesDlg.h"
#include "Thread.h"
#include "SendMsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

using namespace DRKSpy;

/////////////////////////////////////////////////////////////////////////////
// CPropertiesDlg dialog

CPropertiesDlg::CPropertiesDlg(CProcess * process, CWnd* pParent /*=NULL*/)
	: CDialog(CPropertiesDlg::IDD, pParent)
	, m_strParent(_T(""))
  , minWS(0)
  , maxWS(0)
  , hPreselectWnd(0)
{
	//{{AFX_DATA_INIT(CPropertiesDlg)
	m_strName = _T("");
  m_strOwner = _T("");
	m_strPath = _T("");
	m_strPriority = _T("");
	m_strClass = _T("");
	m_strCreated = _T("");
	m_strKernel = _T("");
	m_strUser = _T("");
	m_strInfo = _T("");
	//}}AFX_DATA_INIT
	m_pProcess = process;
	courier = NULL;
}


void CPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPropertiesDlg)
  DDX_Control(pDX, IDC_P_THREADLIST, m_cThreadList);
  DDX_Control(pDX, IDC_T_WNDS, m_cWnds);
  DDX_Control(pDX, IDC_P_THREADS, m_cInfo);
  DDX_Text(pDX, IDC_P_NAME, m_strName);
  DDX_Text(pDX, IDC_P_OWNER, m_strOwner);
  DDX_Text(pDX, IDC_P_PATH, m_strPath);
  DDX_Text(pDX, IDC_P_PRIORITY, m_strPriority);
  DDX_Text(pDX, IDC_P_CLASS, m_strClass);
  DDX_Text(pDX, IDC_P_CREATED, m_strCreated);
  DDX_Text(pDX, IDC_P_KERNELT, m_strKernel);
  DDX_Text(pDX, IDC_P_USERT, m_strUser);
  DDX_Text(pDX, IDC_P_THREADS, m_strInfo);
  //}}AFX_DATA_MAP
  DDX_Text(pDX, IDC_P_PARENT, m_strParent);
  DDX_Text(pDX, IDC_P_MINWS, minWS);
  DDX_Text(pDX, IDC_P_MAXWS, maxWS);
}


BEGIN_MESSAGE_MAP(CPropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CPropertiesDlg)
	ON_NOTIFY(NM_RCLICK, IDC_T_WNDS, OnRclickTWnds)
	ON_BN_CLICKED(IDC_GOOGLE, OnGoogle)
	ON_NOTIFY(NM_DBLCLK, IDC_T_WNDS, OnDblclkTWnds)
	ON_BN_CLICKED(IDC_P_INFOM, OnPInfom)
	ON_BN_CLICKED(IDC_P_INFOT, OnPInfot)
	ON_BN_CLICKED(IDC_P_INFOE, OnPInfoe)
	ON_NOTIFY(NM_RCLICK, IDC_P_THREADLIST, OnRclickPThreadlist)
	//}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_P_INFOIO, &CPropertiesDlg::OnBnClickedPInfoio)
  ON_BN_CLICKED(IDOK, &CPropertiesDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertiesDlg message handlers

BOOL CPropertiesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

  // Statistics
  CDaphneApp::hitStatisticalRecord(L"ProcessProperties");

  // Language
	SetWindowText(_(" Properties"));
	SetDlgItemText(IDCANCEL, _("Close"));
	SetDlgItemText(IDC_GOOGLE, _("...DRK search"));
	SetDlgItemText(IDC_P_INFOE, _("Environment"));
	SetDlgItemText(IDC_P_INFOM, _("Modules"));
	SetDlgItemText(IDC_P_INFOT, _("Threads"));
	SetDlgItemText(IDC_STG_MI, _("More information"));
	SetDlgItemText(IDC_STG_WNDS, _("Windows"));
	SetDlgItemText(IDC_STT_1, _("Process:"));
	SetDlgItemText(IDC_STT_2, _("CmdLine:"));
	SetDlgItemText(IDC_STT_3, _("Priority:"));
	SetDlgItemText(IDC_STT_4, _("Class:"));
	SetDlgItemText(IDC_STT_5, _("Created:"));
	SetDlgItemText(IDC_STT_6, _("Kernel time:"));
	SetDlgItemText(IDC_STT_7, _("User time:"));
	SetDlgItemText(IDC_STT_PARENT, _("Parent:"));
	SetDlgItemText(IDC_STT_OWNER, _("Owner:"));
	SetDlgItemText(IDC_STT_MMWS, _("Min/max working set size:"));

  if (CDaphneApp::rtl) {
    ModifyStyleEx(0, WS_EX_LAYOUTRTL);
  }

	courier = new CFont();
	if (courier) {
		courier->CreatePointFont(90, L"Courier New");

		m_cInfo.SetFont(courier);
		m_cWnds.SetFont(courier);
		m_cThreadList.SetFont(courier);
	}

	// ThreadListCtrl
	LVCOLUMN setCol;

	// Language
	m_cThreadList.InsertColumn(0, _("ThreadID"));
	m_cThreadList.InsertColumn(1, _("References"));
	m_cThreadList.InsertColumn(2, _("Priority"));
	m_cThreadList.InsertColumn(3, _("CPU Time"));

	setCol.fmt = LVCFMT_RIGHT;
	setCol.cx = 75;
	setCol.mask = LVCF_FMT | LVCF_WIDTH;
	m_cThreadList.SetColumn(0, &setCol);

	setCol.fmt = LVCFMT_RIGHT;
	setCol.cx = 50;
	setCol.mask = LVCF_FMT | LVCF_WIDTH;
	m_cThreadList.SetColumn(1, &setCol);

	m_cThreadList.SetColumnWidth(2, 105);
	m_cThreadList.SetColumnWidth(3, 110);

	m_cThreadList.SetExtendedStyle(LVS_EX_FULLROWSELECT);


	// Refresh process window information
	m_pProcess->ReadWindowInformation();

	// Get info
	m_strName = m_pProcess->strName;
	m_strPath = m_pProcess->strFullPathEx;
  m_strOwner = m_pProcess->strUser+"@"+m_pProcess->strDomain;
	m_strParent.Format(L"%s [%u]", m_pProcess->strParentName, m_pProcess->dwParentPID);
	switch(m_pProcess->dwPriority) {
	case HIGH_PRIORITY_CLASS:
		m_strPriority = _("High");
		break;
	case NORMAL_PRIORITY_CLASS:
		m_strPriority = _("Normal");
		break;
	case IDLE_PRIORITY_CLASS:
		m_strPriority = _("Low");
		break;
	case REALTIME_PRIORITY_CLASS:
		m_strPriority = _("Real time");
		break;
	default:
		m_strPriority = _("Not available");
		break;
	}
	if (m_pProcess->bService)
		m_strClass = _("Service [");
	else
		m_strClass = _("Process");

	switch(m_pProcess->dwServiceState) {
		case SERVICE_PAUSED:
			m_strClass += _("paused]");
			break;
		case SERVICE_RUNNING:
			m_strClass += _("running]");
			break;
		case SERVICE_STOPPED:
			m_strClass += _("stoped]");
			break;
		case SERVICE_CONTINUE_PENDING:
			m_strClass += _("continue pending]");
			break;
		case SERVICE_START_PENDING:
			m_strClass += _("start pending]");
			break;
		case SERVICE_STOP_PENDING:
			m_strClass += _("stop pending]");
			break;
		case SERVICE_PAUSE_PENDING:
			m_strClass += _("pause pending]");
			break;
		}


	SYSTEMTIME SystemTime;

	FileTimeToSystemTime(&m_pProcess->tCreationTime, &SystemTime);
	m_strCreated.Format(L"%02u/%02u/%04u %02u:%02u:%02u.%03u", SystemTime.wMonth, SystemTime.wDay, SystemTime.wYear, SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond, SystemTime.wMilliseconds);
	FileTimeToSystemTime(&m_pProcess->tKernelTime, &SystemTime);
	m_strKernel.Format(L"%02u:%02u:%02u.%03u", SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond, SystemTime.wMilliseconds);
	FileTimeToSystemTime(&m_pProcess->tUserTime, &SystemTime);
	m_strUser.Format(L"%02u:%02u:%02u.%03u", SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond, SystemTime.wMilliseconds);

  m_pProcess->getWorkingSetSize(&minWS, &maxWS);
	
	// Windows
	
	images.Create(16, 16, ILC_COLOR8|ILC_MASK, 5, 2);
	images.Add(AfxGetApp()->LoadIcon(IDI_NULL));
	int imgidx;
	HICON icon;
	m_cWnds.SetImageList(&images, TVSIL_NORMAL);

	TreeItemParents parents;
	ListHWND lost;
	CString title, str, classname;
	ListHWND::iterator j;
	HTREEITEM item;
	for(j = m_pProcess->windows.begin(); j != m_pProcess->windows.end(); j++) {

		title = L"[null]";
		if (::IsWindow((HWND)(*j))) {
			CWnd * pWindow = CWnd::FromHandle((HWND)(*j));
			pWindow->GetWindowText(title);
			GetClassName((HWND)(*j), classname.GetBufferSetLength(256), 256);
			classname.ReleaseBuffer();
			icon = getIconFromWindow((HWND)(*j));
			if (icon)
				imgidx = images.Add(icon);
			else
				imgidx = 0;
		}
		str.Format(L"[%08Xh] %s (%s)", (HWND)(*j), title, classname);
		item = m_cWnds.InsertItem(str, TVI_ROOT, TVI_LAST);
		m_cWnds.SetItemData(item, (DWORD)(*j));
		m_cWnds.SetItemImage(item, imgidx, imgidx);

		parents[(HWND)(*j)] = item;
	}

	HWND parent;
	size_t lastround = 0;
	do {

		lastround = m_pProcess->children.size();

		for(j = m_pProcess->children.begin(); j != m_pProcess->children.end(); j++) {

			title = L"[null]";
			if (::IsWindow((HWND)(*j))) {
				CWnd * pWindow = CWnd::FromHandle((HWND)(*j));
				pWindow->GetWindowText(title);
				if (pWindow->GetParent())
					parent = pWindow->GetParent()->m_hWnd;
				GetClassName((HWND)(*j), classname.GetBufferSetLength(256), 256);
				classname.ReleaseBuffer();
				icon = getIconFromWindow((HWND)(*j));
				if (icon)
					imgidx = images.Add(icon);
				else
					imgidx = 0;
			}
			if (parents[parent]) {
				str.Format(L"[%08Xh] %s (%s)", (HWND)(*j), title, classname);
				item = m_cWnds.InsertItem(str, (HTREEITEM)parents[parent], (HTREEITEM)parents[parent]);
				m_cWnds.SetItemData(item, (DWORD)(*j));
				m_cWnds.SetItemImage(item, imgidx, imgidx);
				parents[(HWND)(*j)] = item;
			}
			else {
				lost.push_back((HWND)(*j));
			}
		}

		m_pProcess->children.clear();
		m_pProcess->children = lost;
		lost.clear();
	}
	while((m_pProcess->children.size()!=lastround) && (m_pProcess->children.size()));

	for(j = m_pProcess->children.begin(); j != m_pProcess->children.end(); j++) {

		title = L"[null]";
		if (::IsWindow((HWND)(*j))) {
			CWnd * pWindow = CWnd::FromHandle((HWND)(*j));
			pWindow->GetWindowText(title);
			if (pWindow->GetParent())
				parent = pWindow->GetParent()->m_hWnd;
		}
		str.Format(L"[%08Xh] %s (Orphan: %08Xh)", (HWND)(*j), title, parent);
		item = m_cWnds.InsertItem(str, TVI_ROOT, TVI_LAST);
		m_cWnds.SetItemData(item, (DWORD)(*j));
	}

  // Por último
  if (hPreselectWnd) {
    HTREEITEM hParent = m_cWnds.GetParentItem(parents[hPreselectWnd]);
    if (hParent != NULL)
      m_cWnds.Expand(hParent, TVE_EXPAND);
    m_cWnds.EnsureVisible(parents[hPreselectWnd]);
    m_cWnds.SelectItem(parents[hPreselectWnd]);
    SetFocus();
    m_cWnds.SetActiveWindow();
    m_cWnds.SetFocus();
  }



	RefreshThreads();

  CheckRadioButton(IDC_P_INFOT, IDC_P_INFOIO, IDC_P_INFOT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertiesDlg::OnCancel() 
{
	if (courier)
		delete courier;

	CDialog::OnCancel();
}

void CPropertiesDlg::OnRclickTWnds(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMenu menu;
	POINT point;

	menu.CreatePopupMenu();
	menu.InsertMenu(0, 0, IDM_P_FOCUS, _("Set focus"));
	menu.InsertMenu(-1, MF_BYPOSITION, IDM_P_HIDE, _("Hide"));
	menu.InsertMenu(-1, MF_BYPOSITION, IDM_P_SHOW, _("Show"));
	menu.InsertMenu(-1, MF_BYPOSITION, IDM_P_THIDE, _("Hide application"));
	menu.InsertMenu(-1, MF_BYPOSITION, IDM_P_TSHOW, _("Show application"));
	menu.InsertMenu(-1, MF_BYPOSITION, IDM_P_ONTOP_ON, _("Set always on top"));
	menu.InsertMenu(-1, MF_BYPOSITION, IDM_P_ONTOP_OFF, _("Unset always on top"));
	menu.InsertMenu(-1, MF_BYPOSITION, IDM_P_MKRESIZE, _("Make resizable"));
	menu.InsertMenu(-1, MF_BYPOSITION, IDM_P_NOCAPTION, _("Quit caption"));
	menu.InsertMenu(-1, MF_BYPOSITION, IDM_P_CAPTION, _("Add caption"));
	menu.InsertMenu(-1, MF_BYPOSITION, MF_SEPARATOR);
	menu.InsertMenu(-1, MF_BYPOSITION, IDM_P_MINIMIZE, _("Minimize"));
	menu.InsertMenu(-1, MF_BYPOSITION, IDM_P_RESTORE, _("Restore"));
	menu.InsertMenu(-1, MF_BYPOSITION, IDM_P_MAXIMIZE, _("Maximize"));
	menu.InsertMenu(-1, MF_BYPOSITION, IDM_P_CLOSE, _("Close"));
	menu.InsertMenu(-1, MF_BYPOSITION, MF_SEPARATOR);
	menu.InsertMenu(-1, MF_BYPOSITION, IDM_P_MESSAGE, _("Send message"));

	if (m_pProcess->dwPID == GetCurrentProcessId()) {
		// mejor desactivamos ciertas opciones sobre Daphne
		menu.EnableMenuItem(IDM_P_THIDE, MF_DISABLED|MF_GRAYED);
		menu.EnableMenuItem(IDM_P_NOCAPTION, MF_DISABLED|MF_GRAYED);
		menu.EnableMenuItem(IDM_P_CAPTION, MF_DISABLED|MF_GRAYED);
		menu.EnableMenuItem(IDM_P_MESSAGE, MF_DISABLED|MF_GRAYED);
	}

	menu.SetDefaultItem(IDM_P_FOCUS);

	GetCursorPos(&point);

	menu.TrackPopupMenu(0, point.x, point.y, this);
	
	*pResult = 0;
}

BOOL CPropertiesDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	CSendMsgDlg message_dlg;
	CDaphneDlg * parent = (CDaphneDlg*)GetParent();
	BOOL refresh = FALSE;
	LONG style = 0;

	switch(LOWORD(wParam)) {
	case IDM_P_FOCUS:
		::SetForegroundWindow(GetSelectHandle());
		break;
	case IDM_P_MINIMIZE:
		::ShowWindow(GetSelectHandle(), SW_MINIMIZE); 
		break;
	case IDM_P_RESTORE:
		::ShowWindow(GetSelectHandle(), SW_RESTORE); 
		break;
	case IDM_P_MAXIMIZE:
		::ShowWindow(GetSelectHandle(), SW_MAXIMIZE); 
		break;
	case IDM_P_CLOSE:
		::PostMessage(GetSelectHandle(), WM_CLOSE, 0, 0); 
		break;
	case IDM_P_HIDE:
		::ShowWindow(GetSelectHandle(), SW_HIDE);
		break;
	case IDM_P_SHOW:
		::ShowWindow(GetSelectHandle(), SW_SHOW);
		break;
	case IDM_P_THIDE:
		parent->hideWindow(GetSelectHandle());
		break;
	case IDM_P_TSHOW:
		parent->showWindow(GetSelectHandle());
		break;
	case IDM_P_MESSAGE:
		message_dlg.hTarget = GetSelectHandle();
		message_dlg.DoModal();
		break;
	case IDM_P_ONTOP_ON:
		::SetWindowPos(GetSelectHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		break;
	case IDM_P_ONTOP_OFF:
		::SetWindowPos(GetSelectHandle(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		break;

	case IDM_P_MKRESIZE:
		style = ::GetWindowLong(GetSelectHandle(), GWL_STYLE);
		style |= WS_SIZEBOX;
		::SetWindowLong(GetSelectHandle(), GWL_STYLE, style);
		break;
	case IDM_P_NOCAPTION:
		style = ::GetWindowLong(GetSelectHandle(), GWL_STYLE);
		style &= ~WS_CAPTION;
		style |= WS_BORDER;
		::SetWindowLong(GetSelectHandle(), GWL_STYLE, style);
		::SetWindowPos(GetSelectHandle(), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW|SWP_FRAMECHANGED);
		break;
	case IDM_P_CAPTION:
		style = ::GetWindowLong(GetSelectHandle(), GWL_STYLE);
		style |= WS_CAPTION;
		::SetWindowLong(GetSelectHandle(), GWL_STYLE, style);
		::SetWindowPos(GetSelectHandle(), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW|SWP_FRAMECHANGED);
		break;

	case IDM_P_TSUSPEND:
		CThread::SuspendThread(GetSelectedThreadID());
		refresh = TRUE;
		break;
	case IDM_P_TRESUME:
		CThread::ResumeThread(GetSelectedThreadID());
		refresh = TRUE;
		break;
	case IDM_P_TTERMINATE:
		CThread::TerminateThread(GetSelectedThreadID());
		refresh = TRUE;
		break;
	case IDM_SPT_TC:
		CThread::SetPriority(GetSelectedThreadID(), THREAD_PRIORITY_TIME_CRITICAL);
		refresh = TRUE;
		break;
	case IDM_SPT_HI:
		CThread::SetPriority(GetSelectedThreadID(), THREAD_PRIORITY_HIGHEST);
		refresh = TRUE;
		break;
	case IDM_SPT_AN:
		CThread::SetPriority(GetSelectedThreadID(), THREAD_PRIORITY_ABOVE_NORMAL);
		refresh = TRUE;
		break;
	case IDM_SPT_NO:
		CThread::SetPriority(GetSelectedThreadID(), THREAD_PRIORITY_NORMAL);
		refresh = TRUE;
		break;
	case IDM_SPT_BN:
		CThread::SetPriority(GetSelectedThreadID(), THREAD_PRIORITY_BELOW_NORMAL);
		refresh = TRUE;
		break;
	case IDM_SPT_LO:
		CThread::SetPriority(GetSelectedThreadID(), THREAD_PRIORITY_LOWEST);
		refresh = TRUE;
		break;
	case IDM_SPT_ID:
		CThread::SetPriority(GetSelectedThreadID(), THREAD_PRIORITY_IDLE);
		refresh = TRUE;
		break;
	}

	if (refresh) {
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		Sleep(1000);
		RefreshThreads();
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}

	return CDialog::OnCommand(wParam, lParam);
}

HWND CPropertiesDlg::GetSelectHandle()
{
	HTREEITEM item = m_cWnds.GetSelectedItem();

	if (item)
		return (HWND)m_cWnds.GetItemData(item);
	else
		return NULL;
}

void CPropertiesDlg::OnGoogle() 
{
	m_pProcess->toDRKDB();
}

void CPropertiesDlg::OnDblclkTWnds(NMHDR* pNMHDR, LRESULT* pResult) 
{
	PostMessage(WM_COMMAND, MAKELPARAM(IDM_P_FOCUS ,0), 0);
	
	*pResult = 0;
}

void CPropertiesDlg::OnPInfot() 
{
	m_strInfo.Empty();
	m_cInfo.ShowWindow(SW_HIDE);
	m_cThreadList.ShowWindow(SW_SHOW);
}

void CPropertiesDlg::OnPInfom() 
{
  UpdateData(TRUE);

	m_strInfo.Empty();
	m_cInfo.ShowWindow(SW_SHOW);
	m_cThreadList.ShowWindow(SW_HIDE);

	m_pProcess->ReadAllModules();
	m_strInfo = m_pProcess->strModules;

	UpdateData(FALSE);
}

void CPropertiesDlg::OnPInfoe() 
{
  UpdateData(TRUE);

	m_strInfo.Empty();
	m_cInfo.ShowWindow(SW_SHOW);
	m_cThreadList.ShowWindow(SW_HIDE);

	if (!m_pProcess->ReadEnvironmentBlock(m_strInfo)) {
		g_iAction = 1;
    g_bWorked = false;
		BOOL r = m_pProcess->InjectDLL(strSpyAgentDLL);
		m_pProcess->RemoveDLL();

		if (g_bWorked) {
			m_strInfo = g_strMsg;
			m_strInfo.Replace(L"\n", L"\r\n");
		}
		g_iAction = 0;

		if (m_strInfo.IsEmpty()) {
			m_strInfo = _("Daphne was unable to get environment information.\nThis feature is known to fail\non some Windows versions.");
			m_strInfo.Replace(L"\n", L"\r\n");
		}
	}
	m_strInfo.Replace(L"=::=::\\\r\n", L"");

	UpdateData(FALSE);
}

void CPropertiesDlg::OnBnClickedPInfoio()
{
  UpdateData(TRUE);

	m_cInfo.ShowWindow(SW_SHOW);
	m_cThreadList.ShowWindow(SW_HIDE);

	m_strInfo.Format(_("Read operation count %I64u\n\
Write operation count %I64u\n\
Other operation count %I64u\n\
Read transfer %I64u KBytes\n\
Write operation %I64u KBytes\n\
Other operation %I64u KBytes\n\
")
    , m_pProcess->ReadOperationCount
    , m_pProcess->WriteOperationCount
    , m_pProcess->OtherOperationCount
    , m_pProcess->ReadTransferCount/1024
    , m_pProcess->WriteTransferCount/1024
    , m_pProcess->OtherTransferCount/1024
    );
	m_strInfo.Replace(L"\n", L"\r\n");

	UpdateData(FALSE);
}

void CPropertiesDlg::RefreshThreads()
{
	CString str;
	CThread * pThread;
	int index;

	m_cThreadList.DeleteAllItems();

	ThreadList::iterator i;
	for(i = m_pProcess->threads.begin(); i != m_pProcess->threads.end(); i++) {
		pThread = (CThread*)&(*i);
		str.Format(L"%08Xh", pThread->dwID);
		index = m_cThreadList.InsertItem(0, str);
		str.Format(L"%u", pThread->cntUsage);
		m_cThreadList.SetItemText(index, 1, str);
		
		m_cThreadList.SetItemText(index, 2, CThread::GetPriorityString(CThread::GetPriority(pThread->dwID)));
		
		str.Format(L"%10.6f", (float)CThread::GetThreadCPUTime(pThread->dwID)/10000000);
		m_cThreadList.SetItemText(index, 3, str);

		m_cThreadList.SetItemData(index, pThread->dwID);
	}

	UpdateData(FALSE);
}

void CPropertiesDlg::OnRclickPThreadlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMenu menu;
	CMenu prio_menu;
	POINT point;

	menu.CreatePopupMenu();
	menu.InsertMenu(0, 0, IDM_P_TSUSPEND, _("Suspend"));
	menu.InsertMenu(-1, MF_BYPOSITION, IDM_P_TRESUME, _("Resume"));
	menu.InsertMenu(-1, MF_BYPOSITION, IDM_P_TTERMINATE, _("Terminate"));

	prio_menu.CreatePopupMenu();
	prio_menu.InsertMenu(-1, MF_BYPOSITION, IDM_SPT_TC, _("Time critical"));
	prio_menu.InsertMenu(-1, MF_BYPOSITION, IDM_SPT_HI, _("Highest"));
	prio_menu.InsertMenu(-1, MF_BYPOSITION, IDM_SPT_AN, _("Above normal"));
	prio_menu.InsertMenu(-1, MF_BYPOSITION, IDM_SPT_NO, _("Normal"));
	prio_menu.InsertMenu(-1, MF_BYPOSITION, IDM_SPT_BN, _("Below normal"));
	prio_menu.InsertMenu(-1, MF_BYPOSITION, IDM_SPT_LO, _("Lowest"));
	prio_menu.InsertMenu(-1, MF_BYPOSITION, IDM_SPT_ID, _("IDLE"));
	switch(CThread::GetPriority(GetSelectedThreadID())) {
	case THREAD_PRIORITY_ABOVE_NORMAL:
		prio_menu.CheckMenuItem(IDM_SPT_AN, MF_BYCOMMAND|MF_CHECKED);
		break;
	case THREAD_PRIORITY_BELOW_NORMAL:
		prio_menu.CheckMenuItem(IDM_SPT_BN, MF_BYCOMMAND|MF_CHECKED);
		break;
	case THREAD_PRIORITY_HIGHEST:
		prio_menu.CheckMenuItem(IDM_SPT_HI, MF_BYCOMMAND|MF_CHECKED);
		break;
	case THREAD_PRIORITY_IDLE:
		prio_menu.CheckMenuItem(IDM_SPT_ID, MF_BYCOMMAND|MF_CHECKED);
		break;
	case THREAD_PRIORITY_LOWEST:
		prio_menu.CheckMenuItem(IDM_SPT_LO, MF_BYCOMMAND|MF_CHECKED);
		break;
	case THREAD_PRIORITY_NORMAL:
		prio_menu.CheckMenuItem(IDM_SPT_NO, MF_BYCOMMAND|MF_CHECKED);
		break;
	case THREAD_PRIORITY_TIME_CRITICAL:
		prio_menu.CheckMenuItem(IDM_SPT_TC, MF_BYCOMMAND|MF_CHECKED);
		break;
	}

	menu.InsertMenu(-1, MF_POPUP, (UINT)prio_menu.m_hMenu, _("Set priority"));

	menu.SetDefaultItem(IDM_P_FOCUS);

	GetCursorPos(&point);

	menu.TrackPopupMenu(0, point.x, point.y, this);
	
	*pResult = 0;
}

DWORD CPropertiesDlg::GetSelectedThreadID()
{
	POSITION pos = m_cThreadList.GetFirstSelectedItemPosition();

	return (DWORD)m_cThreadList.GetItemData((int)(pos-1));
}

void CPropertiesDlg::WinHelp(DWORD dwData, UINT nCmd)
{
  AfxGetApp()->WinHelp(dwData, nCmd);
}


void CPropertiesDlg::OnBnClickedOk()
{
  SYSTEM_INFO i;

  UpdateData(TRUE);
  GetSystemInfo(&i);

  if (minWS == maxWS == 0) {
    // Si son cero ambos, fuerzo remoción de páginas
    m_pProcess->emptyWorkingSet();
  }
  else {
    ULONG_PTR min = 20 * i.dwPageSize;
    ULONG_PTR max = 13 * i.dwPageSize;
    minWS = (minWS < min)?min:minWS;
    maxWS = (maxWS < max)?max:maxWS;
    m_pProcess->setWorkingSetSize(minWS, maxWS);
  }


	if (courier)
		delete courier;

  CDialog::OnOK();
}
