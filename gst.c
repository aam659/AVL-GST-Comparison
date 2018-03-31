/* Author: Alan Manning*/
/* Class: CS201 Data Structures and Algorithms */
/* Assignment 2 */
/* Date: 3/8/2018 */

/* This is the implementation file for the header file "gst.h",
which is used to create a green binary search tree 'class' for generic data types.*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <stdbool.h>
#include "bst.h"
#include "gst.h"

// NODE Definition
typedef struct GSTV GSTV;

struct GSTV {
	int count;						// duplicate count
	void *value;						// GSTV value
	void (*disp)(void *,FILE *);				// display method
	int (*compare)(void *,void *);				// compare method
	void (*free)(void *);					// free method
};

// gst struct definition

struct gst {
	void (*disp)(void *, FILE *);				// display field
	int (*compare)(void *, void *);				// compare field
	void (*free)(void *);					// free method
	int size;						// tree size
	BST *binTree;						// Associated bst
};

/*************** private interface ***************/
static BST *getBST(GST *);
static GSTV *newGSTV(
	void (*)(void *,FILE *),
	int (*)(void *,void *),
	void (*)(void *),
	void *);
static void setGSTVvalue(GSTV *,void *);
static void *getGSTVvalue(GSTV *);
static void setGSTVcount(GSTV *,int);
static int getGSTVcount(GSTV *);
static void increaseGSTVcount(GSTV *);
static void decreaseGSTVcount(GSTV *);
static void setGSTsize(GST *,int);
static void displayGSTVvalue(void *,FILE *);
static int compareGSTV(void *,void *);
static void swapper(BSTNODE *,BSTNODE *);
static void freeGSTV(void *);
static int sizeGSTactual(GST *);

/*************** GST public definitions ***************/

/* GST Constructor */

GST *newGST(
	void (*display)(void *, FILE *),
	int (*comparator)(void *, void *),
	void (*freer)(void *)) {
	GST *tree = malloc(sizeof(GST));			// Allocate space for GST object

	assert(tree != 0);

	tree->size = 0;						// Set GST size to 0
	tree->disp = display;					// disp field = display method
	tree->compare = comparator;				// compare field = comparator method
	tree->free = freer;					// free field = freer method
								// binTree field = newBST
	tree->binTree = newBST(displayGSTVvalue, compareGSTV, swapper, freeGSTV);

	return tree; 
}

/* insertGST Method */
/* Calls underlying insert method for BST. */

void insertGST(GST *t, void *v) {
	int count = 0;
	BST *tree = getBST(t);
	int bstSize = sizeBST(tree);
	int gstSize = sizeGSTactual(t);
	BSTNODE *result = NULL;
	BSTNODE *r = NULL;
	GSTV *g = NULL;
	GSTV *searchG = newGSTV(t->disp, t->compare, t->free, v);
	GSTV *foundG = NULL;
	//void *value = NULL;
	
	count = findGSTcount(t, v);				// Determine count
	//value = findGST(t, v);				// Determine value

	if (count == 0) {
		g = newGSTV(t->disp, t->compare, t->free, v);
		increaseGSTVcount(g);				// Update count

		insertBST(tree, g);				// Insert GSTV into BST

		// TODO: MAKE SURE LINES 105-109 ARE OK
		if (bstSize == 0) {				// Root inserted
			r = getBSTroot(tree);
			setBSTNODEparent(r, r);			// Root is own parent
			bstSize = sizeBST(tree);
		}
	}

	else {
		result = findBST(tree, searchG);		// Find BSTNODE with GSTV

		foundG = getBSTNODEvalue(result);		// Get GSTV node

		increaseGSTVcount(foundG);			// Update count
	}

	setGSTsize(t, gstSize + 1);				// Update GST size

	free(searchG);

	return;
}

/* findGSTcount Method */
/* Returns frequency of searched value; 
   else NULL. */

int findGSTcount(GST *t, void *v) {
	int count = 0;
	BSTNODE *result = NULL;
	BST *tree = getBST(t);
	GSTV *g = NULL;
	GSTV *searchG = newGSTV(t->disp, t->compare, t->free, v);

	result = findBST(tree, searchG);			// Find correct BSTNODE
	
	if (result == NULL) {
		free(searchG);					// Free extra node
		return 0;
	}

	g = getBSTNODEvalue(result);				// Get GSTV value

	count = getGSTVcount(g);				// Update count

//	freeGSTV(searchG);					// Free extra node
	free(searchG);
	
	return count;
}

