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

// Process.cpp: implementation of the CProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Process.h"
#include "ProcessList.h"
#include "Sddl.h"
#include "DaphneBase.h"
#include "DRKSpyAgent\DRKSpyAgent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef NTSTATUS (*TQIP)( HANDLE ProcessHandle, PROCESSINFOCLASS ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength );

CString CProcess::strJavaHome;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProcess::CProcess(DWORD pid, CSCManager * scm)
{
	dwPID = pid;
	strName = DAPHNE_UNDEFINEDNAME;

	if ((pid == 0) || (pid == 4) || (pid == 8)) {
		bSYSTEM = TRUE;
		switch(pid) {
		case 0:
			strName = "System Idle Process";
			break;
		case 4:
		case 8:
			strName = "System";
			break;
		}
	}
	else
		bSYSTEM = FALSE;

	hLibModule = 0;
	bValid = FALSE;
	bService = FALSE;
	bInjected = FALSE;
	bZombie = FALSE;
	pSCM = NULL;
	dwServiceState = 0;
	liCPUTimeLast = 0;
	liCPUTimeTotal = 0;
	WorkingSetSize = 0;
	PeakWorkingSetSize = 0;
	PagefileUsage = 0;
	PeakPagefileUsage = 0;
	tCreationTime.dwHighDateTime = 0;
	tCreationTime.dwLowDateTime = 0;
	tKernelTime.dwHighDateTime = 0;
	tKernelTime.dwLowDateTime = 0;
	tUserTime.dwHighDateTime = 0;
	tUserTime.dwLowDateTime = 0;

	if (scm != NULL) {
		bService = scm->isService(dwPID);
		pSCM = scm;
	}

	ReadProcessInformation(TRUE);
}

CProcess::~CProcess()
{

	// Si habíamos inyectado una DLL, la sacamos
	if (bInjected) {
		RemoveDLL();
	}

}

int CProcess::ReadProcessInformation(BOOL full, CProcessList * pl, __int64 totalTime)
{
	HANDLE hProcess;
	HMODULE hMod;
	DWORD cbNeeded;
	WCHAR szProcessName[MAX_PATH] = L"unknown";
	WCHAR szCmdLine[DAPHNE_MAXPATH-1];
	PROCESS_MEMORY_COUNTERS memCounters;
	ULARGE_INTEGER temp;

	bAlive = TRUE;

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

	if (NULL != hProcess ) {

		// Class
		dwPriority = GetPriorityClass(hProcess);

		// Times
		GetProcessTimes(hProcess, &tCreationTime, &tExitTime, &tKernelTime, &tUserTime);

		liCPUTimeLast = liCPUTimeTotal;

		temp.HighPart = tKernelTime.dwHighDateTime;
		temp.LowPart = tKernelTime.dwLowDateTime; 
		liCPUTimeTotal = (__int64)temp.QuadPart;

		temp.HighPart = tUserTime.dwHighDateTime;
		temp.LowPart = tUserTime.dwLowDateTime; 
		liCPUTimeTotal += (__int64)temp.QuadPart;

		GetProcessMemoryInfo( hProcess, &memCounters, sizeof(memCounters));
		WorkingSetSize = memCounters.WorkingSetSize;
		PeakWorkingSetSize = memCounters.PeakWorkingSetSize;
		PagefileUsage = memCounters.PagefileUsage;
		PeakPagefileUsage = memCounters.PeakPagefileUsage;

		IO_COUNTERS io;
		if (GetProcessIoCounters(hProcess, &io)) {
			ReadOperationCount = io.ReadOperationCount;
			WriteOperationCount = io.WriteOperationCount;
			OtherOperationCount = io.OtherOperationCount;
			ReadTransferCount = io.ReadTransferCount;
			WriteTransferCount = io.WriteTransferCount;
			OtherTransferCount = io.OtherTransferCount;
		}

		if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) ) {

			// Si por algún motivo el ID es reutilizado, lo detecto acá...
			GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR) );

			if ((!full) && (strName.Compare(szProcessName)))
				full = TRUE;

			strName = szProcessName;

#ifdef _M_X64
			BOOL isWow64 = FALSE;
			IsWow64Process(hProcess, &isWow64);
			if (isWow64)
				strName += L" [32]";
