; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPPAdv
LastTemplate=CPropertyPage
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Daphne.h"

ClassCount=25
Class1=CDaphneApp
Class2=CDaphneDlg
Class3=CAboutDlg

ResourceCount=46
Resource1=IDD_BACKDOOR
Resource2=IDR_MAINFRAME
Class4=CProcList
Resource3=IDD_KMI_EDIT (English (U.S.))
Class5=CPropertiesDlg
Resource4=IDD_DAPHNE_DIALOG
Class6=CSendMsgDlg
Resource5=IDD_PROPERTIES
Class7=CPPStartup
Resource6=IDD_GETVALUE
Class8=CKillAllDlg
Resource7=IDD_DAPHNE_DIALOG (English (U.S.))
Class9=CKMIEditDlg
Resource8=IDD_SENDMSG
Class10=CW32Values
Resource9=IDD_KMI_CPLIST
Class11=CInspector
Resource10=IDD_PP_ADVANCED
Class12=CKMIEditPListDlg
Resource11=IDD_AUTHUSER
Class13=CPPLook
Class14=CPPKill
Resource12=IDD_PP_TRAPS
Resource13=IDD_KILLALL
Class15=CBackDoorDlg
Resource14=IDD_PP_STARTUP
Class16=CCrashDlg
Resource15=IDD_KMI_CPLIST (English (U.S.))
Class17=CRunProcDlg
Resource16=IDD_ABOUTBOX
Class18=CAuthUserDlg
Resource17=IDD_KMI_EDIT
Resource18=IDD_PROPERTIES (English (U.S.))
Resource19=IDD_ABOUTBOX (English (U.S.))
Resource20=IDD_KILLALL (English (U.S.))
Resource21=IDD_SENDMSG (English (U.S.))
Resource22=IDD_WNDLIST (English (U.S.))
Resource23=IDD_PP_TRAPS (English (U.S.))
Resource24=IDD_INSPECTOR (English (U.S.))
Resource25=IDD_MB_WEBAGREE (English (U.S.))
Resource26=IDD_GETVALUE (English (U.S.))
Resource27=IDD_RUNP (English (U.S.))
Resource28=IDD_AUTHUSER (English (U.S.))
Resource29=IDD_WNDLIST
Resource30=IDD_PP_ADVANCED (English (U.S.))
Resource31=IDD_RUNP
Resource32=IDD_CRASH (English (U.S.))
Class19=CHiddenOwner
Resource33=IDD_MB_WEBAGREE
Class20=CWndsPopUp
Resource34=IDD_NEW_TRAP
Class21=CPPAdv
Resource35=IDD_CRASH
Class22=CMBWebAgreeDlg
Resource36=IDD_PP_LOOK (English (U.S.))
Resource37=IDD_PP_KILL (English (U.S.))
Resource38=IDD_PP_STARTUP (English (U.S.))
Resource39=IDD_PROPPAGE_LARGE (English (U.S.))
Class23=CNewTrapDlg
Resource40=IDD_PP_LOOK
Class24=CPPTraps
Resource41=IDD_PP_KILL
Resource42=IDD_NEW_TRAP (English (U.S.))
Resource43=IDD_INSPECTOR
Class25=CPPHighlight
Resource44=IDD_PP_HIGHLIGHTS
Resource45=IDD_BACKDOOR (English (U.S.))
Resource46=IDD_PP_HIGHLIGHTS (English (U.S.))

[CLS:CDaphneApp]
Type=0
HeaderFile=Daphne.h
ImplementationFile=Daphne.cpp
Filter=W
LastObject=CDaphneApp
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CDaphneDlg]
Type=0
HeaderFile=DaphneDlg.h
ImplementationFile=DaphneDlg.cpp
Filter=W
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CDaphneDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=DaphneDlg.h
ImplementationFile=DaphneDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=13
Control1=IDCANCEL,button,1342275585
Control2=IDC_WEB,button,1342275584
Control3=IDC_LICENSE,button,1342275584
Control4=IDC_STATIC,static,1342181390
Control5=IDC_AB_TITLE,static,1342308737
Control6=IDC_STATIC,static,1342308353
Control7=IDC_STATIC,static,1342308353
Control8=IDC_STG_CONTRIB,button,1342177287
Control9=IDC_STATIC,static,1342308353
Control10=IDC_STT_THANKS,static,1342308353
Control11=IDC_STATIC,static,1342308353
Control12=IDC_STT_BUGS,static,1342308609
Control13=IDC_STT_TRANS_IT,static,1342308353

