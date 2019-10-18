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

// DaphneDlg.h : header file
//

#if !defined(AFX_DAPHNEDLG_H__70E52872_53CE_45A7_9018_14F0566076A7__INCLUDED_)
#define AFX_DAPHNEDLG_H__70E52872_53CE_45A7_9018_14F0566076A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ProcList.h"
#include "ProcessList.h"	// Added by ClassView
#include "Killer.h"

#define PL_COLUMN_COUNT 14

#include <pdh.h>

const UINT WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));

#ifndef PDH_MAX_COUNTER_PATH
	#define PDH_MAX_COUNTER_PATH 255
#endif

/////////////////////////////////////////////////////////////////////////////
// CDaphneDlg dialog

class CDaphneDlg : public CDialog
{
// Construction
public:
	void setDebugPrivilegeState(bool state);
	LRESULT itemLookAndFeel(NMLVCUSTOMDRAW* pLVCD);
	void SetRefreshTimer();
	void ResetWebUpdates();
	void ShowBalloon(CString text, UINT timeout = 10000);
	BOOL m_bAlternateBackground;
	BOOL m_bUpdates;
	BOOL m_bDebugPrivilege;
	void selectProcess(DWORD pid);
	void selectProcess(CString name);
	void showWindow(HWND hWnd);
	void hideWindow(HWND hWnd);
	CKiller * GetKiller(int index);
	int CloseByName(CString wnd_name, BOOL case_sensitive);
	int SwitchToDesktop(INT number);
	void Quit();
	CDaphneDlg(CWnd* pParent = NULL);	// standard constructor
	int orderColumn;
	int orderMode;
	void RefreshList();
	CWinThread * threadUpdate;
	CWinThread * threadWebUpdates;
	CWinThread * threadPopupMessage;
  BOOL stopWebUpdate;
	void SetupTrayIcon(BOOL quit = FALSE);
	void UpdateTrayToolTip();
  void CalculateCPUUsage();
	MapWindows hiddenWindows;
	static int CALLBACK Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
	static UINT staticWebUpdates(LPVOID pParam);
  static UINT staticMessageBox(LPVOID pParam);
	SYSTEM_INFO sysinfo;
	MEMORYSTATUSEX meminfo;
	CProcess * waitForProcess(DWORD pid);
  BOOL enableShutDown();


// Dialog Data
	//{{AFX_DATA(CDaphneDlg)
	enum { IDD = IDD_DAPHNE_DIALOG };
	CButton	m_cSettings;
	CButton	m_cMenub;
	CButton	m_cOntop;
	CButton	m_cFullpath;
	CComboBox	m_cAction;
	CStatic	m_cKill;
	CProcList	m_cProcList;
	BOOL	m_bExtPath;
	BOOL	m_bOnTop;
	int		m_iAction;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDaphneDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void createUpdateThread();
	void ResizeWindow(WINDOWPOS FAR* lpwndpos);
	void Copy(int mode);
	void LoadTargetCursor();
	void AppendKillMenu(CMenu * menu);
  void AppendBaseMenu(CMenu * menu);
	void SetWMode(int mode);
	void DesktopMapInit();
	LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	LRESULT OnTaskBarCreated(WPARAM wp, LPARAM lp);
  LRESULT OnJustStarted(WPARAM wp, LPARAM lp);
	BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	BOOL hasAutomation();
	HICON m_hIcon;
  HICON icon[11];
	HCURSOR m_hTarget;
	HCURSOR m_hNormal;
	NOTIFYICONDATA m_tnd;
  CString m_username;
  CString m_computer;
  CString m_processor;

	// Generated message map functions
	//{{AFX_MSG(CDaphneDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRclickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFullpathex();
	afx_msg void OnSettings();
	afx_msg void OnMenub();
	afx_msg void OnAontop();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCKill();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnSelchangeTaction();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnQueryEndSession();
	afx_msg void OnEndSession(BOOL bEnding);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void RefreshInfo();
	void delayEffect(float delay);
	HWND guessWindow(CProcess *proc, CString substr);
	void saveTraps();
	void loadTraps();
	BOOL bKillTarget;
	HWND hLastSelectedWnd;
	DWORD GetSelectedPID();
	void InsertOrUpdateList(CProcess * process, __int64 cputime, __int64 usedtime);
	void RemoveFromList(CProcess * process);
	CProcessList processes;
	CSCManager scm;
  TaskMap schedule;
	BOOL bSCMOk;
	BOOL trayIconOK;
	BOOL m_bWindowHidden;
	BOOL m_bUpdatePath;
	CRect min_size;
	WINDOWPLACEMENT orig_proclist;
	int orig_combo_margin;
	int orig_target_margin;
	int orig_checkb_margin;
	int orig_button_margin;
	TrapList traps;
	CString highlights;
	BOOL highlights_on;
	BOOL m_bHLSys;
	BOOL m_bHLTop;
	BOOL m_bJavaHide;
	BOOL m_bHLTopMem;
	PROCESS_INFORMATION	desktopProcessInfo[4];
	HDESK hDesktops[4];
	HWND hThumb[4];
	int currentDesktop;
	BOOL mdStarted;
  BOOL bProcessLogging;
  CString strProcessLoggingFile;
  CString runSlots[4];

private:
  LONG days, hours, minutes, seconds;
  TCHAR uptimeCounterPath[PDH_MAX_COUNTER_PATH];
  BOOL showing;
	CString m_strInfoCPUUsage;
	int infoCPUUsage;
  CPUUsageQueue usageQueue;
	CString m_strInfoMEMUsage;
	CString m_strInfoTimeUp;
	CString m_strInfoMEMFree;
  TCHAR toolTipBuffer[DAPHNE_MAX_TOOLTIP_SIZE];

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void CloseWindows(HDESK desktop, DWORD pid);
  void getUpTime();
public:
	static BOOL CALLBACK EnumWindowsDesktop(HWND hwnd, LPARAM lParam);
	int OnDeadProcess(DWORD pid, CString name);
	int OnNewProcess(DWORD pid);

public:
  virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
  BOOL m_bHideZombies;
  BOOL m_bAnimatedIcon;
  void killCurrentProcess();
  void scheduler(void);
protected:
  virtual void PreInitDialog();
public:
  CString m_strInfoPCount;
  CString m_strInfoLogin;
  CString m_strInfoCPUCount;
  CString m_strInfoProcessor;
  CString getTextProcessList(void);
  void processList2Clipboard(void);
  void loadRunSlots(void);
  CString m_strMessage;
  void saveSchedule(void);
  void loadSchedule(void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DAPHNEDLG_H__70E52872_53CE_45A7_9018_14F0566076A7__INCLUDED_)
