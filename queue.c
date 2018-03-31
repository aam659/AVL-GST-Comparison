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
#include "queue.h"
#include "sll.h"

struct queue {						
	void (*display)(void *, FILE *);			// display field
	void (*free)(void *);					// free field
	SLL *queueItem;						// SLL queue items
	int size;						// Queue size
};

/*************** public interface ***************/

/* constructor */
/* This is the constructor for the queue. */

QUEUE *newQUEUE(void (*d)(void *,FILE *),void (*f)(void *)) {
	QUEUE *items = malloc(sizeof(QUEUE));			// Allocate space for queue
	assert(items != 0);

	items->queueItem = newSLL(d, f);			// Set queue items to SLL object
	items->size = 0;					// Set size of queue to 0
	items->display = d;					// Set display method for queue to that given by user
	items->free = f;					// Set free field to passed free method

	return items;
}

/* enqueue method */
/* This is the enqueue method for the queue. */

void enqueue(QUEUE *items,void *value) {
	insertSLL(items->queueItem, sizeSLL(items->queueItem), value);			// Calls insertSLL method to enqueue
	sizeQUEUE(items);					// Corrects size for queue
	return;
}

/* dequeue method */
/* This is the deque method for the queue. */

void *dequeue(QUEUE *items) {
	assert(items->size > 0);				// Calls removeSLL method
	void *dequeuedValue = removeSLL(items->queueItem, 0);
	sizeQUEUE(items);					// Corrects size for queue
	return dequeuedValue;					// Returns dequeued value
}

/* peek method */
/* This method shows the top of the queue. */

void *peekQUEUE(QUEUE *items) {
	assert(items->size > 0);
	void *top = getSLL(items->queueItem, 0);	// Stores top of queue and returns it
	return top;
}

/* queue size */
/* This method returns the size of the queue. */

int sizeQUEUE(QUEUE *items) {
	items->size = sizeSLL(items->queueItem);		// Calls sizeDLL method to update size of queue
	
	return items->size;
}

/* display method */
/* This method displays the items in the queue,
along with an indication of how much space is left
in the queue. */

void displayQUEUE(QUEUE *items, FILE *fp) {
	int i;
	
	fprintf(fp, "<");

	for (i = 0; i < items->size; ++i) {
		void *index = getSLL(items->queueItem, i);
		items->display(index, fp);			// Displays queue items using given display method
		if ((items->size > 1) && (i != items->size - 1)) {
			fprintf(fp, ",");
		}
	}

	fprintf(fp, ">");
	return;
}

/* displayQUEUEdebug method */
/* This method displays the items in the queue
by calling the display method from the parent class. */
void displayQUEUEdebug(QUEUE *items, FILE *fp) {
	displaySLLdebug(items->queueItem, fp);			// Calls displaySLLdebug method to display queue items
	return;
}

/* freeQUEUE method */
/* This method frees the queue's underlying
 data structure and the stack object.  */
void freeQUEUE(QUEUE *items) {
	if (items->free != NULL) {
		while (items->size > 0) {
			void *value = dequeue(items);

			items->free(value);
		}
	}

	else {
		while (items->size > 0) {
			dequeue(items);
		}
	}

	freeSLL(items->queueItem);

	free(items);

	return;
}
