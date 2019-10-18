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

// Process.h: interface for the CProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCESS_H__3FAA08C4_823D_4B3E_82DE_6397F5156B7D__INCLUDED_)
#define AFX_PROCESS_H__3FAA08C4_823D_4B3E_82DE_6397F5156B7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DaphneBase.h"
#include "SCManager.h"
#include "Thread.h"

// simple macro to handle errors
#define SIGNAL_ERROR() { bError = TRUE; __leave; } 
// align pointer
#define ALIGN_DWORD(x) ( (x & 0xFFFFFFFC) ? (x & 0xFFFFFFFC) + sizeof(DWORD) : x )

class CProcessList;

#include <Winternl.h>

typedef struct _PEB_LDR_DATA_DRK
{
   BYTE Reserved1[8];
   PVOID Reserved2[3];
   LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA_DRK, *PPEB_LDR_DATA_DRK;

typedef struct _RTL_USER_PROCESS_PARAMETERS_DRK
{
   ULONG  AllocationSize;
   ULONG  Size;
   ULONG  Flags;
   ULONG  DebugFlags;
   HANDLE  hConsole;
   ULONG  ProcessGroup;
   HANDLE  hStdInput;
   HANDLE  hStdOutput;
   HANDLE  hStdError;
   UNICODE_STRING  CurrentDirectoryName;
   HANDLE  CurrentDirectoryHandle;
   UNICODE_STRING  DllPath;
   UNICODE_STRING  ImagePathName;
   UNICODE_STRING  CommandLine;
   PWSTR  Environment;
   ULONG  dwX;
   ULONG  dwY;
   ULONG  dwXSize;
   ULONG  dwYSize;
   ULONG  dwXCountChars;
   ULONG  dwYCountChars;
   ULONG  dwFillAttribute;
   ULONG  dwFlags;
   ULONG  wShowWindow;
   UNICODE_STRING  WindowTitle;
   UNICODE_STRING  DesktopInfo;
   UNICODE_STRING  ShellInfo;
   UNICODE_STRING  RuntimeInfo;
} RTL_USER_PROCESS_PARAMETERS_DRK, *PRTL_USER_PROCESS_PARAMETERS_DRK;

typedef struct _MYPEB
{
   BYTE Reserved1[2];
   BYTE BeingDebugged;
   BYTE Reserved2[1];
   PVOID Reserved3[2];
   PPEB_LDR_DATA_DRK Ldr;
   PRTL_USER_PROCESS_PARAMETERS_DRK ProcessParameters;
   BYTE Reserved4[104];
   PVOID Reserved5[52];
   PVOID PostProcessInitRoutine;
   BYTE Reserved6[128];
   PVOID Reserved7[1];
   ULONG SessionId;
} MYPEB, *PMYPEB;


// define pointer to NtQueryInformationProcess function
typedef NTSTATUS (WINAPI *NTQIP)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

class CProcess  
{
public:
	void toDRKDB();
	void submitToDRKDB();
	BOOL SetPriority(DWORD pclass);
	size_t GetThreadCount();
	BOOL RemoveDLL();
	BOOL InjectDLL(CString dllpath);
	int KillPolite();
	BOOL servicePause();
	BOOL serviceContinue();
	BOOL serviceStop();
	BOOL serviceStart();
	void SetOnTop(BOOL bOnTop = TRUE);
	void Activate();
	BOOL isValid();
	BOOL isAlive();
	BOOL isInjected();
  BOOL GetAffinityMask(PULONG64 lpProcessAffinityMask,PULONG64 lpSystemAffinityMask);
  BOOL SetAffinityMask(ULONG64 dwProcessAffinityMask);
	int Kill();
	BOOL bAlive;
	BOOL bValid;
	BOOL bService;
	BOOL bSYSTEM;
	BOOL bInjected;
  BOOL bZombie;
	DWORD dwServiceState;
	int ReadProcessInformation(BOOL full = FALSE, CProcessList * pl = NULL, __int64 totalTime = 0);
	int ReadWindowInformation();
	int ReadAllModules();
  BOOL ReadEnvironmentBlock(CString &data);
  static BOOL HasReadAccess( HANDLE hProcess, void* pAddress, SIZE_T& nSize );
	DWORD dwPID;
	DWORD dwParentPID;
	CString strName;
	CString strParentName;
	CString strUser;
	CString strDomain;
	CString strFullPath;
	CString strFullPathEx;
	CString strModules;
	DWORD dwPriority;
	SIZE_T WorkingSetSize;
	SIZE_T PeakWorkingSetSize;
	SIZE_T PagefileUsage;
	SIZE_T PeakPagefileUsage;
	FILETIME tCreationTime;
	FILETIME tExitTime;
	FILETIME tKernelTime;
	FILETIME tUserTime;
	WCHAR szCmdLine[DAPHNE_MAXPATH];
	ThreadList threads;
	ListHWND windows;
	ListHWND children;
	__int64 liCPUTimeLast;
	__int64 liCPUTimeTotal;
  ULONGLONG ReadOperationCount;
  ULONGLONG WriteOperationCount;
  ULONGLONG OtherOperationCount;
  ULONGLONG ReadTransferCount;
  ULONGLONG WriteTransferCount;
  ULONGLONG OtherTransferCount;

	CProcess(DWORD pid, CSCManager * scm = NULL);
	virtual ~CProcess();
	void readPSAPI(HANDLE snapshot, const ProcInfoMap &processes);
  BOOL getWorkingSetSize(PSIZE_T lpMinimumWorkingSetSize, PSIZE_T lpMaximumWorkingSetSize);
  BOOL setWorkingSetSize(SIZE_T dwMinimumWorkingSetSize, SIZE_T dwMaximumWorkingSetSize);
  BOOL emptyWorkingSet(void);
  DWORD waitForInputIdle(DWORD milliseconds);

	static BOOL CALLBACK ActivateApp(HWND hwnd, LPARAM lParam);
	static BOOL CALLBACK OnTopOn(HWND hwnd, LPARAM lParam);
	static BOOL CALLBACK OnTopOff(HWND hwnd, LPARAM lParam);
	static BOOL CALLBACK EnumTopLevelWindows(HWND hwnd, LPARAM lParam);
	static CString strJavaHome;

private:
	BOOL GetProcessCmdLine(HANDLE hProcess, LPWSTR lpszCmdLine, size_t uiCmdSize);
	BOOL GetProcessCmdLineOld(HANDLE hProcess, LPWSTR lpszCmdLine, size_t uiCmdSize);
  
	CSCManager * pSCM;

	/* SpyAgent */
	DWORD hLibModule;   // Base address of loaded module (==HMODULE);
	WCHAR  szLibPath[_MAX_PATH];  // The name of our dll
	/* -------- */
private:
	bool locateEntryPSAPI(HANDLE snapshot, PROCESSENTRY32* pentry);
  static LPTHREAD_START_ROUTINE getStdFuncionAddr(LPCSTR name);
};

#endif // !defined(AFX_PROCESS_H__3FAA08C4_823D_4B3E_82DE_6397F5156B7D__INCLUDED_)
