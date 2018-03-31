/* Author: Alan Manning */
/* Class: CS201 Data Structures and Algorithms */
/* Assignment 0 */
/* Date: 25 January, 2018 */

/*This is the implementation file for the header file "stack.h",
which is used to create a stack 'class' for generic data types.*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include "stack.h"
#include "dll.h"

struct stack {
	void (*display)(void *, FILE *);			// display field
	void (*free)(void *);					// free field
	DLL *stackItem;						// DLL array item for stack
	int size;						// Size of stack
};

/*************** public interface ***************/

/* constructor */
/* This is the constructor for the stack objects. */

STACK *newSTACK(void (*d)(void *,FILE *),void (*f)(void *)) {
	STACK *items = malloc(sizeof(STACK));			// Allocate space for stack object	
	assert(items != 0);

	items->stackItem = newDLL(d, f);			// Initialize DLL for stack
	items->size = 0;					// Set stack size to 0
	items->display = d;					// Set display field to passed display method
	items->free = f;					// Set free field to passed free method

	return items;
}

/* push method */
/* This is the method that pushes items onto the stack. */

void push(STACK *items,void *value) {
	insertDLL(items->stackItem, 0, value);			// Calls insertDLL method to push onto stack
	sizeSTACK(items);					// Calls size method to correct stack size
	return;
}

/* pop method */
/* This is the pop method that pops items from the stack
and returns them. */

void *pop(STACK *items) {
	assert(items->size > 0);
	void *poppedValue = removeDLL(items->stackItem, 0);	// Calls removeDLL method to pop items from stack
	sizeSTACK(items);					// Calls size method to correct stack size
	return poppedValue;					// Returns popped value
}

/* peek method */
/* This is the peek method that shows the top item on the stack. */

void *peekSTACK(STACK *items) {	
	assert(items->size > 0);
	void *top = getDLL(items->stackItem, 0);		// Stores top value on stack and returns value
	return top;
}

/* stack size */
/* This is the method that returns the stack size. */

int sizeSTACK(STACK *items) {
	items->size = sizeDLL(items->stackItem);		// Calls sizeDLL method to get stack size

	return items->size;
}

/* display method */
/* This method displays the values in the stack, 
along with an indication of how much space is left
in the stack. */

void displaySTACK(STACK *items, FILE *fp) {
	int i;

	fprintf(fp, "|");

	for (i = 0; i < items->size; ++i) {			// Displays stack values using user display method
		items->display(getDLL(items->stackItem, i), fp);
	
		if ((items->size > 1) && (i != items->size - 1)) {
			fprintf(fp, ",");
		}
	}

	fprintf(fp, "|");
	return;
}

/* displaySTACKdebug method */
/* This method displays the values in the stack
by calling the display method from the parent class. */

void displaySTACKdebug(STACK *items, FILE *fp) {
	displayDLLdebug(items->stackItem, fp);			// Calls displayDLLdebug method to display values in stack
	return;
}

/* freeSTACK method */
/* This method frees the stack's underlying
data structure and the stack object. */

void freeSTACK(STACK *items) {
	if (items->free != NULL) {
		while (items->size > 0) {
			void *value = pop(items);

			items->free(value);
		}
	}

	else {
		while (items->size > 0) {
			pop(items);
		}
	}

	freeDLL(items->stackItem);	
	
	free(items);

	return;
}
