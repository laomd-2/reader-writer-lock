//
// Created by laomd on 2018/6/1.
//

#ifndef RWLOCK_RWLOCK_H
#define RWLOCK_RWLOCK_H

#include <pthread.h>

typedef struct rwlock_t {
    pthread_mutex_t     mutex;
    pthread_cond_t      read, write;
    int                 r_cnt, w_cnt;           /* number of readers/writers */
    int                 r_wait, w_wait;         /* readers/writers waiting */
    int                 reader_first;           /* priority for read or write mutex*/
} rwlock_t;

void rwlock_init(rwlock_t *rwlock, int reader_first);

void rwlock_destroy(rwlock_t *rwlock);

void rwlock_rdlock(rwlock_t *rwlock);

void rwlock_rdunlock(rwlock_t *rwlock);

void rwlock_wrlock(rwlock_t *rwlock);

void rwlock_wrunlock(rwlock_t *rwlock);

#endif //RWLOCK_RWLOCK_H
