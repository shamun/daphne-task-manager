// DRKShell.cpp : Implementation of DLL Exports.

//
// Note: COM+ 1.0 Information:
//      Please remember to run Microsoft Transaction Explorer to install the component(s).
//      Registration is not done by default. 

#include "stdafx.h"
#include "resource.h"
#include "DRKShell.h"
#include "DRKShellMenu.h"
#include "compreg.h"
#include "logmessages.h"

#include <Psapi.h>
#include <RestartManager.h>

class CDRKShellModule : public CAtlDllModuleT< CDRKShellModule >
{
public :
	DECLARE_LIBID(LIBID_DRKShellLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DRKSHELL, "{EC080A38-A5F5-4B36-BAE8-2D64BD8AB517}")
};

CDRKShellModule _AtlModule;
HANDLE hEventLog = NULL;

#ifdef _MANAGED
#pragma managed(push, off)
#endif

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
  		setlocale(LC_ALL, "");
		bindtextdomain("DAPHNE", "LANG");
		bind_textdomain_codeset("DAPHNE", "UTF-8");
		textdomain("DAPHNE");

		hEventLog = RegisterEventSource(NULL, L"Daphne.DRKShell");
	break;

	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		DeregisterEventSource(hEventLog);
		break;
	}

	return _AtlModule.DllMain(dwReason, lpReserved); 
}

#ifdef _MANAGED
#pragma managed(pop)
#endif




// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}

// Si el usuario guarda el puntero y llama de nuevo a esta
// función antes de usarlo, perderá el valor anterior
// No es thread safe
const WCHAR * UTF8_WC(const char * s, WCHAR * stg, int size) {
  static WCHAR text[16384] = L""; // Inicia vacio
  static HANDLE textMutext = CreateMutex(NULL, FALSE, NULL);

  AutoReleaseMutex arm(textMutext);

  if (stg == NULL) {
    MultiByteToWideChar(CP_UTF8,
                0,
                s,
                -1,
                text,
                sizeof(text));

    return text;
  }
  else {
    MultiByteToWideChar(CP_UTF8,
                0,
                s,
                -1,
                stg,
                size);

    return stg;
  }
}

CString whoIsUsing(CString file)
{
  CString result;
  DWORD dwSession;
  WCHAR szSessionKey[CCH_RM_SESSION_KEY+1] = { 0 };
  LPCTSTR pInsertStrings[2] = {NULL, NULL};

  DWORD dwError = RmStartSession(&dwSession, 0, szSessionKey);
  TRACE(L"RmStartSession returned %d\n", dwError);

  if (dwError == ERROR_SUCCESS) {
    PCWSTR pszFile = file;
    dwError = RmRegisterResources(dwSession, 1, &pszFile, 0, NULL, 0, NULL);
    TRACE(L"RmRegisterResources(%ls) returned %d\n", pszFile, dwError);

    if (dwError == ERROR_SUCCESS) {
      DWORD dwReason;
      UINT i;
      UINT nProcInfoNeeded;
      UINT nProcInfo = 10;
      RM_PROCESS_INFO rgpi[10];

      dwError = RmGetList(dwSession, &nProcInfoNeeded, &nProcInfo, rgpi, &dwReason);
      TRACE(L"RmGetList returned %d\n", dwError);

      if (dwError == ERROR_SUCCESS) {
        TRACE(L"RmGetList returned %d infos (%d needed)\n", nProcInfo, nProcInfoNeeded);

        for (i = 0; i < nProcInfo; i++) {
          TRACE(L"%d.ApplicationType = %d\n", i, rgpi[i].ApplicationType);
          TRACE(L"%d.strAppName = %ls\n", i, rgpi[i].strAppName);
          TRACE(L"%d.Process.dwProcessId = %d\n", i, rgpi[i].Process.dwProcessId);

          HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, rgpi[i].Process.dwProcessId);
          if (hProcess) {
            FILETIME ftCreate, ftExit, ftKernel, ftUser;
            if (GetProcessTimes(hProcess, &ftCreate, &ftExit, &ftKernel, &ftUser) &&
              CompareFileTime(&rgpi[i].Process.ProcessStartTime, &ftCreate) == 0) {
                WCHAR sz[MAX_PATH];
                DWORD cch = MAX_PATH;

                GetProcessImageFileName(hProcess, sz, cch);
                result.Append(sz);
                result.Append(L"\n");
            }
            CloseHandle(hProcess);
          }
        }
      }
    }
    RmEndSession(dwSession);
  }
  else {
    result = _("Can't open restart manager session.");
    CString err;
    err.Format(L"RmStartSession failed with error: %08Xh", dwError);
    pInsertStrings[0] = (LPCTSTR)err;
    ReportEvent(hEventLog, EVENTLOG_ERROR_TYPE, BASE_CATEGORY, MSG_RESTART_MNGR_ERROR, NULL, 1, 0, (LPCWSTR*)pInsertStrings, NULL);
  }

  if (result.IsEmpty())
    result = _("This file is not in use.");

  return result;
}