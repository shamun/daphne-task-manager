// DRKShellMenu.cpp : Implementation of CDRKShellMenu

#include "stdafx.h"
#include "DRKShellMenu.h"
#include "RunWithArgs.h"
#include "ShowMD5.h"
#include "logmessages.h"
#include <Wincrypt.h>

// CDRKShellMenu
#define BUFSIZE 1024
#define MD5LEN  16
#define SHA1LEN 40

HRESULT CDRKShellMenu::Initialize ( 
    LPCITEMIDLIST pidlFolder,
    LPDATAOBJECT pDataObj,
    HKEY hProgID )
{
FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
STGMEDIUM stg = { TYMED_HGLOBAL };
HDROP     hDrop;

    // Look for CF_HDROP data in the data object.
    if ( FAILED( pDataObj->GetData ( &fmt, &stg )))
        {
        // Nope! Return an "invalid argument" error back to Explorer.
        return E_INVALIDARG;
        }

    // Get a pointer to the actual data.
    hDrop = (HDROP) GlobalLock ( stg.hGlobal );

    // Make sure it worked.
    if ( NULL == hDrop )
        {
        return E_INVALIDARG;
        }
    // Sanity check – make sure there is at least one filename.
UINT uNumFiles = DragQueryFile ( hDrop, 0xFFFFFFFF, NULL, 0 );

    if ( 0 == uNumFiles )
        {
        GlobalUnlock ( stg.hGlobal );
        ReleaseStgMedium ( &stg );
        return E_INVALIDARG;
        }

HRESULT hr = S_OK;

    // Get the name of the first file and store it in our member variable m_szFile.
    if ( 0 == DragQueryFile ( hDrop, 0, m_szFile, MAX_PATH ))
        {
        hr = E_INVALIDARG;
        }

    GlobalUnlock ( stg.hGlobal );
    ReleaseStgMedium ( &stg );

    return hr;
}

HRESULT CDRKShellMenu::QueryContextMenu (
    HMENU hmenu,
    UINT  uMenuIndex, 
    UINT  uidFirstCmd,
    UINT  uidLastCmd,
    UINT  uFlags )
{

    // If the flags include CMF_DEFAULTONLY then we shouldn't do anything.
    if ( !(uFlags & CMF_DEFAULTONLY) )
    {
      MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
      HMENU hSubmenu = CreatePopupMenu();
      UINT uID = uidFirstCmd;
      SHFILEINFO sfi;
      TCHAR tmpString[256];
	    
      SHGetFileInfo(m_szFile, 0, &sfi, sizeof(sfi), SHGFI_ATTRIBUTES);

      int p = 0;
      InsertMenu ( hSubmenu, p++, MF_BYPOSITION, uidFirstCmd+IDM_COPYPATH, _("Full path to clipboard") );
      InsertMenu ( hSubmenu, p++, MF_BYPOSITION, uidFirstCmd+IDM_COPYNAME, _("Name to clipboard") );
      HBITMAP cmdbmp = ::LoadBitmap(::GetModuleHandle(_T("DRKShell.dll")), MAKEINTRESOURCE(IDB_CMD));
      __("Open CMD here...", tmpString, sizeof(tmpString));
      mii.fMask = MIIM_STRING | MIIM_ID | MIIM_CHECKMARKS;
      mii.wID = uidFirstCmd+IDM_COMMAND;
      mii.hbmpChecked = cmdbmp;
      mii.hbmpUnchecked = cmdbmp;
      mii.dwTypeData = tmpString;
      InsertMenuItem ( hSubmenu, p++, TRUE, &mii );

      if( sfi.dwAttributes & SFGAO_FOLDER ) {
        InsertMenu ( hSubmenu, p++, MF_BYPOSITION, uidFirstCmd+IDM_DIRLISTING, _("File listing to clipboard") );
        InsertMenu ( hSubmenu, p++, MF_BYPOSITION, uidFirstCmd+IDM_DIRLISTINGFULL, _("Full path file listing to clipboard") );
        InsertMenu ( hSubmenu, p++, MF_BYPOSITION, uidFirstCmd+IDM_GOOGLE, _("Google this folder...") );
        InsertMenu ( hSubmenu, p++, MF_BYPOSITION, uidFirstCmd+IDM_WHOISUSING, _("Who is using this folder?") );
      }
      else {
        if ( StrRStrI(m_szFile, NULL, L".EXE")
          || StrRStrI(m_szFile, NULL, L".COM") 
          || StrRStrI(m_szFile, NULL, L".CMD")
          || StrRStrI(m_szFile, NULL, L".BAT")) {
          InsertMenu ( hSubmenu, p++, MF_BYPOSITION, uidFirstCmd+IDM_RUNARGS, _("Run with arguments...") );
        }
        InsertMenu ( hSubmenu, p++, MF_BYPOSITION, uidFirstCmd+IDM_GOOGLE, _("Google this file...") );
        InsertMenu ( hSubmenu, p++, MF_BYPOSITION, uidFirstCmd+IDM_WHOISUSING, _("Who is using this file?") );
        InsertMenu ( hSubmenu, p++, MF_BYPOSITION, uidFirstCmd+IDM_SHOWMD5, _("Compute MD5") );
        InsertMenu ( hSubmenu, p++, MF_BYPOSITION, uidFirstCmd+IDM_SHOWSHA1, _("Compute SHA1") );
      }

      // Insert the submenu into the ctx menu provided by Explorer.
      HBITMAP bmp = ::LoadBitmap(::GetModuleHandle(_T("DRKShell.dll")), MAKEINTRESOURCE(IDR_MAINFRAME));

      mii.fMask = MIIM_SUBMENU | MIIM_STRING | MIIM_ID | MIIM_CHECKMARKS;
      mii.wID = uID++;
      mii.hSubMenu = hSubmenu;
      mii.hbmpChecked = bmp;
      mii.hbmpUnchecked = bmp;
      mii.dwTypeData = _T("Daphne");

      InsertMenuItem ( hmenu, uMenuIndex, TRUE, &mii );

      return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, USHORT(IDM_SHOWSHA1 + 1) );
    }
    else
    {
        return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 0 );
    }

}

