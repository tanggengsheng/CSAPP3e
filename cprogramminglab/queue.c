/**
 * @file queue.c
 * @brief Implementation of a queue that supports FIFO and LIFO operations.
 *
 * This queue implementation uses a singly-linked list to represent the
 * queue elements. Each queue element stores a string value.
 *
 * Assignment for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Extended to store strings, 2018
 *
 * TODO: fill in your name and Andrew ID
 * @author XXX <XXX@andrew.cmu.edu>
 */

#include "queue.h"
#include "harness.h"

#include <stdlib.h>
#include <string.h>

/**
 * @brief Allocates a new queue
 * @return The new queue, or NULL if memory allocation failed
 */
queue_t *queue_new(void) {
    queue_t *q = malloc(sizeof(queue_t));
    /* What if malloc returned NULL? */
    if (q == NULL) {
        return NULL;
    }

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;

    return q;
}

/**
 * @brief Frees all memory used by a queue
 * @param[in] q The queue to free
 */
void queue_free(queue_t *q) {
    if (q == NULL) {
        return;
    }
    /* How about freeing the list elements and the strings? */
    while (q->head != NULL) {
        list_ele_t *ele = q->head;
        q->head = ele->next;
        free(ele->value);
        free(ele);
    }
    /* Free queue structure */
    free(q);
}

/**
 * @brief Attempts to insert an element at head of a queue
 *
 * This function explicitly allocates space to create a copy of `s`.
 * The inserted element points to a copy of `s`, instead of `s` itself.
 *
 * @param[in] q The queue to insert into
 * @param[in] s String to be copied and inserted into the queue
 *
 * @return true if insertion was successful
 * @return false if q is NULL, or memory allocation failed
 */
bool queue_insert_head(queue_t *q, const char *s) {
    /* What should you do if the q is NULL? */
    if (q == NULL) {
        return false;
    }

    list_ele_t *newh = malloc(sizeof(list_ele_t));
    /* What if either call to malloc returns NULL? */
    if (newh == NULL) {
        return false;
    }

    /* Don't forget to allocate space for the string and copy it */
    size_t n = strlen(s) + 1;
    newh->value = malloc(n);
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, n);
    newh->value[n - 1] = '\0'; // make sure termination
    newh->next = NULL;

    if (q->head == NULL) {
        q->head = q->tail = newh;
    } else {
        newh->next = q->head;
        q->head = newh;
    }

    q->size++;

    return true;
}

/**
 * @brief Attempts to insert an element at tail of a queue
 *
 * This function explicitly allocates space to create a copy of `s`.
 * The inserted element points to a copy of `s`, instead of `s` itself.
 *
 * @param[in] q The queue to insert into
 * @param[in] s String to be copied and inserted into the queue
 *
 * @return true if insertion was successful
 * @return false if q is NULL, or memory allocation failed
 */
bool queue_insert_tail(queue_t *q, const char *s) {
    if (q == NULL) {
        return false;
    }

    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (newt == NULL) {
        return false;
    }

    size_t n = strlen(s) + 1;
    newt->value = malloc(n);
    if (newt->value == NULL) {
        free(newt);
        return false;
    }
    strncpy(newt->value, s, n);
    newt->value[n - 1] = '\0'; // make sure termination
    newt->next = NULL;

    if (q->tail == NULL) {
        q->head = q->tail = newt;
    } else {
        q->tail->next = newt;
        q->tail = newt;
    }

    q->size++;

    return true;
}

/**
 * @brief Attempts to remove an element from head of a queue
 *
 * If removal succeeds, this function frees all memory used by the
 * removed list element and its string value before returning.
 *
 * If removal succeeds and `buf` is non-NULL, this function copies up to
 * `bufsize - 1` characters from the removed string into `buf`, and writes
 * a null terminator '\0' after the copied string.
 *
 * @param[in]  q       The queue to remove from
 * @param[out] buf     Output buffer to write a string value into
 * @param[in]  bufsize Size of the buffer `buf` points to
 *
 * @return true if removal succeeded
 * @return false if q is NULL or empty
 */
bool queue_remove_head(queue_t *q, char *buf, size_t bufsize) {
    if (queue_size(q) < 1) {
        return false;
    }

    list_ele_t *ele = q->head;
    q->head = ele->next;
    if (--q->size == 0) {
        q->tail = NULL;
    }

    if ((buf != NULL) && (bufsize != 0)) {
        strncpy(buf, ele->value, bufsize);
        buf[bufsize - 1] = '\0'; // make sure termination
    }

    free(ele->value);
    free(ele);

    return true;
}

/**
 * @brief Returns the number of elements in a queue
 *
 * This function runs in O(1) time.
 *
 * @param[in] q The queue to examine
 *
 * @return the number of elements in the queue, or
 *         0 if q is NULL or empty
 */
size_t queue_size(queue_t *q) {
    return q == NULL ? 0 : q->size;
}

/**
 * @brief Reverse the elements in a queue
 *
 * This function does not allocate or free any list elements, i.e. it does
 * not call malloc or free, including inside helper functions. Instead, it
 * rearranges the existing elements of the queue.
 *
 * @param[in] q The queue to reverse
 */
void queue_reverse(queue_t *q) {
    if (queue_size(q) < 2) {
        return;
    }

    list_ele_t *newt = q->head;
    list_ele_t *ele = newt->next;
    while (ele != NULL) {
        newt->next = ele->next;
        ele->next = q->head;
        q->head = ele;
        ele = newt->next;
    }
    q->tail = newt;
}
