//
// Created by laomd on 2018/6/1.
//

#include <stdio.h>
#include <stdlib.h>
#include "list.h"
/*-----------------------------------------------------------------*/
/* Insert value in correct numerical location into list */
/* If value is not in list, return 1, else return 0 */
int Insert(struct list_node_s** head, int value) {
    struct list_node_s* curr = *head;
    struct list_node_s* pred = NULL;
    struct list_node_s* temp;
    int rv = 1;

    while (curr != NULL && curr->data < value) {
        pred = curr;
        curr = curr->next;
    }

    if (curr == NULL || curr->data > value) {
        temp = malloc(sizeof(struct list_node_s));
        temp->data = value;
        temp->next = curr;
        if (pred == NULL)
            *head = temp;
        else
            pred->next = temp;
    } else { /* value in list */
        rv = 0;
    }

    return rv;
}  /* Insert */

/*-----------------------------------------------------------------*/
void Print(struct list_node_s* head) {
    struct list_node_s* temp;

    printf("list = ");

    temp = head;
    while (temp != (struct list_node_s*) NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}  /* Print */


/*-----------------------------------------------------------------*/
int  Member(struct list_node_s* head, int value) {
    struct list_node_s* temp;

    temp = head;
    while (temp != NULL && temp->data < value)
        temp = temp->next;

    if (temp == NULL || temp->data > value) {
#     ifdef DEBUG
        printf("%d is not in the list\n", value);
#     endif
        return 0;
    } else {
#     ifdef DEBUG
        printf("%d is in the list\n", value);
#     endif
        return 1;
    }
}  /* Member */

/*-----------------------------------------------------------------*/
/* Deletes value from list */
/* If value is in list, return 1, else return 0 */
int Delete(struct list_node_s** head, int value) {
    struct list_node_s* curr = *head;
    struct list_node_s* pred = NULL;
    int rv = 1;

    /* Find value */
    while (curr != NULL && curr->data < value) {
        pred = curr;
        curr = curr->next;
    }

    if (curr != NULL && curr->data == value) {
        if (pred == NULL) { /* first element in list */
            *head = curr->next;
#        ifdef DEBUG
            printf("Freeing %d\n", value);
#        endif
            free(curr);
        } else {
            pred->next = curr->next;
#        ifdef DEBUG
            printf("Freeing %d\n", value);
#        endif
            free(curr);
        }
    } else { /* Not in list */
        rv = 0;
    }

    return rv;
}  /* Delete */

/*-----------------------------------------------------------------*/
void Free_list(struct list_node_s* head) {
    struct list_node_s* current;
    struct list_node_s* following;

    if (Is_empty(head)) return;
    current = head;
    following = current->next;
    while (following != NULL) {
#     ifdef DEBUG
        printf("Freeing %d\n", current->data);
#     endif
        free(current);
        current = following;
        following = current->next;
    }
#  ifdef DEBUG
    printf("Freeing %d\n", current->data);
#  endif
    free(current);
}  /* Free_list */

/*-----------------------------------------------------------------*/
int  Is_empty(struct list_node_s* head) {
    if (head == NULL)
        return 1;
    else
        return 0;
}  /* Is_empty */
