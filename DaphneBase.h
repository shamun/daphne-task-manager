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

// DaphneBase.h : definiciones básicas de Daphne
//

#ifndef __DAPHNE_BASE_H__
#define __DAPHNE_BASE_H__

#define WM_ICON_NOTIFY WM_APP + 1
#define WM_DRK_SHOW WM_APP + 2
#define WM_DRK_ABOUT WM_APP + 3
#define WM_DRK_QUIT WM_APP + 4
#define WM_DRK_NEWPROC WM_APP + 5
#define WM_DRK_GOOGLE WM_APP + 6
#define WM_DRK_DEADPROC WM_APP + 7
#define WM_DRK_JUST_STARTED WM_APP + 8
#define WM_DRK_DAPHNESTAT WM_APP + 9
#define WM_DRK_MD_CAPTURE WM_APP + 10
#define WM_DRK_MD_SWITCH WM_APP + 11
#define WM_DRK_RS1 (WM_APP + 12)
#define WM_DRK_RS2 (WM_APP + 13)
#define WM_DRK_RS3 (WM_APP + 14)
#define WM_DRK_RS4 (WM_APP + 15)
#define WM_DRK_DAPHNESUPPORT WM_APP + 16
#define WM_DRK_DAPHNEFAQ WM_APP + 17

extern "C" {
#include <libintl.h>
}

#define _(String) UTF8_WC( gettext (String) )
#define __(String, Storage, Size) UTF8_WC( gettext (String), Storage, Size )
#define gettext_noop(String) String
#define N_(String) gettext_noop (String)

#define DAPHNE_MAXPATH 1024
#define DAPHNE_MAX_TOOLTIP_SIZE 256
#define DAPHNE_UNDEFINEDNAME L"<?>"
#define DAPHNE_ZOMBIE L"<Zombie>"
#define DAPHNE_INI_FILE L".\\Daphne.INI"
#define DAPHNE_MAX_QUEUE_CPUDATA 496

#define DAPHNE_HASH_BUFSIZE 1024
#define DAPHNE_MD5LEN  16
#define DAPHNE_SHA1LEN 40

extern OSVERSIONINFO OSVersion;
// Mutex
extern HANDLE hRefreshMutex;
extern HANDLE hRefreshEvent;
extern HANDLE hWebUpdateEnd;
extern HANDLE hRefreshEnd;
extern HANDLE hUpTimeMutex;
extern HANDLE hTrapsMutex;
extern HANDLE hHiddenMutex;

#include <psapi.h>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <queue>
#include <set>
#include <winuser.h>
#include <tlhelp32.h>
#include <winsvc.h>

#include "Thread.h"

typedef std::map<HWND, HTREEITEM> TreeItemParents;
typedef std::map<std::wstring, DWORD> MsgMap;
typedef std::vector<std::wstring> ProcNameList;
typedef std::map<HWND, CWnd*> MapWindows;
typedef std::vector<CThread> ThreadList;
typedef std::vector<HWND> ListHWND;
typedef std::map<INT, ListHWND> DesktopMap;
typedef std::list<int> CPUUsageQueue;

class Task;
typedef std::map<CTime, std::list<Task*> > TaskMap;

class CProcess;
typedef std::map<DWORD, CProcess*> ProcInfoMap;

#include "SCManager.h"
#include "Process.h"
#include "ProcessList.h"
#include "Trap.h"

typedef std::vector<CTrap*> TrapList;

void HighlightFoundWindow(HWND hwndWindow);
void RemoveHighlight(HWND hwndWindow);
CString StringGetWord(std::wstring str, unsigned int pos);
bool EnableDebugPriv(void);
PROCESS_INFORMATION OpenProc( TCHAR *CmdLine, const WCHAR *Desktop);
void CaptureScreen( HWND hWndStatic , HWND hWndPreview);
HICON getIconFromWindow(HWND hWnd);
INT getFullDay();
void ShowMsgLastError(HWND hwnd = NULL);
HRESULT taskBar(HWND hWnd, int action);
BOOL isWinXP();
const WCHAR * UTF8_WC(const char * s, WCHAR * stg = NULL, int size = 0);
int KillByName(CString process_name, BOOL polite = TRUE, BOOL case_sensitive = TRUE);
CString getProcessorType(SYSTEM_INFO &si);
DWORD getHashFor(UINT hashID, const TCHAR * filename, TCHAR * output, DWORD * output_size);