/* findGST Method */
/* Returns searched-for value; else NULL. */

void *findGST(GST *t, void *v) {
	BST *tree = getBST(t);
	BSTNODE *result = NULL;
	GSTV *g = NULL;
	GSTV *searchG = newGSTV(t->disp, t->compare, t->free, v); 
	void *value = NULL;

	result = findBST(tree, searchG);			// Call findBST
	
	if (result == NULL) {
//		printf("Value ");
//		t->disp(getGSTVvalue(searchG), stdout);
//		printf(" not found.\n");
		free(searchG);					// Free extra node
		return NULL;					// Not found
	}

	g = getBSTNODEvalue(result);				// Get GSTV node

	value = getGSTVvalue(g);				// Get actual value

//	freeGSTV(searchG);					// Free extra node
	free(searchG);

	return value;
}

/* deleteGST Method */
/* Returns deleted value. If count = 1,
   node removed from tree. */

void *deleteGST(GST *t, void *v) {
	int count = 0;
	BST *tree = getBST(t);
	int gstSize = sizeGSTactual(t);
	//GSTV *g = NULL;
	GSTV *searchG = newGSTV(t->disp, t->compare, t->free, v);
	GSTV *foundG = NULL;
	BSTNODE *result = NULL;
	BSTNODE *deleted = NULL;
	GSTV *value = NULL;
	void *actualValue = NULL;
	
	count = findGSTcount(t, v);				// Determine count
	//value = findGST(t, v);				// Determine value
	result = findBST(tree, searchG);			// Find BSTNODE with GSTV

	if (result == NULL) {
//		printf("Value ");
//		t->disp(getGSTVvalue(searchG), stdout);
//		printf(" not found.\n");
		free(searchG);					// Free extra node
		return NULL;					// Node not in GST
	}

	foundG = getBSTNODEvalue(result);			// Get GSTV node

	if (count > 1) {					// Duplicates
		decreaseGSTVcount(foundG);			// Update count
		setGSTsize(t, gstSize - 1);
		free(searchG);					// Free extra node	
		return NULL;
	}

	decreaseGSTVcount(foundG);				// Update count
	deleted = deleteBST(tree, foundG);

	value = getBSTNODEvalue(deleted);			// Get GSTV node
	
	actualValue = getGSTVvalue(value);			// Get actual value

	setGSTsize(t, gstSize - 1);				// Update GST size

//	freeGSTV(searchG);					// Free extra node
	free(searchG);
	free(value);
	free(deleted);

	return actualValue;
}

/* sizeGST Method */
/* Returns size of GST. */

int sizeGST(GST *t) {
	BST *tree = getBST(t);
	int size = sizeBST(tree);

	return size;
}

/* duplicates Method */
/* Returns number of tree duplicates. */

int duplicates(GST *t) {
	BST *tree = getBST(t);

	int duplicates = sizeGSTactual(t) - sizeBST(tree);	// No. of duplicates

	return duplicates;
}

/* statisticsGST Method */
/* Displays number of duplicates and 
   BST tree statistics. */

void statisticsGST(GST *t, FILE *fp) {
	int repeats = duplicates(t);
	BST *tree = getBST(t);

	fprintf(fp, "Duplicates: %d\n", repeats);		// Prints duplicates
	statisticsBST(tree, fp);				// Prints BST stats
		
	return;
}

/* displayGST Method */
/* Calls underlying displayBSTdecorated method. */

void displayGST(GST *t, FILE *fp) {
	BST *tree = getBST(t);
	int size = sizeBST(tree);

	if (size > 0) {
		displayBSTdecorated(tree, fp);				// Call displayBSTdecorated
	}

	else {
		fprintf(fp, "EMPTY\n");
	}

	return;
}

/* displayGSTdebug Method */
/* Calls underlying displayBST method. */

void displayGSTdebug(GST *t, FILE *fp) {
	BST *tree = getBST(t);

	displayBST(tree, fp);					// Call underlying display

	return;
}

