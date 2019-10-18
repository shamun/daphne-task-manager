// lssoftDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "SoftList.h"
#include <vector>

class Software;

typedef std::vector< Software * > SoftwareListPtr;
typedef std::vector< Software * >::iterator SoftwareListPtrIt;

// ClssoftDlg dialog
class ClssoftDlg : public CDialog
{
// Construction
public:
	ClssoftDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LSSOFT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
public:
  static int column;
  static bool inverted;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
  void reloadList();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CSoftList listControl;
	SoftwareListPtr software;
	afx_msg void OnBnClickedCancel();
protected:
	virtual void OnCancel();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	void showContextMenu(POINT & point);
	afx_msg void OnNMRclickList(NMHDR *pNMHDR, LRESULT *pResult);
public:
	Software* getSelectedSoftware(void);
	void uninstall(void);
  static bool whichFirst(const Software * left, const Software * right);
public:
//	afx_msg void OnHdnItemdblclickListls(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListls(NMHDR *pNMHDR, LRESULT *pResult);
  virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
  afx_msg void OnBnClickedLssoftExport();
};
