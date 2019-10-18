/***********************************************************************************
	
	Daphne -- GPL task manager replacement for Windows
	Copyright (C) 2005,2014  Leandro H. Fernández

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

***********************************************************************************/

// Killer.cpp: implementation of the CKiller class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Daphne.h"
#include "Killer.h"
#include "DaphneDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKiller::CKiller()
{

}

CKiller::~CKiller()
{

}

void CKiller::Kill(CDaphneDlg *parent)
{
	CString proc;
	int i = 0;

	while((proc = StringGetWord((LPCWSTR)strProcesses, i++)) != "") {
		if (proc.GetLength() > 0) {
			KillByName(proc, FALSE);
		}
	}
}
