#pragma once

#include "lssoftDlg.h"

class Software
{
public:
	Software(const HKEY &hkResult, TCHAR * name);
	CString getName() const { return name; };
	CString getVersion() const { return version; };
	CString getPublisher() const { return publisher; };
	CString getPublisherURL() const { return publisherURL; };
	CString getRAWInformation() const { return raw; };
	CString getUninstallString() const { return uninstallString; };
	~Software(void);

  friend bool ClssoftDlg::whichFirst(const Software * left, const Software * right);

private:
	CString name;
	CString version;
	CString publisher;
	CString publisherURL;
	CString raw;
	CString uninstallString;

};
