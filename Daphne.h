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

// Daphne.h : main header file for the DAPHNE application
//

#if !defined(AFX_DAPHNE_H__B231EDD7_1051_428C_832F_635B7141F218__INCLUDED_)
#define AFX_DAPHNE_H__B231EDD7_1051_428C_832F_635B7141F218__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#pragma warning ( disable : 4786 )

// Visual Leak Detector
//#include "vld.h"

#include "DRKSpyAgent\DRKSpyAgent.h"
#include "DRKHooks\DRKHooks.h"

extern CString strSpyAgentDLL;
extern CString strDRKHooksDLL;
extern CString strDRKShellDLL;
extern CString strDaphneExe;
extern CString strBaseDir;

#include "DaphneBase.h"


/////////////////////////////////////////////////////////////////////////////
// CDaphneApp:
// See Daphne.cpp for the implementation of this class
//

class CDaphneApp : public CWinApp
{
public:
	CDaphneApp();
	bool SettingsAtRegistry(void);
	void ChangeSettingsLocation(bool toINIFile);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDaphneApp)
	public:
	virtual BOOL InitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDaphneApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HANDLE m_hMutex;
  virtual void HtmlHelp(DWORD_PTR dwData, UINT nCmd = 0x000F);
  HINSTANCE m_hInstResDLL;
  CWinThread * sendThread;
  static UINT staticSendStatistics(LPVOID pParam);
  static CString dumpConfig();
  static CString dumpFeatures();
  static CString dumpRegValue(CString sec, CString key, int isNumber);

public:
  static bool rtl;
  static void hitStatisticalRecord(CString item);
  static CString getUserDirectory(void);

public:
  virtual int ExitInstance();
  int SendStatistics(void);
  int statistics_event;
  enum {
		EVENT_NULL      = 0,
		EVENT_FIRST_RUN = 1,
    EVENT_FEATURES  = 2,
    EVENT_CONFIG    = 3,
    EVENT_START     = 4,
    EVENT_STOP      = 5
	};
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DAPHNE_H__B231EDD7_1051_428C_832F_635B7141F218__INCLUDED_)
