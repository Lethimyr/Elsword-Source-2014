#ifndef THREADDEFS_H
#define THREADDEFS_H "$Id: ThreadDefs.h,v 1.1 2003/05/23 01:18:40 cvs Exp $"

#include <process.h>

#ifndef _BEGINTHREADEX 
typedef unsigned (__stdcall* PTHREAD_START)(void*);
#   define _BEGINTHREADEX( psa, \
                           cbStack, \
                           pfnStartAddr, \
                           pvParam, \
                           fdwCreate, \
                           pdwThreadId) \
          ((HANDLE)_beginthreadex( (void*) (psa), \
                                   (unsigned) (cbStack), \
                                   (PTHREAD_START) (pfnStartAddr), \
                                   (void*) (pvParam), \
                                   (unsigned) (fdwCreate), \
                                   (unsigned*) (pdwThreadId)))
#endif

#endif