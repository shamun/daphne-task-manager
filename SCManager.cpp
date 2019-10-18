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

// SCManager.cpp: implementation of the CSCManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DaphneBase.h"
#include "SCManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSCManager::CSCManager()
{
	bRunning = FALSE;
	bCanExecute = FALSE;
	hSCM = NULL;
	hCurrentService = NULL;

	Open();
	ReadServicesMap();
	Close();
}

CSCManager::~CSCManager()
{
	Close();
}

int CSCManager::Open()
{
	ASSERT(hSCM == NULL);

	if ((hSCM = OpenSCManager(NULL, NULL, GENERIC_ALL)) != NULL) {
		bCanExecute = TRUE;
		dwDefaultAccess = GENERIC_ALL;
	}
	else if (GetLastError() == ERROR_ACCESS_DENIED) {
		hSCM = OpenSCManager(NULL, NULL, GENERIC_READ);
		dwDefaultAccess = GENERIC_READ;
	}

	if (hSCM == NULL)
		return 1;

	bRunning = TRUE;

	return 0;
}

void CSCManager::Close()
{
	CloseService();

	::CloseServiceHandle(hSCM);
	hSCM = NULL;

	bCanExecute = FALSE;
	bRunning = TRUE;
}

int CSCManager::OpenService(DWORD pid)
{
	return OpenService(services[pid].c_str());
}

int CSCManager::OpenService(CString name)
{
	if ((hCurrentService = ::OpenService(hSCM, name, dwDefaultAccess)) == NULL)
		hCurrentService = ::OpenService(hSCM, name, GENERIC_READ);

	if (hCurrentService == NULL)
		return 1;

	return 0;
}

void CSCManager::CloseService()
{
	if (hCurrentService != NULL)
		::CloseServiceHandle(hCurrentService);

	hCurrentService = NULL;
}

void CSCManager::ReadServicesMap()
{
	DWORD cbBytesNeeded;
	DWORD cbServicesReturned;
	DWORD hResumeHandle = 0;
	DWORD dwResult;
	ENUM_SERVICE_STATUS_PROCESS vService[65535/sizeof(ENUM_SERVICE_STATUS_PROCESS)];

	services.clear();
	status.clear();

	EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, 
						NULL, 0, &cbBytesNeeded, &cbServicesReturned, &hResumeHandle, NULL);


	do {
		
		dwResult = 0;
		if (!EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, 
								(BYTE*)&vService, sizeof(vService), &cbBytesNeeded, &cbServicesReturned,
								&hResumeHandle, NULL)) {
			dwResult = GetLastError();
		}


		for(DWORD i=0; i<cbServicesReturned; i++) {

			// Posiblemente eso corrija un bug reportado sobre esta clase
			if (vService[i].ServiceStatusProcess.dwProcessId > 0) {
				services[vService[i].ServiceStatusProcess.dwProcessId] = vService[i].lpServiceName;
				status[vService[i].lpServiceName] = vService[i].ServiceStatusProcess;
			}
		}

	}
	while(dwResult == ERROR_MORE_DATA);


}

BOOL CSCManager::isService(DWORD pid)
{
	return (services.find(pid) != services.end());
}


BOOL CSCManager::StartService()
{
	if (hCurrentService != NULL) {
		return ::StartService(hCurrentService, 0, NULL);
	}

	return FALSE;
}

BOOL CSCManager::StopService()
{
	SERVICE_STATUS status;

	if (hCurrentService != NULL) {
		return ControlService(hCurrentService, SERVICE_CONTROL_STOP, &status);
	}

	return FALSE;
}

BOOL CSCManager::PauseService()
{
	SERVICE_STATUS status;

	if (hCurrentService != NULL) {
		return ControlService(hCurrentService, SERVICE_CONTROL_PAUSE, &status);
	}

	return FALSE;
}

BOOL CSCManager::ContinueService()
{
	SERVICE_STATUS status;

	if (hCurrentService != NULL) {
		return ControlService(hCurrentService, SERVICE_CONTROL_CONTINUE, &status);
	}

	return FALSE;
}

/*
 * Se tiene que utilizar con la conexión abierta
 */
DWORD CSCManager::getState(DWORD pid)
{
	ReadServicesMap();

	if (services.find(pid) == services.end())
		return 0;

	LPCWSTR name = services[pid].c_str();

	if (status.find(name) == status.end())
		return 0;

	return status[name].dwCurrentState;
}

/*
 * Se tiene que utilizar con la conexión abierta
 */
DWORD CSCManager::getState(CString name)
{
	ReadServicesMap();

	if (status.find((LPCWSTR)name) == status.end())
		return 0;

	return status[(LPCWSTR)name].dwCurrentState;
}
