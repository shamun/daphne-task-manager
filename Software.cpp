#include "StdAfx.h"
#include "daphne.h"
#include "Software.h"

Software::Software(const HKEY &hkResult, TCHAR * name)
{
	HKEY hkSub;
	if (RegOpenKey(hkResult, name, &hkSub) == ERROR_SUCCESS) {
		DWORD dwType;
		DWORD dwSize = 256;
		if (RegQueryValueEx(hkSub, L"DisplayName", NULL, &dwType, (LPBYTE)this->name.GetBuffer(dwSize), &dwSize) == ERROR_SUCCESS) {
			this->name.ReleaseBuffer();
		}
		dwSize = 256;
		if (RegQueryValueEx(hkSub, L"DisplayVersion", NULL, &dwType, (LPBYTE)this->version.GetBuffer(dwSize), &dwSize) == ERROR_SUCCESS) {
			this->version.ReleaseBuffer();
		}
		dwSize = 256;
		if (RegQueryValueEx(hkSub, L"Publisher", NULL, &dwType, (LPBYTE)this->publisher.GetBuffer(dwSize), &dwSize) == ERROR_SUCCESS) {
			this->publisher.ReleaseBuffer();
		}
		dwSize = 256;
		if (RegQueryValueEx(hkSub, L"URLInfoAbout", NULL, &dwType, (LPBYTE)this->publisherURL.GetBuffer(dwSize), &dwSize) == ERROR_SUCCESS) {
			this->publisherURL.ReleaseBuffer();
		}
		//UninstallString
		dwSize = 512;
		if (RegQueryValueEx(hkSub, L"UninstallString", NULL, &dwType, (LPBYTE)this->uninstallString.GetBuffer(dwSize), &dwSize) == ERROR_SUCCESS) {
			this->uninstallString.ReleaseBuffer();
		}

		// Enum
		DWORD dwIndex = 0;
		TCHAR name[1024];
		DWORD dwNameSize = 1024;
		dwSize = 1024;
		CString value;
		while(RegEnumValue(hkSub, dwIndex, name, &dwNameSize,	NULL, &dwType, (LPBYTE)value.GetBuffer(1024), &dwSize) == ERROR_SUCCESS) {

			raw += name;
			raw += L": ";
			switch (dwType) {
				case REG_DWORD: {
					value.ReleaseBuffer(4);
					CString data;
					data.Format(L"%08Xh", *((DWORD*)value.GetBuffer(4)));
					raw += data;
					}
					break;
				case REG_BINARY:
					raw += _("[Binary data]");
					break;
				case REG_EXPAND_SZ:
				case REG_SZ:
				default:
					value.ReleaseBuffer();
					raw += value;
					break;
			}
			raw += L"\r\n";

			// Next
			dwIndex++;
			dwNameSize = 1024;
			dwSize = 1024;
		}

		RegCloseKey(hkSub);
	}

	if (this->name.IsEmpty())
		throw -1;
}

Software::~Software(void)
{
}
