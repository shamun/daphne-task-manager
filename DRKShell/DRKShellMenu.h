// DRKShellMenu.h : Declaration of the CDRKShellMenu

#pragma once
#include "DRKShell.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>
#include <shlobj.h>
#include <comdef.h>

#define IDM_COPYPATH   0
#define IDM_COPYNAME   1
#define IDM_COMMAND    2
#define IDM_RUNARGS    3
#define IDM_GOOGLE     4
#define IDM_DIRLISTING 5
#define IDM_DIRLISTINGFULL 6
#define IDM_WHOISUSING 7
#define IDM_SHOWMD5    8
#define IDM_SHOWSHA1   9

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

const WCHAR * UTF8_WC(const char * s, WCHAR * stg = NULL, int size = 0);
extern HANDLE hEventLog;

extern "C" {
#include <libintl.h>
}
#define _(String) UTF8_WC( gettext (String) )
#define __(String, Storage, Size) UTF8_WC( gettext (String), Storage, Size )
#define gettext_noop(String) String
#define N_(String) gettext_noop (String)

// Util
CString whoIsUsing(CString file);

// CDRKShellMenu

class ATL_NO_VTABLE CDRKShellMenu :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDRKShellMenu, &CLSID_DRKShellMenu>,
	public IDispatchImpl<IDRKShellMenu, &IID_IDRKShellMenu, &LIBID_DRKShellLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
  public IShellExtInit,
  public IContextMenu

{
public:
	CDRKShellMenu()
	{
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DRKSHELLMENU)

DECLARE_NOT_AGGREGATABLE(CDRKShellMenu)

BEGIN_COM_MAP(CDRKShellMenu)
	COM_INTERFACE_ENTRY(IDRKShellMenu)
	COM_INTERFACE_ENTRY(IDispatch)
  COM_INTERFACE_ENTRY(IShellExtInit)
  COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()


protected:
  TCHAR m_szFile [MAX_PATH];


// IDRKShellMenu
public:

public:
  // IShellExtInit
  STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

public:
    // IContextMenu
    STDMETHOD(GetCommandString)(UINT_PTR, UINT, UINT*, LPSTR, UINT);
    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO);
    STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT);
protected:
  void CopyToClipboard(const TCHAR * text);
public:
  void hitStatisticalRecord(CString item);
  void showHashFor(UINT hashID, const TCHAR * filename, HWND parent);
};

OBJECT_ENTRY_AUTO(__uuidof(DRKShellMenu), CDRKShellMenu)
