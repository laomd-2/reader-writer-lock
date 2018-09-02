//
// Created by laomd on 2018/6/1.
//

#ifndef RWLOCK_LIST_H
#define RWLOCK_LIST_H

/* Struct for list nodes */
struct list_node_s {
    int    data;
    struct list_node_s* next;
};

/* List operations */
int         Insert(struct list_node_s** head, int value);
void        Print(struct list_node_s* head);
int         Member(struct list_node_s* head, int value);
int         Delete(struct list_node_s** head, int value);
void        Free_list(struct list_node_s* head);
int         Is_empty(struct list_node_s* head);

#endif //RWLOCK_LIST_H
