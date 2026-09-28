/*-----------------------------------------------------------------------------
 * EcOsPlatform.h
 * Copyright                acontis technologies GmbH, Ravensburg, Germany
 * Response                 Stefan Zintgraf
 * Description              EC-Master OS-Layer header file for QNX
 *----------------------------------------------------------------------------*/

#ifndef INC_ECOSPLATFORM
#define INC_ECOSPLATFORM

/*-SANITY-CHECK---------------------------------------------------------------*/
#ifndef INC_ECOS
#warning EcOsPlatform.h should not be included directly. Include EcOs.h instead
#endif

/*-SUPPORT-SELECTION----------------------------------------------------------*/
#define EC_SOCKET_IP_SUPPORTED

/*-SYSTEM-INCLUDES------------------------------------------------------------*/
/* Platform identification */
#include <sys/neutrino.h>
#ifdef __cplusplus
/* needed to identify Dinkum/GNU */
#include <cstddef>
#endif

/* c-runtime */
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

/* system */
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#if (defined EC_SOCKET_IP_SUPPORTED)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/poll.h>
#include <fcntl.h>
#if (EC_VERSION_QNX >= 700)
#include <netinet/tcp.h>
#endif
#endif /* EC_SOCKET_IP_SUPPORTED */

#ifndef ATECAT_OSSTR
#define ATECAT_OSSTR "QNX"
#endif

/*-OS-VERSION-IDENTIFICATION--------------------------------------------------*/
#ifndef EC_VERSION_QNX
  #if (defined _NTO_VERSION)
    #if (_NTO_VERSION < 600)
#error QNX version too old
    #endif
    #define EC_VERSION_QNX _NTO_VERSION
  #endif /* _NTO_VERSION */
#endif /* EC_VERSION_QNX */

#ifndef EC_VERSION_QNX
#error unable to determine QNX version
#endif

/*-PLATFORM-IDENTIFICATION----------------------------------------------------*/
#if (defined __arm__)
  #ifndef ATECAT_ARCHSTR
    #define ATECAT_ARCHSTR "arm"
  #endif
  #ifndef EC_ARCH
    #define EC_ARCH EC_ARCH_ARM
  #endif
#endif

#if (defined __aarch64__)
#ifndef ATECAT_ARCHSTR
#define ATECAT_ARCHSTR "aarch64"
#endif
#ifndef EC_ARCH
#define EC_ARCH EC_ARCH_ARM64
#endif
#endif

#if (defined __X86_64__)
  #ifndef ATECAT_ARCHSTR
    #define ATECAT_ARCHSTR "x64"
  #endif
  #ifndef EC_ARCH
    #define EC_ARCH EC_ARCH_X64
  #endif
#endif

#if (defined __i386__)
  #ifndef ATECAT_ARCHSTR
    #define ATECAT_ARCHSTR "x86"
  #endif
  #ifndef EC_ARCH
    #define EC_ARCH EC_ARCH_X86
  #endif
#endif

#ifdef __cplusplus
/* identify ABI for Dinkum C++ vs. GNU C++ libraries from <cstddef> */
#ifdef _GLIBCXX_CSTDDEF
#define ATECAT_CPP_LIB_ABI_STR "gnu"
#else
#define ATECAT_CPP_LIB_ABI_STR ""
#endif /* Dinkum C++ vs. GNU C++ */
#endif /* __cplusplus */

#ifndef ATECAT_CPP_LIB_ABI_STR
#define ATECAT_CPP_LIB_ABI_STR "unknown"
#endif

/* undefined architecture */
#ifndef EC_ARCH
#define EC_ARCH EC_ARCH_UNDEFINED
#endif

#ifndef ATECAT_PLATFORMSTR
#ifdef ATECAT_ARCHSTR
#define ATECAT_PLATFORMSTR ATECAT_OSSTR "_" ATECAT_ARCHSTR ATECAT_CPP_LIB_ABI_STR
#else
#define ATECAT_PLATFORMSTR ATECAT_OSSTR ATECAT_CPP_LIB_ABI_STR
#endif
#endif /* !ATECAT_PLATFORMSTR */

/*-DEFINES--------------------------------------------------------------------*/
/* Structure pack */
#define EC_PACKED_INCLUDESTART(Bytes)   <EcOsPlatform.h>
#define EC_PACKED_INCLUDESTOP           <EcOsPlatform.h>
#define EC_PACKED(Bytes)                __attribute__((aligned(Bytes),packed))

/* Needed for type / function declarations in EcOsPlatform.h */
#ifndef EC_FNNAME
#define EC_FNNAME EC_FNNAME_DEFAULT
#endif
#ifndef EC_API
#define EC_API EC_FNNAME
#endif

#ifndef EC_FNCALL
#if (EC_ARCH == EC_ARCH_X86)
#define EC_FNCALL __attribute__((__cdecl__))
#else
#define EC_FNCALL /* __cdecl__ is not availble for ARM, PPC, x64 */

#endif /* !x86 */
#endif /* EC_FNCALL */

#ifndef EC_API_FNCALL
#define EC_API_FNCALL EC_FNCALL
#endif

#ifdef __arm__
#define WITHALIGNMENT   /* --> for ARM etc. */
#endif

/* project */
/* TODO: remove */
#include <EcType.h>


/*-TYPEDEFS-------------------------------------------------------------------*/
typedef struct _OS_LOCK_DESC
{
    EC_T_OS_LOCK_TYPE   eLockType;            /* type of lock e. g. Default, SpinLock, Semaphore */
    pthread_mutex_t     mutex;
    int                 nLockCnt;
} OS_LOCK_DESC;

typedef va_list             EC_T_VALIST;