#endif

			// Información que no cambia, la leemos solo si nos piden
			if ((full) || (!strName.Compare(DAPHNE_UNDEFINEDNAME)) || (!strName.Compare(DAPHNE_ZOMBIE))){

				// User
				SECURITY_INFORMATION SecInfo = OWNER_SECURITY_INFORMATION;
				SECURITY_DESCRIPTOR *psd = NULL;
				DWORD length = 0;
				GetUserObjectSecurity(hProcess, &SecInfo, psd, 0, &length);  
				psd = (SECURITY_DESCRIPTOR *)malloc(length);  
				memset (psd, 0, length);  
				if (GetUserObjectSecurity(hProcess, &SecInfo, psd, length, &length)) {  
					PSID psidOwner;  
					BOOL bDefaulted;  
					GetSecurityDescriptorOwner(psd, &psidOwner, &bDefaulted);  
					if (IsValidSid(psidOwner)) {  
						DWORD userlength = 254;
						DWORD domainlength = 254;
						SID_NAME_USE snu;
						if (LookupAccountSid(L"", psidOwner, strUser.GetBuffer(userlength), &userlength, strDomain.GetBuffer(domainlength), &domainlength, &snu)) {
							strUser.ReleaseBuffer(userlength);
							strDomain.ReleaseBuffer(domainlength);
						}
					}
				}
				free(psd);

				// Filename
				GetModuleFileNameEx(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR) );
				strFullPath = szProcessName;

				memset(szCmdLine, 0, sizeof(szCmdLine));
				if (GetProcessCmdLine(hProcess, szCmdLine, sizeof(szCmdLine)/sizeof(TCHAR))) {

					strFullPathEx = szCmdLine;

					if ((!strFullPathEx.IsEmpty()) && 
						(!strJavaHome.IsEmpty()) &&
						(strFullPathEx.Find(strJavaHome) != -1)) {

							int space = strFullPathEx.Find(L' ', strFullPathEx.Find(strJavaHome) + strJavaHome.GetLength());
							strFullPathEx = strFullPathEx.Right(strFullPathEx.GetLength() - space);
					}

				}


				/* Suponemos que un proceso dado no cambia su estado de servicio.
				* También aprovechamos para reducir las posibilidades de incurrir
				* en un bug que no pudimos eliminar completamente de la clase CSCManager
				*/
				if ((pSCM != NULL) && (bService)) {
					dwServiceState = pSCM->getState(dwPID);
				}

			}


			bValid = TRUE;

		}
		else {
			if (!bSYSTEM) {
				strName = DAPHNE_ZOMBIE;
				bValid = FALSE;
				bZombie = TRUE;
			}
		}

		CloseHandle( hProcess );
	}
	else {
		bAlive = FALSE;
		bValid = FALSE;
	}

	if (bSYSTEM) {
		bValid = TRUE;
		bAlive = TRUE;

		if (dwPID == 0) {
			liCPUTimeLast = liCPUTimeTotal;
			liCPUTimeTotal = totalTime;
		}
	}

	return 0;
}

int CProcess::Kill()
{
	int iResult = 1;

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

	if (NULL != hProcess ) {

		// Bye bye... 
		if (TerminateProcess(hProcess, 0)) {
			bAlive = FALSE;
			iResult = 0;
		}

		CloseHandle( hProcess );
	}
	else {
		bAlive = FALSE;
	}

	return iResult;
}

BOOL CProcess::isAlive()
{
	return bAlive;
}

BOOL CProcess::isInjected()
{
	return bInjected;
}

BOOL CProcess::isValid()
{
	return bValid;
}

BOOL CProcess::GetAffinityMask(PULONG64 lpProcessAffinityMask, PULONG64 lpSystemAffinityMask)
{
  BOOL result = false;
  DWORD_PTR processAffinityMask, systemAffinityMask;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

	if (NULL != hProcess ) {
    result = GetProcessAffinityMask(hProcess, &processAffinityMask, &systemAffinityMask);
    *lpProcessAffinityMask = processAffinityMask;
    *lpSystemAffinityMask = systemAffinityMask;
    CloseHandle( hProcess );
  }

  return result;
}

BOOL CProcess::SetAffinityMask(ULONG64 dwProcessAffinityMask)
{
  BOOL result = false;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

	if (NULL != hProcess ) {
    result = SetProcessAffinityMask(hProcess, (DWORD_PTR)dwProcessAffinityMask);
    CloseHandle( hProcess );
  }

  return result;
}

