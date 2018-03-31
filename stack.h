/* Author: Alan Manning */
/* Course: CS201 Data Structures and Algorithms */
/* Assignment0 */
/* Date: 25 January , 2018 */

/* This is the header file for a stack class.
The class includes the following methods and fields and 
uses an object oriented approach to implement an array that
contains generic data values as its entries. */

#ifndef __STACK_INCLUDED__
#define __STACK_INCLUDED__

#include <stdio.h>

typedef struct stack STACK;

extern STACK *newSTACK(void (*d)(void *,FILE *), void (*f)(void *)); 
extern void push(STACK *items,void *value);
extern void *pop(STACK *items);
extern void *peekSTACK(STACK *items);
extern int sizeSTACK(STACK *items);
extern void displaySTACK(STACK *items,FILE *);
extern void displaySTACKdebug(STACK *items,FILE *);
extern void freeSTACK(STACK *items);

#endif