[DLG:IDD_DAPHNE_DIALOG]
Type=1
Class=CDaphneDlg
ControlCount=7
Control1=IDC_LIST,SysListView32,1350631437
Control2=IDC_FULLPATHEX,button,1342275587
Control3=IDC_AONTOP,button,1342275587
Control4=IDC_TACTION,combobox,1344339971
Control5=IDC_MENUB,button,1342275585
Control6=IDC_SETTINGS,button,1342275584
Control7=IDC_C_KILL,static,1342177539

[CLS:CProcList]
Type=0
HeaderFile=ProcList.h
ImplementationFile=ProcList.cpp
BaseClass=CListCtrl
Filter=W
VirtualFilter=FWC

[DLG:IDD_PROPERTIES]
Type=1
Class=CPropertiesDlg
ControlCount=24
Control1=IDC_P_NAME,edit,1342244992
Control2=IDC_GOOGLE,button,1342275584
Control3=IDC_P_PATH,edit,1342244992
Control4=IDC_T_WNDS,SysTreeView32,1350631479
Control5=IDC_P_THREADS,edit,1084299396
Control6=IDC_P_INFOT,button,1342214153
Control7=IDC_P_INFOM,button,1342214153
Control8=IDC_P_INFOE,button,1342214153
Control9=IDC_P_PRIORITY,edit,1342244992
Control10=IDC_P_CLASS,edit,1342244992
Control11=IDC_P_CREATED,edit,1342244992
Control12=IDC_P_KERNELT,edit,1342244992
Control13=IDC_P_USERT,edit,1342244992
Control14=IDC_STT_1,static,1342308352
Control15=IDC_STT_2,static,1342308352
Control16=IDC_STT_3,static,1342308352
Control17=IDC_STT_4,static,1342308352
Control18=IDC_STT_5,static,1342308352
Control19=IDC_STT_6,static,1342308352
Control20=IDC_STT_7,static,1342308352
Control21=IDC_STG_MI,button,1342177287
Control22=IDC_STG_WNDS,button,1342177287
Control23=IDCANCEL,button,1342275584
Control24=IDC_P_THREADLIST,SysListView32,1350631437

[CLS:CPropertiesDlg]
Type=0
HeaderFile=PropertiesDlg.h
ImplementationFile=PropertiesDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_P_THREADLIST
VirtualFilter=dWC

[DLG:IDD_SENDMSG]
Type=1
Class=CSendMsgDlg
ControlCount=10
Control1=IDC_M_MSG,combobox,1344340227
Control2=IDC_M_LPARAM,edit,1350631552
Control3=IDC_GETVAL1,button,1342242816
Control4=IDC_M_WPARAM,edit,1350631552
Control5=IDC_GETVAL2,button,1342242816
Control6=IDOK,button,1342275585
Control7=IDCANCEL,button,1342275584
Control8=IDC_STT_MSG,static,1342308354
Control9=IDC_STT_LP,static,1342308354
Control10=IDC_STT_WP,static,1342308354

[CLS:CSendMsgDlg]
Type=0
HeaderFile=SendMsgDlg.h
ImplementationFile=SendMsgDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CSendMsgDlg
VirtualFilter=dWC

[DLG:IDD_KILLALL]
Type=1
Class=CKillAllDlg
ControlCount=5
Control1=IDC_KA_NAME,edit,1350631552
Control2=IDOK,button,1342275585
Control3=IDCANCEL,button,1342275584
Control4=IDC_STT_PROC,static,1342308354
Control5=IDC_KA_CASE,button,1342275587

[CLS:CKillAllDlg]
Type=0
HeaderFile=KillAllDlg.h
ImplementationFile=KillAllDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CKillAllDlg
VirtualFilter=dWC

