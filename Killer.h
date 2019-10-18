/***********************************************************************************
	
	Daphne -- GPL task manager replacement for Windows
	Copyright (C) 2012  Leandro H. Fernández

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

// Killer.h: interface for the CKiller class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KILLER_H__6C66589E_8C85_4569_9D45_219FE3E283D5__INCLUDED_)
#define AFX_KILLER_H__6C66589E_8C85_4569_9D45_219FE3E283D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDaphneDlg;

class CKiller  
{
public:
	void Kill(CDaphneDlg * parent);
	CString strName;
	CString strProcesses;
	UINT iMode;
	CKiller();
	virtual ~CKiller();

};

#endif // !defined(AFX_KILLER_H__6C66589E_8C85_4569_9D45_219FE3E283D5__INCLUDED_)
