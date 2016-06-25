

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Sat Jun 25 09:30:12 2016
 */
/* Compiler settings for Thunder.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __Thunder_h_h__
#define __Thunder_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IThunderProcess_FWD_DEFINED__
#define __IThunderProcess_FWD_DEFINED__
typedef interface IThunderProcess IThunderProcess;

#endif 	/* __IThunderProcess_FWD_DEFINED__ */


#ifndef __ThunderCore_FWD_DEFINED__
#define __ThunderCore_FWD_DEFINED__

#ifdef __cplusplus
typedef class ThunderCore ThunderCore;
#else
typedef struct ThunderCore ThunderCore;
#endif /* __cplusplus */

#endif 	/* __ThunderCore_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IThunderProcess_INTERFACE_DEFINED__
#define __IThunderProcess_INTERFACE_DEFINED__

/* interface IThunderProcess */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IThunderProcess;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("540F822B-E4EF-4A18-9FA4-344460190D71")
    IThunderProcess : public IUnknown
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AttachById( 
            /* [in] */ DWORD processId) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AttachByName( 
            /* [in] */ BSTR processName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateAndAttach( 
            /* [in] */ BSTR exePath,
            /* [defaultvalue][in] */ BSTR commandLineArguments = L"",
            /* [defaultvalue][in] */ BOOL createSuspended = FALSE) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Resume( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Inject( 
            /* [in] */ BSTR dllPath,
            /* [defaultvalue][in] */ BSTR functionName = L"",
            /* [defaultvalue][in] */ BSTR functionArgument = L"") = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ManuallyMap( 
            /* [in] */ BSTR dllPath,
            /* [defaultvalue][in] */ BSTR functionName = L"",
            /* [defaultvalue][in] */ BSTR functionArgument = L"") = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Eject( 
            /* [in] */ BSTR moduleName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Call( 
            /* [in] */ BSTR moduleName,
            /* [in] */ BSTR functionName,
            /* [defaultvalue][in] */ BOOL usesArgument = FALSE,
            /* [defaultvalue][in] */ BSTR functionArgument = L"") = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE InitializeCLR( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DestroyCLR( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ExecuteAssembly( 
            /* [in] */ BSTR assemblyPath,
            /* [in] */ BSTR className,
            /* [in] */ BSTR methodName,
            /* [defaultvalue][in] */ BSTR methodArgument = L"",
            /* [defaultvalue][out] */ DWORD *returnValue = 0) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IThunderProcessVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IThunderProcess * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IThunderProcess * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IThunderProcess * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AttachById )( 
            IThunderProcess * This,
            /* [in] */ DWORD processId);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AttachByName )( 
            IThunderProcess * This,
            /* [in] */ BSTR processName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateAndAttach )( 
            IThunderProcess * This,
            /* [in] */ BSTR exePath,
            /* [defaultvalue][in] */ BSTR commandLineArguments,
            /* [defaultvalue][in] */ BOOL createSuspended);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Resume )( 
            IThunderProcess * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Inject )( 
            IThunderProcess * This,
            /* [in] */ BSTR dllPath,
            /* [defaultvalue][in] */ BSTR functionName,
            /* [defaultvalue][in] */ BSTR functionArgument);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ManuallyMap )( 
            IThunderProcess * This,
            /* [in] */ BSTR dllPath,
            /* [defaultvalue][in] */ BSTR functionName,
            /* [defaultvalue][in] */ BSTR functionArgument);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Eject )( 
            IThunderProcess * This,
            /* [in] */ BSTR moduleName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Call )( 
            IThunderProcess * This,
            /* [in] */ BSTR moduleName,
            /* [in] */ BSTR functionName,
            /* [defaultvalue][in] */ BOOL usesArgument,
            /* [defaultvalue][in] */ BSTR functionArgument);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *InitializeCLR )( 
            IThunderProcess * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DestroyCLR )( 
            IThunderProcess * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ExecuteAssembly )( 
            IThunderProcess * This,
            /* [in] */ BSTR assemblyPath,
            /* [in] */ BSTR className,
            /* [in] */ BSTR methodName,
            /* [defaultvalue][in] */ BSTR methodArgument,
            /* [defaultvalue][out] */ DWORD *returnValue);
        
        END_INTERFACE
    } IThunderProcessVtbl;

    interface IThunderProcess
    {
        CONST_VTBL struct IThunderProcessVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IThunderProcess_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IThunderProcess_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IThunderProcess_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IThunderProcess_AttachById(This,processId)	\
    ( (This)->lpVtbl -> AttachById(This,processId) ) 

#define IThunderProcess_AttachByName(This,processName)	\
    ( (This)->lpVtbl -> AttachByName(This,processName) ) 

#define IThunderProcess_CreateAndAttach(This,exePath,commandLineArguments,createSuspended)	\
    ( (This)->lpVtbl -> CreateAndAttach(This,exePath,commandLineArguments,createSuspended) ) 

#define IThunderProcess_Resume(This)	\
    ( (This)->lpVtbl -> Resume(This) ) 

#define IThunderProcess_Inject(This,dllPath,functionName,functionArgument)	\
    ( (This)->lpVtbl -> Inject(This,dllPath,functionName,functionArgument) ) 

#define IThunderProcess_ManuallyMap(This,dllPath,functionName,functionArgument)	\
    ( (This)->lpVtbl -> ManuallyMap(This,dllPath,functionName,functionArgument) ) 

#define IThunderProcess_Eject(This,moduleName)	\
    ( (This)->lpVtbl -> Eject(This,moduleName) ) 

#define IThunderProcess_Call(This,moduleName,functionName,usesArgument,functionArgument)	\
    ( (This)->lpVtbl -> Call(This,moduleName,functionName,usesArgument,functionArgument) ) 

#define IThunderProcess_InitializeCLR(This)	\
    ( (This)->lpVtbl -> InitializeCLR(This) ) 

#define IThunderProcess_DestroyCLR(This)	\
    ( (This)->lpVtbl -> DestroyCLR(This) ) 

#define IThunderProcess_ExecuteAssembly(This,assemblyPath,className,methodName,methodArgument,returnValue)	\
    ( (This)->lpVtbl -> ExecuteAssembly(This,assemblyPath,className,methodName,methodArgument,returnValue) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IThunderProcess_INTERFACE_DEFINED__ */



#ifndef __ThunderLib_LIBRARY_DEFINED__
#define __ThunderLib_LIBRARY_DEFINED__

/* library ThunderLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_ThunderLib;

EXTERN_C const CLSID CLSID_ThunderCore;

#ifdef __cplusplus

class DECLSPEC_UUID("86C3EF22-C881-422B-80C7-B45264D0BEC1")
ThunderCore;
#endif
#endif /* __ThunderLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


