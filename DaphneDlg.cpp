/***********************************************************************************
	
	Daphne -- GPL task manager replacement for Windows
	Copyright (C) 2012  Leandro H. Fernández

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

// DaphneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "DaphneDlg.h"
#include "PropertiesDlg.h"
#include "WndsPopUp.h"
#include "RunProcDlg.h"
#include "AffinityDlg.h"
#include "AuthUserDlg.h"
#include "MBWebAgreeDlg.h"
#include "NewTrapDlg.h"
#include "KillAllDlg.h"
#include "Inspector.h"
#include "PPKill.h"
#include "PPLook.h"
#include "PPStartup.h"
#include "PPAdv.h"
#include "PPTraps.h"
#include "PPHighlight.h"
#include "PPMultidesktop.h"
#include "PPLogging.h"
#include "Trap.h"
#include "ProcessTreeDlg.h"
#include "lssoftDlg.h"
#include "CloseAllWndByTitle.h"
#include "SplashMDInit.h"
#include "KillAtDlg.h"
#include "ScheduleShutdownDlg.h"
#include "ScheduledTasksDlg.h"
#include "CPUUsageDlg.h"
#include "UnicodeFile.h"
#include "WindowSize.h"
#include "MessageAtDlg.h"

#include <strsafe.h>
#include <math.h>
#include <pdhmsg.h>
#include <Lmcons.h>
#include <Wincrypt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace DRKSpy;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	CString m_strPass;

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_strTitle;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnWeb();
	afx_msg void OnLicense();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedSupport();
public:
  afx_msg void OnBnClickedContribute();
  CString getCreditsFromWeb(void);
private:
  CFont m_Font;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strTitle = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_AB_TITLE, m_strTitle);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_WEB, OnWeb)
	ON_BN_CLICKED(IDC_LICENSE, OnLicense)
	//}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_SUPPORT, &CAboutDlg::OnBnClickedSupport)
  ON_BN_CLICKED(IDC_CONTRIBUTE, &CAboutDlg::OnBnClickedContribute)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDaphneDlg dialog

CDaphneDlg::CDaphneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDaphneDlg::IDD, pParent)
	, m_strInfoCPUUsage(_T(""))
	, infoCPUUsage(0)
	, m_strInfoMEMUsage(_T(""))
	, m_strInfoTimeUp(_T(""))
	, m_strInfoMEMFree(_T(""))
  , m_bHideZombies(FALSE)
  , days(0)
  , hours(0)
  , minutes(0)
  , seconds(0)
  , showing(FALSE)
  , m_strMessage(_T(""))
  , processes(this)
{
  //{{AFX_DATA_INIT(CDaphneDlg)
  m_bExtPath = FALSE;
  m_bOnTop = FALSE;
  m_iAction = -1;
  //}}AFX_DATA_INIT
  // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  m_hTarget = AfxGetApp()->LoadCursor(IDC_KILLCURSOR);
  icon[0] = AfxGetApp()->LoadIcon(IDR_MAINFRAME0);
  icon[1]= AfxGetApp()->LoadIcon(IDR_MAINFRAME1);
  icon[2]= AfxGetApp()->LoadIcon(IDR_MAINFRAME2);
  icon[3]= AfxGetApp()->LoadIcon(IDR_MAINFRAME3);
  icon[4]= AfxGetApp()->LoadIcon(IDR_MAINFRAME4);
  icon[5]= AfxGetApp()->LoadIcon(IDR_MAINFRAME5);
  icon[6]= AfxGetApp()->LoadIcon(IDR_MAINFRAME6);
  icon[7]= AfxGetApp()->LoadIcon(IDR_MAINFRAME7);
  icon[8]= AfxGetApp()->LoadIcon(IDR_MAINFRAME8);
  icon[9]= AfxGetApp()->LoadIcon(IDR_MAINFRAME9);
  icon[10]= AfxGetApp()->LoadIcon(IDR_MAINFRAME10);

  orderColumn = 0;
  orderMode = 0;
  bKillTarget = FALSE;
  m_bUpdatePath = FALSE;
  trayIconOK = FALSE;
  hLastSelectedWnd = 0;
  threadWebUpdates = NULL;
  threadPopupMessage = NULL;
  threadUpdate = NULL;
  stopWebUpdate = FALSE;
  hRefreshEnd = ::CreateEvent(NULL, FALSE, FALSE, NULL);
  hWebUpdateEnd = ::CreateEvent(NULL, FALSE, FALSE, NULL);
  hTrapsMutex = ::CreateMutex(NULL, FALSE, NULL);
  hHiddenMutex = ::CreateMutex(NULL, FALSE, NULL);

  // Obtenemos información fija //
  GetSystemInfo(&sysinfo);
  meminfo.dwLength = sizeof(MEMORYSTATUSEX);
  GlobalMemoryStatusEx(&meminfo);

  DWORD l = UNLEN + 1;
  ::GetUserName(m_username.GetBuffer(l), &l);
  m_username.ReleaseBuffer();
  l = MAX_COMPUTERNAME_LENGTH + 1;
  ::GetComputerName(m_computer.GetBuffer(l), &l);
  m_computer.ReleaseBuffer();

  m_processor.Format(_("%s"), getProcessorType(sysinfo));

  ////////////////////////////////  

  // Determinamos LANG_ID y obtenemos valores dependientes

  wcscpy_s(uptimeCounterPath, PDH_MAX_COUNTER_PATH,  L"\\\\.\\System\\System Up Time");
  TCHAR computerName[80];
  DWORD compNameSize = 80;
  if (::GetComputerName( computerName, &compNameSize )) {

    DWORD objectNameSize = 80;
    TCHAR uptimeObjectName[80];
    DWORD counterNameSize = 80;
    TCHAR uptimeCounterName[80];
    PDH_COUNTER_PATH_ELEMENTS elems;
    memset( &elems, 0, sizeof(PDH_COUNTER_PATH_ELEMENTS) );

    PDH_STATUS status = PdhLookupPerfNameByIndex( computerName, 2, (LPWSTR)uptimeObjectName, &objectNameSize );

    if (status == ERROR_SUCCESS) {
      status = PdhLookupPerfNameByIndex( computerName, 674, (LPWSTR)uptimeCounterName, &counterNameSize );
    }

    if (status == ERROR_SUCCESS) {
      elems.szCounterName = uptimeCounterName;
      elems.szObjectName = uptimeObjectName;
      elems.szMachineName = computerName;

      DWORD counterPathSize = 80;
      status = PdhMakeCounterPath( &elems, uptimeCounterPath, &counterPathSize, 0 );
    }
  }

  if (!AfxGetApp()->GetProfileInt(L"CFG", L"MainWndStartHidden", 0))
    m_bWindowHidden = (BOOL)AfxGetApp()->GetProfileInt(L"CFG", L"MainHidden", 0);
  else
    m_bWindowHidden = TRUE;

  DesktopMapInit();
  m_strInfoPCount = _T("");
  m_strInfoLogin = _T("");
  m_strInfoCPUCount = _T("");
  m_strInfoProcessor = _T("");
}

void CDaphneDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDaphneDlg)
  DDX_Control(pDX, IDC_SETTINGS, m_cSettings);
  DDX_Control(pDX, IDC_MENUB, m_cMenub);
  DDX_Control(pDX, IDC_AONTOP, m_cOntop);
  DDX_Control(pDX, IDC_FULLPATHEX, m_cFullpath);
  DDX_Control(pDX, IDC_TACTION, m_cAction);
  DDX_Control(pDX, IDC_C_KILL, m_cKill);
  DDX_Control(pDX, IDC_LIST, m_cProcList);
  DDX_Check(pDX, IDC_FULLPATHEX, m_bExtPath);
  DDX_Check(pDX, IDC_AONTOP, m_bOnTop);
  DDX_CBIndex(pDX, IDC_TACTION, m_iAction);
  //}}AFX_DATA_MAP
  DDX_Text(pDX, IDC_INF_CPU, m_strInfoCPUUsage);
  DDX_Text(pDX, IDC_INF_MEM, m_strInfoMEMUsage);
  DDX_Text(pDX, IDC_INF_CPUC, m_strInfoTimeUp);
  DDX_Text(pDX, IDC_INF_FMEM, m_strInfoMEMFree);
  DDX_Text(pDX, IDC_INF_COUNT, m_strInfoPCount);
  DDX_Text(pDX, IDC_INF_LOGIN, m_strInfoLogin);
  DDX_Text(pDX, IDC_INF_CPUCOUNT, m_strInfoCPUCount);
  DDX_Text(pDX, IDC_INF_PROC, m_strInfoProcessor);
}

BEGIN_MESSAGE_MAP(CDaphneDlg, CDialog)
	//{{AFX_MSG_MAP(CDaphneDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK, IDC_LIST, OnRclickList)
	ON_BN_CLICKED(IDC_FULLPATHEX, OnFullpathex)
	ON_BN_CLICKED(IDC_SETTINGS, OnSettings)
	ON_BN_CLICKED(IDC_MENUB, OnMenub)
	ON_BN_CLICKED(IDC_AONTOP, OnAontop)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_C_KILL, OnCKill)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_WINDOWPOSCHANGING()
	ON_CBN_SELCHANGE(IDC_TACTION, OnSelchangeTaction)
	ON_WM_SHOWWINDOW()
	ON_WM_QUERYENDSESSION()
	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
  ON_MESSAGE(WM_DRK_JUST_STARTED, OnJustStarted)
	ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, OnTaskBarCreated)
	ON_WM_ENDSESSION()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
  ON_WM_CREATE()
END_MESSAGE_MAP()

//ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)

/////////////////////////////////////////////////////////////////////////////
// CDaphneDlg message handlers

void CDaphneDlg::PreInitDialog()
{
  ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
  CDialog::PreInitDialog();
}

BOOL CDaphneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

  if (AfxGetApp()->GetProfileInt(L"CFG", L"UseMultidesktop", 0) == 1) {
	  BOOL r = true;
	  r &= RegisterHotKey(GetSafeHwnd(), GlobalAddAtom(L"LHF_DSK_0"), MOD_WIN, VK_F5);
	  int maxdesktops = AfxGetApp()->GetProfileInt(L"CFG", L"MDExtraDesktops", 1);
	  for(int i=1; i<=maxdesktops; i++) {
		  CString id;
		  id.Format(L"LHF_DSK_%u", i);
		  r &= RegisterHotKey(GetSafeHwnd(), GlobalAddAtom(id), MOD_WIN, VK_F5+i);
	  }

	  if (!r) {
      WCHAR tmp[30];
		  MessageBox(_("Unable to register global shortcut. Multidesktop may not work."), __("Error", tmp, sizeof(tmp)), MB_OK|MB_ICONSTOP);
	  }
  }

	// Language
	SetDlgItemText(IDC_FULLPATHEX, _("Full path and arguments"));
	SetDlgItemText(IDC_AONTOP, _("Always on top"));
	SetDlgItemText(IDC_MENUB, _("Daphne >"));
	SetDlgItemText(IDC_SETTINGS, _("Settings..."));

	m_cAction.AddString(_("Find (Process)"));
	m_cAction.AddString(_("Find (Window)"));
	m_cAction.AddString(_("Kill"));
	m_cAction.AddString(_("Hide application"));
	m_cAction.AddString(_("Set on top"));
	m_cAction.AddString(_("Unset on top"));
	m_cAction.AddString(_("Set alpha 100%"));
	m_cAction.AddString(_("Set alpha 75%"));
	m_cAction.AddString(_("Set alpha 50%"));
	m_cAction.AddString(_("Unset alpha"));
	m_cAction.AddString(_("Enable"));
	m_cAction.AddString(_("Disable"));
	m_cAction.AddString(_("Change size"));
	m_cAction.AddString(_("Trap size and position"));

	// Guardo el tamaño actual como tamaño mínimo
	GetWindowRect(&min_size);

	m_cProcList.GetWindowPlacement(&orig_proclist);

	WINDOWPLACEMENT wndpos;

	m_cFullpath.GetWindowPlacement(&wndpos);
	orig_checkb_margin = min_size.bottom - wndpos.rcNormalPosition.top;

	m_cKill.GetWindowPlacement(&wndpos);
	orig_target_margin = min_size.bottom - wndpos.rcNormalPosition.top;

	m_cAction.GetWindowPlacement(&wndpos);
	orig_combo_margin = min_size.bottom - wndpos.rcNormalPosition.top;

	m_cMenub.GetWindowPlacement(&wndpos);
	orig_button_margin = min_size.bottom - wndpos.rcNormalPosition.top;


	// Alpha activado?
	if (AfxGetApp()->GetProfileInt(L"CFG", L"MainWndAlphaOn", 0)) {

		ModifyStyleEx(0, WS_EX_LAYERED);
		::SetLayeredWindowAttributes(m_hWnd, RGB(0,0,0), AfxGetApp()->GetProfileInt(L"CFG", L"MainWndAlpha", 255), LWA_ALPHA);
	}

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu = _("&About Daphne...");
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	SetupTrayIcon();

	// Inicializo el SCM
	if (!scm.Open())
		bSCMOk = TRUE;
	else
		bSCMOk = FALSE;

		
	LVCOLUMN setCol;

	m_cProcList.InsertColumn(0, _("PID"));
	m_cProcList.InsertColumn(1, _("Process"));
	m_cProcList.InsertColumn(2, _("Path"));
	m_cProcList.InsertColumn(3, _("Priority"));
	m_cProcList.InsertColumn(4, _("Class"));
	m_cProcList.InsertColumn(5, _("Memory"));
	m_cProcList.InsertColumn(6, _("PeakMemoryUsage"));
	m_cProcList.InsertColumn(7, _("Swap"));
	m_cProcList.InsertColumn(8, _("PeakSwapUsage"));
	m_cProcList.InsertColumn(9, _("Threads"));
	m_cProcList.InsertColumn(10, _("%CPU"));
	m_cProcList.InsertColumn(11, _("Owner"));
	m_cProcList.InsertColumn(12, _("I/O reads"));
	m_cProcList.InsertColumn(13, _("I/O writes"));

  setCol.fmt = LVCFMT_RIGHT;
	setCol.cx = 50;
	setCol.mask = LVCF_FMT | LVCF_WIDTH;
	m_cProcList.SetColumn(0, &setCol);

  setCol.fmt = LVCFMT_LEFT;
	setCol.cx = 110;
	setCol.mask = LVCF_FMT | LVCF_WIDTH;
	m_cProcList.SetColumn(1, &setCol);
  setCol.fmt = LVCFMT_LEFT;
	setCol.cx = 250;
	setCol.mask = LVCF_FMT | LVCF_WIDTH;
	m_cProcList.SetColumn(2, &setCol);

  setCol.fmt = LVCFMT_LEFT;
	setCol.cx = 60;
	setCol.mask = LVCF_FMT | LVCF_WIDTH;
	m_cProcList.SetColumn(3, &setCol);

	m_cProcList.SetColumnWidth(4, 55);

  setCol.fmt = LVCFMT_RIGHT;
	setCol.cx = 60;
	setCol.mask = LVCF_FMT | LVCF_WIDTH;
	m_cProcList.SetColumn(5, &setCol);
	m_cProcList.SetColumn(6, &setCol);
	m_cProcList.SetColumn(7, &setCol);
	m_cProcList.SetColumn(8, &setCol);

	setCol.fmt = LVCFMT_RIGHT;
	setCol.cx = 40;
	setCol.mask = LVCF_FMT | LVCF_WIDTH;
	m_cProcList.SetColumn(9, &setCol);

	setCol.fmt = LVCFMT_RIGHT;
	setCol.cx = 45;
	setCol.mask = LVCF_FMT | LVCF_WIDTH;
	m_cProcList.SetColumn(10, &setCol);

	setCol.fmt = LVCFMT_LEFT;
	setCol.cx = 75;
	setCol.mask = LVCF_FMT | LVCF_WIDTH;
	m_cProcList.SetColumn(11, &setCol);

	setCol.fmt = LVCFMT_RIGHT;
	setCol.cx = 60;
	setCol.mask = LVCF_FMT | LVCF_WIDTH;
	m_cProcList.SetColumn(12, &setCol);
	setCol.fmt = LVCFMT_RIGHT;
	setCol.cx = 60;
	setCol.mask = LVCF_FMT | LVCF_WIDTH;
	m_cProcList.SetColumn(13, &setCol);

  m_cProcList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

  // Columnas del listado
	UINT size = 0;
	int * array;
	int default_order[PL_COLUMN_COUNT] = {10,0,1,2,11,3,4,5,6,7,8,9,12,13};
	if (AfxGetApp()->GetProfileBinary(L"CFG", L"ColumnOrder", (BYTE**)&array, &size)) {
    // Orden
		if ((size/sizeof(int)) == PL_COLUMN_COUNT)
			m_cProcList.SetColumnOrderArray(size/sizeof(int), array);
		delete array;

    // Ancho
    CString str;
    for(int i=0; i<PL_COLUMN_COUNT; i++) {
      str.Format(L"MainColumn%uWidth", i);
      int width = AfxGetApp()->GetProfileInt(L"CFG", str, -1);
      if (width != -1)
        m_cProcList.SetColumnWidth(i, width);
    }
	}
	else
		m_cProcList.SetColumnOrderArray(PL_COLUMN_COUNT, default_order);

	orderColumn = AfxGetApp()->GetProfileInt(L"CFG", L"MainOrderColumn", 0);
	orderMode = AfxGetApp()->GetProfileInt(L"CFG", L"MainOrderMode", 0);
	m_bExtPath = AfxGetApp()->GetProfileInt(L"CFG", L"MainFullPath", 1);
	m_bOnTop = AfxGetApp()->GetProfileInt(L"CFG", L"MainOnTop", 1);
	m_iAction = AfxGetApp()->GetProfileInt(L"CFG", L"Action", 0);
	m_bAlternateBackground = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneAlternateBackground", 0);
	m_bUpdates = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneCheckUpdates", 1);
	highlights = AfxGetApp()->GetProfileString(L"CFG", L"Highlights", L"");
	highlights_on = !highlights.IsEmpty();
	m_bHLSys = AfxGetApp()->GetProfileInt(L"CFG", L"HighlightSystem", 1);
	m_bHLTop = AfxGetApp()->GetProfileInt(L"CFG", L"HighlightTop", 1);
	m_bHLTopMem = AfxGetApp()->GetProfileInt(L"CFG", L"HighlightTopMem", 1);
	m_bJavaHide = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneJavaVMHide", 0);
  m_bHideZombies = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneHideZombies", 0);
  m_bAnimatedIcon = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneAnimatedIcon", 1);

	g_scVK = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneSCVirtualKey", VK_F12);
	g_scNeedsAlt = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneSCAlt", 1);
	g_scNeedsShift = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneSCShift", 1);

	LoadTargetCursor();

	UpdateData(FALSE);

	loadTraps();

  loadSchedule();

  // LOG
  bProcessLogging = AfxGetApp()->GetProfileInt(L"LOG", L"LogProcess", 0);
  strProcessLoggingFile = AfxGetApp()->GetProfileString(L"LOG", L"LogProcessFile", L"");

	// Inicializo la lista con información del sistema
	if (m_bJavaHide) {
		WCHAR strJavaHome[_MAX_PATH];
		if (GetEnvironmentVariableW(L"JAVA_HOME", strJavaHome, sizeof(strJavaHome))) {
			processes.setJavaHome(strJavaHome);
		}
	}
	processes.setSCM((bSCMOk)?&scm:NULL);
  processes.setSysInfo(&sysinfo);
  
  if (bProcessLogging) {
    CStdioFile f;
    CFileException fileException;
    CString line;
    if (f.Open( strProcessLoggingFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite, &fileException ) )
    {
      f.SeekToEnd();
      line.Format(_("* %s ============= Daphne starting... =============\n"), CTime::GetCurrentTime().Format(L"%Y.%m.%d %H:%M:%S"));
      f.WriteString(line);
      f.Close();
    }
  }

  // Carga inicial
	processes.updateList();

	// Actualizo el control
	RefreshList();

	scm.ReadServicesMap();
	SetTimer(IDT_SERVICES, 600000, NULL);

	if (!hasAutomation()) {
		if (!m_bWindowHidden)
			SetEvent(hRefreshEvent);
	}
	else
		SetEvent(hRefreshEvent);

	createUpdateThread();

	ResetWebUpdates();

	// Pongo el Handle de esta ventana en la memoria compartida
	g_hWnd = m_hWnd;
	g_hWndHooks = m_hWnd;

	// Tooltips
	EnableToolTips(TRUE);

	// Esto ajusta la ventana a la última posición y tamaño
	// y nunca debe ubicarse antes de la lectura de posiciones
	// y tamaños de los controles que se realiza al comienzo de
	// este mismo método.
	int x = AfxGetApp()->GetProfileInt(L"CFG", L"MainX", -1);
	int y = AfxGetApp()->GetProfileInt(L"CFG", L"MainY", -1);
	int w = AfxGetApp()->GetProfileInt(L"CFG", L"MainW", -1);
	int h = AfxGetApp()->GetProfileInt(L"CFG", L"MainH", -1);

	const CWnd * zOrder;
	UINT nFlags = SWP_NOMOVE | SWP_NOSIZE;

	if ((x>=0) && (y>=0))
		nFlags &= ~SWP_NOMOVE;

	if ((w>=0) && (h>=0))
		nFlags &= ~SWP_NOSIZE;

	if (m_bOnTop)
		zOrder = &CWnd::wndTopMost;
	else
		zOrder = &CWnd::wndNoTopMost;

	SetWindowPos(zOrder, x, y, w-x, h-y, nFlags);

  //////////
	// SystemWideHook -- No usar en DEBUG
  loadRunSlots();

#ifndef DEBUG
	g_bKBShortCut = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneSC", 1);
	g_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, GetModuleHandle(L"DRKHooks.DLL"), 0);
	g_bKBHookInstalled = TRUE;
#endif
  /////////////

  if (!AfxGetApp()->GetProfileInt(L"CFG", L"DontShowDebugWarning", 0) && !m_bDebugPrivilege) {

		CString url;
		url.LoadString(IDS_WEBDEBUGURL);

    WCHAR tmp[250];
		int answer = MessageBox(_("Daphne needs 'Debug programs' privilges in order to show full process list. Do you want to check DRK web site for a check list of the procedure? (If you click Cancel you won't see this warning again)"), __("Your account lacks debug privileges", tmp, sizeof(tmp)),  MB_YESNOCANCEL | MB_ICONWARNING);

		switch(answer) {
		case IDYES:
			::ShellExecute(NULL, L"OPEN", url, NULL, NULL, SW_SHOWNORMAL);
			break;
		case IDNO:
			break;
		case IDCANCEL:
			// No more
			AfxGetApp()->WriteProfileInt(L"CFG", L"DontShowDebugWarning", 1);
			break;
		}
	}

  hUpTimeMutex = CreateMutex(NULL, FALSE, NULL);
  SetTimer(IDT_SCHEDULER, 60000, NULL);

  RefreshInfo();

 	SetRefreshTimer();

  ::PostMessage(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), WM_DRK_JUST_STARTED, 0, 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}



LRESULT CDaphneDlg::OnJustStarted(WPARAM wp, LPARAM lp) {
  if (bProcessLogging) {
    CStdioFile f;
    CFileException fileException;
    CString line;
    if (f.Open( strProcessLoggingFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite, &fileException ) )
    {
      f.SeekToEnd();
      line.Format(_("* %s ============= Daphne ready =============\n"), CTime::GetCurrentTime().Format(L"%Y.%m.%d %H:%M:%S"));
      f.WriteString(line);
      f.Close();
    }
  }
  return 0;
}

void CDaphneDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDaphneDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
// the minimized window.
HCURSOR CDaphneDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDaphneDlg::OnCancel() 
{
	SetWMode(0);
}

void CDaphneDlg::RefreshList()
{
	if (WaitForSingleObject(hRefreshMutex, 40) == WAIT_OBJECT_0) {

		m_cProcList.SetRedraw(FALSE);

		// TODO: Verificar
    __int64 cputime = processes.getIdleCPUTime();

		// Actualizo lista
		CProcess * proc = processes.getFirstProcess();

		int i=0;
		while(proc != NULL) {

			if (proc->isAlive())
        InsertOrUpdateList(proc, cputime, processes.getUsedCPUTime());
			else 
				RemoveFromList(proc);

			proc = processes.getNextProcess();
		}

		// Vuelo los inválidos
		processes.purgeInvalidProcesses();

		ReleaseMutex(hRefreshMutex);
	}

	if (WaitForSingleObject(hRefreshMutex, 20) == WAIT_OBJECT_0) {

		// Sort
		m_cProcList.SortItems(CDaphneDlg::Compare, (DWORD)this);
		ReleaseMutex(hRefreshMutex);
	}

	m_cProcList.SetRedraw();

	m_bUpdatePath = FALSE;
}

void CDaphneDlg::InsertOrUpdateList(CProcess *process, __int64 cputime, __int64 usedtime)
{
	CString strTmp;
	LVFINDINFO find;
	int index;

  if (m_bHideZombies && process->bZombie)
    return;

	// Busco el item
	find.flags = LVFI_PARAM | LVFI_WRAP;
	find.lParam = process->dwPID;
	index = m_cProcList.FindItem(&find);


	if (index == -1) {

		// Insert
		index = m_cProcList.InsertItem(0, L"<new>");

		// Optimizo...
		m_cProcList.SetItemText(index, 1, process->strName);

		if (!m_bExtPath)
			m_cProcList.SetItemText(index, 2, process->strFullPath);
		else
			m_cProcList.SetItemText(index, 2, process->strFullPathEx);

		// SetData
		m_cProcList.SetItemData(index, process->dwPID);

		strTmp.Format(L"%u", process->dwPID);
		m_cProcList.SetItemText(index, 0, strTmp);

		if (process->bService)
			m_cProcList.SetItemText(index, 4, _("Service"));
		else
			m_cProcList.SetItemText(index, 4, _("Process"));
	}
	else {
		if (m_bUpdatePath) {
			if (!m_bExtPath)
				m_cProcList.SetItemText(index, 2, process->strFullPath);
			else
				m_cProcList.SetItemText(index, 2, process->strFullPathEx);
		}

		// BUG FIXED: En ocasiones queda sin actualizar el nombre
		if ((!m_cProcList.GetItemText(index, 1).Compare(DAPHNE_UNDEFINEDNAME)) ||
			(!m_cProcList.GetItemText(index, 1).Compare(DAPHNE_ZOMBIE)) ||
			(!process->isValid())) {
			m_cProcList.SetItemText(index, 1, process->strName);

			if (!m_bExtPath)
				m_cProcList.SetItemText(index, 2, process->strFullPath);
			else
				m_cProcList.SetItemText(index, 2, process->strFullPathEx);
		}
	}


	// Info...

	switch(process->dwPriority) {
	case HIGH_PRIORITY_CLASS:
		strTmp = _("High");
		break;
	case NORMAL_PRIORITY_CLASS:
		strTmp = _("Normal");
		break;
	case IDLE_PRIORITY_CLASS:
		strTmp = _("Low");
		break;
	case REALTIME_PRIORITY_CLASS:
		strTmp = _("Real time");
		break;
	default:
		strTmp = _("N/A");
		break;
	}
	m_cProcList.SetItemText(index, 3, strTmp);

	strTmp.Format(L"%6.2f", (double)process->WorkingSetSize / (double)(1024*1024));
	m_cProcList.SetItemText(index, 5, strTmp);

	strTmp.Format(L"%6.2f", (double)process->PeakWorkingSetSize / (double)(1024*1024));
	m_cProcList.SetItemText(index, 6, strTmp);

	strTmp.Format(L"%6.2f", (double)process->PagefileUsage / (double)(1024*1024));
	m_cProcList.SetItemText(index, 7, strTmp);

	strTmp.Format(L"%6.2f", (double)process->PeakPagefileUsage / (double)(1024*1024));
	m_cProcList.SetItemText(index, 8, strTmp);
				
	strTmp.Format(L"%u", process->GetThreadCount());
	m_cProcList.SetItemText(index, 9, strTmp);

	// BUG in 1.17: Divide by zero
  if (process->dwPID == 0)
    strTmp.Format(L"%02u", 100 * (process->liCPUTimeTotal - process->liCPUTimeLast - usedtime) / cputime);
	else if (cputime > 0)
		strTmp.Format(L"%02u", 100 * (process->liCPUTimeTotal - process->liCPUTimeLast) / cputime);
	else
		strTmp = L"00";
	m_cProcList.SetItemText(index, 10, strTmp);

  m_cProcList.SetItemText(index, 11, process->strUser);

	strTmp.Format(L"%u", process->ReadOperationCount);
	m_cProcList.SetItemText(index, 12, strTmp);

	strTmp.Format(L"%u", process->WriteOperationCount);
	m_cProcList.SetItemText(index, 13, strTmp);

}

void CDaphneDlg::RemoveFromList(CProcess *process)
{
	LVFINDINFO find;
	int index;

	// Busco el item
	find.flags = LVFI_PARAM | LVFI_WRAP;
	find.lParam = process->dwPID;
	index = m_cProcList.FindItem(&find);


	if (index != -1) {

		// Remove
		m_cProcList.DeleteItem(index);
	}

}

void CDaphneDlg::OnTimer(UINT_PTR nIDEvent) 
{

	switch(nIDEvent) {
	case IDT_REFRESH:
    CalculateCPUUsage();
    if (!m_bWindowHidden || m_bAnimatedIcon) {
			RefreshList();
			RefreshInfo();
		}
		UpdateTrayToolTip();
		break;
	case IDT_UPDATELIST:
		break;
	case IDT_SERVICES:
		// Actualizo SCM
		if (WaitForSingleObject(hRefreshMutex, INFINITE) == WAIT_OBJECT_0) {
  		scm.ReadServicesMap();
			ReleaseMutex(hRefreshMutex);
    }
		break;
  case IDT_SCHEDULER:
    scheduler();
    break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CDaphneDlg::OnRclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMenu menu;
	CMenu copy_menu, prio_menu;
	POINT point;

	CProcess * proc = waitForProcess(GetSelectedPID());

	if (proc != NULL) {

		menu.CreatePopupMenu();
		menu.InsertMenu(0, 0, IDM_KILL, _("Kill"));
		menu.InsertMenu(-1, MF_BYPOSITION, IDM_KILLPOLITE, _("Kill politely"));
		menu.InsertMenu(-1, MF_BYPOSITION, IDM_KILLAT, _("Kill at..."));
		menu.InsertMenu(-1, MF_BYPOSITION, IDM_KILLPOLITEAT, _("Kill politely at..."));
    
		menu.InsertMenu(-1, MF_BYPOSITION, MF_SEPARATOR);
		menu.InsertMenu(-1, MF_BYPOSITION, IDM_FOCUS, _("Set focus"));
		menu.InsertMenu(-1, MF_BYPOSITION, IDM_ONTOP_ON, _("Set always on top"));
		menu.InsertMenu(-1, MF_BYPOSITION, IDM_ONTOP_OFF, _("Unset always on top"));
		menu.InsertMenu(-1, MF_BYPOSITION, IDM_NEWTRAP, _("Create trap"));
		menu.InsertMenu(-1, MF_BYPOSITION, IDM_AFFINITY, _("Set affinity..."));

		prio_menu.CreatePopupMenu();
		prio_menu.InsertMenu(-1, MF_BYPOSITION, IDM_SPC_RT, _("Real time"));
		prio_menu.InsertMenu(-1, MF_BYPOSITION, IDM_SPC_HI, _("High"));
		prio_menu.InsertMenu(-1, MF_BYPOSITION, IDM_SPC_NO, _("Normal"));
		prio_menu.InsertMenu(-1, MF_BYPOSITION, IDM_SPC_LO, _("Low"));
		switch(proc->dwPriority) {
		case REALTIME_PRIORITY_CLASS:
			prio_menu.CheckMenuItem(IDM_SPC_RT, MF_BYCOMMAND|MF_CHECKED);
			break;
		case HIGH_PRIORITY_CLASS:
			prio_menu.CheckMenuItem(IDM_SPC_HI, MF_BYCOMMAND|MF_CHECKED);
			break;
		case NORMAL_PRIORITY_CLASS:
			prio_menu.CheckMenuItem(IDM_SPC_NO, MF_BYCOMMAND|MF_CHECKED);
			break;
		case IDLE_PRIORITY_CLASS:
			prio_menu.CheckMenuItem(IDM_SPC_LO, MF_BYCOMMAND|MF_CHECKED);
			break;
		}
		menu.InsertMenu(-1, MF_POPUP, (UINT)prio_menu.m_hMenu, _("Set priority"));


		if (proc->bService) {
			menu.InsertMenu(-1, MF_BYPOSITION, MF_SEPARATOR);
			menu.InsertMenu(-1, MF_BYPOSITION, IDM_SER_ON, _("Start service"));
			menu.InsertMenu(-1, MF_BYPOSITION, IDM_SER_CONT, _("Continue service"));
			menu.InsertMenu(-1, MF_BYPOSITION, IDM_SER_PAUSE, _("Pause service"));
			menu.InsertMenu(-1, MF_BYPOSITION, IDM_SER_OFF, _("Stop service"));

			switch(proc->dwServiceState) {
			case SERVICE_PAUSED:
				menu.EnableMenuItem(IDM_SER_PAUSE, MF_GRAYED);
				break;
			case SERVICE_RUNNING:
				menu.EnableMenuItem(IDM_SER_ON, MF_GRAYED);
				menu.EnableMenuItem(IDM_SER_CONT, MF_GRAYED);
				break;
			case SERVICE_STOPPED:
				menu.EnableMenuItem(IDM_SER_OFF, MF_GRAYED);
				break;
			case SERVICE_CONTINUE_PENDING:
			case SERVICE_START_PENDING:
			case SERVICE_STOP_PENDING:
			case SERVICE_PAUSE_PENDING:
				menu.EnableMenuItem(IDM_SER_ON, MF_GRAYED);
				menu.EnableMenuItem(IDM_SER_OFF, MF_GRAYED);
				menu.EnableMenuItem(IDM_SER_PAUSE, MF_GRAYED);
				menu.EnableMenuItem(IDM_SER_CONT, MF_GRAYED);
				break;
			}
		}
		menu.InsertMenu(-1, MF_BYPOSITION, MF_SEPARATOR);
		menu.InsertMenu(-1, MF_BYPOSITION, IDM_CPY_NAME, _("Copy name"));
		copy_menu.CreatePopupMenu();
		copy_menu.InsertMenu(-1, MF_BYPOSITION, IDM_CPY_PID, _("PID"));
		copy_menu.InsertMenu(-1, MF_BYPOSITION, IDM_CPY_PATH, _("Path"));
		copy_menu.InsertMenu(-1, MF_BYPOSITION, IDM_CPY_PATHARGS, _("Path and arguments"));
		copy_menu.InsertMenu(-1, MF_BYPOSITION, IDM_CPY_MD5, _("MD5 hash"));
		copy_menu.InsertMenu(-1, MF_BYPOSITION, IDM_CPY_SHA1, _("SHA1 hash"));
		menu.InsertMenu(-1, MF_POPUP, (UINT)copy_menu.m_hMenu, _("Copy..."));
		menu.InsertMenu(-1, MF_BYPOSITION, IDM_DRK_OPENFOLDER, _("Open containing folder..."));
		menu.InsertMenu(-1, MF_BYPOSITION, MF_SEPARATOR);
		menu.InsertMenu(-1, MF_BYPOSITION, IDM_DRK_LOOKUP, _("Look up into DRK DB..."));
		menu.InsertMenu(-1, MF_BYPOSITION, IDM_DRK_SUBMIT, _("Submit to DRK DB..."));
		menu.InsertMenu(-1, MF_BYPOSITION, MF_SEPARATOR);
		menu.InsertMenu(-1, MF_BYPOSITION, IDM_PROPERTIES, _("Properties"));

		menu.SetDefaultItem(IDM_PROPERTIES);

		GetCursorPos(&point);

		menu.TrackPopupMenu(0, point.x, point.y, this);
	}
	
	*pResult = 0;
}

BOOL CDaphneDlg::enableShutDown()
{
    HANDLE token;
    TOKEN_PRIVILEGES privileges;
    BOOL result = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token);

    privileges.PrivilegeCount = 1;
    LookupPrivilegeValue( 
      NULL,            // lookup privilege on local system
      SE_SHUTDOWN_NAME,   // privilege to lookup 
      &privileges.Privileges[0].Luid );
    privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (result)
      result = AdjustTokenPrivileges(token,
        FALSE,
        &privileges,
        sizeof(TOKEN_PRIVILEGES), 
        (PTOKEN_PRIVILEGES) NULL, 
        (PDWORD) NULL);

    CloseHandle(token);

    return result;
}

BOOL CDaphneDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	CAboutDlg about;
	CInspector inspector;
	CKillAllDlg killall;
	CCloseAllWndByTitle closeall;
	CRunProcDlg rundlg;
	CAuthUserDlg authdlg;
	CNewTrapDlg trapdlg;
	CWndsPopUp wnds;
  CAffinityDlg affinity;
	BOOL polite = FALSE;
	CPropertiesDlg * pDlg = NULL;
	CString str;
	int deaths;
	CProcess * proc = NULL;


	int showApp = 0;
	WORD item = LOWORD(wParam);
	if (item >= WM_DRK_UNHIDE && item <= WM_DRK_UNHIDE_END) {
		showApp = item - WM_DRK_UNHIDE;
		item -= showApp;
	}

	switch(item) {
	case IDM_KILL:
		if (proc = waitForProcess(GetSelectedPID()))
			proc->Kill();
    CDaphneApp::hitStatisticalRecord(L"Kill");
		RefreshList();
		break;
	case IDM_KILLPOLITE:
		if (proc = waitForProcess(GetSelectedPID()))
			proc->KillPolite();
    CDaphneApp::hitStatisticalRecord(L"KillPolite");
		RefreshList();
		break;
	case IDM_KILLPOLITEAT:
	case IDM_KILLAT:
    if (proc = waitForProcess(GetSelectedPID())) {
      CKillAtDlg dlg;
      dlg.polite = item == IDM_KILLPOLITEAT;
      dlg.processname = proc->strName;
      if (dlg.DoModal() == IDOK) {
        TRACE(L"Schedule kill at %s\n", dlg.fulltime.Format(L"%d/%m/%Y %H:%M:%S"));
        schedule[dlg.fulltime].push_back(new KillTask(dlg.fulltime, proc->dwPID, proc->strName, dlg.polite == TRUE));

        // Statistics
        CDaphneApp::hitStatisticalRecord(L"ScheduledKill");
      }
    }
		break;
  case WM_K_SCHEDULED:
    {
      CScheduledTasksDlg dlg;
      dlg.schedule = &schedule;
      dlg.DoModal();
    }
    break;
  case WM_K_SCHEDULE_MSG:
    {
      CMessageAtDlg dlg;
      if (dlg.DoModal() == IDOK) {
        schedule[dlg.fulltime].push_back(new PopupMessageTask(dlg.fulltime, dlg.message));
        CDaphneApp::hitStatisticalRecord(L"ScheduledPopupMessage");
      }
    }
    break;
  case WM_K_SCHEDULE_OFF:
    {
      if (!enableShutDown()) {
        MessageBox(_("Daphne was unable to get enough privileges for a system shutdown."), _("Can't schedule shutdown"), MB_OK|MB_ICONSTOP);
      }
      else {
        CScheduleShutdownDlg dlg;
        if (dlg.DoModal() == IDOK) {
          TRACE(L"Schedule system shutdown at %s\n", dlg.fulltime.Format(L"%d/%m/%Y %H:%M:%S"));
          schedule[dlg.fulltime].push_back(new SysShutdownTask(dlg.fulltime, dlg.type));
          CDaphneApp::hitStatisticalRecord(L"ScheduledShutdown");
        }
      }
    }
    break;
	case IDM_FOCUS:
		if (proc = waitForProcess(GetSelectedPID()))
			proc->Activate();
		break;
	case IDM_ONTOP_ON:
		if (proc = waitForProcess(GetSelectedPID()))
			proc->SetOnTop();
		break;
	case IDM_ONTOP_OFF:
		if (proc = waitForProcess(GetSelectedPID()))
			proc->SetOnTop(FALSE);
		break;
	case IDM_SPC_RT:
		if (proc = waitForProcess(GetSelectedPID()))
			proc->SetPriority(REALTIME_PRIORITY_CLASS);
		break;
	case IDM_SPC_HI:
		if (proc = waitForProcess(GetSelectedPID()))
			proc->SetPriority(HIGH_PRIORITY_CLASS);
		break;
	case IDM_SPC_NO:
		if (proc = waitForProcess(GetSelectedPID()))
			proc->SetPriority(NORMAL_PRIORITY_CLASS);
		break;
	case IDM_SPC_LO:
		if (proc = waitForProcess(GetSelectedPID()))
			proc->SetPriority(IDLE_PRIORITY_CLASS);
		break;
	case IDM_SER_ON:
		if (proc = waitForProcess(GetSelectedPID()))
			proc->serviceStart();
    CDaphneApp::hitStatisticalRecord(L"ServiceAction");
		break;
	case IDM_SER_OFF:
		if (proc = waitForProcess(GetSelectedPID()))
			proc->serviceStop();
    CDaphneApp::hitStatisticalRecord(L"ServiceAction");
		break;
	case IDM_SER_CONT:
		if (proc = waitForProcess(GetSelectedPID()))
			proc->serviceContinue();
    CDaphneApp::hitStatisticalRecord(L"ServiceAction");
		break;
	case IDM_SER_PAUSE:
		if (proc = waitForProcess(GetSelectedPID()))
			proc->servicePause();
    CDaphneApp::hitStatisticalRecord(L"ServiceAction");
		break;
	case IDM_INJSA:
		if (proc = waitForProcess(GetSelectedPID()))
			proc->InjectDLL(strSpyAgentDLL);
		break;
	case IDM_RMVSA:
		if (proc = waitForProcess(GetSelectedPID()))
			proc->RemoveDLL();
		break;
	case IDM_NEWTRAP:
		if (proc = waitForProcess(GetSelectedPID())) {
			trapdlg.m_strName = proc->strName;
			if (trapdlg.DoModal() == IDOK) {

				traps.push_back(trapdlg.trap);
			}
		}
		break;

	case IDM_AFFINITY:
		if (proc = waitForProcess(GetSelectedPID())) {
      affinity.m_traps = &traps;
      affinity.m_pProcess = proc;
      affinity.DoModal();
      // Statistics
      CDaphneApp::hitStatisticalRecord(L"AffinityMask");
    }
    break;

	case IDM_DRK_LOOKUP:
		if (proc = waitForProcess(GetSelectedPID()))
			proc->toDRKDB();
		break;

  case IDM_DRK_OPENFOLDER:
    if (proc = waitForProcess(GetSelectedPID())) {
      CString path;
      TCHAR drive[_MAX_DRIVE];
      TCHAR dir[_MAX_DIR];
      TCHAR fname[_MAX_FNAME];
      TCHAR ext[_MAX_EXT];
      _wsplitpath_s((LPCTSTR)proc->strFullPath, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT);
      path.Format(L"%s%s", drive, dir);
      ::ShellExecute(NULL, L"explore", path, NULL, NULL, SW_SHOWNORMAL);
      // Statistics
      CDaphneApp::hitStatisticalRecord(L"OpenContainingFolder");
    }
		break;

	case IDM_DRK_SUBMIT:
		if (proc = waitForProcess(GetSelectedPID()))
			proc->submitToDRKDB();
		break;

	case IDM_PROPERTIES:
		if (proc = waitForProcess(GetSelectedPID())) {
			pDlg = new CPropertiesDlg(proc);
			if (pDlg) {
				pDlg->DoModal();
				delete pDlg;
			}
		}
		break;

	case WM_K_BYNAMEP:
		polite = TRUE;
		killall.m_bPolite = TRUE;
	case WM_K_BYNAME:
		killall.m_strName.Empty();
		if ((killall.DoModal() == IDOK) && (killall.m_strName.GetLength() > 0)) {
			deaths = KillByName(killall.m_strName, polite, killall.m_bCase);
			str.Format((polite)?_("%u processes were politely killed."):_("%u processes were killed."), deaths);
			MessageBox(str, _("Daphne"), MB_OK | MB_ICONINFORMATION);
		}
		break;

	case WM_K_WNDCLOSE:
		closeall.m_strName.Empty();
		if ((closeall.DoModal() == IDOK) && (closeall.m_strName.GetLength() > 0)) {
			deaths = CloseByName(closeall.m_strName, closeall.m_bCase);
			str.Format(_("%u windows were closed."), deaths);
			MessageBox(str, _("Daphne"), MB_OK | MB_ICONINFORMATION);
		}
		break;

	case WM_K_RUNPAS:
		if (authdlg.DoModal() == IDOK)
			rundlg.pInfo = &authdlg;
		else
			break;
	case WM_K_RUNP:
		rundlg.DoModal();
    if (rundlg.pInfo != NULL) {
      SecureZeroMemory(rundlg.pInfo->m_strPasswd.GetBufferSetLength(100), 100);
      rundlg.pInfo->m_strPasswd.ReleaseBuffer(100);
      rundlg.pInfo->m_strPasswd = L"Oíd, mortales, el grito sagrado: Libertad, libertad, libertad.";
    }
		break;

	case WM_K_PTREE: {
			if (WaitForSingleObject(hRefreshMutex, 1500) == WAIT_OBJECT_0) {
				CProcessTreeDlg ptdlg(&processes.processes);
				if (ptdlg.DoModal() == IDOK)
          selectProcess(ptdlg.selectedProcess->dwPID);
				ReleaseMutex(hRefreshMutex);
			}
		}
		break;
  case WM_K_CPUUSAGE: {
    CCPUUsageDlg dlg;
    dlg.graph.initialize(&usageQueue, 0, 100, hRefreshMutex, 3);
    if (!IsWindowVisible())
			SetEvent(hRefreshEvent);
    dlg.DoModal();
  	if (!hasAutomation() && !IsWindowVisible())
  	  ResetEvent(hRefreshEvent);
    }
    break;
	case WM_K_ISOFT: {
			ClssoftDlg dlg;
			dlg.DoModal();
		}
		break;

	case WM_K_INSPECTOR:
		if (IsWindowVisible()) {
			SetWMode(0);
			inspector.DoModal();
			SetWMode(1);
		}
		else
			inspector.DoModal();
		break;

  case WM_K_COPYLIST:
    processList2Clipboard();
    // Statistics
    CDaphneApp::hitStatisticalRecord(L"ProcessList2Clipboard");
    break;
  case WM_K_GOD: {
    CString god;
    god.Format(L"%s\\GodMode.{ED7BA470-8E54-465E-825C-99712043E01C}", CDaphneApp::getUserDirectory());
    CreateDirectory(god, NULL);
    ::ShellExecute(NULL, L"explore", god, NULL, NULL, SW_SHOWNORMAL);
    }
    break;
  case WM_K_PSR: {
    ::ShellExecute(NULL, L"open", L"psr.exe", NULL, NULL, SW_SHOWNORMAL);
    }
    break;

	case IDM_CPY_NAME:
		Copy(0);
		break;
	case IDM_CPY_PATH:
		Copy(1);
		break;
	case IDM_CPY_PATHARGS:
		Copy(2);
		break;
	case IDM_CPY_PID:
		Copy(3);
		break;
  case IDM_CPY_MD5:
		Copy(4);
    break;
  case IDM_CPY_SHA1:
		Copy(5);
    break;

	case IDM_TASKS:
		wnds.hiddenWindows = &hiddenWindows;
		wnds.DoModal();
		break;

	case WM_DRK_UNHIDE:
		{
      if (WaitForSingleObject(hHiddenMutex, INFINITE) == WAIT_OBJECT_0) {
			  int c = 0;
			  CWnd * ts = NULL;
			  MapWindows::iterator i;
			  for(i = hiddenWindows.begin(); i != hiddenWindows.end(); i++) {
				  CWnd * p = i->second;
			
				  if (c == showApp) {
					  ts = p;
				  }
				  ++c;
			  }
			  if (ts != NULL)
				  showWindow(ts->GetSafeHwnd());

        ReleaseMutex(hHiddenMutex);
      }
		}

		break;

	//Open
	case WM_DRK_SHOW:
		if(IsWindowVisible()) {
			SetWMode(0);
		}
		else {
			SetWMode(1);
		}
		break;

  case WM_DRK_RS1:
  case WM_DRK_RS2:
  case WM_DRK_RS3:
  case WM_DRK_RS4:
    {
      
		  STARTUPINFO startup;
		  PROCESS_INFORMATION pinfo;
		  DWORD nFlags = 0;

      ZeroMemory( &pinfo, sizeof(pinfo) );
      ZeroMemory( &startup, sizeof(startup) );
		  startup.cb = sizeof(STARTUPINFO);
		  startup.dwFlags = STARTF_USESHOWWINDOW;
		  startup.wShowWindow = SW_SHOWDEFAULT;
		  startup.lpDesktop = NULL;
		  startup.lpTitle = NULL;
		  startup.cbReserved2 = 0;
		  startup.lpReserved2 = NULL;

      TCHAR cmdline[MAX_PATH];
      wcsncpy_s(cmdline, runSlots[LOWORD(wParam)-WM_DRK_RS1], MAX_PATH);
		  CreateProcess(NULL, cmdline, NULL, NULL, FALSE, nFlags, NULL, NULL, &startup, &pinfo);

      // Statistics
      CDaphneApp::hitStatisticalRecord(L"ExecuteRunSlot");
    }
    break;

	// About
	case WM_DRK_ABOUT:
		about.DoModal();
		break;

	case WM_DRK_DAPHNESTAT:
    {
      CString url;
      url.LoadStringW(IDS_WEBURLSTATINFO);
		  ::ShellExecute(AfxGetApp()->m_pMainWnd->m_hWnd, L"open", url, L"", L"", SW_SHOWNORMAL);	
    }
		break;
  case WM_DRK_DAPHNESUPPORT:
    {
      CString url;
      url.LoadStringW(IDS_WEBURLSUPPORT);
		  ::ShellExecute(AfxGetApp()->m_pMainWnd->m_hWnd, L"open", url, L"", L"", SW_SHOWNORMAL);	
    }
		break;
  case WM_DRK_DAPHNEFAQ:
    {
      CString url;
      url.LoadStringW(IDS_WEBURLFAQ);
		  ::ShellExecute(AfxGetApp()->m_pMainWnd->m_hWnd, L"open", url, L"", L"", SW_SHOWNORMAL);	
    }
		break;

	case WM_DRK_GOOGLE:
    {
      CString url;
      url.LoadStringW(IDS_WEBSEARCH);
		  ::ShellExecute(AfxGetApp()->m_pMainWnd->m_hWnd, L"open", url, L"", L"", SW_SHOWNORMAL);	
    }
		break;

	// QUIT
	case WM_DRK_QUIT:
		Quit();
		break;
	}

	// Killers
	if ((LOWORD(wParam) >= WM_K_KILLER) && (LOWORD(wParam) <= WM_K_KILLER_END)) {
		int index = LOWORD(wParam) - WM_K_KILLER;

		CKiller * p = GetKiller(index);

		p->Kill(this);

		delete p;
	}
	
	return CDialog::OnCommand(wParam, lParam);
}

DWORD CDaphneDlg::GetSelectedPID()
{
	POSITION pos = m_cProcList.GetFirstSelectedItemPosition();

  return (pos>0)?(DWORD)m_cProcList.GetItemData((int)(pos-1)):NULL;
}

LRESULT CALLBACK CDaphneDlg::MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {

	return 0;
}

int CALLBACK CDaphneDlg::Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {

	int result = 0;
	DWORD PID1 = (DWORD)lParam1;
	DWORD PID2 = (DWORD)lParam2;

	CDaphneDlg * list = (CDaphneDlg *)lParamSort;
	CProcess * proc1 = list->processes.getProcess(PID1);
	CProcess * proc2 = list->processes.getProcess(PID2);

	// Solo por si desaparece un proceso
	if ((!proc1) || (!proc2))
		return 1;

	switch(list->orderColumn) {

	case 0:
		if (PID1 >= PID2)
			result = 1;
		else
			result = -1;
		break;
	case 1:
		if (proc1->strName.CompareNoCase(proc2->strName) == 0) {
			if (PID1 >= PID2)
				result = 1;
			else
				result = -1;
		}
		else
			result = proc1->strName.CompareNoCase(proc2->strName);
		break;
	case 2:
		if (list->m_bExtPath) {
			if (proc1->strFullPathEx.CompareNoCase(proc2->strFullPathEx) == 0) {
				if (PID1 >= PID2)
					result = 1;
				else
					result = -1;
			}
			else
				result = proc1->strFullPathEx.CompareNoCase(proc2->strFullPathEx);
		}
		else {
			if (proc1->strFullPath.Compare(proc2->strFullPath) == 0) {
				if (PID1 >= PID2)
					result = 1;
				else
					result = -1;
			}
			else
				result = proc1->strFullPath.CompareNoCase(proc2->strFullPath);
		}
		break;
	case 3:
		if (proc1->dwPriority > proc2->dwPriority)
			result = 1;
		else
			result = -1;
		break;
	case 4:
		if (proc1->bService == proc2->bService)
			if (PID1 >= PID2)
				result = 1;
			else
				result = -1;
		else
      if (proc1->bService && !proc2->bService)
				result = 1;
			else
  			result = -1;
		break;
	case 5:
		if (proc1->WorkingSetSize > proc2->WorkingSetSize)
			result = 1;
		else
			result = -1;
		break;
	case 6:
		if (proc1->PeakWorkingSetSize > proc2->PeakWorkingSetSize)
			result = 1;
		else
			result = -1;
		break;
	case 7:
		if (proc1->PagefileUsage > proc2->PagefileUsage)
			result = 1;
		else
			result = -1;
		break;
	case 8:
		if (proc1->PeakPagefileUsage > proc2->PeakPagefileUsage)
			result = 1;
		else
			result = -1;
		break;
	case 9:
		if (proc1->GetThreadCount() > proc2->GetThreadCount())
			result = 1;
		else
			result = -1;
		break;
	case 10:
		if ((proc1->liCPUTimeTotal - proc1->liCPUTimeLast) > (proc2->liCPUTimeTotal - proc2->liCPUTimeLast))
			result = 1;
		else
			result = -1;
		break;
	case 11:
		if (proc1->strUser.CompareNoCase(proc2->strUser) == 0) {
			if (PID1 >= PID2)
				result = 1;
			else
				result = -1;
		}
		else
			result = proc1->strUser.CompareNoCase(proc2->strUser);
		break;
	case 12:
    if (proc1->ReadOperationCount > proc2->ReadOperationCount)
			result = 1;
		else
			result = -1;
		break;
	case 13:
    if (proc1->WriteOperationCount > proc2->WriteOperationCount)
			result = 1;
		else
			result = -1;
		break;
	}

	// Asc / Desc
	if (list->orderMode)
		result *= -1;

	return result;
}

void CDaphneDlg::OnFullpathex() 
{
	UpdateData(TRUE);
	m_bUpdatePath = TRUE;
	RefreshList();
}

// Thread-safe
int CDaphneDlg::OnDeadProcess(DWORD pid, CString name) {
  CString str;

  // LOG
  if (bProcessLogging) {
    CStdioFile f;
    CFileException fileException;
    CString line, fname;
    if (f.Open( strProcessLoggingFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite, &fileException ) )
    {
      f.SeekToEnd();
      line.Format(L"- %s %s %u\n", CTime::GetCurrentTime().Format(L"%Y.%m.%d %H:%M:%S"), name, pid);
      f.WriteString(line);
      f.Close();
    }
  }

  if (WaitForSingleObject(hTrapsMutex, 250) == WAIT_OBJECT_0) {
    TrapList::iterator i;
	  for (i = traps.begin(); i != traps.end(); i++) {
		  CTrap * trap = (CTrap *)(*i);

		  if (trap->getProcessName().Compare(name) == 0) {

			  switch(trap->getType()) {
			  // Notify
			  case 11:
				  str.Format(_("Process %s [%u] destroyed."), name, pid);
				  ShowBalloon(str);
				  break;
			  default:
				  break;
			  }
		  }
	  }

    ReleaseMutex(hTrapsMutex);
  }

  return 0;
}

// Thread-safe
int CDaphneDlg::OnNewProcess(DWORD dwPID) {
	
	CProcess * p = new CProcess(dwPID);
  CStdioFile f;
  CFileException fileException;
  CString line, fname;
	CString str;
	HWND hWndMain = 0;
	LONG style;
  BOOL log = FALSE;

  // LOG
  if (bProcessLogging && p->strName != L"<?>") {
    if (f.Open( strProcessLoggingFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite, &fileException ) )
    {
      log = TRUE;
      f.SeekToEnd();
      line.Format(L"+ %s %s %u [%s]\n", CTime::GetCurrentTime().Format(L"%Y.%m.%d %H:%M:%S"), p->strName, p->dwPID, p->strUser);
      f.WriteString(line);
    }
  }

  if (WaitForSingleObject(hTrapsMutex, INFINITE) == WAIT_OBJECT_0) {
	  TrapList::iterator i;
	  for (i = traps.begin(); i != traps.end(); i++) {
		  CTrap * trap = (CTrap *)(*i);

		  if (trap->getProcessName().Compare(p->strName) == 0) {

			  switch(trap->getType()) {
			  // Notify
			  case 0:
				  str.Format(_("New process %s created."), p->strName);
				  ShowBalloon(str);
				  break;
			  // Kill
			  case 1:
				  p->Kill();
          if (log) {
            line.Format(L"x %s Killed process %s %u\n", CTime::GetCurrentTime().Format(L"%Y.%m.%d %H:%M:%S"), p->strName, p->dwPID);
            f.WriteString(line);
          }
				  break;
			  // Hide application
			  case 2:
          if (p->waitForInputIdle(500) == 0) {
				    hWndMain = guessWindow(p, trap->getWindowContains());
				    if (hWndMain != 0) {
					    hideWindow(hWndMain);
              if (log) {
                line.Format(L"x %s Windows [%s] hidden for process %s %u\n", CTime::GetCurrentTime().Format(L"%Y.%m.%d %H:%M:%S"), trap->getWindowContains(), p->strName, p->dwPID);
                f.WriteString(line);
              }
            }
          }
				  break;
			  // Ontop ON
			  case 3:
          if (p->waitForInputIdle(500) == 0) {
				    hWndMain = guessWindow(p, trap->getWindowContains());
				    if (hWndMain != 0) {
					    ::SetWindowPos(hWndMain, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
              if (log) {
                line.Format(L"x %s Always on top set for windows [%s], process %s %u\n", CTime::GetCurrentTime().Format(L"%Y.%m.%d %H:%M:%S"), trap->getWindowContains(), p->strName, p->dwPID);
                f.WriteString(line);
              }
				    }
          }
				  break;
			  // Ontop OFF
			  case 4:
          if (p->waitForInputIdle(500) == 0) {
				    hWndMain = guessWindow(p, trap->getWindowContains());
				    if (hWndMain != 0) {
					    ::SetWindowPos(hWndMain, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
              if (log) {
                line.Format(L"x %s Always on top unset for windows [%s], process %s %u\n", CTime::GetCurrentTime().Format(L"%Y.%m.%d %H:%M:%S"), trap->getWindowContains(), p->strName, p->dwPID);
                f.WriteString(line);
              }
				    }
          }
				  break;
			  // Alpha 75
			  case 5:
          if (p->waitForInputIdle(500) == 0) {
            hWndMain = guessWindow(p, trap->getWindowContains());
				    if (hWndMain != 0) {
					    style = ::GetWindowLong(hWndMain, GWL_EXSTYLE);
              if (style == 0) {
                TRACE(L"GetWindowLong() error: %08lxh HWND: %08lxh", GetLastError(), hWndMain);
              }
					    style |= WS_EX_LAYERED;
              SetLastError(0);
              if (::SetWindowLong(hWndMain, GWL_EXSTYLE, style) == 0) {
                TRACE(L"SetWindowLong() error: %08lxh", GetLastError());
              }
					    ::SetLayeredWindowAttributes(hWndMain, RGB(0,0,0), 255*75/100, LWA_ALPHA);
              if (log) {
                line.Format(L"x %s Alpha 75%% set for windows [%s], process %s %u\n", CTime::GetCurrentTime().Format(L"%Y.%m.%d %H:%M:%S"), trap->getWindowContains(), p->strName, p->dwPID);
                f.WriteString(line);
              }
				    }
          }
				  break;
			  // Alpha 50
			  case 6:
          if (p->waitForInputIdle(500) == 0) {
				    hWndMain = guessWindow(p, trap->getWindowContains());
				    if (hWndMain != 0) {
					    style = ::GetWindowLong(hWndMain, GWL_EXSTYLE);
					    style |= WS_EX_LAYERED;
					    ::SetWindowLong(hWndMain, GWL_EXSTYLE, style);
					    ::SetLayeredWindowAttributes(hWndMain, RGB(0,0,0), 255/2, LWA_ALPHA);
              if (log) {
                line.Format(L"x %s Alpha 50%% set for windows [%s], process %s %u\n", CTime::GetCurrentTime().Format(L"%Y.%m.%d %H:%M:%S"), trap->getWindowContains(), p->strName, p->dwPID);
                f.WriteString(line);
              }
				    }
          }
				  break;
			  // Priority
			  case 7:
				  p->SetPriority(REALTIME_PRIORITY_CLASS);
          if (log) {
            line.Format(L"x %s Priority changed to realtime for process %s %u\n", CTime::GetCurrentTime().Format(L"%Y.%m.%d %H:%M:%S"), p->strName, p->dwPID);
            f.WriteString(line);
          }
				  break;
			  // Priority
			  case 8:
				  p->SetPriority(HIGH_PRIORITY_CLASS);
          if (log) {
            line.Format(L"x %s Priority changed to high for process %s %u\n", CTime::GetCurrentTime().Format(L"%Y.%m.%d %H:%M:%S"), p->strName, p->dwPID);
            f.WriteString(line);
          }
				  break;
			  // Priority
			  case 9:
				  p->SetPriority(NORMAL_PRIORITY_CLASS);
          if (log) {
            line.Format(L"x %s Priority changed to normal for process %s %u\n", CTime::GetCurrentTime().Format(L"%Y.%m.%d %H:%M:%S"), p->strName, p->dwPID);
            f.WriteString(line);
          }
				  break;
			  // Priority
			  case 10:
				  p->SetPriority(IDLE_PRIORITY_CLASS);
          if (log) {
            line.Format(L"x %s Priority changed to idle for process %s %u\n", CTime::GetCurrentTime().Format(L"%Y.%m.%d %H:%M:%S"), p->strName, p->dwPID);
            f.WriteString(line);
          }
				  break;
        // Affinity
			  case 12:
          if (p->waitForInputIdle(500) == 0) {
            p->SetAffinityMask(trap->geti64DWORD());
            if (log) {
              line.Format(L"x %s Affinity mask changed to %ul for process %s %u\n", CTime::GetCurrentTime().Format(L"%Y.%m.%d %H:%M:%S"), trap->geti64DWORD(), p->strName, p->dwPID);
              f.WriteString(line);
            }
          }
				  break;
        // Position and size
			  case 13:
          if (p->waitForInputIdle(500) == 0) {
				    hWndMain = guessWindow(p, trap->getWindowContains());
				    if (hWndMain != 0) {
              ULONG64 pos = trap->geti64DWORD();
              ::SetWindowPos(hWndMain, NULL, (pos >> (6*8)) & 0xFFFF, (pos >> (4*8)) & 0xFFFF, (pos >> (2*8)) & 0xFFFF, pos & 0xFFFF, SWP_NOOWNERZORDER|SWP_NOZORDER);
              if (log) {
                line.Format(L"x %s Window position and size restored for [%s], process %s %u\n", CTime::GetCurrentTime().Format(L"%Y.%m.%d %H:%M:%S"), trap->getWindowContains(), p->strName, p->dwPID);
                f.WriteString(line);
              }
				    }
          }
				  break;
			  default:
				  break;
			  }
		  }
	  }

    ReleaseMutex(hTrapsMutex);
  }

  if (log) {
    f.Close();
  }

	delete p;

	return 0;
}

LRESULT CDaphneDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	// Left double
	if (lParam == WM_LBUTTONUP) {
		SetWMode(1);
	}

	// Right
	if (lParam == WM_RBUTTONUP) {

		CMenu menu;
		POINT p;

		menu.CreatePopupMenu();

		AppendKillMenu(&menu);
		menu.AppendMenu(MF_SEPARATOR);

		if (IsWindowVisible()) 
			menu.AppendMenu(MF_STRING, WM_DRK_SHOW, _("Hide"));
		else
			menu.AppendMenu(MF_STRING, WM_DRK_SHOW, _("Show"));

    AppendBaseMenu(&menu);

		GetCursorPos(&p);
    
    SetForegroundWindow();
    menu.TrackPopupMenu(TPM_RIGHTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, p.x, p.y, this);
    PostMessage(WM_NULL, 0, 0);
  }

	return 0;
}

void CDaphneDlg::SetupTrayIcon(BOOL quit)
{
	CString title;
	title.LoadString(IDS_TITLE_VER);

	if (!quit) {

		// Lo pongo en TaskBar
		m_tnd.cbSize = sizeof(NOTIFYICONDATA);
		m_tnd.hWnd = GetSafeHwnd();
		m_tnd.uID = IDX_ICON;
		m_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		m_tnd.uCallbackMessage = WM_ICON_NOTIFY;
		StringCchCopy(m_tnd.szTip, sizeof(m_tnd.szTip), title);
		m_tnd.hIcon = m_hIcon;

		if (!trayIconOK)
			Shell_NotifyIcon(NIM_ADD, &m_tnd);
		else
			Shell_NotifyIcon(NIM_MODIFY, &m_tnd);

		trayIconOK = TRUE;
	}
	else {
		trayIconOK = FALSE;

		// Sacar del system tray
		m_tnd.cbSize = sizeof(NOTIFYICONDATA);
		m_tnd.uID = IDX_ICON;
		Shell_NotifyIcon(NIM_DELETE, &m_tnd);
	}
}

void CDaphneDlg::getUpTime() {
  
  AutoReleaseMutex mux(hUpTimeMutex);

  // Uptime
  PDH_STATUS status;
  PDH_HQUERY perfQuery = NULL;
  HCOUNTER uptimeCounter;
  
  PDH_FMT_COUNTERVALUE uptimeValue;

  if (PdhOpenQuery(NULL, 0, &perfQuery) == ERROR_SUCCESS) {

    status = PdhAddCounter(perfQuery, uptimeCounterPath, 0, &uptimeCounter);

    if (status == ERROR_SUCCESS)
      status = PdhCollectQueryData( perfQuery );
    if (status == ERROR_SUCCESS)
      status = PdhGetFormattedCounterValue(uptimeCounter, PDH_FMT_LARGE , NULL, &uptimeValue );

    PdhRemoveCounter(uptimeCounter);
    PdhCloseQuery( perfQuery );

    if (status == ERROR_SUCCESS) {
      days = (LONG) (uptimeValue.largeValue / 86400);
      uptimeValue.largeValue %= 86400;
      hours = (LONG) (uptimeValue.largeValue / 3600);
      uptimeValue.largeValue %= 3600;
      minutes = (LONG) (uptimeValue.largeValue / 60);
      uptimeValue.largeValue %= 60;
      seconds = (LONG) (uptimeValue.largeValue);
    }
  }
}
void CDaphneDlg::RefreshInfo() {

	getUpTime();

	m_strInfoTimeUp.Format(_("Uptime: %u days, %2u hours, %2u minutes, %2u seconds"), days, hours, minutes, seconds);
	m_strInfoCPUUsage.Format(_("CPU usage %3u%%"), infoCPUUsage);
	meminfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&meminfo);
	m_strInfoMEMUsage.Format(_("Memory usage %3u%%"), meminfo.dwMemoryLoad);
	m_strInfoMEMFree.Format(_("Free memory %u MB"), meminfo.ullAvailPhys/1048576);
  m_strInfoPCount.Format(_("Running processes: %4u"), m_cProcList.GetItemCount());
  m_strInfoLogin.Format(_("Login: %s\\%s"), m_computer, m_username);
  m_strInfoCPUCount.Format(_("Installed CPUs: %2u"), sysinfo.dwNumberOfProcessors);
  m_strInfoProcessor = m_processor;

	UpdateData(FALSE);
}
void CDaphneDlg::CalculateCPUUsage() {
	if (WaitForSingleObject(hRefreshMutex, 40) == WAIT_OBJECT_0) {
		__int64 used, idle;
		processes.getCPUTimes(idle, used);
		infoCPUUsage = ( (idle != 0 || used > idle)?int(100 * used / idle):100 );
		if (usageQueue.size() > DAPHNE_MAX_QUEUE_CPUDATA)
			usageQueue.pop_front();
		usageQueue.push_back(infoCPUUsage);
		ReleaseMutex(hRefreshMutex);
	}
}
void CDaphneDlg::UpdateTrayToolTip() {
	if (trayIconOK) {

		CString title, str;
		title.LoadString(IDS_TITLE_VER);

		if (hiddenWindows.size() == 0)
			str.Format(L"%s - %u%% CPU", title, infoCPUUsage);
		else if (hiddenWindows.size() == 1)
			str.Format(_("%s - %u%% CPU - %u hidden application"), title, infoCPUUsage, hiddenWindows.size());
		else
			str.Format(_("%s - %u%% CPU - %u hidden applications"), title, infoCPUUsage, hiddenWindows.size());

		m_tnd.cbSize = sizeof(NOTIFYICONDATA);
		m_tnd.hWnd = GetSafeHwnd();
		m_tnd.uID = IDX_ICON;
		m_tnd.uFlags = NIF_TIP;
		StringCchCopy(m_tnd.szTip, sizeof(m_tnd.szTip), str);

		if (m_bAnimatedIcon) {
			if (infoCPUUsage < 5)
				m_tnd.hIcon = icon[0];
			else if (infoCPUUsage >= 5 && infoCPUUsage < 10)
				m_tnd.hIcon = icon[1];
			else if (infoCPUUsage >= 10 && infoCPUUsage < 15)
				m_tnd.hIcon = icon[2];
			else if (infoCPUUsage >= 15 && infoCPUUsage < 20)
				m_tnd.hIcon = icon[3];
			else if (infoCPUUsage >= 20 && infoCPUUsage < 30)
				m_tnd.hIcon = icon[4];
			else if (infoCPUUsage >= 30 && infoCPUUsage < 40)
				m_tnd.hIcon = icon[5];
			else if (infoCPUUsage >= 40 && infoCPUUsage < 50)
				m_tnd.hIcon = icon[6];
			else if (infoCPUUsage >= 50 && infoCPUUsage < 60)
				m_tnd.hIcon = icon[7];
			else if (infoCPUUsage >= 60 && infoCPUUsage < 70)
				m_tnd.hIcon = icon[8];
			else if (infoCPUUsage >= 70 && infoCPUUsage < 80)
				m_tnd.hIcon = icon[9];
			else if (infoCPUUsage >= 90)
				m_tnd.hIcon = icon[10];

			m_tnd.uFlags |= NIF_ICON;
		}
		Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
	}
}

LRESULT CDaphneDlg::OnTaskBarCreated(WPARAM wp, LPARAM lp) {

	SetupTrayIcon();

	return 0;
}

void CDaphneDlg::SetWMode(int mode)
{
	switch(mode) {
	case 0: 
		m_bWindowHidden = TRUE;
		ShowWindow(SW_HIDE);
		m_cProcList.DeleteAllItems();
		m_cProcList.clearSearchPattern();
		break;

	case 1: 
		m_bWindowHidden = FALSE;
		ShowWindow(SW_SHOW);
		Sleep(165);
		RefreshList();
		SetForegroundWindow();
		break;
	}
}

void CDaphneDlg::Quit()
{
	UpdateData();

  KillTimer(IDT_SCHEDULER);
  ::CloseHandle(hUpTimeMutex);

	// Hooks
	if (g_bKBHookInstalled) {
		UnhookWindowsHookEx(g_hKeyboardHook);
	}
	if (g_bGMHookInstalled) {
		UnhookWindowsHookEx(g_hMessageHook);
	}

	// Remove
	SetupTrayIcon(TRUE);


	// Guardar configuración...
  CString str;
  for(int i=0; i<PL_COLUMN_COUNT; i++) {
    str.Format(L"MainColumn%uWidth", i);
    AfxGetApp()->WriteProfileInt(L"CFG", str, m_cProcList.GetColumnWidth(i));
  }
	AfxGetApp()->WriteProfileInt(L"CFG", L"MainOrderColumn", orderColumn);
	AfxGetApp()->WriteProfileInt(L"CFG", L"MainOrderMode", orderMode);
	AfxGetApp()->WriteProfileInt(L"CFG", L"MainFullPath", m_bExtPath);
	AfxGetApp()->WriteProfileInt(L"CFG", L"MainOnTop", m_bOnTop);
	AfxGetApp()->WriteProfileInt(L"CFG", L"Action", m_iAction);

	WINDOWPLACEMENT wndinfo;
	GetWindowPlacement(&wndinfo);
	AfxGetApp()->WriteProfileInt(L"CFG", L"MainX", wndinfo.rcNormalPosition.left);
	AfxGetApp()->WriteProfileInt(L"CFG", L"MainY", wndinfo.rcNormalPosition.top);
	AfxGetApp()->WriteProfileInt(L"CFG", L"MainW", wndinfo.rcNormalPosition.right);
	AfxGetApp()->WriteProfileInt(L"CFG", L"MainH", wndinfo.rcNormalPosition.bottom);
	AfxGetApp()->WriteProfileInt(L"CFG", L"MainHidden", m_bWindowHidden);

	saveTraps();

  saveSchedule();

	// Thread WebUpdated
	// Internet puede ser MUY lento...
  if (threadWebUpdates != NULL) {
  
    SetEvent(hWebUpdateEnd);
    TRACE("Signaling WebUpdate to quit.\n");

    if (WaitForSingleObject(threadWebUpdates->m_hThread, 6000) == WAIT_OBJECT_0) {
      DWORD exitcode;
	    ::GetExitCodeThread(threadWebUpdates->m_hThread, &exitcode);

      TRACE("WebUpdate finished %u\n", exitcode);
    }
    else {
		  TerminateThread(threadWebUpdates->m_hThread, 0);
  	}
	  
    threadWebUpdates = NULL;

		::CloseHandle(hWebUpdateEnd);
	}


	// Thread Refresh
	if (threadUpdate != NULL) {

    SetEvent(hRefreshEnd); // Seteo el evento de finalización

    SetEvent(hRefreshEvent); // Lanzo el refresh

    // Espero que termine
		WaitForSingleObject(hRefreshMutex, 1000);

    TRACE("Signaling ThreadUpdate to quit.\n");
    if (WaitForSingleObject(threadUpdate->m_hThread, 2500) == WAIT_OBJECT_0) {
      DWORD exitcode;
	    ::GetExitCodeThread(threadUpdate->m_hThread, &exitcode);

      TRACE("ThreadUpdate finished %u\n", exitcode);
  		threadUpdate = NULL;
    }
		ReleaseMutex(hRefreshMutex);
    ::CloseHandle(hRefreshEnd);
	}

	// Libero las ventanas que puedan estar ocultas
  if (WaitForSingleObject(hHiddenMutex, INFINITE) == WAIT_OBJECT_0) {
	  BOOL show = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneAdvShowHidden", 0);
	  MapWindows::iterator i;
	  for(i = hiddenWindows.begin(); i != hiddenWindows.end(); i++) {
		  CWnd * p = i->second;
		  if (show)
			  p->ShowWindow(TRUE);
		  p->Detach();
		  delete p;
	  }
	  hiddenWindows.clear();
    ReleaseMutex(hHiddenMutex);
  }

	TrapList::iterator j;
	for(j = traps.begin(); j != traps.end(); j++) {
		CTrap * p = (CTrap*)(*j);
		delete p;
	}
	traps.clear();

  if (AfxGetApp()->GetProfileInt(L"CFG", L"UseMultidesktop", 0) == 1) {
	  UnregisterHotKey(GetSafeHwnd(), GlobalAddAtom(L"LHF_DSK_0"));
	  int maxdesktops = AfxGetApp()->GetProfileInt(L"CFG", L"MDExtraDesktops", 1);
	  for(int i=1; i<=maxdesktops; i++) {
		  CString id;
		  id.Format(L"LHF_DSK_%u", i);
		  UnregisterHotKey(GetSafeHwnd(), GlobalAddAtom(id));
	  }
	  if (mdStarted) {
		  for(int i=1; i<=maxdesktops; i++) {
        //CloseWindows(hDesktops[i], desktopProcessInfo[i].dwProcessId);
			  CloseDesktop(hDesktops[i]);
        //TerminateProcess(desktopProcessInfo[i].hProcess, 0);
		  }
    }
	}

	EndModalLoop(0);
}

void CDaphneDlg::CloseWindows(HDESK desktop, DWORD pid) {
  TRACE("Closing explorer PID %u\n", pid);
  ::EnumDesktopWindows(desktop, EnumWindowsDesktop, pid);
}

BOOL CALLBACK CDaphneDlg::EnumWindowsDesktop(HWND hwnd, LPARAM lParam) {
		// Close
		CWnd w;
		CString title;
    title.ReleaseBuffer(::GetWindowText(hwnd, title.GetBufferSetLength(512), 512));
    if (title.Compare(L"Program manager") == 0) { // (GetWindowLong(hwnd ,GWL_STYLE) & WS_VISIBLE)
      TRACE("Found Program manager %08Xh\n", hwnd);
      DWORD pidwin;
      GetWindowThreadProcessId(hwnd, &pidwin);
      if (pidwin == lParam) {
        TRACE("Posted WM_ENDSESSION to %08Xh\n", hwnd);
			  ::PostMessage(hwnd, WM_ENDSESSION, 1, ENDSESSION_LOGOFF);
      }
    }
		return TRUE;
}

void CDaphneDlg::OnSettings() 
{
	CPPKill page_kill;
	CPPLook page_look;
	CPPStartup page_start;
	CPPMultidesktop page_md;
	CPPAdv page_adv;
	CPPTraps page_traps;
	CPPHighlight page_hl;
  CPPLogging page_logging;

	CPropertySheet sheet;

	sheet.Construct(_("Settings..."), this, 0);
  sheet.m_psh.dwFlags &= ~PSP_HASHELP;

	page_start.pDaphne = this;
	page_start.m_psp.pszTitle = _wcsdup(_("Main"));
	page_start.m_psp.dwFlags |= PSP_USETITLE;
  page_start.m_psp.dwFlags &= ~PSP_HASHELP;
	sheet.AddPage(&page_start);

	page_look.pProcList = &m_cProcList;
	page_look.m_psp.pszTitle = _wcsdup(_("Look and feel"));
	page_look.m_psp.dwFlags |= PSP_USETITLE;
  page_look.m_psp.dwFlags &= ~PSP_HASHELP;
	sheet.AddPage(&page_look);

	page_kill.pProcList = &m_cProcList;
	page_kill.m_psp.pszTitle = _wcsdup(_("Kill menu"));
	page_kill.m_psp.dwFlags |= PSP_USETITLE;
  page_kill.m_psp.dwFlags &= ~PSP_HASHELP;
	sheet.AddPage(&page_kill);

	page_traps.traps = &traps;
	page_traps.m_psp.pszTitle = _wcsdup(_("Traps"));
	page_traps.m_psp.dwFlags |= PSP_USETITLE;
  page_traps.m_psp.dwFlags &= ~PSP_HASHELP;
	sheet.AddPage(&page_traps);

	page_hl.pProcList = &m_cProcList;
	page_hl.m_psp.pszTitle = _wcsdup(_("Highlights"));
	page_hl.m_psp.dwFlags |= PSP_USETITLE;
  page_hl.m_psp.dwFlags &= ~PSP_HASHELP;
	sheet.AddPage(&page_hl);

	page_md.m_psp.pszTitle = _wcsdup(_("More..."));
	page_md.m_psp.dwFlags |= PSP_USETITLE;
  page_md.m_psp.dwFlags &= ~PSP_HASHELP;
	sheet.AddPage(&page_md);

  page_logging.m_psp.pszTitle = _wcsdup(_("Logging"));
	page_logging.m_psp.dwFlags |= PSP_USETITLE;
  page_logging.m_psp.dwFlags &= ~PSP_HASHELP;
	sheet.AddPage(&page_logging);

	page_adv.m_psp.pszTitle = _wcsdup(_("Advanced"));
	page_adv.m_psp.dwFlags |= PSP_USETITLE;
  page_adv.m_psp.dwFlags &= ~PSP_HASHELP;
	sheet.AddPage(&page_adv);

	// Si presionó ok, refresco las opciones que son on-line
	if (sheet.DoModal() == IDOK) {

    loadRunSlots();

		// Actualizo el short cut
		g_bKBShortCut = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneSC", 1);
		g_scVK = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneSCVirtualKey", VK_F12);
		g_scNeedsAlt = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneSCAlt", 1);
		g_scNeedsShift = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneSCShift", 1);
		g_scNeedsCtrl = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneSCCtrl", 0);

    bProcessLogging = AfxGetApp()->GetProfileInt(L"LOG", L"LogProcess", 0);
    strProcessLoggingFile = AfxGetApp()->GetProfileString(L"LOG", L"LogProcessFile", L"");

		m_bUpdates = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneCheckUpdates", 1);
		highlights = AfxGetApp()->GetProfileString(L"CFG", L"Highlights", L"");
		highlights_on = !highlights.IsEmpty();
		m_bHLSys = AfxGetApp()->GetProfileInt(L"CFG", L"HighlightSystem", 1);
		m_bHLTop = AfxGetApp()->GetProfileInt(L"CFG", L"HighlightTop", 1);
		m_bJavaHide = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneJavaVMHide", 0);
    m_bHideZombies = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneHideZombies", 0);
		if (m_bJavaHide) {
			WCHAR strJavaHome[_MAX_PATH];
			if (GetEnvironmentVariable(L"JAVA_HOME", strJavaHome, sizeof(strJavaHome))) {
				processes.setJavaHome(strJavaHome);
			}
		}
		else {
			processes.setJavaHome(L"");
		}
    m_bAnimatedIcon = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneAnimatedIcon", 1);
    if (!m_bAnimatedIcon)
      SetupTrayIcon();

		SetRefreshTimer();

		ResetWebUpdates();
	}

	// Esto es cosmética
	m_cProcList.SetFocus();

  free((void*)page_start.m_psp.pszTitle);
  free((void*)page_look.m_psp.pszTitle);
  free((void*)page_kill.m_psp.pszTitle);
  free((void*)page_traps.m_psp.pszTitle);
  free((void*)page_hl.m_psp.pszTitle);
  free((void*)page_md.m_psp.pszTitle);
  free((void*)page_adv.m_psp.pszTitle);
  free((void*)page_logging.m_psp.pszTitle);
}

void CDaphneDlg::OnMenub() 
{
	CMenu menu;
	POINT p;

	menu.CreatePopupMenu();

	AppendKillMenu(&menu);
  menu.AppendMenu(MF_SEPARATOR);
  AppendBaseMenu(&menu);

	menu.SetDefaultItem(0, TRUE);

	GetCursorPos(&p);
	menu.TrackPopupMenu(TPM_LEFTBUTTON | TPM_RIGHTBUTTON, p.x, p.y, this, NULL);

	// Esto es cosmética
	m_cProcList.SetFocus();
}

void CDaphneDlg::AppendKillMenu(CMenu *menu)
{
	CMenu apps;

	menu->AppendMenu(MF_STRING, WM_K_BYNAME, _("Kill all by name"));
	menu->AppendMenu(MF_STRING, WM_K_BYNAMEP, _("Kill all by name (Polite)"));
	menu->AppendMenu(MF_STRING, WM_K_RUNP, _("Run process..."));
	menu->AppendMenu(MF_STRING, WM_K_RUNPAS, _("Run process as ..."));
	menu->AppendMenu(MF_STRING, WM_K_WNDCLOSE, _("Close windows by name"));
	menu->AppendMenu(MF_STRING, WM_K_SCHEDULE_MSG, _("Schedule popup message"));
	menu->AppendMenu(MF_STRING, WM_K_SCHEDULE_OFF, _("Schedule system shutdown"));
	menu->AppendMenu(MF_BYPOSITION, MF_SEPARATOR);
	menu->AppendMenu(MF_STRING, WM_K_CPUUSAGE, _("Show CPU usage ..."));
	menu->AppendMenu(MF_STRING, WM_K_PTREE, _("Show process tree..."));
	menu->AppendMenu(MF_STRING, WM_K_ISOFT, _("Show installed software..."));
	menu->AppendMenu(MF_BYPOSITION, MF_SEPARATOR);
	menu->AppendMenu(MF_STRING, WM_K_INSPECTOR, _("Control inspector..."));
	menu->AppendMenu(MF_STRING, IDM_TASKS, _("Hidden applications..."));
	menu->AppendMenu(MF_STRING, WM_K_SCHEDULED, _("Scheduled tasks..."));
	menu->AppendMenu(MF_BYPOSITION, MF_SEPARATOR);
	menu->AppendMenu(MF_STRING, WM_K_COPYLIST, _("Copy process list to clipboard"));
	menu->AppendMenu(MF_STRING, WM_K_GOD, _("Windows 7 God Mode..."));
	menu->AppendMenu(MF_STRING, WM_K_PSR, _("Windows 7 Problem steps recorder..."));

  
  if (WaitForSingleObject(hHiddenMutex, INFINITE) == WAIT_OBJECT_0) {
	  if (hiddenWindows.size() > 0) {
		  apps.CreatePopupMenu();
		  CString str;
		  int c = 0;
		  MapWindows::iterator i;
		  for(i = hiddenWindows.begin(); i != hiddenWindows.end(); i++) {
			  CWnd * p = i->second;
		
			  // Verifico si sigue viva la aplicación
			  if (GetWindowThreadProcessId(p->GetSafeHwnd(), NULL) != 0) {
				  p->GetWindowText(str);
				  int index = apps.AppendMenu(MF_STRING, WM_DRK_UNHIDE+c, str);
				  ++c;
			  }
		  }
	
		  menu->AppendMenu(MF_POPUP, (UINT_PTR)apps.GetSafeHmenu(), _("Show application"));
	  }
    ReleaseMutex(hHiddenMutex);
  }
	// Killers
	int items = AfxGetApp()->GetProfileInt(L"CFG", L"KillMenuItems", 0);
	if (items > 0)
		menu->AppendMenu(MF_SEPARATOR);

	for(int i = 0; i < items; i++) {

		CKiller * p = GetKiller(i);

		menu->AppendMenu(MF_STRING, WM_K_KILLER+i, p->strName);

		delete p;

		// Limite...
		if (i > 50)
			break;
	}
}

void CDaphneDlg::AppendBaseMenu(CMenu *menu)
{
  CMenu online_menu;

	menu->AppendMenu(MF_STRING, WM_DRK_ABOUT, _("About Daphne"));

	online_menu.CreatePopupMenu();
  online_menu.AppendMenu(MF_STRING, WM_DRK_DAPHNESUPPORT, _("Support..."));
  online_menu.AppendMenu(MF_STRING, WM_DRK_DAPHNEFAQ, _("FAQ..."));
  online_menu.AppendMenu(MF_STRING, WM_DRK_DAPHNESTAT, _("Daphne statistics..."));
	online_menu.AppendMenu(MF_STRING, WM_DRK_GOOGLE, _("Search drk.com.ar..."));
  menu->InsertMenu(-1, MF_POPUP, (UINT)online_menu.m_hMenu, _("Help"));

	menu->AppendMenu(MF_SEPARATOR);
	menu->AppendMenu(MF_STRING, WM_DRK_QUIT, _("Exit"));

	menu->SetDefaultItem(WM_DRK_SHOW);
}

int CDaphneDlg::SwitchToDesktop(INT number)
{
	DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i, deaths = 0;
	DWORD dwPID;

	// Solicito la lista de procesos al sistema
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        return 1; 

    cProcesses = cbNeeded / sizeof(DWORD);

    for (i=0; i<cProcesses; i++) {

		dwPID = aProcesses[i];

		CProcess * proc = new CProcess(dwPID);

		if (!proc->bService) {
			proc->ReadWindowInformation();

			ListHWND::iterator j;
			for(j = proc->windows.begin(); j != proc->windows.end(); j++) {
				CWnd * pWindow = CWnd::FromHandle((HWND)(*j));
				if (pWindow->IsWindowVisible()) {
					CString title;
					pWindow->GetWindowText(title);
				}
			}
		}
		delete proc;
	}

	return deaths;
}

void CDaphneDlg::DesktopMapInit()
{
	mdStarted = FALSE;
	currentDesktop = 0;
  memset(desktopProcessInfo, 0, sizeof(desktopProcessInfo));
	hDesktops[0] = NULL;
	hDesktops[1] = NULL;
	hDesktops[2] = NULL;
	hDesktops[3] = NULL;

	if (AfxGetApp()->GetProfileInt(L"CFG", L"UseMultidesktop", 0)) {

		CSplashMDInit dlg;
		dlg.Create(IDD_SPLASH_DESKTOP);
		dlg.ShowWindow(SW_SHOW);
		dlg.SetMessage( _("Connecting primary desktop...") );
    
		hDesktops[0] = OpenInputDesktop(0, FALSE, DESKTOP_SWITCHDESKTOP);

		int maxdesktops = AfxGetApp()->GetProfileInt(L"CFG", L"MDExtraDesktops", 1);
		for(int i=1; i<=maxdesktops; i++) {
			CString name, txt;
			name.Format(L"dsdsk%u", i);
			
			txt.Format( _("Looking up desktop %u..."), i);
			dlg.SetMessage( txt );

      hDesktops[i] = OpenDesktop(name, 0, FALSE, DESKTOP_SWITCHDESKTOP);

      if (hDesktops[i] == NULL) {
			  txt.Format( _("Creating desktop %u..."), i);
			  dlg.SetMessage( txt );

			  hDesktops[i] = CreateDesktop(name, NULL, NULL, 0, GENERIC_ALL, NULL);
			  desktopProcessInfo[i] = OpenProc(L"explorer.exe", (LPCWSTR)name); 
      }
		}

		dlg.ShowWindow(SW_HIDE);
    dlg.DestroyWindow();

		mdStarted = TRUE;
	}
}

int CDaphneDlg::CloseByName(CString wnd_name, BOOL case_sensitive)
{
	DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i, deaths = 0;
	DWORD dwPID;

	// Solicito la lista de procesos al sistema
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        return 1; 

    cProcesses = cbNeeded / sizeof(DWORD);

    for (i=0; i<cProcesses; i++) {

		dwPID = aProcesses[i];

		CProcess * proc = new CProcess(dwPID);

		if (!proc->bService) {
			proc->ReadWindowInformation();

			ListHWND::iterator j;
			for(j = proc->windows.begin(); j != proc->windows.end(); j++) {
				CWnd * pWindow = CWnd::FromHandle((HWND)(*j));
				if (pWindow->IsWindowVisible()) {
					CString title;
					pWindow->GetWindowText(title);
					if (case_sensitive && title.Find(wnd_name) != -1) {
						::PostMessage(pWindow->GetSafeHwnd(), WM_CLOSE, 0, 0);
						deaths++;
					}
					if (!case_sensitive && title.MakeLower().Find(wnd_name.MakeLower()) != -1) {
						::PostMessage(pWindow->GetSafeHwnd(), WM_CLOSE, 0, 0);
						deaths++;
					}
				}
			}
		}
		delete proc;
	}

	return deaths;
}

CKiller * CDaphneDlg::GetKiller(int index)
{
	CString str;
	CKiller * p = new CKiller();

	str.Format(L"Menu_%u", index);
	p->strName = AfxGetApp()->GetProfileString(L"KMI", str, L"");
	str.Format(L"PList_%u", index);
	p->strProcesses = AfxGetApp()->GetProfileString(L"KMI", str, L"");

	return p;
}

CProcess * CDaphneDlg::waitForProcess(DWORD pid)
{
	CProcess * proc = NULL;

	if (WaitForSingleObject(hRefreshMutex, 1500) == WAIT_OBJECT_0) {
		proc = processes.getProcess(pid);
		ReleaseMutex(hRefreshMutex);
	}

	return proc;
}

void CDaphneDlg::OnAontop() 
{
	UpdateData(TRUE);

	if (m_bOnTop)
		SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	else
		SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
}

void CDaphneDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	PostMessage(WM_COMMAND, MAKELPARAM(IDM_PROPERTIES ,0), 0);
	
	*pResult = 0;
}

void CDaphneDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	HWND hWnd;
	DWORD dwPID;
	
	if (bKillTarget) {

		ClientToScreen(&point);
		hWnd = ::WindowFromPoint(point);

		if (hLastSelectedWnd != hWnd) {
			hLastSelectedWnd = hWnd;
			if (GetWindowThreadProcessId(hWnd, &dwPID))
				selectProcess(dwPID);
		}
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

// Drag-and-drop
void CDaphneDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	LONG style;
	HWND hWnd;
	HWND hCtrlWnd;
	DWORD dwPID;
	
	UpdateData();

	if (bKillTarget) {

		ClientToScreen(&point);

		hWnd = ::WindowFromPoint(point);


		if (GetWindowThreadProcessId(hWnd, &dwPID)) {

			if (dwPID != GetCurrentProcessId()) {

				CProcess * p = new CProcess(dwPID, NULL);

				switch(m_iAction) {
				case 0:
					selectProcess(dwPID);
					break;
				case 1:
          {
            CProcess * proc;
            CPropertiesDlg * pDlg;
					  selectProcess(dwPID);
		        if (proc = waitForProcess(GetSelectedPID())) {
			        pDlg = new CPropertiesDlg(proc);
			        if (pDlg) {
                pDlg->hPreselectWnd = hWnd;
				        pDlg->DoModal();
				        delete pDlg;
			        }
		        }
          }
					break;
				case 2:
					p->Kill();
          // Statistics
          CDaphneApp::hitStatisticalRecord(L"DropKill");
					break;
				case 3:
					hideWindow(hWnd);
          // Statistics
          CDaphneApp::hitStatisticalRecord(L"DropHide");
					break;
				case 4:
					::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
          // Statistics
          CDaphneApp::hitStatisticalRecord(L"DropOnTop");
					break;
				case 5:
					::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
          // Statistics
          CDaphneApp::hitStatisticalRecord(L"DropOnTop");
					break;
				case 6:
					style = ::GetWindowLong(hWnd, GWL_EXSTYLE);
					style |= WS_EX_LAYERED;
					::SetWindowLong(hWnd, GWL_EXSTYLE, style);
					::SetLayeredWindowAttributes(hWnd, RGB(0,0,0), 255, LWA_ALPHA);
          // Statistics
          CDaphneApp::hitStatisticalRecord(L"DropAlpha");
					break;
				case 7:
					style = ::GetWindowLong(hWnd, GWL_EXSTYLE);
					style |= WS_EX_LAYERED;
					::SetWindowLong(hWnd, GWL_EXSTYLE, style);
					::SetLayeredWindowAttributes(hWnd, RGB(0,0,0), 255*75/100, LWA_ALPHA);
          // Statistics
          CDaphneApp::hitStatisticalRecord(L"DropAlpha");
					break;
				case 8:
					style = ::GetWindowLong(hWnd, GWL_EXSTYLE);
					style |= WS_EX_LAYERED;
					::SetWindowLong(hWnd, GWL_EXSTYLE, style);
					::SetLayeredWindowAttributes(hWnd, RGB(0,0,0), 255/2, LWA_ALPHA);
          // Statistics
          CDaphneApp::hitStatisticalRecord(L"DropAlpha");
					break;
				case 9:
					::SetLayeredWindowAttributes(hWnd, RGB(0,0,0), 255, LWA_ALPHA);
					style = ::GetWindowLong(hWnd, GWL_EXSTYLE);
					style &= ~WS_EX_LAYERED;
					::SetWindowLong(hWnd, GWL_EXSTYLE, style);
          // Statistics
          CDaphneApp::hitStatisticalRecord(L"DropAlpha");
					break;
				case 10:
					::ScreenToClient(hWnd, &point);
					hCtrlWnd = ::ChildWindowFromPointEx(hWnd, point, CWP_SKIPINVISIBLE | CWP_SKIPTRANSPARENT);

					if ((hCtrlWnd) && (hCtrlWnd != hWnd))
						::EnableWindow(hCtrlWnd, TRUE);
					else
						::EnableWindow(hWnd, TRUE);
					break;
				case 11:
					::EnableWindow(hWnd, FALSE);
					break;
        case 12:
          {
            // Change window size
            CDaphneApp::hitStatisticalRecord(L"SetWindowSize");
            WINDOWPLACEMENT place;
            place.length = sizeof(WINDOWPLACEMENT);
            ::GetWindowPlacement(hWnd, &place);
            CWindowSizeDlg dlg;
            dlg.m_strSize.Format(L"%ux%u", place.rcNormalPosition.right-place.rcNormalPosition.left, place.rcNormalPosition.bottom-place.rcNormalPosition.top);
            if (dlg.DoModal() == IDOK) {
              int x = dlg.m_strSize.Find('x');
              if (x == -1 || x == 0 || x == dlg.m_strSize.GetLength()-1)
                MessageBox(_("You have to use the WIDTHxHEIGHT format. For example 640x480."), _("Invalid size format"), MB_OK | MB_ICONERROR);
              else {
                // Set size
                LONG width = _wtoi(dlg.m_strSize.Left(x));
                LONG height = _wtoi(dlg.m_strSize.Mid(x+1));
                place.rcNormalPosition.right = place.rcNormalPosition.left + width;
                place.rcNormalPosition.bottom = place.rcNormalPosition.top + height;
                ::SetWindowPlacement(hWnd, &place);
              }
            }
          }
					break;
        case 13:
          {
            // Trap size and position
            CDaphneApp::hitStatisticalRecord(L"TrapWindowSizeAndPosition");
            WINDOWPLACEMENT place;
            ULONG64 sizepos;
            place.length = sizeof(WINDOWPLACEMENT);
            ::GetWindowPlacement(hWnd, &place);
            sizepos = ((ULONG64)place.rcNormalPosition.left) << (6*8) 
                    | ((ULONG64)place.rcNormalPosition.top) << (4*8)
                    | (place.rcNormalPosition.right - place.rcNormalPosition.left) << (2*8)
                    | (place.rcNormalPosition.bottom - place.rcNormalPosition.top);
            // Search trap
            bool found = FALSE;
            if (WaitForSingleObject(hTrapsMutex, INFINITE) == WAIT_OBJECT_0) {

              TrapList::iterator i;
  	          for (i = traps.begin(); i != traps.end(); i++) {
	  	          CTrap * trap = (CTrap *)(*i);
                if (trap->getType() == 13 && p->strName == trap->getProcessName()) {
                  found = TRUE;
                  // Modify
                  trap->seti64DWORD(sizepos);
                }
              }
              if (!found) {
                // Add
                CTrap * trap = new CTrap(p->strName, 13, L"", sizepos);
                traps.push_back(trap);
              }

              ReleaseMutex(hTrapsMutex);
            }
          }
					break;
				}

				delete p;
			}
		}

		ReleaseCapture();
		SetCursor(m_hNormal);
		m_cKill.SetIcon(m_hTarget);

		bKillTarget = FALSE;
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CDaphneDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if (bKillTarget) {
		// Cancelar kill target
		ReleaseCapture();
		SetCursor(m_hNormal);
		m_cKill.SetIcon(m_hTarget);
		bKillTarget = FALSE;
	}
	
	CDialog::OnRButtonUp(nFlags, point);
}

void CDaphneDlg::OnCKill() 
{
	bKillTarget = TRUE;
	m_hNormal = SetCursor(m_hTarget); 
	SetCapture();
	m_cKill.SetIcon(NULL);
}

BOOL CDaphneDlg::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult ) {

	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	UINT_PTR nID = pNMHDR->idFrom;

	if (pTTT->uFlags & TTF_IDISHWND) {
		nID = ::GetDlgCtrlID((HWND)nID);
		if(nID)	{
      pTTT->lpszText = toolTipBuffer;
			switch(nID) {
			case IDC_INF_FMEM:
        __("Amount of physical memory left before swapping occurs.", toolTipBuffer, DAPHNE_MAX_TOOLTIP_SIZE);
				break;
      case IDC_INF_MEM:
        __("Memory used so far in percent.", toolTipBuffer, DAPHNE_MAX_TOOLTIP_SIZE);
        break;
      case IDC_INF_CPU:
        __("CPU time requested in percent.", toolTipBuffer, DAPHNE_MAX_TOOLTIP_SIZE);
        break;
      case IDC_INF_CPUC:
        __("Time the system has been up and running.", toolTipBuffer, DAPHNE_MAX_TOOLTIP_SIZE);
        break;
			case IDC_C_KILL:
        __("Drag this target over a window to execute the action selected in the combo box.", toolTipBuffer, DAPHNE_MAX_TOOLTIP_SIZE);
				break;
			case IDC_TACTION:
        __("Selects the action for the drag and drop target tool.", toolTipBuffer, DAPHNE_MAX_TOOLTIP_SIZE);
				break;
			case IDC_FULLPATHEX:
        __("Displays full path and command line arguments if possible.", toolTipBuffer, DAPHNE_MAX_TOOLTIP_SIZE);
				break;
			case IDC_AONTOP:
        __("Keeps Daphne main window always on top.", toolTipBuffer, DAPHNE_MAX_TOOLTIP_SIZE);
				break;
			case IDC_MENUB:
        __("Opens Daphne main menu.", toolTipBuffer, DAPHNE_MAX_TOOLTIP_SIZE);
				break;
			case IDC_LIST:
        __("Running process list.", toolTipBuffer, DAPHNE_MAX_TOOLTIP_SIZE);
				break;
			case IDC_SETTINGS:
        __("Open configuration screen.", toolTipBuffer, DAPHNE_MAX_TOOLTIP_SIZE);
				break;
			}
			return TRUE;
		}
	}

	return FALSE;
}

void CDaphneDlg::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	if (!(lpwndpos->flags & SWP_NOSIZE))
		ResizeWindow(lpwndpos);

	CDialog::OnWindowPosChanged(lpwndpos);
}

void CDaphneDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	if (m_bWindowHidden)
		lpwndpos->flags &= ~SWP_SHOWWINDOW;

	////////////////////////////////
	// Implemento el resize

	// No cambiar el tamaño si es menor que min_size en X
	if ((!(lpwndpos->flags & SWP_NOSIZE)) && (lpwndpos->cx < min_size.right)) {
		lpwndpos->cx = min_size.right;
	}
	// No cambiar el tamaño si es menor que min_size/2 en X
	if ((!(lpwndpos->flags & SWP_NOSIZE)) && (lpwndpos->cy < (min_size.bottom / 2))) {
		lpwndpos->cy = min_size.bottom / 2;
	}

	CDialog::OnWindowPosChanging(lpwndpos);
}

void CDaphneDlg::LoadTargetCursor()
{
	switch(m_iAction) {
	case 0:
		m_hTarget = AfxGetApp()->LoadCursor(IDC_T_SELECT);
		break;
	case 1:
		m_hTarget = AfxGetApp()->LoadCursor(IDC_T_SELECT);
		break;
	case 2:
		m_hTarget = AfxGetApp()->LoadCursor(IDC_REDKILLCURSOR);
		break;
	case 3:
		m_hTarget = AfxGetApp()->LoadCursor(IDC_T_HIDE);
		break;
	case 4:
		m_hTarget = AfxGetApp()->LoadCursor(IDC_T_ONTOP);
		break;
	case 5:
		m_hTarget = AfxGetApp()->LoadCursor(IDC_T_ONTOPOFF);
		break;
	case 6:
		m_hTarget = AfxGetApp()->LoadCursor(IDC_KILLCURSOR);
		break;
  case 10:
		m_hTarget = AfxGetApp()->LoadCursor(IDC_T_ENABLE);
		break;
	case 11:
		m_hTarget = AfxGetApp()->LoadCursor(IDC_T_DISABLE);
		break;
	case 12:
	case 13:
		m_hTarget = AfxGetApp()->LoadCursor(IDC_T_SIZE);
		break;
	default:
		m_hTarget = AfxGetApp()->LoadCursor(IDC_KILLCURSOR);
		break;
	}

	m_cKill.SetIcon(m_hTarget);
}

void CDaphneDlg::OnSelchangeTaction() 
{
	UpdateData();
	LoadTargetCursor();
}

void CDaphneDlg::Copy(int mode)
{
	CString str;
  CStringA tmp;
  INT bytes, bsize;
	HGLOBAL hData = NULL;
	CProcess * proc = waitForProcess(GetSelectedPID());

	if (!OpenClipboard())
		return;

	if (!EmptyClipboard())
		return;


	switch(mode) {
	case 1:
    if (hData = GlobalAlloc(GMEM_MOVEABLE & GMEM_DDESHARE, proc->strFullPath.GetLength()+1)) {
      bsize = proc->strFullPath.GetLength()+1;
      bytes = WideCharToMultiByte(CP_OEMCP, 0, (LPCWSTR)proc->strFullPath, proc->strFullPath.GetLength(), tmp.GetBufferSetLength(bsize), bsize, NULL, NULL);
      tmp.ReleaseBuffer(bytes);
      CopyMemory(hData, (LPCSTR)tmp.GetBuffer(), bytes+1);
    }
		break;
	case 2:
    if (hData = GlobalAlloc(GMEM_MOVEABLE & GMEM_DDESHARE, proc->strFullPathEx.GetLength()+1)) {
      bsize = proc->strFullPathEx.GetLength()+1;
      bytes = WideCharToMultiByte(CP_OEMCP, 0, (LPCWSTR)proc->strFullPathEx, proc->strFullPathEx.GetLength(), tmp.GetBufferSetLength(bsize), bsize, NULL, NULL);
      tmp.ReleaseBuffer(bytes);
      CopyMemory(hData, (LPCSTR)tmp.GetBuffer(), bytes+1);
    }
		break;
	case 3:
		str.Format(L"%u", proc->dwPID);
    if (hData = GlobalAlloc(GMEM_MOVEABLE & GMEM_DDESHARE, str.GetLength()+1)) {
      bsize = str.GetLength()+1;
      bytes = WideCharToMultiByte(CP_OEMCP, 0, (LPCWSTR)str, str.GetLength(), tmp.GetBufferSetLength(bsize), bsize, NULL, NULL);
      tmp.ReleaseBuffer(bytes);
      CopyMemory(hData, (LPCSTR)tmp.GetBuffer(), bytes+1);
    }
		break;
	case 4: // MD5
    {
      TCHAR hash[DAPHNE_MD5LEN+1];
      DWORD bytes = sizeof(hash);
      getHashFor(CALG_MD5, proc->strFullPath, hash, &bytes);
      hash[bytes] = 0;
      USES_CONVERSION;
      PCSTR ansi = T2CA(hash);
      if (hData = GlobalAlloc(GMEM_MOVEABLE & GMEM_DDESHARE, strlen(ansi)+1)) {
        CopyMemory(hData, ansi, strlen(ansi)+1);
      }
    }
		break;
	case 5: // SHA1
    {
      TCHAR hash[DAPHNE_SHA1LEN+1];
      DWORD bytes = sizeof(hash);
      getHashFor(CALG_SHA1, proc->strFullPath, hash, &bytes);
      hash[bytes] = 0;
      USES_CONVERSION;
      PCSTR ansi = T2CA(hash);
      if (hData = GlobalAlloc(GMEM_MOVEABLE & GMEM_DDESHARE, strlen(ansi)+1)) {
        CopyMemory(hData, ansi, strlen(ansi)+1);
      }
    }
		break;
	default:
    if (hData = GlobalAlloc(GMEM_MOVEABLE & GMEM_DDESHARE, proc->strName.GetLength()+1)) {
      bsize = proc->strName.GetLength()+1;
      bytes = WideCharToMultiByte(CP_OEMCP, 0, (LPCWSTR)proc->strName, proc->strName.GetLength(), tmp.GetBufferSetLength(bsize), bsize, NULL, NULL);
      tmp.ReleaseBuffer(bytes);
      CopyMemory(hData, (LPCSTR)tmp.GetBuffer(), bytes+1);
    }
		break;
	}
	
	if (hData) 
		::SetClipboardData(CF_TEXT, hData);

	CloseClipboard();

  // Statistics
  CDaphneApp::hitStatisticalRecord(L"QuickClipboardCopy");
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//Language
	SetWindowText(_(" About Daphne"));
	SetDlgItemText(IDCANCEL, _("Close"));
	SetDlgItemText(IDC_LICENSE, _("License"));
	SetDlgItemText(IDC_STG_CONTRIB, _("Contributed"));
	SetDlgItemText(IDC_SUPPORT, _("Technical support at www.drk.com.ar"));
	SetDlgItemText(IDC_CONTRIBUTE, _("Contribute"));

  m_Font.CreatePointFont(90, _T("Courier new"));
  GetDlgItem(IDC_CREDITS)->SetFont(&m_Font);

  SetDlgItemText(IDC_CREDITS, getCreditsFromWeb());

	m_strTitle.LoadString(IDS_TITLE_VER);
#ifdef _M_X64
	m_strTitle += L" [x86-64]";
#endif

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnWeb() 
{
	CString str;
	str.LoadString(IDS_WEBURL);
	::ShellExecute(NULL, L"OPEN", str, NULL, NULL, SW_SHOWNORMAL);
}

void CAboutDlg::OnLicense() 
{
	CString str;
	str.LoadString(IDS_LICENSE_URL);
	::ShellExecute(NULL, L"OPEN", str, NULL, NULL, SW_SHOWNORMAL);
}

void CDaphneDlg::hideWindow(HWND hWnd)
{
	CWnd * wnd = NULL;
	MapWindows::iterator i;
	DWORD dwFlags = 0;

  if (WaitForSingleObject(hHiddenMutex, INFINITE) == WAIT_OBJECT_0) {

	  ::ShowWindow(hWnd, SW_HIDE);
	  dwFlags = ::GetWindowLong(hWnd, GWL_EXSTYLE);
	  dwFlags &= ~WS_EX_APPWINDOW;
	  ::SetWindowLong(hWnd, GWL_EXSTYLE, dwFlags);

	  wnd = new CWnd();
	  wnd->Attach(hWnd);
	  hiddenWindows.insert(MapWindows::value_type(hWnd, wnd));

	  UpdateTrayToolTip();

	  ShowBalloon(_("Window hidden"));

    ReleaseMutex(hHiddenMutex);
  }
}

void CDaphneDlg::showWindow(HWND hWnd)
{
	CWnd * wnd = NULL;
	MapWindows::iterator i;
	DWORD dwFlags = 0;

  if (WaitForSingleObject(hHiddenMutex, INFINITE) == WAIT_OBJECT_0) {
	  i = hiddenWindows.find(hWnd);
	  if (i != hiddenWindows.end()) {
		  wnd = i->second;
		  wnd->Detach();
		  delete wnd;
		  hiddenWindows.erase(i);
	  }
	  dwFlags = ::GetWindowLong(hWnd, GWL_EXSTYLE);
	  dwFlags |= WS_EX_APPWINDOW;
	  ::SetWindowLong(hWnd, GWL_EXSTYLE, dwFlags);
	  //taskBar(hWnd, 0); // AddTab
	  ::ShowWindow(hWnd, SW_SHOW);

	  UpdateTrayToolTip();
    ReleaseMutex(hHiddenMutex);
  }
}

void CDaphneDlg::selectProcess(CString name) {

	selectProcess(processes.Find(name));
}

void CDaphneDlg::selectProcess(DWORD pid)
{
	LVFINDINFO find;
	int index;

	// Busco el item
	find.flags = LVFI_PARAM | LVFI_WRAP;
	find.lParam = pid;
	index = m_cProcList.FindItem(&find);

	if (index != -1) {
		m_cProcList.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
		m_cProcList.EnsureVisible(index, FALSE);
	}
}

// Parece thread-safe
void CDaphneDlg::ShowBalloon(CString text, UINT timeout)
{
	m_tnd.cbSize = sizeof(NOTIFYICONDATA);
	m_tnd.hWnd = GetSafeHwnd();
	m_tnd.uFlags = NIF_INFO;

	StringCchCopy(m_tnd.szInfoTitle, sizeof(m_tnd.szInfoTitle), _("Daphne"));
	StringCchCopy(m_tnd.szInfo, sizeof(m_tnd.szInfo), (LPCWSTR)text);

	m_tnd.dwInfoFlags = NIIF_INFO;
	
	m_tnd.uTimeout = timeout; // in milliseconds

	Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}

// Esta función se lanza en un thread aparte
UINT CDaphneDlg::staticWebUpdates(LPVOID pParam) {

	CMBWebAgreeDlg agreedlg;
	BOOL check = FALSE;
	BOOL end = FALSE;
	HWND hWnd = (HWND)pParam;
	CDaphneDlg * pDaphne = (CDaphneDlg *)pParam;

	CString webpage;
	webpage.LoadString(IDS_WEBURL);
	CString version;
	version.LoadString(IDS_TITLE_VER);
	CString referrer, ref_format;
  ref_format.LoadString(IDS_REF_FORMAT);
	referrer.Format(ref_format, version, OSVersion.dwPlatformId, OSVersion.dwMajorVersion, OSVersion.dwMinorVersion);

	INT last = AfxGetApp()->GetProfileInt(L"CFG", L"WebUpdateLaskCheck", 0);

	// Límite de un check por día
	if (last == getFullDay()) {
		return 2;
	}

	// Espero 30 segundos desde el inicio...
	end = WaitForSingleObject(hWebUpdateEnd, 30000) == WAIT_OBJECT_0;

	// Busco updates
	while(!check && !end) {

		CString url;
		CString str, rversion;
		CString proxy;
		CInternetSession * pInternet;

		BOOL bUseProxy = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneProxy", 0);
		CString strProxyAddress = AfxGetApp()->GetProfileString(L"CFG", L"DaphneProxyAddress", L"");
		UINT iProxyPort = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneProxyPort", 8080);

		proxy.Format(L"%s:%u", strProxyAddress, iProxyPort);	

		// Confirmación
		if (AfxGetApp()->GetProfileInt(L"CFG", L"WebUpdateAgreed", 0) == 0) {
			if (agreedlg.DoModal() != IDOK) {

				// Desactivo esta característica
				AfxGetApp()->WriteProfileInt(L"CFG", L"DaphneCheckUpdates", 0);

				// Termino el thread
				return 1;

			}
			else {
				// Recuerdo la confirmación
				AfxGetApp()->WriteProfileInt(L"CFG", L"WebUpdateAgreed", 1);
			}
		}

		if (!bUseProxy)
			pInternet = new CInternetSession(referrer);
		else
			pInternet = new CInternetSession(referrer, 1, INTERNET_OPEN_TYPE_PROXY, proxy, NULL, 0);

		url.LoadString(IDS_URLUPDATES);

		CUnicodeFile * File = NULL;
		TRY
		{
			File = (CUnicodeFile*)pInternet->OpenURL(url, 1, INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_TRANSFER_ASCII);

			if (File) {

        if (File->ReadString(rversion)) {

					if (rversion.Find(L"Daphne") != -1) {

						if (rversion.CollateNoCase(version) > 0) {
							CString news, text;

							// Leemos las novedades
							while(File->ReadString(str)) {
								news.Append(str);
								news.Append(L"\r\n");
							}

							text.Format(_("%s was released and is ready to be downloaded.\n\nWhat's new:\n%s\nWould you like to visit download web page?"),
								version, news);

							// Hay un Daphne nuevo
							if (::MessageBox(hWnd, text, _("Daphne web updates"), MB_YESNO | MB_ICONINFORMATION) == IDYES)
								::ShellExecute(NULL, L"OPEN", webpage, NULL, NULL, SW_SHOWNORMAL);
						}

						check = TRUE;
						// Guardo el día del check para no repetirlo, limito a uno cada 24 horas
						AfxGetApp()->WriteProfileInt(L"CFG", L"WebUpdateLaskCheck", getFullDay());
					}
				}
				File->Close();
				delete File;
			}
		}
		CATCH( CFileException, e )
		{
			if (File != NULL)
				delete File;
		}
		AND_CATCH( CInternetException, e )
		{
			if (File != NULL)
				delete File;
		}
		END_CATCH

			delete pInternet;

		// Espero 15 minutos antes de volver a intentar (60000 * 15)
		end = WaitForSingleObject(hWebUpdateEnd, 60000 * 15) == WAIT_OBJECT_0;
	}

	TRACE("I (WebUpdate) was signaled to quit.\n");
	ResetEvent(hWebUpdateEnd);

	return 0;
}


void CDaphneDlg::ResetWebUpdates()
{
	// Bajo el thread, si es que estaba creado
	if (threadWebUpdates != NULL) {

      TRACE("Signaling WebUpdate to quit.\n");
      SetEvent(hWebUpdateEnd);

		  // Internet puede ser MUY lento...
		  if (WaitForSingleObject(threadWebUpdates->m_hThread, 3000) == WAIT_OBJECT_0) {

			  DWORD exitcode = 0;
		    ::GetExitCodeThread(threadWebUpdates->m_hThread, &exitcode);
        TRACE("WebUpdate have exited %u\n", exitcode);
      }
      else {
  		  TerminateThread(threadWebUpdates->m_hThread, 0);
		  }
		  threadWebUpdates = NULL;
	}

	// Si se configuró para que verifique la web...
	if ((m_bUpdates) && (threadWebUpdates == NULL)) {
		threadWebUpdates = AfxBeginThread(CDaphneDlg::staticWebUpdates, (LPVOID)GetSafeHwnd(), THREAD_PRIORITY_BELOW_NORMAL, 0, 0, NULL);
		//threadWebUpdates->m_bAutoDelete = TRUE;
	}
}

void CDaphneDlg::ResizeWindow(WINDOWPOS FAR* lpwndpos)
{
	WINDOWPLACEMENT pos;
	int height;

	// Esta función es muy pesada y se llama muchas veces
	// por lo que prefiero ignorar ciertas llamadas

	// ProcList
	memcpy(&pos, &orig_proclist, sizeof(WINDOWPLACEMENT));
	pos.rcNormalPosition.right = lpwndpos->cx - 18;
	pos.rcNormalPosition.bottom = lpwndpos->cy - 70;
	m_cProcList.SetWindowPlacement(&pos);

	// Checks
	m_cFullpath.GetWindowPlacement(&pos);
	height = pos.rcNormalPosition.bottom - pos.rcNormalPosition.top;
	pos.rcNormalPosition.top = lpwndpos->cy - orig_checkb_margin;
	pos.rcNormalPosition.bottom = lpwndpos->cy - orig_checkb_margin + height;
	m_cFullpath.SetWindowPlacement(&pos);

	m_cOntop.GetWindowPlacement(&pos);
	pos.rcNormalPosition.top = lpwndpos->cy - orig_checkb_margin;
	pos.rcNormalPosition.bottom = lpwndpos->cy - orig_checkb_margin + height;
	m_cOntop.SetWindowPlacement(&pos);

	// Target
	m_cKill.GetWindowPlacement(&pos);
	height = pos.rcNormalPosition.bottom - pos.rcNormalPosition.top;
	pos.rcNormalPosition.top = lpwndpos->cy - orig_target_margin;
	pos.rcNormalPosition.bottom = lpwndpos->cy - orig_target_margin + height;
	m_cKill.SetWindowPlacement(&pos);

	// Combo
	m_cAction.GetWindowPlacement(&pos);
	height = pos.rcNormalPosition.bottom - pos.rcNormalPosition.top;
	pos.rcNormalPosition.top = lpwndpos->cy - orig_combo_margin;
	pos.rcNormalPosition.bottom = lpwndpos->cy - orig_combo_margin + height;
	m_cAction.SetWindowPlacement(&pos);

	// Buttons
	m_cMenub.GetWindowPlacement(&pos);
	height = pos.rcNormalPosition.bottom - pos.rcNormalPosition.top;
	pos.rcNormalPosition.top = lpwndpos->cy - orig_button_margin;
	pos.rcNormalPosition.bottom = lpwndpos->cy - orig_button_margin + height;
	m_cMenub.SetWindowPlacement(&pos);

	m_cSettings.GetWindowPlacement(&pos);
	pos.rcNormalPosition.top = lpwndpos->cy - orig_button_margin;
	pos.rcNormalPosition.bottom = lpwndpos->cy - orig_button_margin + height;
	m_cSettings.SetWindowPlacement(&pos);
}

void CDaphneDlg::createUpdateThread()
{
	if (threadUpdate == NULL) {
		threadUpdate = AfxBeginThread(CProcessList::staticUpdate, (LPVOID)&processes, THREAD_PRIORITY_ABOVE_NORMAL, 0, 0, NULL);
	}
}

void CDaphneDlg::SetRefreshTimer()
{
	KillTimer(IDT_REFRESH);
	if (AfxGetApp()->GetProfileInt(L"CFG", L"MainFastRefresh", 1)) {
		SetTimer(IDT_REFRESH, 1000, NULL);
	}
	else {
		SetTimer(IDT_REFRESH, 2000, NULL);
	}
}

void CDaphneDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	// Solo si no hay automatismos esperando, suspendo el refresco
	if (!hasAutomation())
		if (bShow)
			SetEvent(hRefreshEvent);
		else
			ResetEvent(hRefreshEvent);
}

BOOL CDaphneDlg::hasAutomation() {

	return (traps.size() > 0);
}

void CDaphneDlg::loadTraps()
{
	CString str;
	CString raw;
  UINT size;
  DWORD_PTR * value;

	int items = AfxGetApp()->GetProfileInt(L"CFG", L"TrapCount", 0);
	for(int i = 0; i < items; i++) {

		CTrap * p = new CTrap();

		str.Format(L"Trap_%u_process_name", i);
		p->setProcessName(AfxGetApp()->GetProfileString(L"Traps", str, L""));
		str.Format(L"Trap_%u_type", i);
		p->setType(AfxGetApp()->GetProfileInt(L"Traps", str, 0));
		str.Format(L"Trap_%u_wcontains", i);
		p->setWindowContains(AfxGetApp()->GetProfileString(L"Traps", str, L""));
		str.Format(L"Trap_%u_i64_dword", i);
    size = sizeof(DWORD_PTR);
    if (AfxGetApp()->GetProfileBinary(L"Traps", str, (LPBYTE*)&value, &size) && size == sizeof(DWORD_PTR)) {
      p->seti64DWORD(*value);
      delete [] value;
    }

		traps.push_back(p);
	}
}

void CDaphneDlg::saveTraps()
{
	int j;
	CString str, tmp;
  ULONG64 value;

	int items = AfxGetApp()->GetProfileInt(L"CFG", L"TrapCount", 0);
	for(j = 0; j < items; j++) {
		str.Format(L"Trap_%u_process_name", j);
		AfxGetApp()->WriteProfileString(L"Traps", str, NULL);
		str.Format(L"Trap_%u_type", j);
		AfxGetApp()->WriteProfileInt(L"Traps", str, 0);
		str.Format(L"Trap_%u_wcontains", j);
		AfxGetApp()->WriteProfileString(L"Traps", str, NULL);
		str.Format(L"Trap_%u_i64_dword", j);
    AfxGetApp()->WriteProfileBinary(L"Traps", str, NULL, 0);
	}

	AfxGetApp()->WriteProfileInt(L"CFG", L"TrapCount", (int)traps.size());
	j = 0;
	TrapList::iterator i;
	for(i = traps.begin(); i != traps.end(); i++) {
		CTrap * p = (CTrap *)(*i);

		str.Format(L"Trap_%u_process_name", j);
		AfxGetApp()->WriteProfileString(L"Traps", str, p->getProcessName());
		str.Format(L"Trap_%u_type", j);
		AfxGetApp()->WriteProfileInt(L"Traps", str, p->getType());
		str.Format(L"Trap_%u_wcontains", j);
		AfxGetApp()->WriteProfileString(L"Traps", str, p->getWindowContains());
		str.Format(L"Trap_%u_i64_dword", j);
    value = p->geti64DWORD();
    AfxGetApp()->WriteProfileBinary(L"Traps", str, (LPBYTE)&value, sizeof(value));

		j++;
	}
}

HWND CDaphneDlg::guessWindow(CProcess *proc, CString substr)
{
	CString title;
	proc->ReadWindowInformation();

	ListHWND::iterator j;
	for(j = proc->windows.begin(); j != proc->windows.end(); j++) {

		if (::IsWindow((HWND)(*j))) {
			CWnd * pWindow = CWnd::FromHandle((HWND)(*j));
			pWindow->GetWindowText(title);
			if (title.Find(substr) != -1)
				return (HWND)(*j);
		}
	}

	return 0;
}

void CDaphneDlg::delayEffect(float delay)
{
	if ((delay > 0.0) && (delay <= 2.5))
		Sleep((DWORD)ceilf(delay * 1000.0F));
}

LRESULT CDaphneDlg::itemLookAndFeel(NMLVCUSTOMDRAW *pLVCD)
{
	CProcess * proc = waitForProcess((DWORD)pLVCD->nmcd.lItemlParam);

	if (proc != NULL) {
		__int64 cputime = processes.getIdleCPUTime() + processes.getUsedCPUTime();

    // Si está configurado, blanco y gris se alternan como fondo
    if (m_bAlternateBackground) {
      if (pLVCD->nmcd.dwItemSpec % 2)
        pLVCD->clrTextBk = RGB(255,255,255);
      else
        pLVCD->clrTextBk = RGB(235,235,235);
    }

		if ((m_bHLSys) && (proc->bSYSTEM))
			pLVCD->clrText = RGB(0,0,255);

		if ((highlights_on) && (highlights.Find(proc->strName) != -1))
			pLVCD->clrText = RGB(46,139,57);

		if ((m_bHLTopMem) && (proc->WorkingSetSize > meminfo.ullAvailPhys / 5))
			pLVCD->clrText = RGB(240,166,10);//f0a60b

		if ((m_bHLTop) && (cputime > 0) && (!proc->bSYSTEM) &&
			((100 * (proc->liCPUTimeTotal - proc->liCPUTimeLast) / cputime) > 94))
			pLVCD->clrText = RGB(255,0,0);


	}

	return CDRF_DODEFAULT;
}

BOOL CDaphneDlg::OnQueryEndSession() 
{
	if (!CDialog::OnQueryEndSession())
		return FALSE;
	
	// Siempre podemos terminar bien
	return TRUE;
}

void CDaphneDlg::OnEndSession(BOOL bEnding) 
{
	CDialog::OnEndSession(bEnding);
	
	if (bEnding) {
		Quit();
	}
}

void CDaphneDlg::setDebugPrivilegeState(bool state)
{
	m_bDebugPrivilege = state;
}

BOOL CDaphneDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_HOTKEY) {

		if (pMsg->wParam == GlobalAddAtom(L"LHF_DSK_0"))
			currentDesktop = 0;
		if (pMsg->wParam == GlobalAddAtom(L"LHF_DSK_1"))
			currentDesktop = 1;
		if (pMsg->wParam == GlobalAddAtom(L"LHF_DSK_2"))
			currentDesktop = 2;
		if (pMsg->wParam == GlobalAddAtom(L"LHF_DSK_3"))
			currentDesktop = 3;

		if (!SwitchDesktop( hDesktops[currentDesktop] ) )
		{
      WCHAR tmp[30];
			::MessageBox(NULL, _("Daphne isn't able to switch Desktop\nIt can be a configuration problem.\nPlease reconfigure or restart Daphne and it will appear on next startup."), __("Error", tmp, sizeof(tmp)),MB_ICONERROR | MB_OK);
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDaphneDlg::WinHelp(DWORD dwData, UINT nCmd)
{
  AfxGetApp()->WinHelp(dwData, nCmd);
}

void CAboutDlg::OnBnClickedSupport()
{
  CString str;
	str.LoadString(IDS_SUPPORTURL);

	::ShellExecute(NULL, L"OPEN", str, NULL, NULL, SW_SHOWNORMAL);
}

void CDaphneDlg::killCurrentProcess() {
  if (MessageBox(_("Are you sure you want to kill this process?"), _("Daphne"), MB_YESNO) == IDYES) {
    CProcess * proc;
		if (proc = waitForProcess(GetSelectedPID()))
  		proc->Kill();
	  RefreshList();
  }
}

void CDaphneDlg::scheduler(void)
{
  CTime n = CTime::GetCurrentTime();
  CTime now(n.GetYear(), n.GetMonth(), n.GetDay(), n.GetHour(), n.GetMinute(), 0);

  TRACE(L"Checking %s\n", now.Format(L"%H:%M:%S"));

  if (schedule.find(now) != schedule.end()) {
    std::list<Task*>::iterator it;
    for (it = schedule[now].begin(); it != schedule[now].end(); it++) {
      (*it)->run(this);
      delete *it;
    }
    schedule[now].clear();
  }
}

void SysShutdownTask::run(CDaphneDlg * dlg) {
    dlg->ShowBalloon(_("System is going down!"));
    Sleep(2000);
    if (!ExitWindowsEx(EWX_FORCEIFHUNG | type, SHTDN_REASON_FLAG_PLANNED)) {
      LPVOID lpMsgBuf;
      CString tmp;
      DWORD dw = GetLastError(); 

      FormatMessage(
          FORMAT_MESSAGE_ALLOCATE_BUFFER | 
          FORMAT_MESSAGE_FROM_SYSTEM,
          NULL,
          dw,
          MAKELANGID(LANG_USER_DEFAULT, SUBLANG_DEFAULT),
          (LPTSTR) &lpMsgBuf,
          0, NULL );

      tmp.Format(L"ERROR %08Xh: %s", dw, (TCHAR*)lpMsgBuf);
      dlg->ShowBalloon(tmp);

      LocalFree(lpMsgBuf);
    }
}



void CAboutDlg::OnBnClickedContribute()
{
  // http://www.drk.com.ar/daphne.php#Contribute
  CString str;
	str.LoadString(IDS_CONTRIBUTEURL);

	::ShellExecute(NULL, L"OPEN", str, NULL, NULL, SW_SHOWNORMAL);
}


CString CAboutDlg::getCreditsFromWeb(void)
{
  CInternetSession * pInternet;
	CString url;
	CString str, credits;
	CString proxy;
	CString version;
	version.LoadString(IDS_TITLE_VER);
	CString referrer, ref_format;
  ref_format.LoadString(IDS_REF_FORMAT);
	referrer.Format(ref_format, version, OSVersion.dwPlatformId, OSVersion.dwMajorVersion, OSVersion.dwMinorVersion);
	BOOL bUseProxy = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneProxy", 0);
	CString strProxyAddress = AfxGetApp()->GetProfileString(L"CFG", L"DaphneProxyAddress", L"");
	UINT iProxyPort = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneProxyPort", 8080);

  if (!bUseProxy)
    pInternet = new CInternetSession(referrer);
  else
    pInternet = new CInternetSession(referrer, 1, INTERNET_OPEN_TYPE_PROXY, proxy, NULL, 0);

  url.LoadString(IDS_WEBCREDITSURL);

  CUnicodeFile * File = NULL;
  TRY
  {
    File = (CUnicodeFile*)pInternet->OpenURL(url, 1, INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_TRANSFER_ASCII);

    if (File) {
      CString tmp;
      while (File->ReadUTF8String(tmp)) {
        credits.Append(tmp);
        credits.Append(L"\r\n");
      }

      File->Close();
      delete File;
    }
  }
  CATCH( CFileException, e )
  {
    if (File != NULL)
      delete File;
  }
  AND_CATCH( CInternetException, e )
  {
    if (File != NULL)
      delete File;
  }
  END_CATCH

    delete pInternet;


  return credits;
}


CString CDaphneDlg::getTextProcessList(void)
{
  CString list, line, priority, cpu;

	if (WaitForSingleObject(hRefreshMutex, INFINITE) == WAIT_OBJECT_0) {

		CProcess * proc = processes.getFirstProcess();

    list += L"\"";
    list += _("PID"); list += L"\",\"";
    list += _("Process"); list += L"\",\"";
    list += _("Path"); list += L"\",\"";
    list += _("Priority"); list += L"\",\"";
    list += _("Class"); list += L"\",\"";
    list += _("Memory"); list += L"\",\"";
    list += _("PeakMemoryUsage");    list += L"\",\"";
    list += _("Swap"); list += L"\",\"";
    list += _("PeakSwapUsage"); list += L"\",\"";
    list += _("Threads"); list += L"\",\"";
    list += _("Owner"); list += L"\",\"";
    list += _("I/O reads"); list += L"\",\"";
    list += _("I/O writes"); list += L"\"\n";

		while(proc != NULL) {

	    switch(proc->dwPriority) {
	    case HIGH_PRIORITY_CLASS:
		    priority = _("High");
		    break;
	    case NORMAL_PRIORITY_CLASS:
		    priority = _("Normal");
		    break;
	    case IDLE_PRIORITY_CLASS:
		    priority = _("Low");
		    break;
	    case REALTIME_PRIORITY_CLASS:
		    priority = _("Real time");
		    break;
	    default:
		    priority = _("N/A");
		    break;
	    }

      line.Format(L"%u,\"%s\",\"%s\",\"%s\",\"%s\",%.2f,%.2f,%.2f,%.2f,%u,\"%s\",%u,%u\n", 
        proc->dwPID,
        proc->strName,
        proc->strFullPath,
        priority,
        ((proc->bService)?(_("Service")):(_("Process"))), 
        ((double)proc->WorkingSetSize / (double)(1024*1024)),
        ((double)proc->PeakWorkingSetSize / (double)(1024*1024)),
        ((double)proc->PagefileUsage / (double)(1024*1024)),
        ((double)proc->PeakPagefileUsage / (double)(1024*1024)),
        proc->GetThreadCount(),
        proc->strUser,
        proc->ReadOperationCount,
        proc->WriteOperationCount
        );
      list += line;
		
			proc = processes.getNextProcess();
		}

		ReleaseMutex(hRefreshMutex);
	}

  return list;
}

void CDaphneDlg::processList2Clipboard(void)
{
  CStringA tmp;
  INT bytes, bsize;
	HGLOBAL hData = NULL;

	if (!OpenClipboard())
		return;

	if (!EmptyClipboard())
		return;

  bsize = getTextProcessList().GetLength()+1;
  if (hData = GlobalAlloc(GMEM_MOVEABLE & GMEM_DDESHARE, bsize)) {
    bytes = WideCharToMultiByte(CP_OEMCP, 0, (LPCWSTR)getTextProcessList(), getTextProcessList().GetLength(), tmp.GetBufferSetLength(bsize), bsize, NULL, NULL);
    tmp.ReleaseBuffer(bytes);
    CopyMemory(hData, (LPCSTR)tmp.GetBuffer(), bytes+1);
  }
	
	if (hData) 
		::SetClipboardData(CF_TEXT, hData);

	CloseClipboard();
}

void CDaphneDlg::loadRunSlots(void)
{
  runSlots[0] = AfxGetApp()->GetProfileString(L"CFG", L"RunSlotPath1", L"");
  runSlots[1] = AfxGetApp()->GetProfileString(L"CFG", L"RunSlotPath2", L"");
  runSlots[2] = AfxGetApp()->GetProfileString(L"CFG", L"RunSlotPath3", L"");
  runSlots[3] = AfxGetApp()->GetProfileString(L"CFG", L"RunSlotPath4", L"");
}

UINT CDaphneDlg::staticMessageBox(LPVOID pParam)
{
  CDaphneDlg * pDaphne = (CDaphneDlg *)pParam;

  pDaphne->threadPopupMessage = NULL;

  ::MessageBox(NULL, pDaphne->m_strMessage, _("Scheduled message"), MB_SYSTEMMODAL|MB_OK|MB_ICONEXCLAMATION);
  return 0;
}


void CDaphneDlg::saveSchedule(void)
{
  CString name;
	UINT items = AfxGetApp()->GetProfileInt(L"CFG", L"TaskCount", 0);
	for(UINT idx = 0; idx < items; idx++) {
    name.Format(L"Task_%u", idx);
    AfxGetApp()->WriteProfileInt(L"Tasks", name, 0); // Inválida
  }

  // Scheduler clean
  UINT idx = 0;
  TaskMap::iterator now = schedule.begin();
  while (now != schedule.end()) {
    std::list<Task*>::iterator it;
    for (it = now->second.begin(); it != now->second.end(); it++) {
      (*it)->storeConfiguration(idx++);
      delete *it;
    }
    now->second.clear();
    ++now;
  }
  AfxGetApp()->WriteProfileInt(L"CFG", L"TaskCount", idx);
}

void CDaphneDlg::loadSchedule(void)
{
	UINT items = AfxGetApp()->GetProfileInt(L"CFG", L"TaskCount", 0);
	for(UINT idx = 0; idx < items; idx++) {

    Task * t = Task::factory(idx);

    if (t) {
      t->restoreConfiguration(idx);
      schedule[t->time].push_back(t);
    }
	}
}


////////// TASK /////////////////
Task * Task::factory(UINT idx) {
  CString name;
  name.Format(L"Task_%u", idx);
  UINT trapType = AfxGetApp()->GetProfileInt(L"Tasks", name, 0);
  switch(trapType) {
  case 3:
    return new PopupMessageTask();
  default:
    return NULL;
  }
}
void Task::storeConfiguration(UINT idx) {
  CString name;
  name.Format(L"Task_%u_time", idx);
  __time64_t ts = time.GetTime();
  AfxGetApp()->WriteProfileBinary(L"Tasks", name, (LPBYTE)&ts, sizeof(ts));
};
void Task::restoreConfiguration(UINT idx) {
  CString name;
  name.Format(L"Task_%u_time", idx);
  __time64_t * ts;
  UINT bytes = sizeof(__time64_t);
  AfxGetApp()->GetProfileBinary(L"Tasks", name, (LPBYTE*)&ts, &bytes);
  if (bytes == sizeof(__time64_t)) {
    time = CTime(*ts);
  }
  else {
    time = CTime(0);
  }
  delete[] ts;
};

void PopupMessageTask::run(CDaphneDlg * dlg) {
	if (dlg->threadPopupMessage == NULL) {
    dlg->m_strMessage = message;
    dlg->threadPopupMessage = AfxBeginThread(CDaphneDlg::staticMessageBox, dlg, THREAD_PRIORITY_NORMAL, CREATE_SUSPENDED, 0, NULL);
    dlg->threadPopupMessage->m_bAutoDelete = TRUE;
    dlg->threadPopupMessage->ResumeThread();
	}
}
const CString PopupMessageTask::toString() const {
  CString tmp; 
  tmp.Format(_("%s display %s"), time.Format(L"%x %X"), message);
  return tmp;
}
void PopupMessageTask::storeConfiguration(UINT idx) {
  CString name;

  name.Format(L"Task_%u", idx);
  AfxGetApp()->WriteProfileInt(L"Tasks", name, getTrapType());
  name.Format(L"Task_%u_msg", idx);
  AfxGetApp()->WriteProfileString(L"Tasks", name, message);

  Task::storeConfiguration(idx);
}
void PopupMessageTask::restoreConfiguration(UINT idx) {
  CString name;

  name.Format(L"Task_%u_msg", idx);
  message = AfxGetApp()->GetProfileString(L"Tasks", name, L"");

  Task::restoreConfiguration(idx);
}

const CString SysShutdownTask::toString() const {
  CString tmp; 
  switch(type) {
    case 8:
      tmp.Format(_("%s system shutdown."), time.Format(L"%x %X"));
      break;
    case 2:
      tmp.Format(_("%s system restart."), time.Format(L"%x %X"));
      break;
    case 0:
      tmp.Format(_("%s system log off."), time.Format(L"%x %X"));
      break;
  }
  return tmp;
}
void KillTask::run(CDaphneDlg * dlg) {
  CProcess * p = dlg->waitForProcess(pid);

  if (p != NULL && name == p->strName) {
    // Lo liquidamos
    if (polite) {
      if (p->KillPolite()) {
        CString msg;
        msg.Format(_("Unable to kill scheduled process %s, PID %u."), name, pid);
        dlg->ShowBalloon(msg);
      }
      else {
        CString msg;
        msg.Format(_("Politely killed scheduled process %s, PID %u."), name, pid);
        dlg->ShowBalloon(msg);
      }
    }
    else {
      if (p->Kill()) {
        CString msg;
        msg.Format(_("Unable to kill scheduled process %s, PID %u."), name, pid);
        dlg->ShowBalloon(msg);
      }
      else {
        CString msg;
        msg.Format(_("Killed scheduled process %s, PID %u."), name, pid);
        dlg->ShowBalloon(msg);
      }
    }
  }
  else {
      CString msg;
      msg.Format(_("Can't find scheduled process %s, PID %u."), name, pid);
      dlg->ShowBalloon(msg);
  }
}
const CString KillTask::toString() const {
  CString tmp; 
  tmp.Format(_("%s kill %s [%u]"), time.Format(L"%x %X"), name, pid);
  return tmp;
}
