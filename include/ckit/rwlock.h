#ifndef __CKIT_RWLOCK_H__
#define __CKIT_RWLOCK_H__

#include <ckit/info.h>
#include <ckit/utils.h>
#include <ckit/error.h>
#include <ckit/config.h>

#if CKIT_ENABLE_RWLOCK
    #ifdef INFO_OS_UNKNOWN
        #error CKIT_ERROR_UNKOWN_OS
    #endif

    #ifdef INFO_OS_WINDOWS
        #include <windows.h>
        #define RWLOCK_TYPE             SRWLOCK
        #define RWLOCK_INIT(lock)       InitializeSRWLock(&(lock))
        #define RWLOCK_RDLOCK(lock)     AcquireSRWLockShared(&(lock))
        #define RWLOCK_WRLOCK(lock)     AcquireSRWLockExclusive(&(lock))
        #define RWLOCK_RDUNLOCK(lock)   ReleaseSRWLockShared(&(lock))
        #define RWLOCK_WRUNLOCK(lock)   ReleaseSRWLockExclusive(&(lock))
        #define RWLOCK_DESTROY(lock)    NOOP
    #else
        #include <pthread.h>
        #define RWLOCK_TYPE             pthread_rwlock_t
        #define RWLOCK_INIT(lock)       pthread_rwlock_init(&(lock), NULL)
        #define RWLOCK_RDLOCK(lock)     pthread_rwlock_rdlock(&(lock))
        #define RWLOCK_WRLOCK(lock)     pthread_rwlock_wrlock(&(lock))
        #define RWLOCK_RDUNLOCK(lock)   pthread_rwlock_unlock(&(lock))
        #define RWLOCK_WRUNLOCK(lock)   pthread_rwlock_unlock(&(lock))
        #define RWLOCK_DESTROY(lock)    pthread_rwlock_destroy(&(lock))
    #endif
#else
    #define RWLOCK_TYPE                 void*
    #define RWLOCK_INIT(lock)           NOOP
    #define RWLOCK_RDLOCK(lock)         NOOP
    #define RWLOCK_WRLOCK(lock)         NOOP
    #define RWLOCK_RDUNLOCK(lock)       NOOP
    #define RWLOCK_WRUNLOCK(lock)       NOOP
    #define RWLOCK_DESTROY(lock)        NOOP
#endif

#endif /* __CKIT_RWLOCK_H__ */