[DLG:IDD_KMI_EDIT]
Type=1
Class=CKMIEditDlg
ControlCount=7
Control1=IDC_KMI_NAME,edit,1350631552
Control2=IDC_KMI_PLIST,edit,1352732804
Control3=IDOK,button,1342275585
Control4=IDCANCEL,button,1342275584
Control5=IDC_STT_MENU,static,1342308354
Control6=IDC_STG_PROCL,button,1342177287
Control7=IDC_KMI_PROCS,button,1342242816

[CLS:CKMIEditDlg]
Type=0
HeaderFile=KMIEditDlg.h
ImplementationFile=KMIEditDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[DLG:IDD_GETVALUE]
Type=1
Class=CW32Values
ControlCount=5
Control1=IDC_V_FILTER,edit,1350631552
Control2=IDC_V_LIST,listbox,1352728835
Control3=IDOK,button,1342275585
Control4=IDCANCEL,button,1342275584
Control5=IDC_STT_FILTER,static,1342308354

[CLS:CW32Values]
Type=0
HeaderFile=W32Values.h
ImplementationFile=W32Values.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_V_LIST
VirtualFilter=dWC

[DLG:IDD_INSPECTOR]
Type=1
Class=CInspector
ControlCount=12
Control1=IDC_LUPACTRL,static,1342177539
Control2=IDC_STT_TITLE,static,1342308354
Control3=IDC_I_TITLE,edit,1342244992
Control4=IDC_STT_CLASS,static,1342308354
Control5=IDC_I_CLASS,edit,1342244992
Control6=IDC_STT_TEXT,static,1342308354
Control7=IDC_I_TEXT,edit,1342244992
Control8=IDC_STT_DRAG,static,1342308352
Control9=IDC_STT_SZ,static,1342308354
Control10=IDC_STT_LOC,static,1342308354
Control11=IDC_I_LOCATION,edit,1342244992
Control12=IDC_I_SIZE,edit,1342244992

[CLS:CInspector]
Type=0
HeaderFile=Inspector.h
ImplementationFile=Inspector.cpp
BaseClass=CDialog
Filter=D
LastObject=CInspector
VirtualFilter=dWC

[DLG:IDD_KMI_CPLIST]
Type=1
Class=CKMIEditPListDlg
ControlCount=3
Control1=IDOK,button,1342275585
Control2=IDCANCEL,button,1342275584
Control3=IDC_KMI_PLIST,listbox,1352730883

[CLS:CKMIEditPListDlg]
Type=0
HeaderFile=KMIEditPListDlg.h
ImplementationFile=KMIEditPListDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_KMI_PLIST
VirtualFilter=dWC

[DLG:IDD_PP_LOOK]
Type=1
Class=CPPLook
ControlCount=9
Control1=IDC_WNDALPHA,msctls_trackbar32,1342242840
Control2=IDC_STT_MAINW,button,1342177287
Control3=IDC_PS_100,static,1342308352
Control4=IDC_PS_25,static,1342308352
Control5=IDC_STG_PROCLC,button,1342177287
Control6=IDC_S_COLUMNS,SysListView32,1350647821
Control7=IDC_S_UP2,button,1342275584
Control8=IDC_S_DOWN2,button,1342275584
Control9=IDC_S_TRANSP,button,1342275587

[CLS:CPPLook]
Type=0
HeaderFile=PPLook.h
ImplementationFile=PPLook.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=CPPLook
VirtualFilter=idWC

[DLG:IDD_PP_KILL]
Type=1
Class=CPPKill
ControlCount=7
Control1=IDC_KILLERS,SysListView32,1350647821
Control2=IDC_S_ADD,button,1342275584
Control3=IDC_S_EDIT,button,1342275584
Control4=IDC_S_REMOVE,button,1342275584
Control5=IDC_S_UP,button,1342275584
Control6=IDC_S_DOWN,button,1342275584
Control7=IDC_STG_DMENU,button,1342177287

[CLS:CPPKill]
Type=0
HeaderFile=PPKill.h
ImplementationFile=PPKill.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=IDC_KILLERS
VirtualFilter=idWC