#include <atlconv.h>  // for ATL string conversion macros

HRESULT CDRKShellMenu::GetCommandString (
    UINT_PTR  idCmd,
    UINT  uFlags,
    UINT* pwReserved,
    LPSTR pszName,
    UINT  cchMax )
{
    USES_CONVERSION;

    // Check idCmd
    if ( idCmd > IDM_SHOWMD5 )
        return E_INVALIDARG;

    // If Explorer is asking for a help string, copy our string into the
    // supplied buffer.
    if ( uFlags & GCS_HELPTEXT )
        {
          LPCTSTR szText;
          switch(idCmd) {
          case IDM_COPYPATH: szText = _T("Copy full path into Windows clipboard.");
            break;
          case IDM_COPYNAME: szText = _T("Copy file name into Windows clipboard.");
            break;
          case IDM_COMMAND: szText = _T("Open command shell whit this folder as starting directory.");
            break;
          case IDM_RUNARGS: szText = _T("Execute this program with command line arguments.");
            break;
          case IDM_GOOGLE: szText = _T("Google this file or folder at DRK search page.");
            break;
          case IDM_DIRLISTING: szText = _T("List folder content as text into the clipboard.");
            break;
          case IDM_DIRLISTINGFULL: szText = _T("List full path folder content as text into the clipboard.");
            break;
          case IDM_WHOISUSING: szText = _T("List processes using this file.");
            break;
          case IDM_SHOWMD5: szText = _T("Compute and show MD5 digest for this file.");
            break;
          case IDM_SHOWSHA1: szText = _T("Compute and show SHA1 digest for this file.");
            break;
          default: szText = _T("Daphne shell extension");
            break;
          }

        if ( uFlags & GCS_UNICODE )
            {
            // We need to cast pszName to a Unicode string, and then use the
            // Unicode string copy API.
            lstrcpynW ( (LPWSTR) pszName, T2CW(szText), cchMax );
            }
        else
            {
            // Use the ANSI string copy API to return the help string.
            lstrcpynA ( pszName, T2CA(szText), cchMax );
            }

        return S_OK;
        }

    return E_INVALIDARG;
}

