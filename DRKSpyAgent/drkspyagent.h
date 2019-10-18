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
#include "stdafx.h"

#ifdef DRKSPYAGENT_EXPORTS
#define DRKSPYAGENT_API __declspec(dllexport)
#else
#define DRKSPYAGENT_API __declspec(dllimport)
#endif

#define LHF_BUFF_SIZE 65535

namespace DRKSpy {

extern DRKSPYAGENT_API HWND g_hWnd;
extern DRKSPYAGENT_API HWND g_hWndCtrl;
extern DRKSPYAGENT_API WCHAR g_strMsg[LHF_BUFF_SIZE];
extern DRKSPYAGENT_API int g_iAction;
extern DRKSPYAGENT_API bool g_bWorked;

}