[DLG:IDD_PP_STARTUP]
Type=1
Class=CPPStartup
ControlCount=12
Control1=IDC_STG_STARTU,button,1342177287
Control2=IDC_S_START_WIN,button,1342275587
Control3=IDC_S_START_HIDE,button,1342275587
Control4=IDC_STG_BEHAVE,button,1342177287
Control5=IDC_S_SELECTD,button,1342275587
Control6=IDC_S_HIGHL_INSP,button,1342275587
Control7=IDC_S_CHECKUP,button,1342275587
Control8=IDC_S_PROXY,button,1342275587
Control9=IDC_S_PADDR,edit,1350631552
Control10=IDC_S_PPORT,edit,1350631552
Control11=IDC_STS_PROXY,static,1342308352
Control12=IDC_S_FASTR,button,1342275587

[CLS:CPPStartup]
Type=0
HeaderFile=PPStartup.h
ImplementationFile=PPStartup.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=CPPStartup
VirtualFilter=idWC

[DLG:IDD_BACKDOOR]
Type=1
Class=CBackDoorDlg
ControlCount=7
Control1=IDC_BD_CRASH,button,1342275584
Control2=IDC_BD_SUSPEND,button,1342275584
Control3=IDC_BD_RESUME,button,1342275584
Control4=IDC_BD_SETUNSET,button,1342275587
Control5=IDCANCEL,button,1342275585
Control6=IDC_STATIC,static,1342308352
Control7=IDC_BD_CREATEZOMBIE,button,1342275584

[CLS:CBackDoorDlg]
Type=0
HeaderFile=BackDoorDlg.h
ImplementationFile=BackDoorDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CBackDoorDlg
VirtualFilter=dWC

[DLG:IDD_CRASH]
Type=1
Class=CCrashDlg
ControlCount=6
Control1=IDOK,button,1342275585
Control2=IDCANCEL,button,1342275584
Control3=IDC_STT_EXCEP,static,1342308352
Control4=IDC_STG_ERRINFO,button,1342177287
Control5=IDC_CR_ERROR,edit,1353783300
Control6=IDC_STT_STAR,static,1342308352

[CLS:CCrashDlg]
Type=0
HeaderFile=CrashDlg.h
ImplementationFile=CrashDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_CR_ERROR
VirtualFilter=dWC

[DLG:IDD_RUNP]
Type=1
Class=CRunProcDlg
ControlCount=11
Control1=IDC_R_PRG,edit,1350631552
Control2=IDC_EXPLORE,button,1342242816
Control3=IDC_R_ARG,edit,1350631552
Control4=IDC_R_DIR,edit,1350631552
Control5=IDC_EXPLORE2,button,1342242816
Control6=IDC_RP_SUSP,button,1342275587
Control7=IDOK,button,1342275585
Control8=IDCANCEL,button,1342275584
Control9=IDC_STT_PRG,static,1342308354
Control10=IDC_STT_ARG,static,1342308354
Control11=IDC_STT_DIR,static,1342308354

[CLS:CRunProcDlg]
Type=0
HeaderFile=RunProcDlg.h
ImplementationFile=RunProcDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CRunProcDlg
VirtualFilter=dWC

[DLG:IDD_AUTHUSER]
Type=1
Class=CAuthUserDlg
ControlCount=6
Control1=IDC_A_USER,edit,1350631552
Control2=IDC_A_PASSWD,edit,1350631584
Control3=IDOK,button,1342275585
Control4=IDCANCEL,button,1342275584
Control5=IDC_STT_USERN,static,1342308354
Control6=IDC_STT_PASSW,static,1342308354

[CLS:CAuthUserDlg]
Type=0
HeaderFile=AuthUserDlg.h
ImplementationFile=AuthUserDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CAuthUserDlg
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=14
Control1=IDCANCEL,button,1342275585
Control2=IDC_WEB,button,1342275584
Control3=IDC_LICENSE,button,1342275584
Control4=IDC_STATIC,static,1342181390
Control5=IDC_AB_TITLE,static,1342308737
Control6=IDC_STATIC,static,1342308353
Control7=IDC_STATIC,static,1342308353
Control8=IDC_STG_CONTRIB,button,1342177287
Control9=IDC_STATIC,static,1342308353
Control10=IDC_STT_THANKS,static,1342308353
Control11=IDC_STATIC,static,1342308353
Control12=IDC_STT_BUGS,static,1342308609
Control13=IDC_STT_TRANS_IT,static,1342308353
Control14=IDC_STT_TRANS_DE,static,1342308353

