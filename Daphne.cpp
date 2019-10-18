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

// Daphne.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Daphne.h"
#include "DaphneDlg.h"
#include <iostream>
#include <sstream>

#include "Userenv.h"
#include "UnicodeFile.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDaphneApp

BEGIN_MESSAGE_MAP(CDaphneApp, CWinApp)
	//{{AFX_MSG_MAP(CDaphneApp)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDaphneApp construction

CDaphneApp::CDaphneApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

bool CDaphneApp::rtl = false;

/////////////////////////////////////////////////////////////////////////////
// The one and only CDaphneApp object

CDaphneApp theApp;
WSADATA wsaData;

CString strSpyAgentDLL;
CString strDRKHooksDLL;
CString strDRKShellDLL;
CString strDaphneExe;
CString strBaseDir;
TCHAR szDirectory[MAX_PATH];

// MessageMap
MsgMap commands;
MsgMap values;

// Globals
void SetupMsgMap() {
	commands[L"WM_ACTIVATE"] = 0x0006;
	commands[L"WM_ACTIVATEAPP"] = 0x001C;
	commands[L"WM_AFXFIRST"] = 0x0360;
	commands[L"WM_AFXLAST"] = 0x037F;
	commands[L"WM_APP"] = 0x8000;
	commands[L"WM_APPCOMMAND"] = 0x0319;
	commands[L"WM_ASKCBFORMATNAME"] = 0x030C;
	commands[L"WM_CANCELJOURNAL"] = 0x004B;
	commands[L"WM_CANCELMODE"] = 0x001F;
	commands[L"WM_CAPTURECHANGED"] = 0x0215;
	commands[L"WM_CHANGECBCHAIN"] = 0x030D;
	commands[L"WM_CHANGEUISTATE"] = 0x0127;
	commands[L"WM_CHAR"] = 0x0102;
	commands[L"WM_CHARTOITEM"] = 0x002F;
	commands[L"WM_CHILDACTIVATE"] = 0x0022;
	commands[L"WM_CLEAR"] = 0x0303;
	commands[L"WM_CLOSE"] = 0x0010;
	commands[L"WM_COMMAND"] = 0x0111;
	commands[L"WM_COMMNOTIFY"] = 0x0044;
	commands[L"WM_COMPACTING"] = 0x0041;
	commands[L"WM_COMPAREITEM"] = 0x0039;
	commands[L"WM_CONTEXTMENU"] = 0x007B;
	commands[L"WM_COPY"] = 0x0301;
	commands[L"WM_COPYDATA"] = 0x004A;
	commands[L"WM_CREATE"] = 0x0001;
	commands[L"WM_CTLCOLORBTN"] = 0x0135;
	commands[L"WM_CTLCOLORDLG"] = 0x0136;
	commands[L"WM_CTLCOLOREDIT"] = 0x0133;
	commands[L"WM_CTLCOLORLISTBOX"] = 0x0134;
	commands[L"WM_CTLCOLORMSGBOX"] = 0x0132;
	commands[L"WM_CTLCOLORSCROLLBAR"] = 0x0137;
	commands[L"WM_CTLCOLORSTATIC"] = 0x0138;
	commands[L"WM_CUT"] = 0x0300;
	commands[L"WM_DEADCHAR"] = 0x0103;
	commands[L"WM_DELETEITEM"] = 0x002D;
	commands[L"WM_DESTROY"] = 0x0002;
	commands[L"WM_DESTROYCLIPBOARD"] = 0x0307;
	commands[L"WM_DEVICECHANGE"] = 0x0219;
	commands[L"WM_DEVMODECHANGE"] = 0x001B;
	commands[L"WM_DISPLAYCHANGE"] = 0x007E;
	commands[L"WM_DRAWCLIPBOARD"] = 0x0308;
	commands[L"WM_DRAWITEM"] = 0x002B;
	commands[L"WM_DROPFILES"] = 0x0233;
	commands[L"WM_ENABLE"] = 0x000A;
	commands[L"WM_ENDSESSION"] = 0x0016;
	commands[L"WM_ENTERIDLE"] = 0x0121;
	commands[L"WM_ENTERMENULOOP"] = 0x0211;
	commands[L"WM_ENTERSIZEMOVE"] = 0x0231;
	commands[L"WM_ERASEBKGND"] = 0x0014;
	commands[L"WM_EXITMENULOOP"] = 0x0212;
	commands[L"WM_EXITSIZEMOVE"] = 0x0232;
	commands[L"WM_FONTCHANGE"] = 0x001D;
	commands[L"WM_GETDLGCODE"] = 0x0087;
	commands[L"WM_GETFONT"] = 0x0031;
	commands[L"WM_GETHOTKEY"] = 0x0033;
	commands[L"WM_GETICON"] = 0x007F;
	commands[L"WM_GETMINMAXINFO"] = 0x0024;
	commands[L"WM_GETOBJECT"] = 0x003D;
	commands[L"WM_GETTEXT"] = 0x000D;
	commands[L"WM_GETTEXTLENGTH"] = 0x000E;
	commands[L"WM_HANDHELDFIRST"] = 0x0358;
	commands[L"WM_HANDHELDLAST"] = 0x035F;
	commands[L"WM_HELP"] = 0x0053;
	commands[L"WM_HOTKEY"] = 0x0312;
	commands[L"WM_HSCROLL"] = 0x0114;
	commands[L"WM_HSCROLLCLIPBOARD"] = 0x030E;
	commands[L"WM_ICONERASEBKGND"] = 0x0027;
	commands[L"WM_IME_CHAR"] = 0x0286;
	commands[L"WM_IME_COMPOSITION"] = 0x010F;
	commands[L"WM_IME_COMPOSITIONFULL"] = 0x0284;
	commands[L"WM_IME_CONTROL"] = 0x0283;
	commands[L"WM_IME_ENDCOMPOSITION"] = 0x010E;
	commands[L"WM_IME_KEYDOWN"] = 0x0290;
	commands[L"WM_IME_KEYLAST"] = 0x010F;
	commands[L"WM_IME_KEYUP"] = 0x0291;
	commands[L"WM_IME_NOTIFY"] = 0x0282;
	commands[L"WM_IME_REQUEST"] = 0x0288;
	commands[L"WM_IME_SELECT"] = 0x0285;
	commands[L"WM_IME_SETCONTEXT"] = 0x0281;
	commands[L"WM_IME_STARTCOMPOSITION"] = 0x010D;
	commands[L"WM_INITDIALOG"] = 0x0110;
	commands[L"WM_INITMENU"] = 0x0116;
	commands[L"WM_INITMENUPOPUP"] = 0x0117;
	commands[L"WM_INPUT"] = 0x00FF;
	commands[L"WM_INPUTLANGCHANGE"] = 0x0051;
	commands[L"WM_INPUTLANGCHANGEREQUEST"] = 0x0050;
	commands[L"WM_KEYDOWN"] = 0x0100;
	commands[L"WM_KEYFIRST"] = 0x0100;
	commands[L"WM_KEYLAST"] = 0x0108;
	commands[L"WM_KEYLAST"] = 0x0109;
	commands[L"WM_KEYUP"] = 0x0101;
	commands[L"WM_KILLFOCUS"] = 0x0008;
	commands[L"WM_LBUTTONDBLCLK"] = 0x0203;
	commands[L"WM_LBUTTONDOWN"] = 0x0201;
	commands[L"WM_LBUTTONUP"] = 0x0202;
	commands[L"WM_MBUTTONDBLCLK"] = 0x0209;
	commands[L"WM_MBUTTONDOWN"] = 0x0207;
	commands[L"WM_MBUTTONUP"] = 0x0208;
	commands[L"WM_MDIACTIVATE"] = 0x0222;
	commands[L"WM_MDICASCADE"] = 0x0227;
	commands[L"WM_MDICREATE"] = 0x0220;
	commands[L"WM_MDIDESTROY"] = 0x0221;
	commands[L"WM_MDIGETACTIVE"] = 0x0229;
	commands[L"WM_MDIICONARRANGE"] = 0x0228;
	commands[L"WM_MDIMAXIMIZE"] = 0x0225;
	commands[L"WM_MDINEXT"] = 0x0224;
	commands[L"WM_MDIREFRESHMENU"] = 0x0234;
	commands[L"WM_MDIRESTORE"] = 0x0223;
	commands[L"WM_MDISETMENU"] = 0x0230;
	commands[L"WM_MDITILE"] = 0x0226;
	commands[L"WM_MEASUREITEM"] = 0x002C;
	commands[L"WM_MENUCHAR"] = 0x0120;
	commands[L"WM_MENUCOMMAND"] = 0x0126;
	commands[L"WM_MENUDRAG"] = 0x0123;
	commands[L"WM_MENUGETOBJECT"] = 0x0124;
	commands[L"WM_MENURBUTTONUP"] = 0x0122;
	commands[L"WM_MENUSELECT"] = 0x011F;
	commands[L"WM_MOUSEACTIVATE"] = 0x0021;
	commands[L"WM_MOUSEFIRST"] = 0x0200;
	commands[L"WM_MOUSEHOVER"] = 0x02A1;
	commands[L"WM_MOUSELAST"] = 0x020A;
	commands[L"WM_MOUSELAST"] = 0x020D;
	commands[L"WM_MOUSELAST"] = 0x0209;
	commands[L"WM_MOUSELEAVE"] = 0x02A3;
	commands[L"WM_MOUSEMOVE"] = 0x0200;
	commands[L"WM_MOUSEWHEEL"] = 0x020A;
	commands[L"WM_MOVE"] = 0x0003;
	commands[L"WM_MOVING"] = 0x0216;
	commands[L"WM_NCACTIVATE"] = 0x0086;
	commands[L"WM_NCCALCSIZE"] = 0x0083;
	commands[L"WM_NCCREATE"] = 0x0081;
	commands[L"WM_NCDESTROY"] = 0x0082;
	commands[L"WM_NCHITTEST"] = 0x0084;
	commands[L"WM_NCLBUTTONDBLCLK"] = 0x00A3;
	commands[L"WM_NCLBUTTONDOWN"] = 0x00A1;
	commands[L"WM_NCLBUTTONUP"] = 0x00A2;
	commands[L"WM_NCMBUTTONDBLCLK"] = 0x00A9;
	commands[L"WM_NCMBUTTONDOWN"] = 0x00A7;
	commands[L"WM_NCMBUTTONUP"] = 0x00A8;
	commands[L"WM_NCMOUSEHOVER"] = 0x02A0;
	commands[L"WM_NCMOUSELEAVE"] = 0x02A2;
	commands[L"WM_NCMOUSEMOVE"] = 0x00A0;
	commands[L"WM_NCPAINT"] = 0x0085;
	commands[L"WM_NCRBUTTONDBLCLK"] = 0x00A6;
	commands[L"WM_NCRBUTTONDOWN"] = 0x00A4;
	commands[L"WM_NCRBUTTONUP"] = 0x00A5;
	commands[L"WM_NCXBUTTONDBLCLK"] = 0x00AD;
	commands[L"WM_NCXBUTTONDOWN"] = 0x00AB;
	commands[L"WM_NCXBUTTONUP"] = 0x00AC;
	commands[L"WM_NEXTDLGCTL"] = 0x0028;
	commands[L"WM_NEXTMENU"] = 0x0213;
	commands[L"WM_NOTIFY"] = 0x004E;
	commands[L"WM_NOTIFYFORMAT"] = 0x0055;
	commands[L"WM_NULL"] = 0x0000;
	commands[L"WM_PAINT"] = 0x000F;
	commands[L"WM_PAINTCLIPBOARD"] = 0x0309;
	commands[L"WM_PAINTICON"] = 0x0026;
	commands[L"WM_PALETTECHANGED"] = 0x0311;
	commands[L"WM_PALETTEISCHANGING"] = 0x0310;
	commands[L"WM_PARENTNOTIFY"] = 0x0210;
	commands[L"WM_PASTE"] = 0x0302;
	commands[L"WM_PENWINFIRST"] = 0x0380;
	commands[L"WM_PENWINLAST"] = 0x038F;
	commands[L"WM_POWER"] = 0x0048;
	commands[L"WM_POWERBROADCAST"] = 0x0218;
	commands[L"WM_PRINT"] = 0x0317;
	commands[L"WM_PRINTCLIENT"] = 0x0318;
	commands[L"WM_QUERYDRAGICON"] = 0x0037;
	commands[L"WM_QUERYENDSESSION"] = 0x0011;
	commands[L"WM_QUERYNEWPALETTE"] = 0x030F;
	commands[L"WM_QUERYOPEN"] = 0x0013;
	commands[L"WM_QUERYUISTATE"] = 0x0129;
	commands[L"WM_QUEUESYNC"] = 0x0023;
	commands[L"WM_QUIT"] = 0x0012;
	commands[L"WM_RBUTTONDBLCLK"] = 0x0206;
	commands[L"WM_RBUTTONDOWN"] = 0x0204;
	commands[L"WM_RBUTTONUP"] = 0x0205;
	commands[L"WM_RENDERALLFORMATS"] = 0x0306;
	commands[L"WM_RENDERFORMAT"] = 0x0305;
	commands[L"WM_SETCURSOR"] = 0x0020;
	commands[L"WM_SETFOCUS"] = 0x0007;
	commands[L"WM_SETFONT"] = 0x0030;
	commands[L"WM_SETHOTKEY"] = 0x0032;
	commands[L"WM_SETICON"] = 0x0080;
	commands[L"WM_SETREDRAW"] = 0x000B;
	commands[L"WM_SETTEXT"] = 0x000C;
	commands[L"WM_SETTINGCHANGE"] = 0x001A;
	commands[L"WM_SHOWWINDOW"] = 0x0018;
	commands[L"WM_SIZE"] = 0x0005;
	commands[L"WM_SIZECLIPBOARD"] = 0x030B;
	commands[L"WM_SIZING"] = 0x0214;
	commands[L"WM_SPOOLERSTATUS"] = 0x002A;
	commands[L"WM_STYLECHANGED"] = 0x007D;
	commands[L"WM_STYLECHANGING"] = 0x007C;
	commands[L"WM_SYNCPAINT"] = 0x0088;
	commands[L"WM_SYSCHAR"] = 0x0106;
	commands[L"WM_SYSCOLORCHANGE"] = 0x0015;
	commands[L"WM_SYSCOMMAND"] = 0x0112;
	commands[L"WM_SYSDEADCHAR"] = 0x0107;
	commands[L"WM_SYSKEYDOWN"] = 0x0104;
	commands[L"WM_SYSKEYUP"] = 0x0105;
	commands[L"WM_TABLET_FIRST"] = 0x02c0;
	commands[L"WM_TABLET_LAST"] = 0x02df;
	commands[L"WM_TCARD"] = 0x0052;
	commands[L"WM_THEMECHANGED"] = 0x031A;
	commands[L"WM_TIMECHANGE"] = 0x001E;
	commands[L"WM_TIMER"] = 0x0113;
	commands[L"WM_UNDO"] = 0x0304;
	commands[L"WM_UNICHAR"] = 0x0109;
	commands[L"WM_UNINITMENUPOPUP"] = 0x0125;
	commands[L"WM_UPDATEUISTATE"] = 0x0128;
	commands[L"WM_USER"] = 0x0400;
	commands[L"WM_USERCHANGED"] = 0x0054;
	commands[L"WM_VKEYTOITEM"] = 0x002E;
	commands[L"WM_VSCROLL"] = 0x0115;
	commands[L"WM_VSCROLLCLIPBOARD"] = 0x030A;
	commands[L"WM_WINDOWPOSCHANGED"] = 0x0047;
	commands[L"WM_WINDOWPOSCHANGING"] = 0x0046;
	commands[L"WM_WININICHANGE"] = 0x001A;
	commands[L"WM_WTSSESSION_CHANGE"] = 0x02B1;
	commands[L"WM_XBUTTONDBLCLK"] = 0x020D;
	commands[L"WM_XBUTTONDOWN"] = 0x020B;
	commands[L"WM_XBUTTONUP"] = 0x020C;

	values[L"SW_HIDE"] = 0;
	values[L"SW_SHOWNORMAL"] = 1;
	values[L"SW_NORMAL"] = 1;
	values[L"SW_SHOWMINIMIZED"] = 2;
	values[L"SW_SHOWMAXIMIZED"] = 3;
	values[L"SW_MAXIMIZE"] = 3;
	values[L"SW_SHOWNOACTIVATE"] = 4;
	values[L"SW_SHOW"] = 5;
	values[L"SW_MINIMIZE"] = 6;
	values[L"SW_SHOWMINNOACTIVE"] = 7;
	values[L"SW_SHOWNA"] = 8;
	values[L"SW_RESTORE"] = 9;
	values[L"SW_SHOWDEFAULT"] = 10;
	values[L"SW_FORCEMINIMIZE"] = 11;
	values[L"SW_MAX"] = 11;

	values[L"VK_LBUTTON"] = 0x01;
	values[L"VK_RBUTTON"] = 0x02;
	values[L"VK_CANCEL"] = 0x03;
	values[L"VK_MBUTTON"] = 0x04;
	values[L"VK_XBUTTON1"] = 0x05;
	values[L"VK_XBUTTON2"] = 0x06;
	values[L"VK_BACK"] = 0x08;
	values[L"VK_TAB"] = 0x09;
	values[L"VK_CLEAR"] = 0x0C;
	values[L"VK_RETURN"] = 0x0D;
	values[L"VK_SHIFT"] = 0x10;
	values[L"VK_CONTROL"] = 0x11;
	values[L"VK_MENU"] = 0x12;
	values[L"VK_PAUSE"] = 0x13;
	values[L"VK_CAPITAL"] = 0x14;
	values[L"VK_KANA"] = 0x15;
	values[L"VK_HANGEUL"] = 0x15;
	values[L"VK_HANGUL"] = 0x15;
	values[L"VK_JUNJA"] = 0x17;
	values[L"VK_FINAL"] = 0x18;
	values[L"VK_HANJA"] = 0x19;
	values[L"VK_KANJI"] = 0x19;
	values[L"VK_ESCAPE"] = 0x1B;
	values[L"VK_CONVERT"] = 0x1C;
	values[L"VK_NONCONVERT"] = 0x1D;
	values[L"VK_ACCEPT"] = 0x1E;
	values[L"VK_MODECHANGE"] = 0x1F;
	values[L"VK_SPACE"] = 0x20;
	values[L"VK_PRIOR"] = 0x21;
	values[L"VK_NEXT"] = 0x22;
	values[L"VK_END"] = 0x23;
	values[L"VK_HOME"] = 0x24;
	values[L"VK_LEFT"] = 0x25;
	values[L"VK_UP"] = 0x26;
	values[L"VK_RIGHT"] = 0x27;
	values[L"VK_DOWN"] = 0x28;
	values[L"VK_SELECT"] = 0x29;
	values[L"VK_PRINT"] = 0x2A;
	values[L"VK_EXECUTE"] = 0x2B;
	values[L"VK_SNAPSHOT"] = 0x2C;
	values[L"VK_INSERT"] = 0x2D;
	values[L"VK_DELETE"] = 0x2E;
	values[L"VK_HELP"] = 0x2F;
	values[L"VK_LWIN"] = 0x5B;
	values[L"VK_RWIN"] = 0x5C;
	values[L"VK_APPS"] = 0x5D;
	values[L"VK_SLEEP"] = 0x5F;
	values[L"VK_NUMPAD0"] = 0x60;
	values[L"VK_NUMPAD1"] = 0x61;
	values[L"VK_NUMPAD2"] = 0x62;
	values[L"VK_NUMPAD3"] = 0x63;
	values[L"VK_NUMPAD4"] = 0x64;
	values[L"VK_NUMPAD5"] = 0x65;
	values[L"VK_NUMPAD6"] = 0x66;
	values[L"VK_NUMPAD7"] = 0x67;
	values[L"VK_NUMPAD8"] = 0x68;
	values[L"VK_NUMPAD9"] = 0x69;
	values[L"VK_MULTIPLY"] = 0x6A;
	values[L"VK_ADD"] = 0x6B;
	values[L"VK_SEPARATOR"] = 0x6C;
	values[L"VK_SUBTRACT"] = 0x6D;
	values[L"VK_DECIMAL"] = 0x6E;
	values[L"VK_DIVIDE"] = 0x6F;
	values[L"VK_F1"] = 0x70;
	values[L"VK_F2"] = 0x71;
	values[L"VK_F3"] = 0x72;
	values[L"VK_F4"] = 0x73;
	values[L"VK_F5"] = 0x74;
	values[L"VK_F6"] = 0x75;
	values[L"VK_F7"] = 0x76;
	values[L"VK_F8"] = 0x77;
	values[L"VK_F9"] = 0x78;
	values[L"VK_F10"] = 0x79;
	values[L"VK_F11"] = 0x7A;
	values[L"VK_F12"] = 0x7B;
	values[L"VK_F13"] = 0x7C;
	values[L"VK_F14"] = 0x7D;
	values[L"VK_F15"] = 0x7E;
	values[L"VK_F16"] = 0x7F;
	values[L"VK_F17"] = 0x80;
	values[L"VK_F18"] = 0x81;
	values[L"VK_F19"] = 0x82;
	values[L"VK_F20"] = 0x83;
	values[L"VK_F21"] = 0x84;
	values[L"VK_F22"] = 0x85;
	values[L"VK_F23"] = 0x86;
	values[L"VK_F24"] = 0x87;
	values[L"VK_NUMLOCK"] = 0x90;
	values[L"VK_SCROLL"] = 0x91;
	values[L"VK_OEM_NEC_EQUAL"] = 0x92;
	values[L"VK_OEM_FJ_JISHO"] = 0x92;
	values[L"VK_OEM_FJ_MASSHOU"] = 0x93;
	values[L"VK_OEM_FJ_TOUROKU"] = 0x94;
	values[L"VK_OEM_FJ_LOYA"] = 0x95;
	values[L"VK_OEM_FJ_ROYA"] = 0x96;
	values[L"VK_LSHIFT"] = 0xA0;
	values[L"VK_RSHIFT"] = 0xA1;
	values[L"VK_LCONTROL"] = 0xA2;
	values[L"VK_RCONTROL"] = 0xA3;
	values[L"VK_LMENU"] = 0xA4;
	values[L"VK_RMENU"] = 0xA5;
	values[L"VK_BROWSER_BACK"] = 0xA6;
	values[L"VK_BROWSER_FORWARD"] = 0xA7;
	values[L"VK_BROWSER_REFRESH"] = 0xA8;
	values[L"VK_BROWSER_STOP"] = 0xA9;
	values[L"VK_BROWSER_SEARCH"] = 0xAA;
	values[L"VK_BROWSER_FAVORITES"] = 0xAB;
	values[L"VK_BROWSER_HOME"] = 0xAC;
	values[L"VK_VOLUME_MUTE"] = 0xAD;
	values[L"VK_VOLUME_DOWN"] = 0xAE;
	values[L"VK_VOLUME_UP"] = 0xAF;
	values[L"VK_MEDIA_NEXT_TRACK"] = 0xB0;
	values[L"VK_MEDIA_PREV_TRACK"] = 0xB1;
	values[L"VK_MEDIA_STOP"] = 0xB2;
	values[L"VK_MEDIA_PLAY_PAUSE"] = 0xB3;
	values[L"VK_LAUNCH_MAIL"] = 0xB4;
	values[L"VK_LAUNCH_MEDIA_SELECT"] = 0xB5;
	values[L"VK_LAUNCH_APP1"] = 0xB6;
	values[L"VK_LAUNCH_APP2"] = 0xB7;
	values[L"VK_OEM_1"] = 0xBA;
	values[L"VK_OEM_PLUS"] = 0xBB;
	values[L"VK_OEM_COMMA"] = 0xBC;
	values[L"VK_OEM_MINUS"] = 0xBD;
	values[L"VK_OEM_PERIOD"] = 0xBE;
	values[L"VK_OEM_2"] = 0xBF;
	values[L"VK_OEM_3"] = 0xC0;
	values[L"VK_OEM_4"] = 0xDB;
	values[L"VK_OEM_5"] = 0xDC;
	values[L"VK_OEM_6"] = 0xDD;
	values[L"VK_OEM_7"] = 0xDE;
	values[L"VK_OEM_8"] = 0xDF;
	values[L"VK_OEM_AX"] = 0xE1;
	values[L"VK_OEM_102"] = 0xE2;
	values[L"VK_ICO_HELP"] = 0xE3;
	values[L"VK_ICO_00"] = 0xE4;
	values[L"VK_PROCESSKEY"] = 0xE5;
	values[L"VK_ICO_CLEAR"] = 0xE6;
	values[L"VK_PACKET"] = 0xE7;
	values[L"VK_OEM_RESET"] = 0xE9;
	values[L"VK_OEM_JUMP"] = 0xEA;
	values[L"VK_OEM_PA1"] = 0xEB;
	values[L"VK_OEM_PA2"] = 0xEC;
	values[L"VK_OEM_PA3"] = 0xED;
	values[L"VK_OEM_WSCTRL"] = 0xEE;
	values[L"VK_OEM_CUSEL"] = 0xEF;
	values[L"VK_OEM_ATTN"] = 0xF0;
	values[L"VK_OEM_FINISH"] = 0xF1;
	values[L"VK_OEM_COPY"] = 0xF2;
	values[L"VK_OEM_AUTO"] = 0xF3;
	values[L"VK_OEM_ENLW"] = 0xF4;
	values[L"VK_OEM_BACKTAB"] = 0xF5;
	values[L"VK_ATTN"] = 0xF6;
	values[L"VK_CRSEL"] = 0xF7;
	values[L"VK_EXSEL"] = 0xF8;
	values[L"VK_EREOF"] = 0xF9;
	values[L"VK_PLAY"] = 0xFA;
	values[L"VK_ZOOM"] = 0xFB;
	values[L"VK_NONAME"] = 0xFC;
	values[L"VK_PA1"] = 0xFD;
	values[L"VK_OEM_CLEAR"] = 0xFE;

	values[L"WA_INACTIVE"] = 0;
	values[L"WA_ACTIVE"] = 1;
	values[L"WA_CLICKACTIVE"] = 2;

	values[L"PWR_OK"] = 1;
	values[L"PWR_FAIL"] = -1;
	values[L"PWR_SUSPENDREQUEST"] = 1;
	values[L"PWR_SUSPENDRESUME"] = 2;
	values[L"PWR_CRITICALRESUME"] = 3;

	values[L"NFR_ANSI"] = 1;
	values[L"NFR_UNICODE"] = 2;
	values[L"NF_QUERY"] = 3;
	values[L"NF_REQUERY"] = 4;

	values[L"UIS_SET"] = 1;
	values[L"UIS_CLEAR"] = 2;
	values[L"UIS_INITIALIZE"] = 3;
	values[L"UISF_HIDEFOCUS"] = 0x1;
	values[L"UISF_HIDEACCEL"] = 0x2;
	values[L"UISF_ACTIVE"] = 0x4;

	values[L"MN_GETHMENU"] = 0x01E1;

	values[L"WMSZ_LEFT"] = 1;
	values[L"WMSZ_RIGHT"] = 2;
	values[L"WMSZ_TOP"] = 3;
	values[L"WMSZ_TOPLEFT"] = 4;
	values[L"WMSZ_TOPRIGHT"] = 5;
	values[L"WMSZ_BOTTOM"] = 6;
	values[L"WMSZ_BOTTOMLEFT"] = 7;
	values[L"WMSZ_BOTTOMRIGHT"] = 8;

	values[L"HTERROR"] = -2;
	values[L"HTTRANSPARENT"] = -1;
	values[L"HTNOWHERE"] = 0;
	values[L"HTCLIENT"] = 1;
	values[L"HTCAPTION"] = 2;
	values[L"HTSYSMENU"] = 3;
	values[L"HTGROWBOX"] = 4;
	values[L"HTSIZE"] = 4;
	values[L"HTMENU"] = 5;
	values[L"HTHSCROLL"] = 6;
	values[L"HTVSCROLL"] = 7;
	values[L"HTMINBUTTON"] = 8;
	values[L"HTMAXBUTTON"] = 9;
	values[L"HTLEFT"] = 10;
	values[L"HTRIGHT"] = 11;
	values[L"HTTOP"] = 12;
	values[L"HTTOPLEFT"] = 13;
	values[L"HTTOPRIGHT"] = 14;
	values[L"HTBOTTOM"] = 15;
	values[L"HTBOTTOMLEFT"] = 16;
	values[L"HTBOTTOMRIGHT"] = 17;
	values[L"HTBORDER"] = 18;
	values[L"HTREDUCE"] = 8;
	values[L"HTZOOM"] = 9;
	values[L"HTSIZEFIRST"] = 10;
	values[L"HTSIZELAST"] = 17;
	values[L"HTOBJECT"] = 19;
	values[L"HTCLOSE"] = 20;
	values[L"HTHELP"] = 21;

	values[L"ICON_SMALL"] = 0;
	values[L"ICON_BIG"] = 1;
	values[L"ICON_SMALL2"] = 2;

	values[L"SIZE_RESTORED"] = 0;
	values[L"SIZE_MINIMIZED"] = 1;
	values[L"SIZE_MAXIMIZED"] = 2;
	values[L"SIZE_MAXSHOW"] = 3;
	values[L"SIZE_MAXHIDE"] = 4;

	values[L"WS_OVERLAPPED"] = 0x00000000L;
	values[L"WS_POPUP"] = 0x80000000L;
	values[L"WS_CHILD"] = 0x40000000L;
	values[L"WS_MINIMIZE"] = 0x20000000L;
	values[L"WS_VISIBLE"] = 0x10000000L;
	values[L"WS_DISABLED"] = 0x08000000L;
	values[L"WS_CLIPSIBLINGS"] = 0x04000000L;
	values[L"WS_CLIPCHILDREN"] = 0x02000000L;
	values[L"WS_MAXIMIZE"] = 0x01000000L;
	values[L"WS_CAPTION"] = 0x00C00000L;
	values[L"WS_BORDER"] = 0x00800000L;
	values[L"WS_DLGFRAME"] = 0x00400000L;
	values[L"WS_VSCROLL"] = 0x00200000L;
	values[L"WS_HSCROLL"] = 0x00100000L;
	values[L"WS_SYSMENU"] = 0x00080000L;
	values[L"WS_THICKFRAME"] = 0x00040000L;
	values[L"WS_GROUP"] = 0x00020000L;
	values[L"WS_TABSTOP"] = 0x00010000L;
	values[L"WS_MINIMIZEBOX"] = 0x00020000L;
	values[L"WS_MAXIMIZEBOX"] = 0x00010000L;
	values[L"WS_TILED"] = 0x00000000L;
	values[L"WS_ICONIC"] = 0x20000000L;
	values[L"WS_SIZEBOX"] = 0x00040000L;
	values[L"WS_CHILDWINDOW"] = 0x40000000L;
	values[L"WS_TILEDWINDOW"] = WS_OVERLAPPEDWINDOW;
	values[L"WS_OVERLAPPEDWINDOW"] = (WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX);
	values[L"WS_POPUPWINDOW"] = (WS_POPUP|WS_BORDER|WS_SYSMENU);

	values[L"WS_EX_DLGMODALFRAME"] = 0x00000001L;
	values[L"WS_EX_NOPARENTNOTIFY"] = 0x00000004L;
	values[L"WS_EX_TOPMOST"] = 0x00000008L;
	values[L"WS_EX_ACCEPTFILES"] = 0x00000010L;
	values[L"WS_EX_TRANSPARENT"] = 0x00000020L;
	values[L"WS_EX_MDICHILD"] = 0x00000040L;
	values[L"WS_EX_TOOLWINDOW"] = 0x00000080L;
	values[L"WS_EX_WINDOWEDGE"] = 0x00000100L;
	values[L"WS_EX_CLIENTEDGE"] = 0x00000200L;
	values[L"WS_EX_CONTEXTHELP"] = 0x00000400L;
	values[L"WS_EX_RIGHT"] = 0x00001000L;
	values[L"WS_EX_LEFT"] = 0x00000000L;
	values[L"WS_EX_RTLREADING"] = 0x00002000L;
	values[L"WS_EX_LTRREADING"] = 0x00000000L;
	values[L"WS_EX_LEFTSCROLLBAR"] = 0x00004000L;
	values[L"WS_EX_RIGHTSCROLLBAR"] = 0x00000000L;
	values[L"WS_EX_CONTROLPARENT"] = 0x00010000L;
	values[L"WS_EX_STATICEDGE"] = 0x00020000L;
	values[L"WS_EX_APPWINDOW"] = 0x00040000L;
	values[L"WS_EX_OVERLAPPEDWINDOW"] = (WS_EX_WINDOWEDGE|WS_EX_CLIENTEDGE);
	values[L"WS_EX_PALETTEWINDOW"] = (WS_EX_WINDOWEDGE|WS_EX_TOOLWINDOW|WS_EX_TOPMOST);
	values[L"WS_EX_LAYERED"] = 0x00080000;
	values[L"WS_EX_NOINHERITLAYOUT"] = 0x00100000L;
	values[L"WS_EX_LAYOUTRTL"] = 0x00400000L;
	values[L"WS_EX_COMPOSITED"] = 0x02000000L;
	values[L"WS_EX_NOACTIVATE"] = 0x08000000L;

	values[L"CS_VREDRAW"] = 0x0001;
	values[L"CS_HREDRAW"] = 0x0002;
	values[L"CS_DBLCLKS"] = 0x0008;
	values[L"CS_OWNDC"] = 0x0020;
	values[L"CS_CLASSDC"] = 0x0040;
	values[L"CS_PARENTDC"] = 0x0080;
	values[L"CS_NOCLOSE"] = 0x0200;
	values[L"CS_SAVEBITS"] = 0x0800;
	values[L"CS_BYTEALIGNCLIENT"] = 0x1000;
	values[L"CS_BYTEALIGNWINDOW"] = 0x2000;
	values[L"CS_GLOBALCLASS"] = 0x4000;
	values[L"CS_IME"] = 0x00010000;
	values[L"CS_DROPSHADOW"] = 0x00020000;

	values[L"CF_TEXT"] = 1;
	values[L"CF_BITMAP"] = 2;
	values[L"CF_METAFILEPICT"] = 3;
	values[L"CF_SYLK"] = 4;
	values[L"CF_DIF"] = 5;
	values[L"CF_TIFF"] = 6;
	values[L"CF_OEMTEXT"] = 7;
	values[L"CF_DIB"] = 8;
	values[L"CF_PALETTE"] = 9;
	values[L"CF_PENDATA"] = 10;
	values[L"CF_RIFF"] = 11;
	values[L"CF_WAVE"] = 12;
	values[L"CF_UNICODETEXT"] = 13;
	values[L"CF_ENHMETAFILE"] = 14;
	values[L"CF_HDROP"] = 15;
	values[L"CF_LOCALE"] = 16;
	values[L"CF_DIBV5"] = 17;
	values[L"CF_MAX"] = 18;
	values[L"CF_MAX"] = 17;
	values[L"CF_MAX"] = 15;
	values[L"CF_OWNERDISPLAY"] = 0x0080;
	values[L"CF_DSPTEXT"] = 0x0081;
	values[L"CF_DSPBITMAP"] = 0x0082;
	values[L"CF_DSPMETAFILEPICT"] = 0x0083;
	values[L"CF_DSPENHMETAFILE"] = 0x008E;
	values[L"CF_PRIVATEFIRST"] = 0x0200;
	values[L"CF_PRIVATELAST"] = 0x02FF;
	values[L"CF_GDIOBJFIRST"] = 0x0300;
	values[L"CF_GDIOBJLAST"] = 0x03FF;

	values[L"MF_INSERT"] = 0x00000000L;
	values[L"MF_CHANGE"] = 0x00000080L;
	values[L"MF_APPEND"] = 0x00000100L;
	values[L"MF_DELETE"] = 0x00000200L;
	values[L"MF_REMOVE"] = 0x00001000L;
	values[L"MF_BYCOMMAND"] = 0x00000000L;
	values[L"MF_BYPOSITION"] = 0x00000400L;
	values[L"MF_SEPARATOR"] = 0x00000800L;
	values[L"MF_ENABLED"] = 0x00000000L;
	values[L"MF_GRAYED"] = 0x00000001L;
	values[L"MF_DISABLED"] = 0x00000002L;
	values[L"MF_UNCHECKED"] = 0x00000000L;
	values[L"MF_CHECKED"] = 0x00000008L;
	values[L"MF_USECHECKBITMAPS"] = 0x00000200L;
	values[L"MF_STRING"] = 0x00000000L;
	values[L"MF_BITMAP"] = 0x00000004L;
	values[L"MF_OWNERDRAW"] = 0x00000100L;
	values[L"MF_POPUP"] = 0x00000010L;
	values[L"MF_MENUBARBREAK"] = 0x00000020L;
	values[L"MF_MENUBREAK"] = 0x00000040L;
	values[L"MF_UNHILITE"] = 0x00000000L;
	values[L"MF_HILITE"] = 0x00000080L;
	values[L"MF_DEFAULT"] = 0x00001000L;
	values[L"MF_SYSMENU"] = 0x00002000L;
	values[L"MF_HELP"] = 0x00004000L;
	values[L"MF_RIGHTJUSTIFY"] = 0x00004000L;
	values[L"MF_MOUSESELECT"] = 0x00008000L;
	values[L"MF_END"] = 0x00000080L;
	values[L"MFT_STRING"] = MF_STRING;
	values[L"MFT_BITMAP"] = MF_BITMAP;
	values[L"MFT_MENUBARBREAK"] = MF_MENUBARBREAK;
	values[L"MFT_MENUBREAK"] = MF_MENUBREAK;
	values[L"MFT_OWNERDRAW"] = MF_OWNERDRAW;
	values[L"MFT_RADIOCHECK"] = 0x00000200L;
	values[L"MFT_SEPARATOR"] = MF_SEPARATOR;
	values[L"MFT_RIGHTORDER"] = 0x00002000L;
	values[L"MFT_RIGHTJUSTIFY"] = MF_RIGHTJUSTIFY;
	values[L"MFS_GRAYED"] = 0x00000003L;
	values[L"MFS_DISABLED"] = MFS_GRAYED;
	values[L"MFS_CHECKED"] = MF_CHECKED;
	values[L"MFS_HILITE"] = MF_HILITE;
	values[L"MFS_ENABLED"] = MF_ENABLED;
	values[L"MFS_UNCHECKED"] = MF_UNCHECKED;
	values[L"MFS_UNHILITE"] = MF_UNHILITE;
	values[L"MFS_DEFAULT"] = MF_DEFAULT;
	values[L"MF_END"] = 0x00000080L;

	values[L"MFT_STRING"] = MF_STRING;
	values[L"MFT_BITMAP"] = MF_BITMAP;
	values[L"MFT_MENUBARBREAK"] = MF_MENUBARBREAK;
	values[L"MFT_MENUBREAK"] = MF_MENUBREAK;
	values[L"MFT_OWNERDRAW"] = MF_OWNERDRAW;
	values[L"MFT_RADIOCHECK"] = 0x00000200L;
	values[L"MFT_SEPARATOR"] = MF_SEPARATOR;
	values[L"MFT_RIGHTORDER"] = 0x00002000L;
	values[L"MFT_RIGHTJUSTIFY"] = MF_RIGHTJUSTIFY;
	values[L"MFS_GRAYED"] = 0x00000003L;
	values[L"MFS_DISABLED"] = MFS_GRAYED;
	values[L"MFS_CHECKED"] = MF_CHECKED;
	values[L"MFS_HILITE"] = MF_HILITE;
	values[L"MFS_ENABLED"] = MF_ENABLED;
	values[L"MFS_UNCHECKED"] = MF_UNCHECKED;
	values[L"MFS_UNHILITE"] = MF_UNHILITE;
	values[L"MFS_DEFAULT"] = MF_DEFAULT;
	values[L"MF_END"] = 0x00000080L;

	values[L"SC_SIZE"] = 0xF000;
	values[L"SC_MOVE"] = 0xF010;
	values[L"SC_MINIMIZE"] = 0xF020;
	values[L"SC_MAXIMIZE"] = 0xF030;
	values[L"SC_NEXTWINDOW"] = 0xF040;
	values[L"SC_PREVWINDOW"] = 0xF050;
	values[L"SC_CLOSE"] = 0xF060;
	values[L"SC_VSCROLL"] = 0xF070;
	values[L"SC_HSCROLL"] = 0xF080;
	values[L"SC_MOUSEMENU"] = 0xF090;
	values[L"SC_KEYMENU"] = 0xF100;
	values[L"SC_ARRANGE"] = 0xF110;
	values[L"SC_RESTORE"] = 0xF120;
	values[L"SC_TASKLIST"] = 0xF130;
	values[L"SC_SCREENSAVE"] = 0xF140;
	values[L"SC_HOTKEY"] = 0xF150;
	values[L"SC_DEFAULT"] = 0xF160;
	values[L"SC_MONITORPOWER"] = 0xF170;
	values[L"SC_CONTEXTHELP"] = 0xF180;
	values[L"SC_SEPARATOR"] = 0xF00F;
	values[L"SC_ICON"] = SC_MINIMIZE;
	values[L"SC_ZOOM"] = SC_MAXIMIZE;

	values[L"OBM_CLOSE"] = 32754;
	values[L"OBM_UPARROW"] = 32753;
	values[L"OBM_DNARROW"] = 32752;
	values[L"OBM_RGARROW"] = 32751;
	values[L"OBM_LFARROW"] = 32750;
	values[L"OBM_REDUCE"] = 32749;
	values[L"OBM_ZOOM"] = 32748;
	values[L"OBM_RESTORE"] = 32747;
	values[L"OBM_REDUCED"] = 32746;
	values[L"OBM_ZOOMD"] = 32745;
	values[L"OBM_RESTORED"] = 32744;
	values[L"OBM_UPARROWD"] = 32743;
	values[L"OBM_DNARROWD"] = 32742;
	values[L"OBM_RGARROWD"] = 32741;
	values[L"OBM_LFARROWD"] = 32740;
	values[L"OBM_MNARROW"] = 32739;
	values[L"OBM_COMBO"] = 32738;
	values[L"OBM_UPARROWI"] = 32737;
	values[L"OBM_DNARROWI"] = 32736;
	values[L"OBM_RGARROWI"] = 32735;
	values[L"OBM_LFARROWI"] = 32734;
	values[L"OBM_OLD_CLOSE"] = 32767;
	values[L"OBM_SIZE"] = 32766;
	values[L"OBM_OLD_UPARROW"] = 32765;
	values[L"OBM_OLD_DNARROW"] = 32764;
	values[L"OBM_OLD_RGARROW"] = 32763;
	values[L"OBM_OLD_LFARROW"] = 32762;
	values[L"OBM_BTSIZE"] = 32761;
	values[L"OBM_CHECK"] = 32760;
	values[L"OBM_CHECKBOXES"] = 32759;
	values[L"OBM_BTNCORNERS"] = 32758;
	values[L"OBM_OLD_REDUCE"] = 32757;
	values[L"OBM_OLD_ZOOM"] = 32756;
	values[L"OBM_OLD_RESTORE"] = 32755;

	values[L"OCR_NORMAL"] = 32512;
	values[L"OCR_IBEAM"] = 32513;
	values[L"OCR_WAIT"] = 32514;
	values[L"OCR_CROSS"] = 32515;
	values[L"OCR_UP"] = 32516;
	values[L"OCR_SIZE"] = 32640;
	values[L"OCR_ICON"] = 32641;
	values[L"OCR_SIZENWSE"] = 32642;
	values[L"OCR_SIZENESW"] = 32643;
	values[L"OCR_SIZEWE"] = 32644;
	values[L"OCR_SIZENS"] = 32645;
	values[L"OCR_SIZEALL"] = 32646;
	values[L"OCR_ICOCUR"] = 32647;
	values[L"OCR_NO"] = 32648;
	values[L"OCR_HAND"] = 32649;
	values[L"OCR_APPSTARTING"] = 32650;

	values[L"OIC_SAMPLE"] = 32512;
	values[L"OIC_HAND"] = 32513;
	values[L"OIC_QUES"] = 32514;
	values[L"OIC_BANG"] = 32515;
	values[L"OIC_NOTE"] = 32516;
	values[L"OIC_WINLOGO"] = 32517;
	values[L"OIC_WARNING"] = 32515;
	values[L"OIC_ERROR"] = 32513;
	values[L"OIC_INFORMATION"] = 32516;

	values[L"IDI_APPLICATION"] = 32512;
	values[L"IDI_HAND"] = 32513;
	values[L"IDI_QUESTION"] = 32514;
	values[L"IDI_EXCLAMATION"] = 32515;
	values[L"IDI_ASTERISK"] = 32516;
	values[L"IDI_WINLOGO"] = 32517;
	values[L"IDI_WARNING"] = 32515;
	values[L"IDI_ERROR"] = 32513;
	values[L"IDI_INFORMATION"] = 32516;

	values[L"IDOK"] = 1;
	values[L"IDCANCEL"] = 2;
	values[L"IDABORT"] = 3;
	values[L"IDRETRY"] = 4;
	values[L"IDIGNORE"] = 5;
	values[L"IDYES"] = 6;
	values[L"IDNO"] = 7;
	values[L"IDCLOSE"] = 8;
	values[L"IDHELP"] = 9;
	values[L"IDTRYAGAIN"] = 10;
	values[L"IDCONTINUE"] = 11;
	values[L"IDTIMEOUT"] = 32000;

	values[L"ES_LEFT"] = 0x0000L;
	values[L"ES_CENTER"] = 0x0001L;
	values[L"ES_RIGHT"] = 0x0002L;
	values[L"ES_MULTILINE"] = 0x0004L;
	values[L"ES_UPPERCASE"] = 0x0008L;
	values[L"ES_LOWERCASE"] = 0x0010L;
	values[L"ES_PASSWORD"] = 0x0020L;
	values[L"ES_AUTOVSCROLL"] = 0x0040L;
	values[L"ES_AUTOHSCROLL"] = 0x0080L;
	values[L"ES_NOHIDESEL"] = 0x0100L;
	values[L"ES_OEMCONVERT"] = 0x0400L;
	values[L"ES_READONLY"] = 0x0800L;
	values[L"ES_WANTRETURN"] = 0x1000L;
	values[L"ES_NUMBER"] = 0x2000L;

	values[L"EM_GETSEL"] = 0x00B0;
	values[L"EM_SETSEL"] = 0x00B1;
	values[L"EM_GETRECT"] = 0x00B2;
	values[L"EM_SETRECT"] = 0x00B3;
	values[L"EM_SETRECTNP"] = 0x00B4;
	values[L"EM_SCROLL"] = 0x00B5;
	values[L"EM_LINESCROLL"] = 0x00B6;
	values[L"EM_SCROLLCARET"] = 0x00B7;
	values[L"EM_GETMODIFY"] = 0x00B8;
	values[L"EM_SETMODIFY"] = 0x00B9;
	values[L"EM_GETLINECOUNT"] = 0x00BA;
	values[L"EM_LINEINDEX"] = 0x00BB;
	values[L"EM_SETHANDLE"] = 0x00BC;
	values[L"EM_GETHANDLE"] = 0x00BD;
	values[L"EM_GETTHUMB"] = 0x00BE;
	values[L"EM_LINELENGTH"] = 0x00C1;
	values[L"EM_REPLACESEL"] = 0x00C2;
	values[L"EM_GETLINE"] = 0x00C4;
	values[L"EM_LIMITTEXT"] = 0x00C5;
	values[L"EM_CANUNDO"] = 0x00C6;
	values[L"EM_UNDO"] = 0x00C7;
	values[L"EM_FMTLINES"] = 0x00C8;
	values[L"EM_LINEFROMCHAR"] = 0x00C9;
	values[L"EM_SETTABSTOPS"] = 0x00CB;
	values[L"EM_SETPASSWORDCHAR"] = 0x00CC;
	values[L"EM_EMPTYUNDOBUFFER"] = 0x00CD;
	values[L"EM_GETFIRSTVISIBLELINE"] = 0x00CE;
	values[L"EM_SETREADONLY"] = 0x00CF;
	values[L"EM_SETWORDBREAKPROC"] = 0x00D0;
	values[L"EM_GETWORDBREAKPROC"] = 0x00D1;
	values[L"EM_GETPASSWORDCHAR"] = 0x00D2;
	values[L"EM_SETMARGINS"] = 0x00D3;
	values[L"EM_GETMARGINS"] = 0x00D4;
	values[L"EM_SETLIMITTEXT"] = EM_LIMITTEXT;
	values[L"EM_GETLIMITTEXT"] = 0x00D5;
	values[L"EM_POSFROMCHAR"] = 0x00D6;
	values[L"EM_CHARFROMPOS"] = 0x00D7;
	values[L"EM_SETIMESTATUS"] = 0x00D8;
	values[L"EM_GETIMESTATUS"] = 0x00D9;

	values[L"BS_PUSHBUTTON"] = 0x00000000L;
	values[L"BS_DEFPUSHBUTTON"] = 0x00000001L;
	values[L"BS_CHECKBOX"] = 0x00000002L;
	values[L"BS_AUTOCHECKBOX"] = 0x00000003L;
	values[L"BS_RADIOBUTTON"] = 0x00000004L;
	values[L"BS_3STATE"] = 0x00000005L;
	values[L"BS_AUTO3STATE"] = 0x00000006L;
	values[L"BS_GROUPBOX"] = 0x00000007L;
	values[L"BS_USERBUTTON"] = 0x00000008L;
	values[L"BS_AUTORADIOBUTTON"] = 0x00000009L;
	values[L"BS_PUSHBOX"] = 0x0000000AL;
	values[L"BS_OWNERDRAW"] = 0x0000000BL;
	values[L"BS_TYPEMASK"] = 0x0000000FL;
	values[L"BS_LEFTTEXT"] = 0x00000020L;
	values[L"BS_TEXT"] = 0x00000000L;
	values[L"BS_ICON"] = 0x00000040L;
	values[L"BS_BITMAP"] = 0x00000080L;
	values[L"BS_LEFT"] = 0x00000100L;
	values[L"BS_RIGHT"] = 0x00000200L;
	values[L"BS_CENTER"] = 0x00000300L;
	values[L"BS_TOP"] = 0x00000400L;
	values[L"BS_BOTTOM"] = 0x00000800L;
	values[L"BS_VCENTER"] = 0x00000C00L;
	values[L"BS_PUSHLIKE"] = 0x00001000L;
	values[L"BS_MULTILINE"] = 0x00002000L;
	values[L"BS_NOTIFY"] = 0x00004000L;
	values[L"BS_FLAT"] = 0x00008000L;
	values[L"BS_RIGHTBUTTON"] = BS_LEFTTEXT;

	values[L"BN_CLICKED"] = 0;
	values[L"BN_PAINT"] = 1;
	values[L"BN_HILITE"] = 2;
	values[L"BN_UNHILITE"] = 3;
	values[L"BN_DISABLE"] = 4;
	values[L"BN_DOUBLECLICKED"] = 5;
	values[L"BN_PUSHED"] = BN_HILITE;
	values[L"BN_UNPUSHED"] = BN_UNHILITE;
	values[L"BN_DBLCLK"] = BN_DOUBLECLICKED;
	values[L"BN_SETFOCUS"] = 6;
	values[L"BN_KILLFOCUS"] = 7;

	values[L"BM_GETCHECK"] = 0x00F0;
	values[L"BM_SETCHECK"] = 0x00F1;
	values[L"BM_GETSTATE"] = 0x00F2;
	values[L"BM_SETSTATE"] = 0x00F3;
	values[L"BM_SETSTYLE"] = 0x00F4;
	values[L"BM_CLICK"] = 0x00F5;
	values[L"BM_GETIMAGE"] = 0x00F6;
	values[L"BM_SETIMAGE"] = 0x00F7;

	values[L"BST_UNCHECKED"] = 0x0000;
	values[L"BST_CHECKED"] = 0x0001;
	values[L"BST_INDETERMINATE"] = 0x0002;
	values[L"BST_PUSHED"] = 0x0004;
	values[L"BST_FOCUS"] = 0x0008;

	values[L"SS_LEFT"] = 0x00000000L;
	values[L"SS_CENTER"] = 0x00000001L;
	values[L"SS_RIGHT"] = 0x00000002L;
	values[L"SS_ICON"] = 0x00000003L;
	values[L"SS_BLACKRECT"] = 0x00000004L;
	values[L"SS_GRAYRECT"] = 0x00000005L;
	values[L"SS_WHITERECT"] = 0x00000006L;
	values[L"SS_BLACKFRAME"] = 0x00000007L;
	values[L"SS_GRAYFRAME"] = 0x00000008L;
	values[L"SS_WHITEFRAME"] = 0x00000009L;
	values[L"SS_USERITEM"] = 0x0000000AL;
	values[L"SS_SIMPLE"] = 0x0000000BL;
	values[L"SS_LEFTNOWORDWRAP"] = 0x0000000CL;
	values[L"SS_OWNERDRAW"] = 0x0000000DL;
	values[L"SS_BITMAP"] = 0x0000000EL;
	values[L"SS_ENHMETAFILE"] = 0x0000000FL;
	values[L"SS_ETCHEDHORZ"] = 0x00000010L;
	values[L"SS_ETCHEDVERT"] = 0x00000011L;
	values[L"SS_ETCHEDFRAME"] = 0x00000012L;
	values[L"SS_TYPEMASK"] = 0x0000001FL;
	values[L"SS_REALSIZECONTROL"] = 0x00000040L;
	values[L"SS_NOPREFIX"] = 0x00000080L;
	values[L"SS_NOTIFY"] = 0x00000100L;
	values[L"SS_CENTERIMAGE"] = 0x00000200L;
	values[L"SS_RIGHTJUST"] = 0x00000400L;
	values[L"SS_REALSIZEIMAGE"] = 0x00000800L;
	values[L"SS_SUNKEN"] = 0x00001000L;
	values[L"SS_EDITCONTROL"] = 0x00002000L;
	values[L"SS_ENDELLIPSIS"] = 0x00004000L;
	values[L"SS_PATHELLIPSIS"] = 0x00008000L;
	values[L"SS_WORDELLIPSIS"] = 0x0000C000L;
	values[L"SS_ELLIPSISMASK"] = 0x0000C000L;

	values[L"DS_ABSALIGN"] = 0x01L;
	values[L"DS_SYSMODAL"] = 0x02L;
	values[L"DS_LOCALEDIT"] = 0x20L;
	values[L"DS_SETFONT"] = 0x40L;
	values[L"DS_MODALFRAME"] = 0x80L;
	values[L"DS_NOIDLEMSG"] = 0x100L;
	values[L"DS_SETFOREGROUND"] = 0x200L;
	values[L"DS_3DLOOK"] = 0x0004L;
	values[L"DS_FIXEDSYS"] = 0x0008L;
	values[L"DS_NOFAILCREATE"] = 0x0010L;
	values[L"DS_CONTROL"] = 0x0400L;
	values[L"DS_CENTER"] = 0x0800L;
	values[L"DS_CENTERMOUSE"] = 0x1000L;
	values[L"DS_CONTEXTHELP"] = 0x2000L;
	values[L"DS_SHELLFONT"] = (DS_SETFONT|DS_FIXEDSYS);
	values[L"DS_USEPIXELS"] = 0x8000L;

	values[L"LBS_NOTIFY"] = 0x0001L;
	values[L"LBS_SORT"] = 0x0002L;
	values[L"LBS_NOREDRAW"] = 0x0004L;
	values[L"LBS_MULTIPLESEL"] = 0x0008L;
	values[L"LBS_OWNERDRAWFIXED"] = 0x0010L;
	values[L"LBS_OWNERDRAWVARIABLE"] = 0x0020L;
	values[L"LBS_HASSTRINGS"] = 0x0040L;
	values[L"LBS_USETABSTOPS"] = 0x0080L;
	values[L"LBS_NOINTEGRALHEIGHT"] = 0x0100L;
	values[L"LBS_MULTICOLUMN"] = 0x0200L;
	values[L"LBS_WANTKEYBOARDINPUT"] = 0x0400L;
	values[L"LBS_EXTENDEDSEL"] = 0x0800L;
	values[L"LBS_DISABLENOSCROLL"] = 0x1000L;
	values[L"LBS_NODATA"] = 0x2000L;
	values[L"LBS_NOSEL"] = 0x4000L;
	values[L"LBS_COMBOBOX"] = 0x8000L;
	values[L"LBS_STANDARD"] = (LBS_NOTIFY|LBS_SORT|WS_VSCROLL|WS_BORDER);

	values[L"CBS_SIMPLE"] = 0x0001L;
	values[L"CBS_DROPDOWN"] = 0x0002L;
	values[L"CBS_DROPDOWNLIST"] = 0x0003L;
	values[L"CBS_OWNERDRAWFIXED"] = 0x0010L;
	values[L"CBS_OWNERDRAWVARIABLE"] = 0x0020L;
	values[L"CBS_AUTOHSCROLL"] = 0x0040L;
	values[L"CBS_OEMCONVERT"] = 0x0080L;
	values[L"CBS_SORT"] = 0x0100L;
	values[L"CBS_HASSTRINGS"] = 0x0200L;
	values[L"CBS_NOINTEGRALHEIGHT"] = 0x0400L;
	values[L"CBS_DISABLENOSCROLL"] = 0x0800L;
	values[L"CBS_UPPERCASE"] = 0x2000L;
	values[L"CBS_LOWERCASE"] = 0x4000L;

	values[L"SBS_HORZ"] = 0x0000L;
	values[L"SBS_VERT"] = 0x0001L;
	values[L"SBS_TOPALIGN"] = 0x0002L;
	values[L"SBS_LEFTALIGN"] = 0x0002L;
	values[L"SBS_BOTTOMALIGN"] = 0x0004L;
	values[L"SBS_RIGHTALIGN"] = 0x0004L;
	values[L"SBS_SIZEBOXTOPLEFTALIGN"] = 0x0002L;
	values[L"SBS_SIZEBOXBOTTOMRIGHTALIGN"] = 0x0004L;
	values[L"SBS_SIZEBOX"] = 0x0008L;
	values[L"SBS_SIZEGRIP"] = 0x0010L;

}

