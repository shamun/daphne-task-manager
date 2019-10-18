/***********************************************************************************
	
	Daphne -- GPL task manager replacement for Windows
	Copyright (C) 2014  Leandro H. Fernández

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

// ProcessList.cpp: implementation of the CProcessList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Daphne.h"
#include "DaphneBase.h"
#include "ProcessList.h"
#include "Process.h"
#include "DaphneDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSCManager * CProcessList::scm = NULL;

CProcessList::CProcessList(CDaphneDlg * daphne, CSCManager * pscm) : daphne(daphne)
{
	CProcessList::scm = pscm;


	processes.clear();

	it = processes.end();
	hRefreshMutex = ::CreateMutex(NULL, FALSE, NULL);
	hRefreshEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
}

CProcessList::~CProcessList()
{
	RemoveAll();
	::CloseHandle(hRefreshEvent);
	::CloseHandle(hRefreshMutex);
}

int CProcessList::updateList()
{
	DWORD aProcesses[1024], cbNeeded, cProcesses;
  unsigned int i;
	DWORD dwPID;
	ULARGE_INTEGER tt;
	HANDLE hProcessSnap;

	// Calculo el tiempo que pasó sin esta parte del código
	FILETIME systemTimeAsFileTime;
	
	GetSystemTimeAsFileTime(&systemTimeAsFileTime);
	tt.HighPart = systemTimeAsFileTime.dwHighDateTime;
	tt.LowPart = systemTimeAsFileTime.dwLowDateTime; 
	
	// Solicito la lista de procesos al sistema
  if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    return 1; 

  cProcesses = cbNeeded / sizeof(DWORD);

  // Busco los muertos
  ProcInfoMap::iterator k;
  for (k=processes.begin(); k != processes.end(); k++) {
    bool muerto = true;
    if ((*k->second).isAlive()) {
      for (i=0; i<cProcesses; i++)
        if (aProcesses[i] == (*k->second).dwPID) {
          muerto = false;
          break;
        }
      // Muerto
      if (muerto) {
        DeadProcessNotify((*k->second).dwPID, &(*k->second).strName);
      }
    }
  }

  for (i=0; i<cProcesses; i++) {

		dwPID = aProcesses[i];
    

    ProcInfoMap::iterator found = processes.find(dwPID);
    if (found == processes.end() || !(found->second)->isAlive()) {

      TRACE(L"+%u\n", dwPID);
			// Es un proceso nuevo
			processes[dwPID] = new CProcess(dwPID, scm);

			// Notifico que hay un nuevo proceso
			NewProcessNotify(dwPID);
		}
	}

	try {
		// Obtengo el snapshot para información extra
		hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	}
	catch(...) {
		hProcessSnap = INVALID_HANDLE_VALUE;
	}

	// Refresco los datos y por último el IDLE
  usedTime = 0;
	CProcess * idle = NULL;
	ProcInfoMap::iterator j;
	for(j = processes.begin(); j != processes.end(); j++) {
		if ((j->second)->dwPID != 0) {
			(j->second)->ReadProcessInformation(FALSE, this, (__int64)tt.QuadPart);
      usedTime += ((j->second)->liCPUTimeTotal - (j->second)->liCPUTimeLast);
			(j->second)->threads.clear(); // Para luego completar la info

			if (hProcessSnap != INVALID_HANDLE_VALUE)
				(j->second)->readPSAPI(hProcessSnap, processes);
		}
		else {
			idle = (j->second);
		}
	}
	idle->ReadProcessInformation(FALSE, this, (__int64)tt.QuadPart * sysinfo->dwNumberOfProcessors);

	if (hProcessSnap != INVALID_HANDLE_VALUE) {
		CloseHandle( hProcessSnap );
	}

	// Completo la info de threads
	AddThreadInformation();

	return 0;
}

void CProcessList::NewProcessNotify(DWORD pid) {
// Para que no falle DC
#ifndef NO_DAPHNE_DLG
  daphne->OnNewProcess(pid);
#endif
}

void CProcessList::DeadProcessNotify(DWORD pid, CString * name) {
// Para que no falle DC
#ifndef NO_DAPHNE_DLG
  daphne->OnDeadProcess(pid, *name);
#endif
}

void CProcessList::RemoveAll()
{
	ProcInfoMap::iterator i;
	for(i = processes.begin(); i != processes.end(); i++) {
		delete i->second;
	}
	processes.clear();
}

CProcess * CProcessList::getFirstProcess()
{
	it = processes.begin();

	if (it != processes.end())
		return it->second;

	return NULL;
}

CProcess * CProcessList::getNextProcess()
{
	// Si el iterador está inicializado
	if (it != processes.end()) {

		// Proximo
		it++;

		if (it != processes.end())
			return it->second;
	}

	return NULL;
}

CProcess * CProcessList::getProcess(DWORD pid)
{
	if (processes.find(pid) != processes.end())
		return processes.find(pid)->second;
	else
		return NULL;
}

void CProcessList::purgeInvalidProcesses()
{
	std::set<DWORD> deaths;

	ProcInfoMap::iterator i;
	for(i = processes.begin(); i != processes.end(); i++) {
		if (!((CProcess*)i->second)->isAlive()) {
			deaths.insert(i->first);
		}
	}

	std::set<DWORD>::iterator j;
	for(j = deaths.begin(); j != deaths.end(); j++) {
		i = processes.find((DWORD)*j);
		delete ((CProcess*)i->second);
		processes.erase(i->first);
	}

}

// Esta función se lanza en un thread aparte
UINT CProcessList::staticUpdate(LPVOID pParam) {
  BOOL end = FALSE;
	CProcessList * pList = (CProcessList*)pParam;

	do {
		if (WaitForSingleObject(hRefreshMutex, INFINITE) == WAIT_OBJECT_0) {
			
			if (WaitForSingleObject(hRefreshEvent, INFINITE) == WAIT_OBJECT_0)
				pList->updateList();

			ReleaseMutex(hRefreshMutex);
		}

    // Pausa 875 milisegundos
    end = WaitForSingleObject(hRefreshEnd, 875) == WAIT_OBJECT_0;
	}
	while(!end);

	return 0;
}


void CProcessList::setSCM(CSCManager *p)
{
	CProcessList::scm = p;
}

void CProcessList::setSysInfo(SYSTEM_INFO * sysinfo)
{
  this->sysinfo = sysinfo;
}

__int64 CProcessList::getIdleCPUTime()
{
	__int64 total = 0;
	ProcInfoMap::iterator i;

	CProcess * idle = getProcess(0);

	// BUG in 1.25, idle may be null
	if (idle == NULL)
		return 0;

	total = idle->liCPUTimeTotal - idle->liCPUTimeLast;

	return total;
}

__int64 CProcessList::getUsedCPUTime()
{
  return usedTime;
}

void CProcessList::getCPUTimes(__int64 &idle, __int64 &used)
{
	ProcInfoMap::iterator i;

	CProcess * idlep = getProcess(0);

	// BUG in 1.25, idle may be null
  if (idlep == NULL) {
    idle = used = 0;
		return;
  }

  idle = (idlep->liCPUTimeTotal - idlep->liCPUTimeLast);

  used = 0;
	// Sumo todos los procesos
	if (processes.size() > 0) {
		for(i = processes.begin(); i != processes.end(); i++) {

			CProcess * p = (CProcess*)i->second;
			if (p != idlep) {
				used += (p->liCPUTimeTotal - p->liCPUTimeLast);
			}
		}
	}
}

int CProcessList::AddThreadInformation()
{
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE; 
	THREADENTRY32 te32; 
	int count = 0;
 
	// Take a snapshot of all running threads  
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if(hThreadSnap == INVALID_HANDLE_VALUE) 
		return 0; 

	// Fill in the size of the structure before using it. 
	te32.dwSize = sizeof(THREADENTRY32);

	// Retrieve information about the first thread,
	// and exit if unsuccessful
	if(!Thread32First(hThreadSnap, &te32)) {
		//printError( "Thread32First" );  // Show cause of failure
		CloseHandle( hThreadSnap );     // Must clean up the snapshot object!
		return 0;
	}

	// Now walk the thread list of the system,
	// and display information about each thread
	// associated with the specified process
	
	do {
		if (processes.find(te32.th32OwnerProcessID) != processes.end())
			processes[te32.th32OwnerProcessID]->threads.push_back(CThread(&te32));
	}
	while(Thread32Next(hThreadSnap, &te32));

	//  Don't forget to clean up the snapshot object.
	CloseHandle(hThreadSnap);

	return count;
}

DWORD CProcessList::Find(CString find)
{
	CProcess * proc = getFirstProcess();

	find.MakeLower();

	while(proc != NULL) {

		CString name = proc->strName;
		name.MakeLower();

		if (name.Find(find) == 0) {
			return proc->dwPID;
		}

		proc = getNextProcess();
	}

	return 0;
}

void CProcessList::setJavaHome(const WCHAR * home)
{
	CProcess::strJavaHome = home;
}