class AutoReleaseMutex {
private:
  HANDLE &mutex;
public:
  AutoReleaseMutex(HANDLE &mutref) : mutex(mutref) {
    WaitForSingleObject(mutex, INFINITE);
  };
  ~AutoReleaseMutex() {
    ReleaseMutex(mutex);
  };
};

// Template definition
template<class Content> class Tree
{
public:
  Tree(Content v) : value(v) {};
  ~Tree() { 
    std::vector<Tree*>::iterator i=children.begin();
    while(i != children.end()) { delete *i; i++; }
  };
  Tree * addChild(Content v) { Tree * n = new Tree(v); children.push_back(n); return n; };
  Tree * find(Content s, bool (*comparator)(Content, Content)) {
    std::vector<Tree*>::iterator i=children.begin();
    while(i != children.end()) { if ( (*comparator)((*i)->value, s)) return *i; i++; }
    Tree * r = NULL;
    i=children.begin();
    while(i != children.end()) { r = (*i)->find(s, comparator); if (r != NULL) return r; i++; }
    return NULL;
  };

  Content value;
  std::vector< Tree<Content> * > children;
};

Tree<CProcess*> * CreateProcessTree(ProcInfoMap processes);

class CDaphneDlg;

class Task {
public:
  CTime time;
  Task(CTime t) : time(t) {};
  virtual ~Task() {};
  virtual void storeConfiguration(UINT idx);
  virtual void restoreConfiguration(UINT idx);
  virtual void run(CDaphneDlg * dlg) = 0;
  virtual const CString toString() const = 0;
  virtual UINT getTrapType() const = 0;

  static Task * factory(UINT idx);
};

class KillTask : public Task {
private:
  DWORD pid;
  CString name;
  bool polite;
public:
  KillTask(CTime t, DWORD pid, CString name, bool polite) : Task(t), pid(pid), name(name), polite(polite) {};
  void run(CDaphneDlg * dlg);
  const CString toString() const;
  virtual UINT getTrapType() const { return 1; };
};

class SysShutdownTask : public Task {
private:
  UINT type;
public:
  SysShutdownTask(CTime t,UINT type) : Task(t), type(type) {};
  void run(CDaphneDlg * dlg);
  const CString toString() const;
  virtual UINT getTrapType() const { return 2; };
};

class PopupMessageTask : public Task {
private:
  CString message;
public:
  PopupMessageTask() : Task(0), message(L"") {};
  PopupMessageTask(CTime t, CString message) : Task(t), message(message) {};
  void run(CDaphneDlg * dlg);
  const CString toString() const;
  virtual void storeConfiguration(UINT idx);
  virtual void restoreConfiguration(UINT idx);
  virtual UINT getTrapType() const { return 3; };
};


// NtCreateThreadEx Undocumented

typedef LONG (WINAPI * LPFUN_NtCreateThreadEx) 
(
  OUT PHANDLE hThread,
  IN ACCESS_MASK DesiredAccess,
  IN LPVOID ObjectAttributes,
  IN HANDLE ProcessHandle,
  IN LPTHREAD_START_ROUTINE lpStartAddress,
  IN LPVOID lpParameter,
  IN BOOL CreateSuspended, 
  IN ULONG StackZeroBits,
  IN ULONG SizeOfStackCommit,
  IN ULONG SizeOfStackReserve,
  OUT LPVOID lpBytesBuffer
);
struct NtCreateThreadExBuffer
{
  ULONG Size;
  ULONG Unknown1;
  ULONG Unknown2;
  PULONG Unknown3;
  ULONG Unknown4;
  ULONG Unknown5;
  ULONG Unknown6;
  PULONG Unknown7;
  ULONG Unknown8;
};
#endif //__DAPHNE_BASE_H__