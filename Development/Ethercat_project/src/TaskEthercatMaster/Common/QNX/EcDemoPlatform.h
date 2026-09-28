/*-----------------------------------------------------------------------------
 * EcDemoPlatform.h
 * Copyright                acontis technologies GmbH, Ravensburg, Germany
 * Response                 Holger Oelhaf
 * Description              Platform specific settings for EC demo
 *---------------------------------------------------------------------------*/

#ifndef INC_ECDEMOPLATFORM_H
#define INC_ECDEMOPLATFORM_H 1

/*-DEFINES-------------------------------------------------------------------*/
/* we need to set all threads to the highest possible priority to avoid errors! */

#define TIMER_THREAD_PRIO           ((EC_T_DWORD)100)   /* EtherCAT master trigger thread priority */
#define JOBS_THREAD_PRIO            ((EC_T_DWORD)99)    /* EtherCAT master job thread priority */
#define RECV_THREAD_PRIO            ((EC_T_DWORD)98)    /* EtherCAT master interrupt service thread priority */
#define LOG_THREAD_PRIO             ((EC_T_DWORD)40)    /* EtherCAT message logging thread priority */
#define MAIN_THREAD_PRIO            ((EC_T_DWORD)30)
#define REMOTE_RECV_THREAD_PRIO     ((EC_T_DWORD)50)    /* slightly higher than logger */

#define TIMER_THREAD_STACKSIZE       0x1000
#if ((EC_ARCH == EC_ARCH_X64) || (EC_ARCH == EC_ARCH_ARM64))
#define JOBS_THREAD_STACKSIZE        0x8000
#define LOG_THREAD_STACKSIZE         0x8000
#else
#define JOBS_THREAD_STACKSIZE        0x4000
#define LOG_THREAD_STACKSIZE         0x4000
#endif

#endif /* INC_ECDEMOPLATFORM_H */

/*-END OF SOURCE FILE--------------------------------------------------------*/