/* freeGST Method */
/* Frees GST. */

void freeGST(GST *t) {
	BST *tree = getBST(t);

	freeBST(tree);						// Free BST and GSTV nodes

	free(t);						// Free GST

	return;
}

/*************** Private Method Definitions ***************/

/* GSTV Node Constructor */

static GSTV *newGSTV(
	void (*d)(void *,FILE *),
	int (*c)(void *,void *),
	void (*f)(void *),
	void *value) {
	GSTV *gstValue = malloc(sizeof(GSTV));			// Allocate space

	assert(gstValue != NULL);

	gstValue->value = value;	
	gstValue->count = 0;					// Initialize count
	gstValue->disp = d;					// disp field = display method
	gstValue->compare = c;					// compare field = compare method
	gstValue->free = f;					// free field = free method

	return gstValue;
}

/* getBST Method */
/* Returns GST's BST field. */

static BST *getBST(GST *t) {
	BST *tree = t->binTree;

	if (tree == NULL) return NULL;

	return tree;	
}

/* compareGSTV Method */
/* Custom compare method for GSTV node. */

static int compareGSTV(void *a, void *b) {
	GSTV *x = a;						// Values are GSTV nodes
	GSTV *y = b;

	void *p = getGSTVvalue(x);				// Get GSTV value fields
	void *q = getGSTVvalue(y);

	return x->compare(p, q);				// Use GSTV compare
}

/* swapper Method */
/* Custom swapper method for GSTV node. */

static void swapper(BSTNODE *a, BSTNODE *b) {
	GSTV *ta = getBSTNODEvalue(a);				// GSTV nodes to be swapped
	GSTV *tb = getBSTNODEvalue(b);

	// swap the values stored in the GSTV objects

	void *vtemp = getGSTVvalue(ta);
	void *bValue = getGSTVvalue(tb);
	setGSTVvalue(ta, bValue);
	setGSTVvalue(tb, vtemp);

	// swap the counts stored in the GST value objects
	int ctemp = getGSTVcount(ta);
	int bCount = getGSTVcount(tb);
	setGSTVcount(ta, bCount);
	setGSTVcount(tb, ctemp);

	return;
}

/* displayGSTVvalue Method */
/* Custom display method for GSTV node. */

static void displayGSTVvalue(void *v, FILE *fp) {
	GSTV *w = v;						// Value is GSTV node
	void *value = getGSTVvalue(w);
	int count = getGSTVcount(w);
	w->disp(value, fp);					// Use GSTV display
	
	if (count > 1) {
		fprintf(fp, "[%d]", count);			// Show GSTV count
	}

	return;
}

/* freeGSTV Method */
/* Frees GSTV node and value. */

static void freeGSTV(void *v) {
	GSTV *oldGSTV = v;
	void *old = getGSTVvalue(oldGSTV);
	
	if (oldGSTV->free != NULL) {
		oldGSTV->free(old);				// Free node value
	}
	
	free(oldGSTV);						// Free GSTV node

	return;
}

/* setGSTVvalue Method */
/* Sets GSTV value. */

static void setGSTVvalue(GSTV *g, void *v) {
	g->value = v;

	return;
}

/* getGSTVvalue Method */
/* Returns GSTV value. */

static void *getGSTVvalue(GSTV *g) {
	return g->value;
}

/* setGSTVcount Method */
/* Updates GSTV count. */

static void setGSTVcount(GSTV *g, int c) {
	g->count = c;

	return;
}

/* getGSTVcount Method */
/* Returns GSTV count. */

static int getGSTVcount(GSTV *g) {
	return g->count;
}

/* increaseGSTVcount Method */
/* Increases GSTV count. */

static void increaseGSTVcount(GSTV *g) {
	int count = getGSTVcount(g);

	setGSTVcount(g, count + 1);

	return;
}

/* decreaseGSTVcount Method */
/* Decreases GSTV count. */

static void decreaseGSTVcount(GSTV *g) {
	int count = getGSTVcount(g);

	setGSTVcount(g, count - 1);

	return;
}

/* setGSTsize Method */
/* Updates GST size. */

static void setGSTsize(GST *t, int size) {
	t->size = size;

	return;
}

static int sizeGSTactual(GST *t) {
	return t->size;
}
