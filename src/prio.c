#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "prio.h"
#include "macro.h"

#define MAX_PRIO_CAPACITY 1024

#define GET_PARENT_IDX(i) ((((int)i) - 1) / (2)) 
#define GET_LEFT_CHILD(i) ((2 * (i)) + (1)) 
#define GET_RIGHT_CHILD(i) ((2 * (i)) + (2)) 

struct prio_queue {
    int heap[MAX_PRIO_CAPACITY];
    unsigned int len;
};

static void swap(int *p, int *q) {
    int tmp = *p;
    *p = *q; 
    *q = tmp;
}

static void bubble_up(PrioQueue *pq, unsigned int i) {
    int p = GET_PARENT_IDX(i);
    while(p >= 0) {
        if(pq->heap[p] < pq->heap[i]) {
            swap(&pq->heap[p], &pq->heap[i]);
            i = p;
            p = GET_PARENT_IDX(i);
        } else {
            break;
        }
    }
}

static void bubble_down(PrioQueue *pq, unsigned int i) {
    unsigned int left_child, right_child;
    unsigned int sub;

    left_child = GET_LEFT_CHILD(i); 
    right_child = GET_RIGHT_CHILD(i); 
    sub = i;

    if(left_child < pq->len 
            && pq->heap[left_child] > pq->heap[sub]) {
        sub = left_child;
    }
    if(right_child < pq->len 
            && pq->heap[right_child] > pq->heap[sub]) {
        sub = right_child;
    }
    if(sub != i) {
        swap(&pq->heap[sub], &pq->heap[i]);
        bubble_down(pq, sub);
    }
}

PrioQueue *prio_init() {
    PrioQueue *pq = malloc(sizeof(PrioQueue));
    if(pq == NULL) {
        return NULL;
    }
    memset(pq->heap, 0xFF, sizeof(pq->heap));
    pq->len = 0;

    return pq;
}

int prio_peek_max(PrioQueue *pq, int *get_max) {
    if(pq == NULL || get_max == NULL || pq->len <= 0) {
        return -1;
    }
    *get_max = pq->heap[0];
    return 0;
}

int prio_insert(PrioQueue *pq, int val) {
    if(pq == NULL) {
        return -1;
    }
    if(pq->len >= MAX_PRIO_CAPACITY) {
        return -1;
    }
    pq->heap[pq->len] = val;
    bubble_up(pq, pq->len); 
    pq->len++;

    return 0;
}

int prio_remove_max(PrioQueue *pq, int *get_max) {
    if(pq == NULL) {
        return -1;
    }
    int ret;
    if((ret = prio_peek_max(pq, get_max)) == -1) {
        return -1;
    }
    swap(&pq->heap[0], &pq->heap[pq->len - 1]);
    pq->len--; 
    bubble_down(pq, 0);

    return ret;
}

void prio_destroy(PrioQueue *pq) {
    if(pq == NULL) {
        return;
    }
    free(pq);
}