[DLG:IDD_DAPHNE_DIALOG (English (U.S.))]
Type=1
Class=CDaphneDlg
ControlCount=7
Control1=IDC_LIST,SysListView32,1350631437
Control2=IDC_FULLPATHEX,button,1342275587
Control3=IDC_AONTOP,button,1342275587
Control4=IDC_TACTION,combobox,1344339971
Control5=IDC_MENUB,button,1342275585
Control6=IDC_SETTINGS,button,1342275584
Control7=IDC_C_KILL,static,1342177539

[DLG:IDD_PROPERTIES (English (U.S.))]
Type=1
Class=CPropertiesDlg
ControlCount=24
Control1=IDC_P_NAME,edit,1342244992
Control2=IDC_GOOGLE,button,1342275584
Control3=IDC_P_PATH,edit,1342244992
Control4=IDC_T_WNDS,SysTreeView32,1350631479
Control5=IDC_P_THREADS,edit,1084299396
Control6=IDC_P_INFOT,button,1342214153
Control7=IDC_P_INFOM,button,1342214153
Control8=IDC_P_INFOE,button,1342214153
Control9=IDC_P_PRIORITY,edit,1342244992
Control10=IDC_P_CLASS,edit,1342244992
Control11=IDC_P_CREATED,edit,1342244992
Control12=IDC_P_KERNELT,edit,1342244992
Control13=IDC_P_USERT,edit,1342244992
Control14=IDC_STT_1,static,1342308352
Control15=IDC_STT_2,static,1342308352
Control16=IDC_STT_3,static,1342308352
Control17=IDC_STT_4,static,1342308352
Control18=IDC_STT_5,static,1342308352
Control19=IDC_STT_6,static,1342308352
Control20=IDC_STT_7,static,1342308352
Control21=IDC_STG_MI,button,1342177287
Control22=IDC_STG_WNDS,button,1342177287
Control23=IDCANCEL,button,1342275584
Control24=IDC_P_THREADLIST,SysListView32,1350631437

[DLG:IDD_SENDMSG (English (U.S.))]
Type=1
Class=CSendMsgDlg
ControlCount=10
Control1=IDC_M_MSG,combobox,1344340227
Control2=IDC_M_LPARAM,edit,1350631552
Control3=IDC_GETVAL1,button,1342242816
Control4=IDC_M_WPARAM,edit,1350631552
Control5=IDC_GETVAL2,button,1342242816
Control6=IDOK,button,1342275585
Control7=IDCANCEL,button,1342275584
Control8=IDC_STT_MSG,static,1342308354
Control9=IDC_STT_LP,static,1342308354
Control10=IDC_STT_WP,static,1342308354

[DLG:IDD_KILLALL (English (U.S.))]
Type=1
Class=CKillAllDlg
ControlCount=5
Control1=IDC_KA_NAME,edit,1350631552
Control2=IDOK,button,1342275585
Control3=IDCANCEL,button,1342275584
Control4=IDC_STT_PROC,static,1342308354
Control5=IDC_KA_CASE,button,1342275587

[DLG:IDD_KMI_EDIT (English (U.S.))]
Type=1
Class=CKMIEditDlg
ControlCount=7
Control1=IDC_KMI_NAME,edit,1350631552
Control2=IDC_KMI_PLIST,edit,1352732804
Control3=IDOK,button,1342275585
Control4=IDCANCEL,button,1342275584
Control5=IDC_STT_MENU,static,1342308354
Control6=IDC_STG_PROCL,button,1342177287
Control7=IDC_KMI_PROCS,button,1342242816

