/*-----------------------------------------------------------------------------
 * EcMemoryLog.h
 * Copyright                acontis technologies GmbH, Ravensburg, Germany
 * Description              Memory Logger implementation
 *---------------------------------------------------------------------------*/

#ifndef INC_ECMEMORYLOG
#define INC_ECMEMORYLOG

/*-INCLUDES------------------------------------------------------------------*/
#ifndef INC_ECOS
#include "EcOs.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*-TYPEDEFS------------------------------------------------------------------*/
typedef struct _EC_T_MEMORY_LOGGER
{
    size_t      nCurrUsage;         /* current dynamic memory usage */
    size_t      nMaxUsage;          /* maximum dynamic memory usage */
} EC_T_MEMORY_LOGGER;

/*-MACROS--------------------------------------------------------------------*/

#ifndef EC_TRACE_ADDMEM_LOG
static EC_INLINESTART EC_T_VOID EC_TRACE_ADDMEM_LOG(EC_T_MEMORY_LOGGER* pLog, size_t nSize)
{
    pLog->nCurrUsage += nSize;
    if (pLog->nCurrUsage > pLog->nMaxUsage)
    {
        pLog->nMaxUsage = pLog->nCurrUsage;
    }
} EC_INLINESTOP

#ifdef pEcLogParms
#define EC_TRACE_ADDMEM_LOG(Mask, pLog, szLoc, dwAddress, nSize)                        \
{   /* only evaluate parameters once */                                                 \
    EC_T_MEMORY_LOGGER* _pLog = pLog;                                                   \
    size_t              _nSize = nSize;                                                 \
    if (EC_NULL != _pLog)                                                               \
    {                                                                                   \
        EC_TRACE_ADDMEM_LOG(_pLog, _nSize);                                             \
        EcLogMsg(EC_LOG_LEVEL_VERBOSE_CYC, (pEcLogContext, EC_LOG_LEVEL_VERBOSE_CYC,    \
            "+MEM: %60s =%6d (addr = 0x%08x), curr/max = %6d/%6d\n",                    \
            (szLoc), _nSize, dwAddress, _pLog->nCurrUsage, _pLog->nMaxUsage));          \
    }                                                                                   \
}
#else
#define EC_TRACE_ADDMEM_LOG(Mask, pLog, szLoc, dwAddress, nSize)                \
{   /* only evaluate parameters once */                                         \
    EC_T_MEMORY_LOGGER* _pLog = pLog;                                           \
    if (EC_NULL != _pLog)                                                       \
    {                                                                           \
        EC_TRACE_ADDMEM_LOG(_pLog, nSize);                                      \
    }                                                                           \
}
#endif

#endif



#ifndef EC_TRACE_SUBMEM_LOG
static EC_INLINESTART EC_T_VOID EC_TRACE_SUBMEM_LOG(EC_T_MEMORY_LOGGER* pLog, size_t nSize)
{
    if (EC_NULL != pLog)
    {
        if(nSize < pLog->nCurrUsage)
        {
            pLog->nCurrUsage -= nSize;
        }
        else
        {
            pLog->nCurrUsage = 0;
        }
    }
} EC_INLINESTOP

#define EC_TRACE_SUBMEM_LOG(Mask, pLogger, szLoc, dwAddress, nSize) \
    EC_TRACE_SUBMEM_LOG(pLogger, nSize)
#endif

#ifndef EC_TRACE_ADDMEM
#define EC_TRACE_ADDMEM(Mask, szLoc, dwAddress, dwSize) \
            EC_TRACE_ADDMEM_LOG(Mask, GetMemLog(), szLoc, dwAddress, dwSize)
#endif

#ifndef EC_TRACE_SUBMEM
#define EC_TRACE_SUBMEM(Mask, szLoc, dwAddress, dwSize) \
            EC_TRACE_SUBMEM_LOG(Mask, GetMemLog(), szLoc, dwAddress, dwSize)
#endif

#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif /* INC_ECMEMORYLOG */

/*-END OF SOURCE FILE--------------------------------------------------------*/
