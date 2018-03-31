/* Author: Alan Manning */
/* Class: CS201 Data Structures and Algorithms */
/* Assignment 0 */
/* Date: 25 January, 2018 */

/* This is the implementation file for the header file
'dll.h', which is used to create a stack 'class' for
generic data types.*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include "dll.h"
//#include <math.h>

/* dllNODE Definition */
typedef struct dllNODE DLLNODE;

struct dllNODE {
	void *value;						// DLL data
	DLLNODE *next;						// next pointer
	DLLNODE *previous;					// previous pointer
};

struct dll {
	int size;						// DLL size
	DLLNODE *head;						// head node
	DLLNODE *tail;						// tail node
	void (*display)(void *, FILE*);				// display field
	void (*free)(void *);					// free field
};

/*************** private interface ***************/
static DLLNODE *newNODE(void *);
static DLLNODE *search(int,DLLNODE *);
static DLLNODE *searchReverse(int,int,DLLNODE *);
static void insertLast(DLL *,DLLNODE *,DLLNODE *);
static void insertFirst(DLL *,DLLNODE *,DLLNODE *);
static void insertMiddle(DLLNODE *,DLLNODE *);

/*************** public interface ***************/

/* constructor */
/* This is the constructor for the DLL objects. */

DLL *newDLL(void (*d)(void *,FILE *),void (*f)(void *)) {
	DLL *items = malloc(sizeof(DLL));
	assert(items != 0);
	items->head = 0;
	items->tail = 0;
	items->size = 0;
	items->display = d;
	items->free = f;

	return items;
}

/* insert method */
/* This is the method used to insert items into the DLL. */ 
void insertDLL(DLL *items,int index,void *value) {
	DLLNODE *newNode = newNODE(value);
	DLLNODE *cur = 0;

	assert((index >= 0) && (index <= items->size));

	if (items->size == 0) {					// Empty list
		items->head = newNode;
		(items->head)->next = 0;
		(items->head)->previous = 0;
		items->tail = newNode;
	}

	else {							// Non-empty list
		if (index == items->size) {			// Insert tail
			cur = search(index - 1, items->head);
			insertLast(items, newNode, cur);
		}	

		else if (index == 0) {				// Insert head
			cur = search(index - 1, items->head);
			insertFirst(items, newNode, cur);
		}

		else {
			if (items->size / index < 2) {
		        	cur = searchReverse(index - 1, items->size - 1, items->tail);		
			}

			else {					// index near head
				cur = search(index - 1, items->head);
			}

			insertMiddle(newNode, cur);		// Insert middle
		}
	}	
	
	++items->size;

	return;
}

/* remove method */
/* This is the method used to remove items from the DLL. */
void *removeDLL(DLL *items,int index) {
	int i = 0;
	DLLNODE *cur = items->head;
	DLLNODE *oldNode = 0;
	void *old = 0;

	assert(items->size > 0);
	assert((index >= 0) && (index < items->size));

	if (index == 0) {					// Remove head

		old = (items->head)->value;
		oldNode = items->head;
		items->head = (items->head)->next;
		
		if (items->size == 1) {				// One node
			items->tail = 0;
		}

		else {
			(items->head)->previous = 0;
		}
	}

	else {							// Remove elsewhere
		if (items->size / index < 2) {
			cur = searchReverse(index, items->size - 1, items->tail);
		}
		
		else {
			while (i < index) {
				++i;
				cur = cur->next;
			}
		}
			old = cur->value;
			oldNode = cur;
			(cur->previous)->next = cur->next;

			if (cur->next == 0) {				// Remove tail
				items->tail = cur->previous;
			}
		
			else {
				(cur->next)->previous = cur->previous;	// Remove middle
			}
	}

	free(oldNode);

	--items->size;

	return old;
}

/* union method */
/* This is the method used to union two DLL objects. */
void unionDLL(DLL *recipient,DLL *donor) {
	int sizeDonor = sizeDLL(donor);
	int sizeRecip = sizeDLL(recipient);

	if (sizeDonor == 0) {
		return;
	}

	else if (sizeRecip == 0) {
		recipient->head = donor->head;
		recipient->tail = donor->tail;
		recipient->size = donor->size;

		donor->head = donor->tail = 0;
		donor->size = 0;

		return;
	}

	(recipient->tail)->next = donor->head;
	(donor->head)->previous = recipient->tail;
	recipient->tail = donor->tail;
	recipient->size += donor->size;
	
	donor->head = donor->tail = 0;
	donor->size = 0;

	return;
}