[DLG:IDD_GETVALUE (English (U.S.))]
Type=1
Class=CW32Values
ControlCount=5
Control1=IDC_V_FILTER,edit,1350631552
Control2=IDC_V_LIST,listbox,1352728835
Control3=IDOK,button,1342275585
Control4=IDCANCEL,button,1342275584
Control5=IDC_STT_FILTER,static,1342308354

[DLG:IDD_INSPECTOR (English (U.S.))]
Type=1
Class=CInspector
ControlCount=12
Control1=IDC_LUPACTRL,static,1342177539
Control2=IDC_STT_TITLE,static,1342308354
Control3=IDC_I_TITLE,edit,1342244992
Control4=IDC_STT_CLASS,static,1342308354
Control5=IDC_I_CLASS,edit,1342244992
Control6=IDC_STT_TEXT,static,1342308354
Control7=IDC_I_TEXT,edit,1342244992
Control8=IDC_STT_DRAG,static,1342308352
Control9=IDC_STT_SZ,static,1342308354
Control10=IDC_STT_LOC,static,1342308354
Control11=IDC_I_LOCATION,edit,1342244992
Control12=IDC_I_SIZE,edit,1342244992

[DLG:IDD_KMI_CPLIST (English (U.S.))]
Type=1
Class=CKMIEditPListDlg
ControlCount=3
Control1=IDOK,button,1342275585
Control2=IDCANCEL,button,1342275584
Control3=IDC_KMI_PLIST,listbox,1352730883

[DLG:IDD_PP_LOOK (English (U.S.))]
Type=1
Class=CPPLook
ControlCount=9
Control1=IDC_WNDALPHA,msctls_trackbar32,1342242840
Control2=IDC_STT_MAINW,button,1342177287
Control3=IDC_PS_100,static,1342308352
Control4=IDC_PS_25,static,1342308352
Control5=IDC_STG_PROCLC,button,1342177287
Control6=IDC_S_COLUMNS,SysListView32,1350647821
Control7=IDC_S_UP2,button,1342275584
Control8=IDC_S_DOWN2,button,1342275584
Control9=IDC_S_TRANSP,button,1342275587

[DLG:IDD_PP_KILL (English (U.S.))]
Type=1
Class=CPPKill
ControlCount=7
Control1=IDC_KILLERS,SysListView32,1350647821
Control2=IDC_S_ADD,button,1342275584
Control3=IDC_S_EDIT,button,1342275584
Control4=IDC_S_REMOVE,button,1342275584
Control5=IDC_S_UP,button,1342275584
Control6=IDC_S_DOWN,button,1342275584
Control7=IDC_STG_DMENU,button,1342177287

[DLG:IDD_PP_STARTUP (English (U.S.))]
Type=1
Class=CPPStartup
ControlCount=12
Control1=IDC_STG_STARTU,button,1342177287
Control2=IDC_S_START_WIN,button,1342275587
Control3=IDC_S_START_HIDE,button,1342275587
Control4=IDC_STG_BEHAVE,button,1342177287
Control5=IDC_S_SELECTD,button,1342275587
Control6=IDC_S_HIGHL_INSP,button,1342275587
Control7=IDC_S_CHECKUP,button,1342275587
Control8=IDC_S_PROXY,button,1342275587
Control9=IDC_S_PADDR,edit,1350631552
Control10=IDC_S_PPORT,edit,1350631552
Control11=IDC_STS_PROXY,static,1342308352
Control12=IDC_S_FASTR,button,1342275587

[DLG:IDD_BACKDOOR (English (U.S.))]
Type=1
Class=CBackDoorDlg
ControlCount=7
Control1=IDC_BD_CRASH,button,1342275584
Control2=IDC_BD_SUSPEND,button,1342275584
Control3=IDC_BD_RESUME,button,1342275584
Control4=IDC_BD_SETUNSET,button,1342275587
Control5=IDCANCEL,button,1342275585
Control6=IDC_STATIC,static,1342308352
Control7=IDC_BD_CREATEZOMBIE,button,1342275584

[DLG:IDD_CRASH (English (U.S.))]
Type=1
Class=CCrashDlg
ControlCount=6
Control1=IDOK,button,1342275585
Control2=IDCANCEL,button,1342275584
Control3=IDC_STT_EXCEP,static,1342308352
Control4=IDC_STG_ERRINFO,button,1342177287
Control5=IDC_CR_ERROR,edit,1353783300
Control6=IDC_STT_STAR,static,1342308352

