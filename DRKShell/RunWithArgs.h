// CRunWithArgs.h : Declaration of the CRunWithArgs

#pragma once

#include "resource.h"       // main symbols

#include <atlhost.h>

using namespace ATL;

// CRunWithArgs

class CRunWithArgs : 
	public CAxDialogImpl<CRunWithArgs>
{
public:
	CRunWithArgs()
	{
	}

	~CRunWithArgs()
	{
    m_ctrlArgs.Detach();
	}

	enum { IDD = IDD_RUNARGS };

BEGIN_MSG_MAP(CRunWithArgs)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	CHAIN_MSG_MAP(CAxDialogImpl<CRunWithArgs>)
END_MSG_MAP()

  CString m_strArgs;
  CString m_strProgram;
  CEdit m_ctrlArgs;

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<CRunWithArgs>::OnInitDialog(uMsg, wParam, lParam, bHandled);
		bHandled = TRUE;
    SetDlgItemText(IDC_PROGRAM, m_strProgram);
    m_ctrlArgs.Attach(GetDlgItem(IDC_ARGS));
    m_ctrlArgs.SetWindowText(m_strArgs);
    m_ctrlArgs.SetFocus();
    
		return 1;  // Let the system set the focus
	}

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
    m_ctrlArgs.GetWindowText(m_strArgs);
		return 0;
	}

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
};


