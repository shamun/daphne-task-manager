// ProcessList.h: interface for the CProcessList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCESSLIST_H__3759C4D6_687E_4890_81E8_3FB51D5FF3A1__INCLUDED_)
#define AFX_PROCESSLIST_H__3759C4D6_687E_4890_81E8_3FB51D5FF3A1__INCLUDED_

#include "DaphneBase.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDaphneDlg;

class CProcessList  
{
public:
	void setJavaHome(const WCHAR * home);
	DWORD Find(CString find);
	int AddThreadInformation();
	__int64 getIdleCPUTime();
	__int64 getUsedCPUTime();
  void getCPUTimes(__int64 &idle, __int64 &used);
	void setSCM(CSCManager * p);
  void setSysInfo(SYSTEM_INFO * sysinfo);
	void purgeInvalidProcesses();
	CProcess * getProcess(DWORD pid);
	CProcess * getNextProcess();
	CProcess * getFirstProcess();
	ProcInfoMap processes;
	int updateList();
	CProcessList(CDaphneDlg * daphne = NULL, CSCManager * pscm = NULL);
	void NewProcessNotify(DWORD pid);
  void DeadProcessNotify(DWORD pid, CString * name);
	virtual ~CProcessList();

	static UINT staticUpdate(LPVOID pParam);
	static CSCManager * scm;

protected:
	void RemoveAll();
private:
  SYSTEM_INFO * sysinfo;
	ProcInfoMap::iterator it;
  __int64 usedTime;
  CDaphneDlg * daphne;
};

#endif // !defined(AFX_PROCESSLIST_H__3759C4D6_687E_4890_81E8_3FB51D5FF3A1__INCLUDED_)
