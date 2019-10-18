#include "stdafx.h"
#include <string>
#include <vector>
#include <set>
#include <map>
#include <Wincrypt.h>
#include "DaphneBase.h"

OSVERSIONINFO OSVersion;
HANDLE hRefreshMutex;
HANDLE hRefreshEvent;
HANDLE hWebUpdateEnd;
HANDLE hRefreshEnd;
HANDLE hUpTimeMutex;
HANDLE hTrapsMutex;
HANDLE hHiddenMutex;

/////////////////////////////////////////////////////////////////////////////
// Funciones utilitarias


// Arma cadena de procesador
CString getProcessorType(SYSTEM_INFO &si)
{
  std::map<DWORD, CString> type;
  type[PROCESSOR_INTEL_386] = "386";
  type[PROCESSOR_INTEL_486] = "486";
  type[PROCESSOR_INTEL_PENTIUM] = "Pentium";
  type[PROCESSOR_INTEL_IA64] = "Itanium 64";
  type[PROCESSOR_AMD_X8664] = "AMD x86-64";

  return type[si.dwProcessorType];
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

BOOL isWinXP() {
	return ((OSVersion.dwPlatformId == 2) && (OSVersion.dwMajorVersion == 5) && (OSVersion.dwMinorVersion == 1));
}

void HighlightFoundWindow(HWND hwndWindow) {
	HDC	hWindowDC = NULL;
	HGDIOBJ	hPrevPen = NULL;
	HGDIOBJ	hPrevBrush = NULL;
	RECT rect;

	GetWindowRect(hwndWindow, &rect);
	hWindowDC = GetWindowDC(hwndWindow);

	if (hWindowDC) {
		hPrevPen = SelectObject(hWindowDC, CreatePen(PS_SOLID, 1, RGB(255, 0, 0)));
		hPrevBrush = SelectObject(hWindowDC, GetStockObject(HOLLOW_BRUSH));

		Rectangle(hWindowDC, 1, 1, rect.right - rect.left - 1, rect.bottom - rect.top - 1);

		SelectObject(hWindowDC, hPrevPen);
		SelectObject(hWindowDC, hPrevBrush);

		ReleaseDC(hwndWindow, hWindowDC);
	}

}

void RemoveHighlight(HWND hwndWindow) {
  InvalidateRect(hwndWindow, NULL, TRUE);
  UpdateWindow(hwndWindow);
  RedrawWindow(hwndWindow, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
}

HRESULT taskBar(HWND hWnd, int action) {

	HRESULT result = S_FALSE;

	result = CoInitialize(NULL);
	if (result == S_OK) {
		ITaskbarList * taskbar;
		result = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList, (void**)&taskbar);
		if (result == S_OK) {
			taskbar->HrInit();
			switch(action) {
			case 0:
				result = taskbar->AddTab(hWnd);
				break;
			case 1:
				result = taskbar->DeleteTab(hWnd);
				break;
			case 2:
				result = taskbar->ActivateTab(hWnd);
				break;
			}

			taskbar->Release();
		}
		CoUninitialize();
	}

	return result;
}

HICON getIconFromWindow(HWND hWnd) {

	SIZE_T dwResult = NULL;

	SendMessageTimeout(hWnd, WM_GETICON, ICON_SMALL, 0, SMTO_ABORTIFHUNG, 1000, &dwResult);

	if (dwResult == NULL)
		dwResult = GetClassLong(hWnd, GCLP_HICONSM);

	return (HICON)dwResult;
}

INT getFullDay() {
  CTime t = CTime::GetCurrentTime();
  return (t.GetYear() * 10000) + (t.GetMonth() * 100) + t.GetDay();
}

CString StringGetWord(std::wstring str, unsigned int pos) {

  std::vector<std::wstring> words;
	  
  size_t begin = 0, end;
  
  do {
    end = str.find(L"\r\n", begin);
    words.push_back(str.substr(begin, end - begin));
    begin = end + 2;
  }    
  while(end != std::string::npos);

  if (pos < words.size())
    return words.at(pos).c_str();

  return L"";
}

void ShowMsgLastError(HWND hwnd) {
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, LANG_NEUTRAL), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);
	::MessageBox(hwnd, (LPCTSTR)lpMsgBuf, _("Error"), MB_OK | MB_ICONINFORMATION );
	LocalFree( lpMsgBuf );
}

