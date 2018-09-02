/* File:     pth_ll_rwl.c
 *
 * Purpose:  Implement a multi-threaded sorted linked list of 
 *           ints with ops insert, print, member, delete, free list.  
 *           This version uses read-write locks
 * 
 * Compile:  gcc -g -Wall -o pth_ll_rwl pth_ll_rwl.c 
 *              my_rand.c -lpthread
 *           needs timer.h and my_rand.h
 * Usage:    ./pth_ll_rwl <thread_count>
 * Input:    total number of keys inserted by main thread
 *           total number of ops of each type carried out by each
 *              thread.
 * Output:   Elapsed time to carry out the ops
 *
 * Notes:
 *    1.  Repeated values are not allowed in the list
 *    2.  DEBUG compile flag used.  To get debug output compile with
 *        -DDEBUG command line flag.
 *    3.  Uses the Unix 98 Standard implementation of read-write locks.
 *    4.  The random function is not threadsafe.  So this program
 *        uses a simple linear congruential generator.
 *    5.  -DOUTPUT flag to gcc will show list before and after
 *        threads have worked on it.
 *
 * IPP:   Section 4.9.3 (pp. 187 and ff.)
 */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "list.h"
#include "rwlock.h"
#include "my_rand.h"
#include "timer.h"

/* Random ints are less than MAX_KEY */
const int MAX_KEY = 100000000;

/* Shared variables */
struct      list_node_s* head = NULL;
int         thread_count;
int         total_ops;
double      insert_percent;
double      search_percent;

pthread_mutex_t     count_mutex;
int         member_count = 0, insert_count = 0, delete_count = 0;

rwlock_t rwlock;

/* Setup and cleanup */
void        Usage(char* prog_name);

/* Thread function */
void*       Thread_work(void* rank);

/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {
    long i;
    int key, success, attempts;
    unsigned seed = 1;
    pthread_t* thread_handles;
    int inserts_in_main;
    double start, finish;

    if (argc != 7) Usage(argv[0]);
    thread_count = strtol(argv[1],NULL,10);
    inserts_in_main = atoi(argv[2]);
    total_ops = atoi(argv[3]);
    search_percent = strtod(argv[4], NULL);
    insert_percent = strtod(argv[5], NULL);

    /* Try to insert inserts_in_main keys, but give up after */
    /* 2*inserts_in_main attempts.                           */
    i = attempts = 0;
    while ( i < inserts_in_main && attempts < 2*inserts_in_main ) {
        key = my_rand(&seed) % MAX_KEY;
        success = Insert(&head, key);
        attempts++;
        if (success) i++;
    }
//   printf("Inserted %ld keys in empty list\n", i);

#  ifdef OUTPUT
    printf("Before starting threads, list = \n");
    Print(head);
    printf("\n");
#  endif

    thread_handles = malloc(thread_count*sizeof(pthread_t));
 
    pthread_mutex_init(&count_mutex, NULL);
    rwlock_init(&rwlock, atoi(argv[6]));

    GET_TIME(start);
    for (i = 0; i < thread_count; i++)
        pthread_create(&thread_handles[i], NULL, Thread_work, (void*)i);

    for (i = 0; i < thread_count; i++)
        pthread_join(thread_handles[i], NULL);
    GET_TIME(finish);

    printf("%f\n", finish - start);
//    printf("Total ops = %d\n", total_ops);
//    printf("member ops = %d\n", member_count);
//    printf("insert ops = %d\n", insert_count);
//    printf("delete ops = %d\n", delete_count);

#  ifdef OUTPUT
    printf("After threads terminate, list = \n");
    Print(head);
    printf("\n");
#  endif

    Free_list(head);
    pthread_mutex_destroy(&count_mutex);
    rwlock_destroy(&rwlock);
    free(thread_handles);

    return 0;
}  /* main */


/*-----------------------------------------------------------------*/
void Usage(char* prog_name) {
    fprintf(stderr, "usage: %s <thread_count> <inserts in main> <total ops> <search percent> <insert percent> <rf>\n", prog_name);
    exit(0);
}  /* Usage */


/*-----------------------------------------------------------------*/
void* Thread_work(void* rank) {
    unsigned my_rank = (unsigned) rank;
    int i, val;
    double which_op;
    unsigned seed = my_rank + 1;
    int my_member_count = 0, my_insert_count = 0, my_delete_count = 0;
    int ops_per_thread = total_ops/thread_count;

    for (i = 0; i < ops_per_thread; i++) {
        which_op = my_drand(&seed);
        val = my_rand(&seed) % MAX_KEY;
//        printf("%f %d\n", which_op, val);
        if (which_op < search_percent) {
            rwlock_rdlock(&rwlock);
            Member(head, val);
            rwlock_rdunlock(&rwlock);
            my_member_count++;
        } else if (which_op < search_percent + insert_percent) {
            rwlock_wrlock(&rwlock);
            Insert(&head, val);
            rwlock_wrunlock(&rwlock);
            my_insert_count++;
        } else { /* delete */
            rwlock_wrlock(&rwlock);
            Delete(&head, val);
            rwlock_wrunlock(&rwlock);
            my_delete_count++;
        }
    }  /* for */

    pthread_mutex_lock(&count_mutex);
    member_count += my_member_count;
    insert_count += my_insert_count;
    delete_count += my_delete_count;
    pthread_mutex_unlock(&count_mutex);

    return NULL;
}  /* Thread_work */