BOOL CALLBACK CProcess::EnumTopLevelWindows(HWND hwnd, LPARAM lParam) {
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);

	CProcess * proc = (CProcess*)lParam;

	if (pid == proc->dwPID) {
		if (GetParent(hwnd))
			proc->children.push_back(hwnd);
		else
			proc->windows.push_back(hwnd);
	}

	return TRUE;
}

BOOL CALLBACK CProcess::ActivateApp(HWND hwnd, LPARAM lParam) {
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);

	if (pid == lParam) {
		SetForegroundWindow(hwnd);
		return FALSE;
	}

	return TRUE;
}

BOOL CALLBACK CProcess::OnTopOn(HWND hwnd, LPARAM lParam) {
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);

	if (pid == lParam) {
		::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	}

	return TRUE;
}

BOOL CALLBACK CProcess::OnTopOff(HWND hwnd, LPARAM lParam) {
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);

	if (pid == lParam) {
		::SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////
// The following code was published by Tomas Restrepo at CodeProject //
///////////////////////////////////////////////////////////////////////
//
// Copyright(C) 1999, Tomas Restrepo. All rights reserved
//
// Revisions: 	Created 12/02/99
//
///////////////////////////////////////////////////////////////////

// found by experimentation this is where the some
// process data block is found in an NT machine.
// On an Intel system, 0x00020000 is the 32
// memory page. At offset 0x0498 is the process
// current directory (or startup directory, not sure yet)
// followed by the system's PATH. After that is the  
// process full command command line, followed by 
// the exe name and the window
// station it's running on
#define BLOCK_ADDRESS	(LPVOID)0x00020498
// Additional comments:
// From experimentation I've found
// two notable exceptions where this doesn't seem to apply:
// smss.exe : the page is reserved, but not commited
//			which will get as an invalid memory address
//			error
// crss.exe : although we can read the memory, it's filled
//			  with 00 comepletely. No trace of command line
//			  information


////////////////////////////////////////////////////////////////////
//
// Function: GetProcessCmdLine()
//
// This function's code is credited to Manko from AutoIt forums.
////////////////////////////////////////////////////////////////////
BOOL CProcess::GetProcessCmdLine(HANDLE hProcess, LPWSTR lpszCmdLine, size_t uiCmdSize)
{
	if (bSYSTEM)
		return FALSE;

  // load ntdll.dll and get function pointer
  HMODULE ntdll = LoadLibrary(L"ntdll.dll");
  NTQIP myntqip = (NTQIP) GetProcAddress(ntdll, "NtQueryInformationProcess");

  // call function to get PROCESS_BASIC_INFORMATION
  PROCESS_BASIC_INFORMATION mypbi = {0};
  myntqip(hProcess, (PROCESSINFOCLASS) 0, (LPVOID) &mypbi, sizeof(PROCESS_BASIC_INFORMATION), NULL);
  FreeLibrary(ntdll);

  // read PEB structure
  MYPEB mypeb = {0};
  ReadProcessMemory(hProcess, (LPVOID) mypbi.PebBaseAddress, (LPVOID) &mypeb, sizeof(PEB), NULL);

  // read PROCESS_PARAMETERS
  RTL_USER_PROCESS_PARAMETERS myupp = {0};
  ReadProcessMemory(hProcess, (LPVOID) mypeb.ProcessParameters, (LPVOID) &myupp, sizeof(RTL_USER_PROCESS_PARAMETERS), NULL);

  // read process commandline + terminating null (2 bytes in unicode)
  SIZE_T read;
  wmemset(lpszCmdLine, '\0', uiCmdSize);
  ReadProcessMemory(hProcess, (LPVOID) myupp.CommandLine.Buffer, (LPVOID) lpszCmdLine, uiCmdSize, &read);

  return TRUE;
}

////////////////////////////////////////////////////////////////////
//
// Function: GetProcessCmdLineOld()
//
// Added: 19/02/99
//
// Description: Gets the command line for the given process
//				NOTE: hProcess should be opened with
//				PROCESS_VM_READ , PROCESS_VM_OPERATION
//				and PROCESS_QUERY_INFORMATION rights
//
///////////////////////////////////////////////////////////////////
BOOL CProcess::GetProcessCmdLineOld(HANDLE hProcess, LPWSTR lpszCmdLine, size_t uiCmdSize)
{
	LPBYTE						lpBuffer = NULL;
	LPBYTE						lpPos = NULL; // offset from the start of the buffer
	SIZE_T						dwBytesRead;
	MEMORY_BASIC_INFORMATION	mbi;
	SYSTEM_INFO					sysinfo;
	BOOL						bError = FALSE;

	if (bSYSTEM)
		return FALSE;

	__try {
		// Get the system page size by using GetSystemInfo()
		GetSystemInfo(&sysinfo);
		// allocate one on the heap to retrieve a full page
		// of memory
		lpBuffer = (LPBYTE)malloc(sysinfo.dwPageSize + (4*sizeof(WCHAR)));
		if (lpBuffer == NULL)
			SIGNAL_ERROR ();

		memset(lpBuffer, 0, sysinfo.dwPageSize + (4*sizeof(WCHAR)));

		// first of all, use VirtualQuery to get the start of the memory
		// block
		if ( VirtualQueryEx ( hProcess, BLOCK_ADDRESS, &mbi, sizeof(mbi) ) == 0 )
			SIGNAL_ERROR ();
		
		// read memory begining at the start of the page
		// after that, we know that the env strings block
		// will be 0x498 bytes after the start of the page
		if ( !ReadProcessMemory ( hProcess, mbi.BaseAddress, (LPVOID)lpBuffer, 
								  sysinfo.dwPageSize, &dwBytesRead ) )
			 SIGNAL_ERROR ();

		// now we've got the buffer on our side of the fence.
		// first, lpPos points to a string containing the current directory
		/// plus the path.
		lpPos = lpBuffer + ((DWORD)BLOCK_ADDRESS - (DWORD)mbi.BaseAddress);
		lpPos = lpPos + (wcslen ( (LPWSTR)lpPos ) + 1) * sizeof(WCHAR);

		// now goes full path an filename, aligned on a DWORD boundary
		// skip it
		lpPos = (LPBYTE)ALIGN_DWORD((DWORD)lpPos);
		lpPos = lpPos + (wcslen ( (LPWSTR)lpPos ) + 1) * sizeof(WCHAR);

		// hack: Sometimes, there will be another '\0' at this position
		// if that's so, skip it
		if ( *lpPos == '\0' ) lpPos += sizeof(WCHAR);

		if (lpPos > (lpBuffer + sysinfo.dwPageSize))
			return FALSE;

		// now we have the actual command line
		// copy it to the buffer
		wcsncpy_s(lpszCmdLine, uiCmdSize, (LPWSTR)lpPos, DAPHNE_MAXPATH);

		// make sure the path is null-terminted
		lpszCmdLine[uiCmdSize-1] = L'\0';

	}
	__finally  {
		// clean up
		if ( lpBuffer != NULL ) free ( lpBuffer );
	}
	
	return bError ? FALSE : TRUE;
}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

void CProcess::toDRKDB()
{
	CString str;
	str.Format(L"http://www.drk.com.ar/search.php?redir=%s", strName);
	::ShellExecute(AfxGetApp()->m_pMainWnd->m_hWnd, L"open", str, L"", L"", SW_SHOWNORMAL);	
}

void CProcess::submitToDRKDB()
{
	CString str;
	str.Format(L"http://www.drk.com.ar/contribute.php?p=%s", strName);
	::ShellExecute(AfxGetApp()->m_pMainWnd->m_hWnd, L"open", str, L"", L"", SW_SHOWNORMAL);	
}

void CProcess::Activate()
{
	EnumWindows(CProcess::ActivateApp, (LPARAM)dwPID);
}

void CProcess::SetOnTop(BOOL bOnTop)
{
	if (bOnTop)
		EnumWindows(CProcess::OnTopOn, (LPARAM)dwPID);
	else
		EnumWindows(CProcess::OnTopOff, (LPARAM)dwPID);
}

BOOL CProcess::serviceStart()
{
	if (pSCM) {
		pSCM->OpenService(dwPID);
		pSCM->StartService();
		pSCM->CloseService();
	}

	return FALSE;
}

BOOL CProcess::serviceStop()
{
	if (pSCM) {
		pSCM->OpenService(dwPID);
		pSCM->StopService();
		pSCM->CloseService();
	}

	return FALSE;
}

BOOL CProcess::serviceContinue()
{
	if (pSCM) {
		pSCM->OpenService(dwPID);
		pSCM->ContinueService();
		pSCM->CloseService();
	}

	return FALSE;
}

BOOL CProcess::servicePause()
{
	if (pSCM) {
		pSCM->OpenService(dwPID);
		pSCM->PauseService();
		pSCM->CloseService();
	}

	return FALSE;
}

int CProcess::ReadWindowInformation()
{
	if (bSYSTEM)
		return 1;

	windows.clear();
	children.clear();
	return EnumWindows(CProcess::EnumTopLevelWindows, (LPARAM)this);
}

int CProcess::KillPolite()
{
	ReadWindowInformation();

	ListHWND::iterator j;
	for(j = windows.begin(); j != windows.end(); j++) {

		if (::IsWindow((HWND)(*j))) {
			PostMessage((HWND)(*j), WM_CLOSE, 0, 0);
		}
	}

	return 0;
}

BOOL CProcess::InjectDLL(CString dllpath)
{
	HANDLE hThread = 0;
	HANDLE hProcess;
	void* pLibRemote;   // The address (in the remote process)
	// where szLibPath will be copied to;

	if (!bInjected) {

		// Tomamos el path completo
		wcsncpy_s(szLibPath, _MAX_PATH, (LPCWSTR)dllpath, _MAX_PATH);

		hProcess = OpenProcess(PROCESS_CREATE_THREAD|PROCESS_QUERY_INFORMATION|PROCESS_VM_OPERATION|PROCESS_VM_WRITE|PROCESS_VM_READ, FALSE, dwPID);

		if (NULL != hProcess ) {

			// Inyectamos
			// 1. Allocate memory in the remote process for szLibPath
			// 2. Write szLibPath to the allocated memory
			pLibRemote = ::VirtualAllocEx(hProcess, NULL, sizeof(szLibPath), MEM_COMMIT, PAGE_READWRITE);
			SIZE_T dwWritten;
			BOOL r = ::WriteProcessMemory(hProcess, pLibRemote, (void*)szLibPath, sizeof(szLibPath),&dwWritten);

			// Load DLL into the remote process
			// (via CreateRemoteThread & LoadLibrary)
			HMODULE hUserDLL = GetModuleHandle(L"kernel32.dll");
			LPVOID  LoadLibAddr = GetProcAddress(hUserDLL, "LoadLibraryW");
			hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibAddr, pLibRemote, 0, (LPDWORD)&hThread);
			::WaitForSingleObject(hThread, INFINITE);

      // Get handle of the loaded module
			if (!::GetExitCodeThread(hThread, &hLibModule)) {
        TRACE("GetExitCodeThread error %Xh\n", GetLastError());
      }
      TRACE("Remote DLL handle %Xh\n", hLibModule);

			// Clean up
			::CloseHandle(hThread);
			::VirtualFreeEx(hProcess, pLibRemote, sizeof(szLibPath),MEM_RELEASE);

			bInjected = (hLibModule != NULL);

			CloseHandle( hProcess );
		}
		else
			bInjected = FALSE;
	}

	return bInjected;
}

