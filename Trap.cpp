// Trap.cpp: implementation of the CTrap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Daphne.h"
#include "Trap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrap::CTrap(CString process, UINT trap_type, CString wcontains, UINT64 i64_dword)
{
	this->processName = process;
	this->type = trap_type;
	this->windowContains = wcontains;
  this->i64_dword = i64_dword;
}

CTrap::CTrap(CString process, UINT trap_type, CString wcontains)
{
	this->processName = process;
	this->type = trap_type;
	this->windowContains = wcontains;
  this->i64_dword = 0;
}

CTrap::~CTrap()
{

}

CString CTrap::getProcessName()
{
	return processName;
}

UINT CTrap::getType()
{
	return type;
}

BOOL CTrap::canEdit()
{
	return TRUE;
}

BOOL CTrap::canEdit(UINT type)
{
	return TRUE;
}

BOOL CTrap::appliesToWindow(UINT type) {

	switch(type) {
	case 0:
		return FALSE;
		break;
	case 1:
		return FALSE;
		break;
	case 2:
		return TRUE;
		break;
	case 3:
		return TRUE;
		break;
	case 4:
		return TRUE;
		break;
	case 5:
		return TRUE;
		break;
	case 6:
		return TRUE;
		break;
	case 7:
		return FALSE;
		break;
	case 8:
		return FALSE;
		break;
	case 9:
		return FALSE;
		break;
	case 10:
		return FALSE;
		break;
	case 11:
		return FALSE;
		break;
	case 12:
		return FALSE;
		break;
	case 13:
		return TRUE;
		break;
	}

	return FALSE;
}

CString CTrap::getTypeName(UINT type) {

	switch(type) {
	case 0:
		return _("Notify process created");
		break;
	case 1:
		return _("Kill");
		break;
	case 2:
		return _("Hide application");
		break;
	case 3:
		return _("Set on top");
		break;
	case 4:
		return _("Unset on top");
		break;
	case 5:
		return _("Set alpha 75%");
		break;
	case 6:
		return _("Set alpha 50%");
		break;
	case 7:
		return _("Set priority to real-time");
		break;
	case 8:
		return _("Set priority to high");
		break;
	case 9:
		return _("Set priority to normal");
		break;
	case 10:
		return _("Set priority to low");
		break;
	case 11:
		return _("Notify process destroyed");
		break;
  case 12:
    return _("Set process affinity mask");
    break;
  case 13:
    return _("Keep windows position and size");
    break;
	}

	return "";
}

UINT CTrap::getMaxType() {
	return 13;
}

void CTrap::setType(UINT type)
{
	this->type = type;
}

void CTrap::setProcessName(CString name)
{
	this->processName = name;
}

CTrap::CTrap()
{

}

CString CTrap::getWindowContains()
{
	return windowContains;
}

void CTrap::setWindowContains(CString substring)
{
	windowContains = substring;
}

UINT64 CTrap::geti64DWORD()
{
	return i64_dword;
}

void CTrap::seti64DWORD(UINT64 value)
{
	i64_dword = value;
}