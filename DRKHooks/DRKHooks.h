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

#ifdef DRKHOOKS_EXPORTS
#define DRKHOOKS_API __declspec(dllexport)
#else
#define DRKHOOKS_API __declspec(dllimport)
#endif

extern DRKHOOKS_API HWND g_hWndHooks;
extern DRKHOOKS_API HHOOK	g_hMessageHook;
extern DRKHOOKS_API int		g_scVK;
extern DRKHOOKS_API BOOL	g_scNeedsShift;
extern DRKHOOKS_API BOOL	g_scNeedsCtrl;
extern DRKHOOKS_API BOOL	g_scNeedsAlt;
extern DRKHOOKS_API BOOL	g_bKBShortCut;
extern DRKHOOKS_API HHOOK	g_hKeyboardHook;
extern DRKHOOKS_API BOOL	g_bKBHookInstalled;
extern DRKHOOKS_API BOOL	g_bGMHookInstalled;

extern DRKHOOKS_API LRESULT CALLBACK KeyboardProc(
  int code,       // hook code
  WPARAM wParam,  // virtual-key code
  LPARAM lParam   // keystroke-message information
  );

extern DRKHOOKS_API LRESULT CALLBACK GetMsgProc(
	int code,       // hook code
	WPARAM wParam,  // removal option
	LPARAM lParam   // message
	);