BOOL CProcess::RemoveDLL()
{
	HANDLE hThread;
	HANDLE hProcess;

	if (bInjected) {

		hProcess = OpenProcess(PROCESS_CREATE_THREAD|PROCESS_QUERY_INFORMATION|PROCESS_VM_OPERATION|PROCESS_VM_WRITE|PROCESS_VM_READ, FALSE, dwPID);

		if (NULL != hProcess ) {

			// Unload DLL from the target process
			// (via CreateRemoteThread & FreeLibrary)
			HMODULE hUserDLL = GetModuleHandle(L"kernel32.dll");
			LPVOID  FreeLibAddr = GetProcAddress(hUserDLL, "FreeLibrary");
			hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)FreeLibAddr, (LPVOID)hLibModule, 0, (LPDWORD)&hThread);
			::WaitForSingleObject( hThread, INFINITE );
      
      DWORD ret;
  		::GetExitCodeThread(hThread, &ret);
      TRACE("FreeLibrary returned %Xh\n", ret);

			// Clean up
			::CloseHandle( hThread );
			::CloseHandle( hProcess );

			bInjected = FALSE;
		}
		else
			bInjected = TRUE;
	}

	return bInjected;
}

BOOL CProcess::ReadEnvironmentBlock(CString &data)
{
// No funciona en 32 bits
#ifndef _M_X64
	return FALSE;
#endif

  BOOL ret = FALSE;

  if (bZombie)
	  return FALSE;

  HMODULE ntdll = LoadLibrary(L"ntdll.dll");

  if (ntdll == NULL)
    return FALSE;

  TQIP QueryInformationProcess = (TQIP) GetProcAddress(ntdll, "NtQueryInformationProcess");

 	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
  
	if (NULL != hProcess && QueryInformationProcess != NULL) {

    // Buffer to hold the string read from process
    UCHAR* pchBuffEnvString = 0;
    try
    {
        PROCESS_BASIC_INFORMATION stBasiProcInfo;
        ULONG uReturnLength = 0;
        NTSTATUS ntStat = QueryInformationProcess( hProcess,
            ProcessBasicInformation,
            &stBasiProcInfo,
            sizeof(stBasiProcInfo),
            &uReturnLength );
 
        // Read the process environment block
        PEB peb = { 0 };
        SIZE_T nReturnNumBytes = 0;
        // Check read access of specified location in the processs 
        // and get the size of block
        ReadProcessMemory( hProcess,(LPCVOID)stBasiProcInfo.PebBaseAddress, &peb, sizeof(peb), &nReturnNumBytes );

        if (nReturnNumBytes == 0)
          throw 1;

        // Get the address of RTL_USER_PROCESS_PARAMETERS structure
        SIZE_T nReadbleSize = 0;
        if( !HasReadAccess( hProcess, peb.ProcessParameters, nReadbleSize ))
        {
          throw 2;
        }
        // Get the first 0x64 bytes of RTL_USER_PROCESS_PARAMETERS strcuture
        RTL_USER_PROCESS_PARAMETERS_DRK RTLUserPP;
        ReadProcessMemory( hProcess,(LPCVOID)peb.ProcessParameters, &RTLUserPP, sizeof(RTLUserPP) , &nReturnNumBytes );

        // Validate the read operation
        if( !nReturnNumBytes )
        {
          throw 3;
        }
        // Get the value at offset 0x48 to get the pointer to environment string block
        UCHAR* pAddrEnvStrBlock = (UCHAR*)RTLUserPP.Environment;
        if( !HasReadAccess( hProcess, pAddrEnvStrBlock, nReadbleSize ))
        {
          throw 4;
        }
        // Allocate buffer for to copy the block
        pchBuffEnvString = new UCHAR[nReadbleSize*2];
        //memset( pchBuffEnvString, 0, sizeof( UCHAR )* nReadbleSize * 2);
        // Read the environment block
        ReadProcessMemory( hProcess,(LPCVOID)pAddrEnvStrBlock,
            pchBuffEnvString, nReadbleSize , &nReturnNumBytes );

        if( nReturnNumBytes )
        {
          SIZE_T dl = 0;
          int lsl=1;
          while(dl/2 < nReturnNumBytes-4 && lsl > 0) {
            lsl = data.GetLength();

			      data.Append((WCHAR*)pchBuffEnvString+dl);
            lsl = data.GetLength() - lsl;
            data += L"\r\n";
            dl += lsl+1; 
          }

          ret = TRUE;
        }
        else
        {
            //SHOW_ERR_DLG(_T("Error Reading Process Memory"));
        }
		delete[] pchBuffEnvString;
    }
    catch(int r)
    {
      TRACE("Error %d", r);
		  r = FALSE; // Sólo por el warning
    }

    CloseHandle(hProcess);
    FreeLibrary(ntdll); 
  }

  return ret;
}

