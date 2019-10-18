

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Sep 26 11:04:00 2014
 */
/* Compiler settings for DRKShell.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IComponentRegistrar,0xa817e7a2,0x43fa,0x11d0,0x9e,0x44,0x00,0xaa,0x00,0xb6,0x77,0x0a);


MIDL_DEFINE_GUID(IID, IID_IDRKShellMenu,0x8DEF031A,0x4091,0x40D7,0x9E,0x09,0xC3,0x4F,0xEB,0x0F,0xB8,0x7C);


MIDL_DEFINE_GUID(IID, LIBID_DRKShellLib,0xAE58AD11,0xDB9B,0x47C5,0xAA,0x49,0x92,0xFC,0xC9,0xCD,0xE3,0xC0);


MIDL_DEFINE_GUID(CLSID, CLSID_CompReg,0x8F564BAB,0x5D2A,0x45B8,0x8F,0x45,0x4C,0xC3,0x37,0xA9,0x9B,0xBE);


MIDL_DEFINE_GUID(CLSID, CLSID_DRKShellMenu,0xE18C8C1B,0xFD35,0x455E,0xBB,0x21,0x4B,0xA2,0x6B,0x1E,0x75,0x1D);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



