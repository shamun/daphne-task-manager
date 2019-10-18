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

// Thread.h: interface for the CThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREAD_H__07D8440C_2646_4C32_A591_3AF3DE3628C4__INCLUDED_)
#define AFX_THREAD_H__07D8440C_2646_4C32_A591_3AF3DE3628C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tlhelp32.h>

class CThread  
{
public:
	static __int64 GetThreadCPUTime(DWORD id);
	static CString GetPriorityString(int priority);
	static int GetPriority(DWORD id);
	static BOOL SetPriority(DWORD id, int priority);
	static BOOL ResumeThread(DWORD id);
	static BOOL SuspendThread(DWORD id);
	static BOOL TerminateThread(DWORD id);
	CString GetPriority();
	CString toString();
	CThread(LPTHREADENTRY32 entry);
	DWORD dwID;
    DWORD  cntUsage; 
    DWORD  dwOwnerProcessID; 
    LONG   tpBasePri; 
    LONG   tpDeltaPri; 
    DWORD  dwFlags;
	CThread();
	virtual ~CThread();

};

#endif // !defined(AFX_THREAD_H__07D8440C_2646_4C32_A591_3AF3DE3628C4__INCLUDED_)