BOOL CProcess::HasReadAccess( HANDLE hProcess, void* pAddress, SIZE_T& nSize )
{
  MEMORY_BASIC_INFORMATION memInfo;
  try
  {
    SIZE_T r = VirtualQueryEx( hProcess, pAddress, &memInfo, sizeof(memInfo));
    TRACE("VirtualQueryEx -> %d\r\n", r);
    TRACE("memInfo.Protect -> %d\r\n", memInfo.Protect);
    TRACE("memInfo.RegionSize -> %d\r\n", memInfo.RegionSize);
    if( r == 0 || 
      PAGE_NOACCESS == memInfo.Protect ||
      PAGE_EXECUTE == memInfo.Protect )
    {
      nSize = 0;
      return FALSE;
    }
    nSize = memInfo.RegionSize;
    return TRUE;
  }
  catch(...)
  {
  }
  return FALSE;
} 

int CProcess::ReadAllModules()
{
  HMODULE hMods[1024];
  HANDLE hProcess;
  DWORD cbNeeded;
  unsigned int i;
	WCHAR szModName[MAX_PATH];
	CString str;

    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID);

    if (NULL != hProcess) {

		if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
			
			for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {

				if (GetModuleFileNameEx( hProcess, hMods[i], szModName, sizeof(szModName))) {

					// Ponemos esto porque hay un error reportado en la versión
					// 1.05 sobre la dirección 0x00421922 y posiblemente haya explotado
					// este buffer.
					szModName[MAX_PATH-1] = 0;

					str.Format(L"%s [%08Xh]\r\n", szModName, hMods[i]);
					strModules += str;
				}
			}
		}

		CloseHandle( hProcess );
	}

	return 0;
}