/////////////////////////////////////////////////////////////////////////////
// CDaphneApp initialization

BOOL CDaphneApp::InitInstance() {

  // Gettext
	setlocale(LC_ALL, "");
	bindtextdomain("DAPHNE", "LANG");
  bind_textdomain_codeset("DAPHNE", "UTF-8");
	textdomain("DAPHNE");

	// Exception handlers
	CString program;
	program.LoadString(IDS_TITLE_VER);

	AfxEnableControlContainer();

	if (!AfxSocketInit(&wsaData)) {
		AfxMessageBox(_("Unable to initialize Sockets API."), MB_OK | MB_ICONEXCLAMATION);
	}

	// Vemos si está el INI
	HANDLE check = CreateFile(DAPHNE_INI_FILE, FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (check != INVALID_HANDLE_VALUE) {
		CloseHandle(check);

		//First free the string allocated by MFC at CWinApp startup.
		//The string is allocated before InitInstance is called.
		free((void*)m_pszProfileName);
		//Change the name of the .INI file.
		//The CWinApp destructor will free the memory.
		m_pszProfileName=_tcsdup(DAPHNE_INI_FILE);
	}
	else
		SetRegistryKey(L"LHF");

	// Tratamos de obtener privilegios de DEBUG
	bool debugPrivileges = EnableDebugPriv();

	// Setup globales
	SetupMsgMap();

  // RTL
  DWORD flid = GetUserDefaultLangID();
  if ((PRIMARYLANGID(flid) == LANG_ARABIC || PRIMARYLANGID(flid) == LANG_HEBREW) &&
        AfxGetApp()->GetProfileInt(L"Advanced", L"IgnoreRTL", 0) == 0) {
    rtl = true;
    SetProcessDefaultLayout(LAYOUT_RTL);
    m_hInstResDLL = LoadLibrary(L"DRKRTL.dll");
  }
  else {
    m_hInstResDLL = LoadLibrary(L"DRKLTR.dll");
  }

  ASSERT( m_hInstResDLL != NULL );

  AfxSetResourceHandle(m_hInstResDLL);

	// Obtener version de Windows
	OSVersion.dwOSVersionInfoSize = sizeof(OSVersion);
	GetVersionEx(&OSVersion);

	::GetCurrentDirectory(sizeof(szDirectory) - 1, szDirectory);
	strSpyAgentDLL.Format(L"%s\\DRKSpyAgent.dll", szDirectory);
	strDRKHooksDLL.Format(L"%s\\DRKHooks.dll", szDirectory);
	strDRKShellDLL.Format(L"%s\\DRKShell.dll", szDirectory);

  TRACE("Working directory: %ws\n", szDirectory);

	GetModuleFileNameEx(GetCurrentProcess(), GetModuleHandle(NULL), strDaphneExe.GetBufferSetLength(MAX_PATH), MAX_PATH);
	strDaphneExe.ReleaseBuffer();

	strBaseDir.Format(L"%s\\", szDirectory);

  // Before we check for mutex, be sure it's not a command line execution
  if (wcslen(m_lpCmdLine) > 0) {
    // Command line

    // No se puede usar printf()
  }
  else {
    // GUI

	  // Running instances....
	  m_hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"LHFDRK_DAPHNE");

	  if (m_hMutex != NULL) {
      WCHAR tmp[30];
		  MessageBox(NULL, _("Another instance of Daphne is already running."), __("Daphne", tmp, sizeof(tmp)), MB_OK | MB_ICONINFORMATION);
	  }
	  else {
		  m_hMutex = CreateMutex(NULL, FALSE, L"LHFDRK_DAPHNE");

      // Advertencia de versión 32 bits corriendo en Windows x64
      BOOL isWow64 = TRUE;
	    IsWow64Process(GetCurrentProcess(), &isWow64);
	    if (isWow64) {
        WCHAR tmp[30];
	  	  MessageBox(NULL, _("You are running Daphne for 32 bits under a 64-bits Windows platform.\nMost features won't work. Please download and install Daphne for 64 bits."), __("Daphne", tmp, sizeof(tmp)), MB_OK | MB_ICONWARNING);
      }
      else {
        // Si está todo bien, vamos a las estadísticas
        CString version;
	      version.LoadString(IDS_TITLE_VER);
        CString cfg_version = AfxGetApp()->GetProfileStringW(L"CFG", L"Version", L"");
        bool first = version != cfg_version;

        /// OJO
        //first = true;
        //statistics_event = EVENT_CONFIG;

        //////////////////////////////////////////////////////////
        // A partir de 2.00 es obligatorio...

        // Sincrónico
        statistics_event = EVENT_START;
        CDaphneApp::staticSendStatistics(0);

        if (first) {
          // Si es la primera ejecución

          if (cfg_version != L"") {
            // Había una versión anterior así que envío estadísticas ahora, sincrónico
              statistics_event = EVENT_FEATURES;
              CDaphneApp::staticSendStatistics(0);
              statistics_event = EVENT_CONFIG;
              CDaphneApp::staticSendStatistics(0);
          }

          time_t t;
          time(&t);
          AfxGetApp()->WriteProfileString(L"CFG", L"Version", version); // Guardo versión
          AfxGetApp()->WriteProfileBinary(L"CFG", L"LastStatisticsReport", (LPBYTE)&t, sizeof(time_t));

          // Send...
          statistics_event = EVENT_FIRST_RUN;
          SendStatistics();
        }
        else {
          // Si no es la primera ejecución posinstalación
          time_t * t;
          UINT size;
          if (AfxGetApp()->GetProfileBinary(L"CFG", L"LastStatisticsReport", (LPBYTE*)&t, &size)) {
            time_t now, diff;
            time(&now);
            diff = now - *t;

            TRACE(L"Diff %u\n", diff);

            // Cada 10 días
            if (diff > 86400 * 10) {
              statistics_event = EVENT_FEATURES;
              SendStatistics();
            }
            // Cada 7 días
            if (diff > 86400 * 7) {
              statistics_event = EVENT_CONFIG;
              SendStatistics();
            }

            delete t;
          }
        }
      }

		  // Abrimos la ventana principal
		  CDaphneDlg dlg;
		  dlg.setDebugPrivilegeState(debugPrivileges);
		  m_pMainWnd = &dlg;
		  dlg.DoModal();

  	
	  } // Endif - mutex
  
  } // Endif - GUI


	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CDaphneApp::WinHelp(DWORD dwData, UINT nCmd) 
{
  CString str;
	str.LoadString(IDS_SUPPORTURL);

	::ShellExecute(NULL, L"OPEN", str, NULL, NULL, SW_SHOWNORMAL);	
}

void CDaphneApp::ChangeSettingsLocation(bool toINIFile)
{
	if (toINIFile && ((CDaphneApp*)AfxGetApp())->SettingsAtRegistry()) {
		HANDLE check = CreateFile(DAPHNE_INI_FILE, FILE_WRITE_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (check != INVALID_HANDLE_VALUE) {
			CloseHandle(check);

			free((void*)m_pszRegistryKey);
			m_pszRegistryKey = NULL;

			free((void*)m_pszProfileName);
			m_pszProfileName=_tcsdup(DAPHNE_INI_FILE);
		}
	}
	else if (!toINIFile && !((CDaphneApp*)AfxGetApp())->SettingsAtRegistry()) {
		free((void*)m_pszProfileName);
		m_pszProfileName = NULL;

		DeleteFile(DAPHNE_INI_FILE);
		SetRegistryKey(L"LHF");
	}
}

bool CDaphneApp::SettingsAtRegistry(void)
{
	return wcscmp(m_pszProfileName, DAPHNE_INI_FILE) != 0;
}

void CDaphneApp::HtmlHelp(DWORD_PTR dwData, UINT nCmd)
{
}

int CDaphneApp::ExitInstance()
{
  // Sincrónico
  statistics_event = EVENT_STOP;
  CDaphneApp::staticSendStatistics(0);

  FreeLibrary(m_hInstResDLL);
  return CWinApp::ExitInstance();
}


int CDaphneApp::SendStatistics(void)
{
  sendThread = AfxBeginThread(CDaphneApp::staticSendStatistics, (LPVOID)0, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
  return 0;
}

//static
UINT CDaphneApp::staticSendStatistics(LPVOID pParam)
{
  int s_event = theApp.statistics_event;
  CString str, rversion;
	CString proxy;
	CInternetSession * pInternet;
	CString url_server, url_path;
	url_server.LoadString(IDS_STAT_WEBURL_S);
  url_path.LoadString(IDS_STAT_WEBURL_P);
	CString version;
	version.LoadString(IDS_TITLE_VER);
	CString referrer, ref_format;
  ref_format.LoadString(IDS_REF_FORMAT);
	referrer.Format(ref_format, version, OSVersion.dwPlatformId, OSVersion.dwMajorVersion, OSVersion.dwMinorVersion);
  
	BOOL bUseProxy = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneProxy", 0);
	CString strProxyAddress = AfxGetApp()->GetProfileString(L"CFG", L"DaphneProxyAddress", L"");
	UINT iProxyPort = AfxGetApp()->GetProfileInt(L"CFG", L"DaphneProxyPort", 8080);

	proxy.Format(L"%s:%u", strProxyAddress, iProxyPort);	

	if (!bUseProxy)
		pInternet = new CInternetSession(referrer);
	else
		pInternet = new CInternetSession(referrer, 1, INTERNET_OPEN_TYPE_PROXY, proxy, NULL, 0);

  CString uuid = AfxGetApp()->GetProfileStringW(L"CFG", L"AnonymousUUID");
  bool saveUUID = uuid.IsEmpty();
  CString strHeaders = L"Content-Type: application/x-www-form-urlencoded";
  CStringA strFormData;

  // Tipo de evento
  if (saveUUID) s_event = EVENT_FIRST_RUN; // Si no hay UUID fuerzo first run

  switch(s_event) {
  case EVENT_FIRST_RUN:
    strFormData.Format("event=%u&anonymous_uuid=%ls&dv=%ls&osa=%u&osb=%u&data=build %u", s_event, uuid, version, OSVersion.dwMajorVersion, OSVersion.dwMinorVersion, OSVersion.dwBuildNumber);
    break;
  case EVENT_FEATURES:
    strFormData.Format("event=%u&anonymous_uuid=%ls&%ls", s_event, uuid, dumpFeatures());
    break;
  case EVENT_CONFIG:
    strFormData.Format("event=%u&anonymous_uuid=%ls&%ls", s_event, uuid, dumpConfig());
    break;
  case EVENT_START:
  case EVENT_STOP:
    strFormData.Format("event=%u&anonymous_uuid=%ls", s_event, uuid);
    break;
  }
  //InternetCanonicalizeUrl();
  
  int result = 0;
	TRY
	{
    CString uuid;
    CHttpConnection *pConnection = pInternet->GetHttpConnection(url_server, (DWORD)INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE);
    CHttpFile *pFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, (LPCTSTR)url_path, NULL, 1UL, NULL, LPCTSTR("HTTP/1.1"), INTERNET_FLAG_EXISTING_CONNECT);
    result = pFile->SendRequest(strHeaders, (LPVOID) (LPCSTR)strFormData, strFormData.GetLength());
    if (saveUUID) {
      pFile->ReadString(uuid);
      AfxGetApp()->WriteProfileStringW(L"CFG", L"AnonymousUUID", uuid);
    }
    pFile->Close();
    pConnection->Close();

    // OK
    switch(s_event) {
    case EVENT_FIRST_RUN:
      break;
    case EVENT_FEATURES: {
        // Actualizo timestamp de último envío
        time_t t;
        time(&t);
        AfxGetApp()->WriteProfileBinary(L"CFG", L"LastStatisticsReport", (LPBYTE)&t, sizeof(time_t));
      }
      break;
    case EVENT_CONFIG:
      break;
    }

    delete pFile;
    delete pConnection;
	}
	CATCH( CInternetException, e )
	{
    TCHAR str[512];
    e->GetErrorMessage(str, 511);
    TRACE("CInternetException: %s", str);
	}
	END_CATCH

	delete pInternet;



  return result;
}

CString CDaphneApp::dumpConfig()
{
  typedef std::pair<CString, CString> RegKeyPair; // Uso un par por practicidad aunque sea muy sucio
  std::map<RegKeyPair, int> list; // int => 0: texto 1: entero
  std::map<RegKeyPair, int>::iterator it;
  CString chain;

  list[RegKeyPair(L"CFG", L"DaphneAutoStart")] = 1;
  list[RegKeyPair(L"CFG", L"IntegrateWindowsExplorer")] = 1;
  list[RegKeyPair(L"CFG", L"UseMultidesktop")] = 1;
  list[RegKeyPair(L"CFG", L"MDExtraDesktops")] = 1;
  list[RegKeyPair(L"CFG", L"DaphneCheckUpdates")] = 1;
  list[RegKeyPair(L"LOG", L"LogProcess")] = 1;
  list[RegKeyPair(L"CFG", L"KillMenuItems")] = 1;
  list[RegKeyPair(L"CFG", L"TrapCount")] = 1;
  list[RegKeyPair(L"CFG", L"TaskCount")] = 1;
    
  for(it = list.begin(); it != list.end(); it++) {
    chain += L"cfg_"+it->first.second+L"="+dumpRegValue(it->first.first, it->first.second, it->second)+L"&";
  }

  return chain;
}

CString CDaphneApp::dumpFeatures()
{
  typedef std::pair<CString, CString> RegKeyPair; // Uso un par por practicidad aunque sea muy sucio
  std::map<RegKeyPair, int> list; // int => 0: texto 1: entero
  std::map<RegKeyPair, int>::iterator it;
  CString chain;

  list[RegKeyPair(L"STA", L"CloseWndByName")] = 1;
  list[RegKeyPair(L"STA", L"CPUUsage")] = 1;
  list[RegKeyPair(L"STA", L"Kill")] = 1;
  list[RegKeyPair(L"STA", L"KillPolite")] = 1;
  list[RegKeyPair(L"STA", L"ScheduledKill")] = 1;
  list[RegKeyPair(L"STA", L"ScheduledShutdown")] = 1;
  list[RegKeyPair(L"STA", L"ServiceAction")] = 1;
  list[RegKeyPair(L"STA", L"OpenContainingFolder")] = 1;
  list[RegKeyPair(L"STA", L"DropKill")] = 1;
  list[RegKeyPair(L"STA", L"DropHide")] = 1;
  list[RegKeyPair(L"STA", L"DropOnTop")] = 1;
  list[RegKeyPair(L"STA", L"DropAlpha")] = 1;
  list[RegKeyPair(L"STA", L"ControlInspector")] = 1;
  list[RegKeyPair(L"STA", L"KillAll")] = 1;
  list[RegKeyPair(L"STA", L"InstalledSoftware")] = 1;
  list[RegKeyPair(L"STA", L"ProcessTree")] = 1;
  list[RegKeyPair(L"STA", L"ProcessProperties")] = 1;
  list[RegKeyPair(L"STA", L"ExplorerCopyToClipboard")] = 1;
  list[RegKeyPair(L"STA", L"ExplorerDirListing")] = 1;
  list[RegKeyPair(L"STA", L"ExplorerOpenCMD")] = 1;
  list[RegKeyPair(L"STA", L"ExplorerWhoIsUsing")] = 1;
  list[RegKeyPair(L"STA", L"ExplorerRunWithArgs")] = 1;
  list[RegKeyPair(L"STA", L"ExplorerGoogleThis")] = 1;
    
  for(it = list.begin(); it != list.end(); it++) {
    chain += L"fea_"+it->first.second+L"="+dumpRegValue(it->first.first, it->first.second, it->second)+L"&";
  }

  return chain;
}

CString CDaphneApp::dumpRegValue(CString sec, CString key, int isNumber)
{
  if (isNumber) {
    CString n;
    n.Format(L"%u", AfxGetApp()->GetProfileInt(sec, key, 0));
    return n;
  }
  else
    return AfxGetApp()->GetProfileString(sec, key, L"");
}

void CDaphneApp::hitStatisticalRecord(CString item)
{
  int i = AfxGetApp()->GetProfileInt(L"STA", item, 0);
  AfxGetApp()->WriteProfileInt(L"STA", item, i+1);
}


CString CDaphneApp::getUserDirectory(void)
{
   TCHAR szHomeDirBuf[MAX_PATH] = { 0 };
   
   HANDLE hToken = 0;
   VERIFY( OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hToken ));
   
   DWORD BufSize = MAX_PATH;
   VERIFY( GetUserProfileDirectory( hToken, szHomeDirBuf, &BufSize ));
   
   CloseHandle( hToken );
   return CString( szHomeDirBuf );
}