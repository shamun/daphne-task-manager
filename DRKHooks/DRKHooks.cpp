/***********************************************************************************
	
	Daphne -- GPL task manager replacement for Windows
	Copyright (C) 2005  Leandro H. Fernández

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

// DRKHooks.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "DRKHooks.h"

#pragma data_seg (".shared_drkhooks")
HWND	g_hWndHooks = 0;
int		g_scVK = VK_F12;
BOOL	g_scNeedsShift = TRUE;
BOOL	g_scNeedsAlt = TRUE;
BOOL	g_scNeedsCtrl = FALSE;
BOOL	g_bKBShortCut = TRUE;
HHOOK	g_hKeyboardHook = NULL;
HHOOK	g_hMessageHook = NULL;
BOOL	g_bKBHookInstalled = FALSE;
BOOL	g_bGMHookInstalled = FALSE;
#pragma data_seg ()

#pragma comment(linker,"/SECTION:.shared_drkhooks,RWS") 

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

LRESULT CALLBACK KeyboardProc(
  int code,       // hook code
  WPARAM wParam,  // virtual-key code
  LPARAM lParam   // keystroke-message information
  ) {

	// Si es la combinación configurada activo la aplicación
	if ((g_bKBShortCut) && (code == HC_ACTION) && (HIWORD(lParam) & KF_UP) && (wParam == (UINT)g_scVK)) {
		
		if ((g_scNeedsAlt) && (!(HIWORD(lParam) & KF_ALTDOWN))) return ::CallNextHookEx(g_hKeyboardHook, code, wParam, lParam);
		if ((g_scNeedsShift) && (!(GetKeyState(VK_SHIFT) & 0xF0))) return ::CallNextHookEx(g_hKeyboardHook, code, wParam, lParam);
		if ((g_scNeedsCtrl) && (!(GetKeyState(VK_CONTROL) & 0xF0))) return ::CallNextHookEx(g_hKeyboardHook, code, wParam, lParam);

		::SendNotifyMessage(g_hWndHooks, WM_COMMAND, MAKEWPARAM(WM_APP + 2, 0), NULL); // WM_APP + 2 == WM_DRK_SHOW, ver Daphne.h
	}

  if ((code == HC_ACTION) && (HIWORD(lParam) & KF_UP) && (wParam >= VK_F1) && (wParam <= VK_F4)) {
    if ((GetKeyState(VK_CONTROL) & 0xF0) && (GetKeyState(VK_SHIFT) & 0xF0)) {
   		::SendNotifyMessage(g_hWndHooks, WM_COMMAND, MAKEWPARAM(WM_APP + 12 + (wParam - VK_F1), 0), NULL); // WM_APP + 12 == WM_DRK_RS1, ver Daphne.h
    }
  }

	return ::CallNextHookEx(g_hKeyboardHook, code, wParam, lParam);
}


LRESULT CALLBACK GetMsgProc(
	int code,       // hook code
	WPARAM wParam,  // removal option
	LPARAM lParam   // message
	)
{

	return ::CallNextHookEx(g_hMessageHook, code, wParam, lParam);
}