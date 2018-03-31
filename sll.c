/* Author: Alan Manning */
/* Class: CS201 Data Structures and Algorithms */
/* Assignment 0 */
/* Date: 25 January, 2018 */

/*This is the implementation file for the header file "dll.h",
which is used to create a singularly-linked list 'class'
for generic data types. */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include "sll.h"

/* sllNODE Definition */
typedef struct sllNODE SLLNODE;

struct sllNODE {
	void *value;						// SLL data	
	SLLNODE *next;						// next pointer
};

struct sll {
	int size;						// SLL size
	SLLNODE *head;						// head node
	SLLNODE *tail;						// tail node	
	void (*display)(void *, FILE *);			// display field
	void (*free)(void *);					// free field
};

/*************** private interface ***************/
static SLLNODE *newNODE(void *);
static SLLNODE *search(int,SLLNODE *);
static void insertHead(SLL *,SLLNODE *);
static void insertTail(SLL *,SLLNODE *,SLLNODE *);
static void insertMiddle(SLLNODE *,SLLNODE *);
static void *removeHead(SLL *);
static void *removeMiddle(SLL *,int); 

/*************** public interface ***************/ 

/* constructor */
/* This is the constructor that initializes
the fields for the given struct SLL. */
SLL *newSLL(void (*d)(void *,FILE *),void (*f)(void *)) {
	SLL *items = malloc(sizeof(SLL));
	assert(items != 0);
	items->head = 0;
	items->tail = 0;
	items->size = 0;
	items->display = d;
	items->free = f;

	return items;
}

/* insert method */
/* This is the SLL insertion method. */
void insertSLL(SLL *items,int index,void *value) {
	SLLNODE *newNode = newNODE(value);
	SLLNODE *cur = 0;

	assert((index >= 0) && (index <= items->size));

	if (index == 0) {					// Insert at head
		insertHead(items, newNode);
	}

	else if (index == items->size) {
		insertTail(items, newNode, items->tail);
	}

	else {							// Insert elsewhere
	cur = search(index - 1, items->head);
		insertMiddle(newNode, cur);			// Insert in middle
	}

	++items->size;

	return;
}

/* remove method */
/* This is the SLL remove method. */
void *removeSLL(SLL *items,int index) {
	void *old = 0;

	assert(items->size > 0);
	assert((index >= 0) && (index < items->size));

	if (index == 0) {					// Remove head
		old = removeHead(items);
	}

	else {							// Remove other
		old = removeMiddle(items, index);
	}

	--items->size;

	return old;
}

/* union method */
/* This is the method that unions two SLL objects. */
void unionSLL(SLL *recipient,SLL *donor) {
	int sizeDonor = sizeSLL(donor);
	int sizeRecip = sizeSLL(recipient);

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
	recipient->tail = donor->tail;
	recipient->size += donor->size;

	donor->head = donor->tail = 0;
	donor->size = 0;

	return;
}

/* accessor */
/* This is the accessor for the class. */
void *getSLL(SLL *items,int index) {
	SLLNODE *cur = 0;

	assert((index >= 0) && (index < items->size));

	if (index == items->size - 1) {
		return (items->tail)->value;
	}

	cur = search(index, items->head);

	return cur->value;					// Value found
}

/* mutator */
/* This is the mutator for the class. */
void *setSLL(SLL *items,int index,void *value) {
	SLLNODE *cur = 0;

	assert((index >= 0) && (index <= items->size));

	if (index == items->size) {				// Set after tail
		insertSLL(items, index, value);

		return NULL;
	}

	else if (index == items->size - 1) {
		void *old = (items->tail)->value;

		(items->tail)->value = value;

		return old;
	}

	cur = search(index, items->head);

	void *old = cur->value;					// Store old value

	cur->value = value;					// Set new value

	return old;
}

/* array size */
/* This method gives the SLL size. */
int sizeSLL(SLL *items) {
	return items->size;
}

/* display method */
/* This method displays the given SLL 
using the given display method. */
void displaySLL(SLL *items,FILE *fp) {
	SLLNODE *cur = items->head;

	fprintf(fp, "{");

	while (cur) {						// Traverse list
		items->display(cur->value, fp);			// Display nodes
		
		if ((items->size > 1) && (cur->next)) {
			fprintf(fp, ",");
		}
		
		cur = cur->next;				// Update pointer
	}

	fprintf(fp, "}");
	return;
}

/* debugged display method */
/* This method displays the given SLL
using the given display method
in the proper format for a SLL. */
void displaySLLdebug(SLL *items,FILE *fp) {
	SLLNODE *cur = items->head;

	fprintf(fp, "head->{");					// Print head
	
	while (cur) {						// Traverse list
		items->display(cur->value, fp);			// Display nodes

		if ((items->size > 1) && (cur->next)) {
			fprintf(fp, ",");
		}

		cur = cur->next;				// Update pointer
	}

	fprintf(fp, "},tail->{");				// Print tail

	if (items->tail) {
		items->display((items->tail)->value, fp);
	}

	fprintf(fp, "}");

	return;
}

/* free method */
/* This method traverses the SLL and frees
the generic values, as well as the nodes that hold. 
If null is returned, the generic value isn't found. */
void freeSLL(SLL *items) {
	if (items->free != NULL) {
		while (items->size > 0) {
			void *value = removeSLL(items, 0);
		
			items->free(value);
		}

	}

	else {
		while (items->size > 0) {
			removeSLL(items, 0);
		}
	}

	free(items);

	return;
}

/* SLLNODE constructor */
/* This is the constructor for the SLLNODE object. */
static SLLNODE *newNODE(void *val) {
	SLLNODE *node = malloc(sizeof(SLLNODE));
	assert(node != 0);
	node->value = val;
	node->next = 0;

	return node;
}

/* search method */
/* This is the method used for searching for a 
given SLLNODE object in a SLL. */
static SLLNODE *search(int index,SLLNODE *node) {
	int i = 0;
	while (i < index) {
        	++i;
        	node = node->next;
        }

	return node;
}

/* insertHead method */
/* Inserts at the head of a SLL. */
static void insertHead(SLL *items,SLLNODE *newNode) {
	if (items->size == 0) {
	        items->tail = newNode;
        }
 
        newNode->next = items->head;                    // Update head
	items->head = newNode;
	return;
}

/* insertTail method */
/* Inserts at the tail of a SLL. */
static void insertTail(SLL *items,SLLNODE *newNode,SLLNODE *current) {
	current->next = newNode;
	items->tail = newNode;
	newNode->next = 0;

	return;
}

/* insertMiddle method */
/* Inserts in the middle of a SLL. */
static void insertMiddle(SLLNODE *newNode,SLLNODE *current) {
	newNode->next = current->next;
        current->next = newNode;

	return;
}

/* removeHead method */
/* Removes the head of a SLL. */
static void *removeHead(SLL *items) {
	SLLNODE *oldNode = 0;
	void *old = 0;

	old = (items->head)->value;
        oldNode = items->head;
        items->head = (items->head)->next;
 
        if (items->size == 1) {                         // One node
        	items->tail = 0;
        }

	free(oldNode);

	return old;
}

/* removeMiddle method */
/* Removes something other than
the head of a SLL. */
static void *removeMiddle(SLL *items,int index) {
	SLLNODE *oldNode = 0;
	SLLNODE *former = 0;
	void *old = 0;

	former = search(index - 1, items->head);
        old = (former->next)->value;
        oldNode = former->next;
        former->next = (former->next)->next;                  // Remove in middle
        
	if (former->next == 0) { 
        	items->tail = former;
        }

	free(oldNode);

	return old;
}