bool EnableDebugPriv(void) {
	HANDLE hToken = NULL;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		return false;
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue)) {
		CloseHandle(hToken);
		return false;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if ((AdjustTokenPrivileges(hToken, FALSE, &tkp, NULL, NULL, NULL) != ERROR_SUCCESS) && (GetLastError() != ERROR_SUCCESS)) {
		CloseHandle(hToken);
		return false;
	}

	CloseHandle(hToken);

	return true;
}

/************************************************************************/
/* This code is part of "Multiple Desktops" 
/* article at http://www.codeproject.com/KB/cpp/Multi_Desktop.aspx
/* by GauranG Shah
/************************************************************************/
/* Input:	 Cmdline ->		Name of the Process to open
/*			 Desktop ->		Desktop Name In Which you Want to open the process		
/* Output	True : If Succeed
/*			FALSE	: Fail
/* Purpose:	Open the Process in the different Desktop
/************************************************************************/
PROCESS_INFORMATION OpenProc( WCHAR *CmdLine, const WCHAR *Desktop)
{
  DWORD	dwExitCode;

	TCHAR str[100]={};
	TCHAR dsk[100];
	_tcscpy_s( str, 100, CmdLine );
	_tcscpy_s( dsk, 100, Desktop );

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	BOOL ProcCreated;
	ZeroMemory(&si,sizeof(si));
	si.cb			= sizeof(si);
	si.dwFlags		= STARTF_USESHOWWINDOW;
	si.wShowWindow	= SW_SHOWMAXIMIZED;
	ZeroMemory(&pi,sizeof(pi));

	si.lpDesktop = dsk;
	ProcCreated = CreateProcess( NULL,str,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi );          
	
	WaitForInputIdle(pi.hProcess, 1000);

	GetExitCodeProcess(pi.hProcess, &dwExitCode);

	return pi;
}

void CaptureScreen( HWND hWndStatic , HWND hWndPreview )
{
	RECT target, source;

	HDC hdc = GetDC(hWndPreview);
	HDC DeskDC = GetDC(GetShellWindow()); //GetDC(NULL);
	HDC hdcMem = CreateCompatibleDC(DeskDC);

  TRACE("CS: DesktopDC %08Xh\n", DeskDC);

	GetWindowRect(hWndStatic, &target);
	GetWindowRect(GetDesktopWindow(), &source);

	TRACE("Size: %u, %u\n", source.right - source.left, source.bottom - source.top);

	HBITMAP hb = CreateCompatibleBitmap(DeskDC, target.right - target.left, target.bottom - target.top);
	HGDIOBJ hOldBM = SelectObject(hdcMem,hb);

	StretchBlt(hdcMem, 0, 0, target.right - target.left, target.bottom - target.top, DeskDC, 0, 0, source.right - source.left, source.bottom - source.top, SRCCOPY);
	SelectObject(hdcMem, hOldBM);
	SendMessage(hWndStatic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hb);
}

int KillByName(CString process_name, BOOL polite, BOOL case_sensitive)
{
	DWORD aProcesses[1024], cbNeeded, cProcesses;
  unsigned int i, deaths = 0;
	DWORD dwPID;

	// Solicito la lista de procesos al sistema
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        return 1; 

    cProcesses = cbNeeded / sizeof(DWORD);

    for (i=0; i<cProcesses; i++) {

		dwPID = aProcesses[i];

		CProcess * proc = new CProcess(dwPID);

		if (((case_sensitive) && (proc->strName.Compare(process_name) == 0)) ||
			((!case_sensitive) && (proc->strName.CollateNoCase(process_name) == 0))) {
			if (polite)
				proc->KillPolite();
			else
				proc->Kill();

			deaths++;
		}

		delete proc;
	}

	return deaths;
}