/* accessor */
/* This is the accessor for the DLL class. */
void *getDLL(DLL *items,int index) {
	DLLNODE *cur = 0;

	assert((index >= 0) && (index < items->size));

	if (index == items->size - 1) {
		return (items->tail)->value;
	}

	if (index == 0) {
		return (items->head)->value;
	}

	if (items->size / index < 2) {
		cur = searchReverse(index, items->size - 1, items->tail);
	}

	else {
		cur = search(index, items->head);
	}

	return cur->value;					// Value not found
}

/* mutator */
/* This is the mutator for the DLL class. */
void *setDLL(DLL *items,int index,void *value) {
	DLLNODE *cur = 0;

	assert((index >= 0) && (index <= items->size));

	if (index == items->size) {				// Set after tail
		insertDLL(items, index, value);

		return NULL;
	}

	else if (index == items->size - 1) {
		void *old = (items->tail)->value;

		(items->tail)->value = value;

		return old;
	}

	else if (index == 0) {
		cur = items->head;

		void *old = cur->value;

		cur->value = value;

		return old;
	}

	if (items->size / index < 2) {
		cur = searchReverse(index, items->size - 1, items->tail);
	}

	else {
		cur = search(index, items->head);
	}

	void *old = cur->value;					// Store old value

	cur->value = value;					// Set new value

	return old;
}

/* DLL size */
/* This method returns the size of the DLL. */
int sizeDLL(DLL *items) {
	return items->size;
}


/* display method */
/* This method displays the DLL using the passed
display method. */
void displayDLL(DLL *items,FILE *fp) {
	DLLNODE *cur = items->head;

	fprintf(fp, "{{");

	while (cur) {						// Traverse list
		items->display(cur->value, fp);			// Display nodes

		if ((items->size > 1) && (cur->next)) {
			fprintf(fp, ",");
		}

		cur = cur->next;				// Update pointer
	}

	fprintf(fp, "}}");
	return;
}

/* debugged display method */
/* This method displays the DLL using the passed 
display method in the correct format for a DLL. */
void displayDLLdebug(DLL *items,FILE *fp) {
	DLLNODE *cur = items->head;

	fprintf(fp, "head->{{");					// Print head

	while (cur) {						// Traverse list
		items->display(cur->value, fp);			// Display nodes

		if ((items->size > 1) && (cur->next)) {
			fprintf(fp, ",");
		}

		cur = cur->next;				// Update pointer
	}

	fprintf(fp, "}},tail->{{");				// Print tail

	if (items->tail) {
		items->display((items->tail)->value, fp);
	}

	fprintf(fp, "}}");

	return;
}

/* free method */
/* This method traverses the DLL and frees
the generic values, as well as the nodes that hold. 
If null is returned, the generic value isn't found. */
void freeDLL(DLL *items) {
	if (items->free != NULL) {
		while (items->size > 0) {
			void *value = removeDLL(items, 0);
	
			items->free(value);
		}
	}

	else {
		while (items->size > 0) {
			removeDLL(items, 0);
		}
	}

	free(items);
	
	return;
}

/* DLLNODE constructor */
/* This is the constructor for the DLLNODE object. */
static DLLNODE *newNODE(void *val) {
	DLLNODE *node = malloc(sizeof(DLLNODE));

	assert(node != 0);
	node->value = val;
	node->next = 0;
	node->previous = 0;

	return node;
}

/* search method */
/* This method searches for a given node in a
DLL object.*/
static DLLNODE *search(int index,DLLNODE *node) {
	int i = 0;
	
	while (i < index) {
        	++i;
		node = node->next;
	}

	return node;
}

/* search reverse method */
/* Searches for a given node in reverse in a
DLL object.*/
static DLLNODE *searchReverse(int index,int size,DLLNODE *node) {
	int i = 0;

	while (i < size - index) {
		++i;	
		node = node->previous;
	}

	return node;
}

/* insertLast method */
/* Inserts at the tail of the DLL. */
static void insertLast(DLL *items,DLLNODE *newNode,DLLNODE *current) {
	newNode->next = current->next;
	newNode->previous = current;
                         
	if (current->next == 0) {
        	items->tail = newNode;
        }
 
        else {
        	(current->next)->previous = newNode;
        }
               
        current->next = newNode;

	return;
}

/* insertFirst method */
/* Inserts at the head of the DLL. */
static void insertFirst(DLL *items,DLLNODE *newNode,DLLNODE *current) {
	newNode->next = current;
        newNode->previous = current->previous;
                         
        if (current->previous == 0) {
        	items->head = newNode;
        }
                         
        else {
        	(current->previous)->next = newNode;
        }
                  
        current->previous = newNode;

	return;
}

/* insertMiddle method */
/* Inserts in the middle of the DLL. */
static void insertMiddle(DLLNODE *newNode,DLLNODE *current) {
	newNode->next = current->next;             
	newNode->previous = current;
	(current->next)->previous = newNode;
        current->next = newNode;

	return;
}