[DLG:IDD_RUNP (English (U.S.))]
Type=1
Class=CRunProcDlg
ControlCount=11
Control1=IDC_R_PRG,edit,1350631552
Control2=IDC_EXPLORE,button,1342242816
Control3=IDC_R_ARG,edit,1350631552
Control4=IDC_R_DIR,edit,1350631552
Control5=IDC_EXPLORE2,button,1342242816
Control6=IDC_RP_SUSP,button,1342275587
Control7=IDOK,button,1342275585
Control8=IDCANCEL,button,1342275584
Control9=IDC_STT_PRG,static,1342308354
Control10=IDC_STT_ARG,static,1342308354
Control11=IDC_STT_DIR,static,1342308354

[DLG:IDD_AUTHUSER (English (U.S.))]
Type=1
Class=CAuthUserDlg
ControlCount=6
Control1=IDC_A_USER,edit,1350631552
Control2=IDC_A_PASSWD,edit,1350631584
Control3=IDOK,button,1342275585
Control4=IDCANCEL,button,1342275584
Control5=IDC_STT_USERN,static,1342308354
Control6=IDC_STT_PASSW,static,1342308354

[CLS:CHiddenOwner]
Type=0
HeaderFile=HiddenOwner.h
ImplementationFile=HiddenOwner.cpp
BaseClass=CDialog
Filter=D
LastObject=CHiddenOwner

[DLG:IDD_WNDLIST]
Type=1
Class=CWndsPopUp
ControlCount=1
Control1=IDC_WNDS,SysListView32,1350680589

[CLS:CWndsPopUp]
Type=0
HeaderFile=WndsPopUp.h
ImplementationFile=WndsPopUp.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_WNDS
VirtualFilter=dWC

[DLG:IDD_PP_ADVANCED]
Type=1
Class=CPPAdv
ControlCount=8
Control1=IDC_STG_SC,button,1342177287
Control2=IDC_SC_SHIFT,button,1342275587
Control3=IDC_SC_ALT,button,1342275587
Control4=IDC_SC_VKEY,combobox,1344339971
Control5=IDC_SC_ON,button,1342275587
Control6=IDC_ADV_UNHIDE,button,1342275587
Control7=IDC_SC_CTRL,button,1342275587
Control8=IDC_ADV_JAVAHIDE,button,1342275587

[CLS:CPPAdv]
Type=0
HeaderFile=PPAdv.h
ImplementationFile=PPAdv.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=IDC_CHECKDEBUG
VirtualFilter=idWC

[DLG:IDD_MB_WEBAGREE]
Type=1
Class=CMBWebAgreeDlg
ControlCount=4
Control1=IDOK,button,1342275585
Control2=IDCANCEL,button,1342275584
Control3=IDC_STT_NOTE,static,1342308352
Control4=IDC_STT_Q,static,1342308352

[CLS:CMBWebAgreeDlg]
Type=0
HeaderFile=MBWebAgreeDlg.h
ImplementationFile=MBWebAgreeDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CMBWebAgreeDlg
VirtualFilter=dWC

[DLG:IDD_WNDLIST (English (U.S.))]
Type=1
Class=CWndsPopUp
ControlCount=1
Control1=IDC_WNDS,SysListView32,1350680589

[DLG:IDD_PP_ADVANCED (English (U.S.))]
Type=1
Class=CPPAdv
ControlCount=9
Control1=IDC_STG_SC,button,1342177287
Control2=IDC_SC_SHIFT,button,1342275587
Control3=IDC_SC_ALT,button,1342275587
Control4=IDC_SC_VKEY,combobox,1344339971
Control5=IDC_SC_ON,button,1342275587
Control6=IDC_ADV_UNHIDE,button,1342275587
Control7=IDC_SC_CTRL,button,1342275587
Control8=IDC_ADV_JAVAHIDE,button,1342275587
Control9=IDC_CHECKDEBUG,button,1342275587

