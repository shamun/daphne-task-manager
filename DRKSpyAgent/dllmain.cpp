// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "drkspyagent.h"

using namespace DRKSpy;

#pragma data_seg (".shared_drkspyagent")
HWND	DRKSpy::g_hWnd = 0;
HWND	DRKSpy::g_hWndCtrl = 0;
WCHAR	DRKSpy::g_strMsg[LHF_BUFF_SIZE] = L"";
int		DRKSpy::g_iAction = -1;
bool	DRKSpy::g_bWorked = 0;
#pragma data_seg ()

#pragma comment(linker,"/SECTION:.shared_drkspyagent,RWS") 

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		switch(g_iAction) {
		case 0:
			if ((g_hWnd) && (g_hWndCtrl)) {

				::SendMessage(g_hWndCtrl, WM_GETTEXT, LHF_BUFF_SIZE, (LPARAM)g_strMsg);
			}
      g_bWorked = true;
			break;
		case 1:
			if (g_hWnd) {
				WCHAR * env = (WCHAR*)GetEnvironmentStrings();
				if (env != NULL) {
          size_t len = 0;
          WCHAR * p = env;
          while((p[0] || p[1]) && len < LHF_BUFF_SIZE) {
            ++len;
            ++p;
          }
          ++len;
					memcpy_s(g_strMsg, sizeof(g_strMsg), env, len*2); // WCHAR
          for(size_t i=0; i<len; ++i) {
            if (g_strMsg[i] == 0)
              g_strMsg[i] = L'\n';
          }
          g_strMsg[len+1]=0;
					FreeEnvironmentStrings(env);
				}
				else
					memset(g_strMsg, 0, sizeof(g_strMsg));
			}
      g_bWorked = true;
			break;
		case 2:
			::MessageBox(NULL, L"Working!", L"DRKSpyAgent.DLL", MB_OK);
      g_bWorked = true;
			break;
		}
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