Tree<CProcess*> * CreateProcessTree(ProcInfoMap processes)
{
  Tree<CProcess*> * root = new Tree<CProcess*>(NULL);

	std::map<DWORD, Tree<CProcess*> * > tmap;
	std::vector<CProcess*> left, lost;
	tmap[0] = root;

	ProcInfoMap::iterator j;
	for(j = processes.begin(); j != processes.end(); j++) {
		if ((j->second)->dwPID != 0) {
      if ((j->second)->dwParentPID == 0) {
        tmap[(j->second)->dwPID] = root->addChild(j->second);
      }
			else
				left.push_back(j->second);
		}
	}

	std::vector<CProcess*>::iterator k;
	bool inserted;
	do {
		inserted = false;
		for(k = left.begin(); k != left.end(); k++) {
			if (tmap.find((*k)->dwParentPID) != tmap.end()) {
				tmap[(*k)->dwPID] = tmap[(*k)->dwParentPID]->addChild(*k);
				inserted = true;
			}
			else {
				if (processes.find((*k)->dwParentPID) != processes.end())
					lost.push_back(*k);
        else {
					tmap[(*k)->dwPID] = root->addChild(*k);
        }
			}
		}

		left = lost;
		lost.clear();
	}
	while(!left.empty() && inserted);

	for(k = left.begin(); k != left.end(); k++) {
		tmap[(*k)->dwPID] = root->addChild(*k);
	}

  return root;
}

DWORD getHashFor(UINT hashID, const TCHAR * filename, TCHAR * output, DWORD * output_size)
{
    DWORD dwStatus = 0;
    BOOL bResult = FALSE;
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    HANDLE hFile = NULL, hHashFile = NULL;
    CString hashfilename;
    CString hashtext;
    BYTE rgbFile[DAPHNE_HASH_BUFSIZE];
    DWORD cbRead = 0;
    BYTE rgbHash[DAPHNE_SHA1LEN];
    DWORD hashLEN = 0;
    CHAR rgbDigits[] = "0123456789abcdef";

    DWORD cbHash = (hashID == CALG_MD5)?DAPHNE_MD5LEN:DAPHNE_SHA1LEN;
    
    if (*output_size < cbHash)
      return ERROR_INSUFFICIENT_BUFFER;

    hFile = CreateFile(filename,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_SEQUENTIAL_SCAN,
        NULL);

    if (INVALID_HANDLE_VALUE == hFile)
    {
      return GetLastError();
    }

    // Get handle to the crypto provider
    if (!CryptAcquireContext(&hProv,
        NULL,
        NULL,
        PROV_RSA_FULL,
        CRYPT_VERIFYCONTEXT))
    {
      CloseHandle(hFile);
      return GetLastError();
    }

    if (!CryptCreateHash(hProv, hashID, 0, 0, &hHash))
    {
      CloseHandle(hFile);
      CryptReleaseContext(hProv, 0);
      return GetLastError();
    }

    while (bResult = ReadFile(hFile, rgbFile, DAPHNE_HASH_BUFSIZE, &cbRead, NULL))
    {
        if (0 == cbRead)
        {
            break;
        }

        if (!CryptHashData(hHash, rgbFile, cbRead, 0))
        {
            CloseHandle(hFile);
            CryptDestroyHash(hHash);
            CryptReleaseContext(hProv, 0);
            return GetLastError();
        }
    }

    if (!bResult)
    {
      CloseHandle(hFile);
      CryptDestroyHash(hHash);
      CryptReleaseContext(hProv, 0);
      return GetLastError();
    }

    // El tamaño del buffer ya se validó
    UINT p = 0;
    if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
    {

      for (DWORD i = 0; i < cbHash; i++)
      {
        output[p++] = rgbDigits[rgbHash[i] >> 4];
        output[p++] = rgbDigits[rgbHash[i] & 0xf];
      }
      *output_size = p;
    }
    else
    {
      CryptDestroyHash(hHash);
      CryptReleaseContext(hProv, 0);
      CloseHandle(hFile);
      return GetLastError();
    }

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    CloseHandle(hFile);

    return ERROR_SUCCESS;
}

