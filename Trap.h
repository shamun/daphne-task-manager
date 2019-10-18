// Trap.h: interface for the CTrap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRAP_H__2F79443B_2BB1_46DA_8606_1FAEA4E0192C__INCLUDED_)
#define AFX_TRAP_H__2F79443B_2BB1_46DA_8606_1FAEA4E0192C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTrap  
{
public:
	CTrap();
	CTrap(CString process, UINT trap_type, CString wcontains);
	CTrap(CString process, UINT trap_type, CString wcontains, UINT64 i64_dword);
	virtual ~CTrap();
	void setWindowContains(CString substring);
	CString getWindowContains();
	void setProcessName(CString name);
	void setType(UINT type);
  void seti64DWORD(UINT64 value);
  UINT64 geti64DWORD();
	CString getProcessName();
	UINT getType();
	BOOL canEdit();
	static BOOL canEdit(UINT type);
	static CString getTypeName(UINT type);
	static UINT getMaxType();
	static BOOL appliesToWindow(UINT type);

private:
	CString windowContains;
	UINT type;
	CString processName;
  UINT64 i64_dword;
};

#endif // !defined(AFX_TRAP_H__2F79443B_2BB1_46DA_8606_1FAEA4E0192C__INCLUDED_)
