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

// Thread.cpp: implementation of the CThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DaphneBase.h"
#include "Thread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThread::CThread()
{

}

CThread::~CThread()
{

}

CThread::CThread(LPTHREADENTRY32 entry)
{
	dwID = entry->th32ThreadID;
	dwOwnerProcessID = entry->th32OwnerProcessID;
	dwFlags = entry->dwFlags;
	cntUsage = entry->cntUsage;
	tpBasePri = entry->tpBasePri;
	tpDeltaPri = entry->tpDeltaPri;
}

CString CThread::toString()
{
	CString str;
	str.Format(_("%08Xh - References %u - Priority %u/%u"), dwID, cntUsage, tpBasePri, tpDeltaPri);

	return str;
}

CString CThread::GetPriority()
{
	return CThread::GetPriorityString(CThread::GetPriority(dwID));
}

BOOL CThread::TerminateThread(DWORD id)
{
	BOOL result = FALSE;

	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, id);

	if (hThread != NULL) {

		result = ::TerminateThread(hThread, 0);

		CloseHandle(hThread);
	}

	return result;
}

BOOL CThread::SuspendThread(DWORD id)
{
	BOOL result = FALSE;

	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, id);

	if (hThread != NULL) {

		result = ::SuspendThread(hThread);

		CloseHandle(hThread);
	}

	return result;
}

BOOL CThread::ResumeThread(DWORD id)
{
	BOOL result = FALSE;

	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, id);

	if (hThread != NULL) {

		result = ::ResumeThread(hThread);

		CloseHandle(hThread);
	}

	return result;
}

BOOL CThread::SetPriority(DWORD id, int priority)
{
	BOOL result = FALSE;

	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, id);

	if (hThread != NULL) {

		result = ::SetThreadPriority(hThread, priority);

		CloseHandle(hThread);
	}

	return result;
}

int CThread::GetPriority(DWORD id)
{
	int result = 0;

	HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, id);

	if (hThread != NULL) {

		result = ::GetThreadPriority(hThread);

		CloseHandle(hThread);
	}

	return result;
}

CString CThread::GetPriorityString(int priority)
{
	switch(priority) {
	case THREAD_PRIORITY_ABOVE_NORMAL:
		return _("Above normal");
	case THREAD_PRIORITY_BELOW_NORMAL:
		return _("Below normal");
	case THREAD_PRIORITY_HIGHEST:
		return _("Highest");
	case THREAD_PRIORITY_IDLE:
		return _("IDLE");
	case THREAD_PRIORITY_LOWEST:
		return _("Lowest");
	case THREAD_PRIORITY_NORMAL:
		return _("Normal");
	case THREAD_PRIORITY_TIME_CRITICAL:
		return _("Time critical");
	}

	return _("N/A");
}

__int64 CThread::GetThreadCPUTime(DWORD id)
{
	int result = 0;
	__int64 TimeTotal = 0;
	FILETIME CreationTime;
	FILETIME ExitTime;
	FILETIME KernelTime;
	FILETIME UserTime;
	ULARGE_INTEGER temp;

	HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, id);

	if (hThread != NULL) {

		::GetThreadTimes(hThread, &CreationTime, &ExitTime, &KernelTime, &UserTime);

		temp.HighPart = KernelTime.dwHighDateTime;
		temp.LowPart = KernelTime.dwLowDateTime; 
		TimeTotal = (__int64)temp.QuadPart;

		temp.HighPart = UserTime.dwHighDateTime;
		temp.LowPart = UserTime.dwLowDateTime; 
		TimeTotal += (__int64)temp.QuadPart;

		CloseHandle(hThread);

	}

	return TimeTotal;

}