size_t CProcess::GetThreadCount()
{
	return threads.size();
}

BOOL CProcess::SetPriority(DWORD pclass)
{
  HANDLE hProcess;
  BOOL result = FALSE;

  hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

  if (NULL != hProcess) {

		result = SetPriorityClass(hProcess, pclass);
		CloseHandle( hProcess );
	}

	return result;
}

void CProcess::readPSAPI(HANDLE snapshot, const ProcInfoMap &processes)
{
	PROCESSENTRY32 pentry;

	if ( snapshot != INVALID_HANDLE_VALUE ) {
		if (locateEntryPSAPI(snapshot, &pentry)) {

			// Parent
			dwParentPID = pentry.th32ParentProcessID;
			if (processes.find(dwParentPID) != processes.end())
				strParentName = (processes.find(dwParentPID)->second)->strName;
		}
	}
}

bool CProcess::locateEntryPSAPI(HANDLE snapshot, PROCESSENTRY32* pentry)
{
	// Set the size of the structure before using it.
	pentry->dwSize = sizeof( PROCESSENTRY32 );

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if( !Process32First( snapshot, pentry ) ) {
		return false;
	}

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do {
		if (dwPID == pentry->th32ProcessID)
			return true;
	}
	while( Process32Next( snapshot, pentry ) );

	return false;
}