HRESULT CDRKShellMenu::InvokeCommand ( LPCMINVOKECOMMANDINFO pCmdInfo )
{
    // If lpVerb really points to a string, ignore this function call and bail out.
    if ( 0 != HIWORD( pCmdInfo->lpVerb ))
        return E_INVALIDARG;

    HRESULT result = S_OK;
    TCHAR drive[10];
    TCHAR * dir = new TCHAR[wcslen(m_szFile)+1];
    TCHAR * name = new TCHAR[wcslen(m_szFile)+1];
    TCHAR * ext = new TCHAR[wcslen(m_szFile)+1];
    bool split_ok = !_wsplitpath_s(m_szFile, drive, 10, dir, wcslen(m_szFile), name, wcslen(m_szFile), ext, wcslen(m_szFile));
    bool usefullpath = false;
    // Get the command index - the only valid one is 0.
    switch ( LOWORD( pCmdInfo->lpVerb ))
        {
        case IDM_COPYPATH:
            {
            CopyToClipboard( m_szFile );
            hitStatisticalRecord(L"ExplorerCopyToClipboard");
            }
        break;
        case IDM_COPYNAME:
            {
              if (split_ok) {
                wcscat_s(name, wcslen(m_szFile)+1, ext);
                CopyToClipboard( name );
                hitStatisticalRecord(L"ExplorerCopyToClipboard");
              }
            }
        break;
        case IDM_COMMAND:
            {
              hitStatisticalRecord(L"ExplorerOpenCMD");
              TCHAR cmd[MAX_PATH+1];
              _snwprintf_s(cmd, MAX_PATH, MAX_PATH, _T("/k cd \"%s%s\""), drive, dir);
              ::ShellExecute(NULL, _T("OPEN"), _T("CMD.EXE"), cmd, m_szFile, SW_SHOW);
            }
        break;
        case IDM_RUNARGS:
            {
              CRunWithArgs dlg;
              dlg.m_strProgram = m_szFile;
              if (dlg.DoModal() == IDOK) {

                hitStatisticalRecord(L"ExplorerRunWithArgs");
                ::ShellExecute(NULL, _T("OPEN"), m_szFile, dlg.m_strArgs, NULL, SW_SHOW);
              }
            }
        break;
        case IDM_GOOGLE:
          {
            hitStatisticalRecord(L"ExplorerGoogleThis");
            CString str;
            wcscat_s(name, wcslen(m_szFile)+1, ext);
	          str.Format(L"http://www.drk.com.ar/search.php?redir_file=%s", name);
	          ::ShellExecute(NULL, L"open", str, L"", L"", SW_SHOWNORMAL);	
          }
        break;
        case IDM_DIRLISTINGFULL:
          usefullpath = true;
        case IDM_DIRLISTING:
          {
            hitStatisticalRecord(L"ExplorerDirListing");
            WIN32_FIND_DATA FindFileData;
            HANDLE hFind;
            CString listing, mask;

            mask = m_szFile;
            mask += "\\*.*";
            hFind = FindFirstFile(mask, &FindFileData);
            if (hFind != INVALID_HANDLE_VALUE) 
            {
              do {
                if (usefullpath) {
                  listing += m_szFile;
                  listing += "\\";
                }
                listing += FindFileData.cFileName;
                listing += "\r\n";
              }
              while(FindNextFile(hFind, &FindFileData) != 0);
              FindClose(hFind);

              CopyToClipboard( (LPCTSTR)listing );
            }
          }
        break;
      case IDM_WHOISUSING: {
          hitStatisticalRecord(L"ExplorerWhoIsUsing");
          CString title;
          title.Format(L"Daphne - %s is in use by...", m_szFile);
          ::MessageBox(pCmdInfo->hwnd, whoIsUsing(m_szFile), title, MB_OK);
        }
        break;
      case IDM_SHOWMD5: {
          hitStatisticalRecord(L"ExplorerShowMD5");
          showHashFor(CALG_MD5, m_szFile, pCmdInfo->hwnd);
        }
        break;
      case IDM_SHOWSHA1: {
          hitStatisticalRecord(L"ExplorerShowSHA1");
          showHashFor(CALG_SHA1, m_szFile, pCmdInfo->hwnd);
        }
        break;

        default:
          result = E_INVALIDARG;
        }
        delete[] dir;
        delete[] name;
        delete[] ext;

      return result;
}
void CDRKShellMenu::CopyToClipboard(const TCHAR * text)
{
  char * buffer;
  int bytes, bsize;
	HGLOBAL hData = NULL;
  LPWSTR pInsertStrings[2] = {NULL, NULL};


	if (!OpenClipboard(NULL)) {
    pInsertStrings[0] = L"Can't open clipboard.";
    ReportEvent(hEventLog, EVENTLOG_ERROR_TYPE, BASE_CATEGORY, MSG_CLIPBOARD_ERROR, NULL, 1, 0, (LPCWSTR*)pInsertStrings, NULL);
		return;
  }

	if (!EmptyClipboard()) {
    pInsertStrings[0] = L"Clipboard is not empty.";
    ReportEvent(hEventLog, EVENTLOG_ERROR_TYPE, BASE_CATEGORY, MSG_CLIPBOARD_ERROR, NULL, 1, 0, (LPCWSTR*)pInsertStrings, NULL);
		return;
  }

  if (hData = GlobalAlloc(GMEM_MOVEABLE & GMEM_DDESHARE, wcslen(text)+1)) {
    bsize = WideCharToMultiByte(CP_OEMCP, 0, (LPCWSTR)text, (int)wcslen(text), NULL, 0, NULL, NULL);
    buffer = new char[bsize+1];
    bytes = WideCharToMultiByte(CP_OEMCP, 0, (LPCWSTR)text, (int)wcslen(text), buffer, bsize, NULL, NULL);
    CopyMemory(hData, (LPCSTR)buffer, bytes+1);
    delete[] buffer;
  }
  else {
    pInsertStrings[0] = L"Unable to alloc global memory for passing data into the clipboard.";
    ReportEvent(hEventLog, EVENTLOG_ERROR_TYPE, BASE_CATEGORY, MSG_CLIPBOARD_ERROR, NULL, 1, 0, (LPCWSTR*)pInsertStrings, NULL);
  }
	
	if (hData) 
		if (!::SetClipboardData(CF_TEXT, hData)) {
      pInsertStrings[0] = L"Unable to pass data into the clipboard.";
      ReportEvent(hEventLog, EVENTLOG_ERROR_TYPE, BASE_CATEGORY, MSG_CLIPBOARD_ERROR, NULL, 1, 0, (LPCWSTR*)pInsertStrings, NULL);
    }

	CloseClipboard();
}