#ifdef _MSC_VER
typedef unsigned long       EC_T_UINT64;
typedef signed long         EC_T_INT64;
#else
typedef unsigned long long  EC_T_UINT64;
typedef signed long long    EC_T_INT64;
#endif

#define EC_CPUSET_DEFINED 1
#if (defined EC_ARCH_X64)
  typedef long EC_T_CPUSET;                                       /* CPU-set for SMP systems */
#else
  typedef int EC_T_CPUSET;                                        /* CPU-set for SMP systems */
#endif

/*-MACROS---------------------------------------------------------------------*/

#define EC_VASTART   va_start
#define EC_VAEND     va_end
#define EC_VAARG     va_arg

/* EC_INLINEINT provides needed param at ## when issuing string */
#define EC_INLINEINT(dummy, functiondecl)     __inline dummy ##functiondecl
#define EC_INLINE(funcdecl) EC_INLINEINT(,funcdecl)

/* use macros for the most important OS layer routines */
EC_FNNAME EC_T_DWORD EC_FNCALL OsSetLastError(EC_T_DWORD dwError);
#define OsSetLastError OsSetLastError
#if (EC_VERSION_QNX < 700)
#define OsSleep(dwMsec)                         delay((unsigned int)(dwMsec==0?0:dwMsec>2?(dwMsec-2):dwMsec-1))
#define OsStrncmp(szStr1,szStr2, nSize)         strnicmp((const char*)szStr1,(const char*)szStr2, (size_t)nSize)
#else
#define OsSleep(dwMsec)                         delay((unsigned int)(dwMsec))
#define OsStrncmp(szStr1,szStr2, nSize)         strncasecmp((const char*)szStr1,(const char*)szStr2, (size_t)nSize)
#define OsStricmp(szStr1,szStr2)                strcasecmp((const char*)szStr1,(const char*)szStr2 )
#endif
#ifdef DEBUG
#ifdef ASSERT_SUSPEND
#define OsDbgAssert(bCond)                      OsDbgAssertFunc((bCond),__FILE__,__LINE__)
#else
#define OsDbgAssert                             assert
#endif
#endif
#ifndef OsDbgAssert
#define OsDbgAssert(x)
#endif

#define OsSetEvent(pvEvent)                     SetEventFunc((sem_t*)(pvEvent))
#define OsResetEvent(pvEvent)                   sem_trywait((sem_t*)(pvEvent))
#define OsMemoryBarrier()

#if (defined EC_SOCKET_IP_SUPPORTED)
#if !(defined SOCKET)
typedef int SOCKET;
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET  ((EC_T_INT)-1)
#endif

#ifndef closesocket
#define closesocket(s) \
    s=INVALID_SOCKET
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR ((EC_T_INT)-1)
#endif

#ifndef SOCKADDR
typedef struct sockaddr SOCKADDR;
#endif
typedef struct timeval  TIMEVAL;

#ifndef SHUT_RD
#ifndef SD_RECEIVE
#define SD_RECEIVE  ((EC_T_INT)0)
#endif
#define SHUTD_RD    ((EC_T_INT)0)
#endif

#ifndef SHUT_WR
#ifndef SD_SEND
#define SD_SEND     ((EC_T_INT)1)
#endif
#define SHUTD_WR    ((EC_T_INT)1)
#endif

#ifndef SHUT_RDWR
#ifndef SD_BOTH
#define SD_BOTH     ((EC_T_INT)2)
#endif
#define SHUTD_RDWR  ((EC_T_INT)2)
#endif

#ifndef SD_RECEIVE
#define SD_RECEIVE  SHUT_RD
#endif

#ifndef SD_SEND
#define SD_SEND     SHUT_WR
#endif

#ifndef SD_BOTH
#define SD_BOTH     SHUT_RDWR
#endif

#endif /* #if (defined EC_SOCKET_IP_SUPPORTED) */

#undef INCLUDE_PROFILING
/*#define INCLUDE_PROFILING */  /* use profiling */
#undef LPT_PROFILING       /* enable printer port profiling signals */
#define INCLUDE_PROFILING_EVENTS         /* enable WindView profiling */

#define OsTscMeasDisableIrq(pTscMeasDesc,dwIndex)  { ThreadCtl( _NTO_TCTL_IO, 0 );InterruptDisable(); };
#define OsTscMeasEnableIrq(pTscMeasDesc,dwIndex)   { ThreadCtl( _NTO_TCTL_IO, 0 );InterruptEnable(); };

#define OsReleaseLinkLayerRegFunc OsReleaseLinkLayerRegFunc

/*-PROTOTYPES-----------------------------------------------------------------*/

EC_FNNAME EC_T_VOID EC_FNCALL SetEventFunc(sem_t* pEvent);

#if (defined EC_SOCKET_IP_SUPPORTED)
 #define OsSocketAccept(hSockHandle, oSockAddr, nSockAddrLen) accept(hSockHandle, oSockAddr, (socklen_t*) nSockAddrLen)
 #define OsSocketRecvFrom(hSockHandle, pbyBuffer, dwBufferLen, dwFlags, oSrcAddr, dwSrcAddrLen) recvfrom(hSockHandle, pbyBuffer, dwBufferLen, dwFlags, oSrcAddr, (socklen_t*)dwSrcAddrLen)
 #define OsSocketGetLastError() 0
#endif /* #if (defined EC_SOCKET_IP_SUPPORTED) */

EC_FNNAME EC_T_VOID EC_FNCALL OsDbgAssertFunc(EC_T_BOOL bAssertCondition, const EC_T_CHAR* szFile, EC_T_DWORD dwLine);

#endif /* INC_ECOSPLATFORM */

/*-END OF SOURCE FILE---------------------------------------------------------*/