LPTHREAD_START_ROUTINE CProcess::getStdFuncionAddr(LPCSTR name)
{
  return (LPTHREAD_START_ROUTINE)::GetProcAddress(::GetModuleHandle(L"Kernel32"), name);
}


BOOL CProcess::getWorkingSetSize(PSIZE_T lpMinimumWorkingSetSize, PSIZE_T lpMaximumWorkingSetSize)
{
  HANDLE hProcess;
  BOOL result = FALSE;

  hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

  if (NULL != hProcess) {

		result = ::GetProcessWorkingSetSize(hProcess, lpMinimumWorkingSetSize, lpMaximumWorkingSetSize);
		CloseHandle( hProcess );
	}

	return result;
}


BOOL CProcess::setWorkingSetSize(SIZE_T dwMinimumWorkingSetSize, SIZE_T dwMaximumWorkingSetSize)
{
  HANDLE hProcess;
  BOOL result = FALSE;

  hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

  if (NULL != hProcess) {

		result = ::SetProcessWorkingSetSize(hProcess, dwMinimumWorkingSetSize, dwMaximumWorkingSetSize);
		CloseHandle( hProcess );
	}

	return result;
}


DWORD CProcess::waitForInputIdle(DWORD milliseconds)
{
  HANDLE hProcess;
  DWORD result = WAIT_FAILED;

  hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

  if (NULL != hProcess) {

		result = ::WaitForInputIdle(hProcess, milliseconds);
		CloseHandle( hProcess );
	}

	return result;
}


BOOL CProcess::emptyWorkingSet(void)
{
  HANDLE hProcess;
  BOOL result = FALSE;

  hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

  if (NULL != hProcess) {

		result = ::EmptyWorkingSet(hProcess);
		CloseHandle( hProcess );
	}

	return result;
}