void CDRKShellMenu::hitStatisticalRecord(CString item)
{
  CRegKey st;
  DWORD i = 0;
  
  if (st.Open(HKEY_CURRENT_USER, L"Software\\LHF\\Daphne\\STA") == ERROR_SUCCESS) {
    st.QueryDWORDValue(item, i);
    st.SetDWORDValue(item, i+1);
  }

  st.Close();
}

void CDRKShellMenu::showHashFor(UINT hashID, const TCHAR * filename, HWND parent)
{
    DWORD dwStatus = 0;
    BOOL bResult = FALSE;
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    HANDLE hFile = NULL, hHashFile = NULL;
    CString hashfilename;
    CString hashtext;
    LPCTSTR hashFileBuffer;
    BYTE rgbFile[BUFSIZE];
    DWORD cbRead = 0;
    BYTE rgbHash[SHA1LEN];
    DWORD cbHash = 0, hashLEN = 0;
    CHAR rgbDigits[] = "0123456789abcdef";

    CString title;
    title.Format(L"Compute %s", (hashID == CALG_MD5)?L"MD5":L"SHA1");

    hFile = CreateFile(filename,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_SEQUENTIAL_SCAN,
        NULL);

    if (INVALID_HANDLE_VALUE == hFile)
    {
      CString error;
      error.Format(L"Can't open file %s, error %08Xh", m_szFile, GetLastError());
      ::MessageBox(parent, error, title, MB_ICONSTOP|MB_OK);
      return;
    }

    // Get handle to the crypto provider
    if (!CryptAcquireContext(&hProv,
        NULL,
        NULL,
        PROV_RSA_FULL,
        CRYPT_VERIFYCONTEXT))
    {
      CString error;
      error.Format(L"Can't open CryptoAPI. Error %08Xh", GetLastError());
      ::MessageBox(parent, error, title, MB_ICONSTOP|MB_OK);
      CloseHandle(hFile);
      return;
    }

    if (!CryptCreateHash(hProv, hashID, 0, 0, &hHash))
    {
      CString error;
      error.Format(L"Can't open CryptoAPI. Error %08Xh", GetLastError());
      ::MessageBox(parent, error, title, MB_ICONSTOP|MB_OK);
      CloseHandle(hFile);
      CryptReleaseContext(hProv, 0);
      return;
    }

    while (bResult = ReadFile(hFile, rgbFile, BUFSIZE, &cbRead, NULL))
    {
        if (0 == cbRead)
        {
            break;
        }

        if (!CryptHashData(hHash, rgbFile, cbRead, 0))
        {
            CString error;
            error.Format(L"CryptoAPI hash data failed. Error %08Xh", GetLastError());
            ::MessageBox(parent, error, title, MB_ICONSTOP|MB_OK);
            CloseHandle(hFile);
            CryptDestroyHash(hHash);
            CryptReleaseContext(hProv, 0);
            return;
        }
    }

    if (!bResult)
    {
      CString error;
      error.Format(L"Read failed. Error %08Xh", GetLastError());
      ::MessageBox(parent, error, title, MB_ICONSTOP|MB_OK);
      CloseHandle(hFile);
      CryptDestroyHash(hHash);
      CryptReleaseContext(hProv, 0);
      return;
    }

    // Dependiendo del hash
    cbHash = (hashID == CALG_MD5)?MD5LEN:SHA1LEN;
    if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
    {

      for (DWORD i = 0; i < cbHash; i++)
      {
        hashtext.AppendChar(rgbDigits[rgbHash[i] >> 4]);
        hashtext.AppendChar(rgbDigits[rgbHash[i] & 0xf]);
      }

      CShowMD5 dlg;
      dlg.m_title = title;
      dlg.m_md5 = L"";
      dlg.m_file = filename;
      dlg.m_md5 = hashtext;
      // Si existe abro .md5
      hashfilename.Format(L"%s.%s", filename, (hashID == CALG_MD5)?L"md5":L"sha1");
      hHashFile = CreateFile(hashfilename,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_SEQUENTIAL_SCAN,
        NULL);

      if (hHashFile !=  INVALID_HANDLE_VALUE) {
        CHAR * hashTmpBuffer = new CHAR[cbHash * 2 + 1];
        DWORD bytesRead;
        ReadFile(hHashFile, hashTmpBuffer, cbHash * 2, &bytesRead, NULL);
        hashTmpBuffer[bytesRead] = 0;
        USES_CONVERSION;
        hashFileBuffer = A2W(hashTmpBuffer);
        CloseHandle(hHashFile);
        if (hashtext == hashFileBuffer)
          dlg.m_validation = L"OK - Hash file content matches computed hash";
        else
          dlg.m_validation = L"ERROR - Hash file content doesn't matches computed hash";
        delete[] hashTmpBuffer;
      }
      else {
        dlg.m_validation = L"There isn't hash file to validate computed hash";
      }


      if (dlg.DoModal() != IDCANCEL) {
        if (dlg.createHashFile) {
          hHashFile = CreateFile(hashfilename,
            GENERIC_WRITE,
            0,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

          if (hHashFile !=  INVALID_HANDLE_VALUE) {
            DWORD writtenBytes;
            USES_CONVERSION;
            PCSTR ansi = T2CA(hashtext);
            WriteFile(hHashFile, ansi, hashtext.GetLength(), &writtenBytes, NULL);
            CloseHandle(hHashFile);
          }
          else {
            CString error;
            error.Format(L"Create hash file failed. Error %08Xh", GetLastError());
            ::MessageBox(parent, error, title, MB_ICONSTOP|MB_OK);
          }
        }
        else {
          CopyToClipboard(dlg.m_md5);
        }
      }
    }
    else
    {
      CString error;
      error.Format(L"CryptoAPI CryptGetHashParam failed. Error %08Xh", GetLastError());
      ::MessageBox(parent, error, title, MB_ICONSTOP|MB_OK);
    }

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    CloseHandle(hFile);
}