[DLG:IDD_MB_WEBAGREE (English (U.S.))]
Type=1
Class=CMBWebAgreeDlg
ControlCount=4
Control1=IDOK,button,1342275585
Control2=IDCANCEL,button,1342275584
Control3=IDC_STT_NOTE,static,1342308352
Control4=IDC_STT_Q,static,1342308352

[DLG:IDD_NEW_TRAP]
Type=1
Class=CNewTrapDlg
ControlCount=8
Control1=IDC_NAME,edit,1350631552
Control2=IDC_TRAP,combobox,1344339971
Control3=IDOK,button,1342275585
Control4=IDCANCEL,button,1342275584
Control5=IDC_STT_PNAME,static,1342308352
Control6=IDC_STT_TYPE,static,1342308352
Control7=IDC_STT_WCONT,static,1342308352
Control8=IDC_TR_WCONTAINS,edit,1350631552

[CLS:CNewTrapDlg]
Type=0
HeaderFile=NewTrapDlg.h
ImplementationFile=NewTrapDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CNewTrapDlg
VirtualFilter=dWC

[CLS:CPPTraps]
Type=0
HeaderFile=PPTraps.h
ImplementationFile=PPTraps.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=IDC_TR_LIST
VirtualFilter=idWC

[DLG:IDD_PROPPAGE_LARGE (English (U.S.))]
Type=1
Class=?
ControlCount=5
Control1=IDC_STG_TRAPS,button,1342177287
Control2=IDC_TR_LIST,SysListView32,1350664213
Control3=IDC_TR_ADD,button,1342275584
Control4=IDC_TR_EDIT,button,1342275584
Control5=IDC_TR_REMOVE,button,1342275584

[DLG:IDD_NEW_TRAP (English (U.S.))]
Type=1
Class=CNewTrapDlg
ControlCount=8
Control1=IDC_NAME,edit,1350631552
Control2=IDC_TRAP,combobox,1344339971
Control3=IDOK,button,1342275585
Control4=IDCANCEL,button,1342275584
Control5=IDC_STT_PNAME,static,1342308352
Control6=IDC_STT_TYPE,static,1342308352
Control7=IDC_STT_WCONT,static,1342308352
Control8=IDC_TR_WCONTAINS,edit,1350631552

[DLG:IDD_PP_TRAPS]
Type=1
Class=CPPTraps
ControlCount=5
Control1=IDC_STG_TRAPS,button,1342177287
Control2=IDC_TR_LIST,SysListView32,1350664213
Control3=IDC_TR_ADD,button,1342275584
Control4=IDC_TR_EDIT,button,1342275584
Control5=IDC_TR_REMOVE,button,1342275584

[DLG:IDD_PP_HIGHLIGHTS]
Type=1
Class=CPPHighlight
ControlCount=5
Control1=IDC_HL_PROCS,button,1342242816
Control2=IDC_HL_LIST,edit,1352728708
Control3=IDC_STT_HLLIST,static,1342308352
Control4=IDC_HL_HLSYSTEM,button,1342242819
Control5=IDC_HL_HLTOP,button,1342242819

[CLS:CPPHighlight]
Type=0
HeaderFile=PPHighlight.h
ImplementationFile=PPHighlight.cpp
BaseClass=CPropertyPage
Filter=W
LastObject=CPPHighlight
VirtualFilter=idWC

[DLG:IDD_PP_TRAPS (English (U.S.))]
Type=1
Class=?
ControlCount=5
Control1=IDC_STG_TRAPS,button,1342177287
Control2=IDC_TR_LIST,SysListView32,1350664213
Control3=IDC_TR_ADD,button,1342275584
Control4=IDC_TR_EDIT,button,1342275584
Control5=IDC_TR_REMOVE,button,1342275584

[DLG:IDD_PP_HIGHLIGHTS (English (U.S.))]
Type=1
Class=?
ControlCount=5
Control1=IDC_HL_PROCS,button,1342242816
Control2=IDC_HL_LIST,edit,1352728708
Control3=IDC_STT_HLLIST,static,1342308352
Control4=IDC_HL_HLSYSTEM,button,1342242819
Control5=IDC_HL_HLTOP,button,1342242819

