// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__5CDC6A50_09BF_42A8_99A2_03AC5CB5F689__INCLUDED_)
#define AFX_STDAFX_H__5CDC6A50_09BF_42A8_99A2_03AC5CB5F689__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <WinSDKVer.h>
#ifndef _WIN32_WINNT
#define _WIN32_WINNT      _WIN32_WINNT_WS03
#endif
#include <SDKDDKVer.h>

#include <windows.h>

// TODO: reference additional headers your program requires here
#include <stdio.h>


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__5CDC6A50_09BF_42A8_99A2_03AC5CB5F689__INCLUDED_)
