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

// SCManager.h: interface for the CSCManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCMANAGER_H__22C5D267_B05E_4CE5_A04C_3C55677D9CF3__INCLUDED_)
#define AFX_SCMANAGER_H__22C5D267_B05E_4CE5_A04C_3C55677D9CF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef std::map<DWORD, std::wstring> MapPID2Service;
typedef std::map<std::wstring, SERVICE_STATUS_PROCESS> MapService2Status;

class CSCManager  
{
public:
	DWORD getState(CString name);
	DWORD getState(DWORD pid);
	BOOL ContinueService();
	BOOL PauseService();
	BOOL StopService();
	BOOL StartService();
	int OpenService(DWORD pid);
	BOOL isService(DWORD pid);
	void ReadServicesMap();
	void CloseService();
	int OpenService(CString name);
	void Close();
	int Open();
	CSCManager();
	virtual ~CSCManager();

private:
	SC_HANDLE hSCM;
	SC_HANDLE hCurrentService;
	BOOL bCanExecute;
	BOOL bRunning;
	DWORD dwDefaultAccess;
	MapPID2Service services;
	MapService2Status status;
};

#endif // !defined(AFX_SCMANAGER_H__22C5D267_B05E_4CE5_A04C_3C55677D9CF3__INCLUDED_)
