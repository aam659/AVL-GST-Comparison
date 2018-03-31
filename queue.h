/* Author: Alan Manning */
/* Course: CS 201 Data Structures and Algorithms */
/* Assignment 0 */
/* Date: 25 January, 2018 */

/* This is the header file for a queue class.
The class includes the following methods and fields and 
uses an object oriented approach to implement an array that
contains generic data values as its entries. */

#ifndef __QUEUE_INCLUDED__
#define __QUEUE_INCLUDED__

#include <stdio.h>

typedef struct queue QUEUE;

extern QUEUE *newQUEUE(void (*d)(void *,FILE *), void (*f)(void *)); 
extern void enqueue(QUEUE *items,void *value);
extern void *dequeue(QUEUE *items);
extern void *peekQUEUE(QUEUE *items);
extern int sizeQUEUE(QUEUE *items);
extern void displayQUEUE(QUEUE *items, FILE *);
extern void displayQUEUEdebug(QUEUE *items,FILE *);
extern void freeQUEUE(QUEUE *items);

#endif

