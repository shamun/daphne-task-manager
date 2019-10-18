// ShowMD5.h : Declaration of the CShowMD5

#pragma once

#include "resource.h"       // main symbols

#include <atlhost.h>

using namespace ATL;

// CShowMD5

class CShowMD5 : 
	public CAxDialogImpl<CShowMD5>
{
public:
	CShowMD5() : createHashFile(false)
	{
	}

	~CShowMD5()
	{
	}

	enum { IDD = IDD_SHOWMD5 };

BEGIN_MSG_MAP(CShowMD5)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
  COMMAND_HANDLER(IDC_CREATE_MD5, BN_CLICKED, OnBnClickedcreateHashFile)
	CHAIN_MSG_MAP(CAxDialogImpl<CShowMD5>)
END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

  CString m_title;
  CString m_file;
  CString m_md5;
  CString m_validation;
  bool createHashFile;

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<CShowMD5>::OnInitDialog(uMsg, wParam, lParam, bHandled);
		bHandled = TRUE;
    SetWindowText(m_title);
    SetDlgItemText(IDC_MD5_FILE, m_file);
    SetDlgItemText(IDC_MD5, m_md5);
    SetDlgItemText(IDC_MD5_VALID, m_validation);
		return 1;  // Let the system set the focus
	}

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
  LRESULT OnBnClickedcreateHashFile(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
  {
    createHashFile = true;
    EndDialog(wID);
    return 0;
  }

};


