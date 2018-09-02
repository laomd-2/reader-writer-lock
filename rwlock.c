//
// Created by laomd on 18-6-2.
//

#include <stdio.h>
#include "rwlock.h"

void rwlock_init(rwlock_t *rwl, int reader_first)
{
    rwl->reader_first = reader_first;
    rwl->r_cnt = rwl->w_cnt = 0;
    rwl->r_wait = rwl->w_wait = 0;
    pthread_mutex_init (&rwl->mutex, NULL);
    pthread_cond_init (&rwl->read, NULL);
    pthread_cond_init (&rwl->write, NULL);
}

void rwlock_destroy(rwlock_t *rwl)
{
    pthread_mutex_destroy (&rwl->mutex);
    pthread_cond_destroy (&rwl->read);
    pthread_cond_destroy (&rwl->write);
}

void rwlock_rdlock(rwlock_t *rwl)
{
    pthread_mutex_lock (&rwl->mutex);
    while(rwl->w_cnt > 0) {
        rwl->r_wait++;
        while(pthread_cond_wait(&rwl->read, &rwl->mutex) != 0);
        rwl->r_wait--;
    }

    rwl->r_cnt++;

    pthread_mutex_unlock (&rwl->mutex);
}

void rwlock_rdunlock(rwlock_t *rwl)
{
    pthread_mutex_lock (&rwl->mutex);

    rwl->r_cnt--;
    if (rwl->r_cnt == 0 && rwl->w_wait > 0) {
        pthread_cond_signal (&rwl->write);
    }

    pthread_mutex_unlock (&rwl->mutex);
}

void rwlock_wrlock(rwlock_t *rwl)
{
    pthread_mutex_lock (&rwl->mutex);

    while (rwl->w_cnt || rwl->r_cnt > 0 || rwl->reader_first && rwl->r_wait > 0) {
        rwl->w_wait++;
        while (pthread_cond_wait (&rwl->write, &rwl->mutex) != 0);
        rwl->w_wait--;
    }
    rwl->w_cnt++;

    pthread_mutex_unlock (&rwl->mutex);
}

void rwlock_wrunlock(rwlock_t *rwl)
{
    pthread_mutex_lock (&rwl->mutex);

    rwl->w_cnt--;
    if (rwl->reader_first) {
        if (rwl->r_wait > 0) {
            pthread_cond_broadcast (&rwl->read);
        } else if (rwl->w_wait > 0) {
            pthread_cond_signal (&rwl->write);
        }
    } else {
        if (rwl->w_wait > 0) {
            pthread_cond_signal (&rwl->write);
        } else if (rwl->r_wait > 0) {
            pthread_cond_broadcast(&rwl->read);
        }
    }

    pthread_mutex_unlock (&rwl->mutex);
}

