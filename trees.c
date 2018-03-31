/* Author: Alan Manning */
/* Class: CS201 Data Structures and Algorithms */
/* Assignment 2 */
/* Date 12 March, 2018 */

/* This is the main file for implementing trees.c, which
reads in a corpus of text and compares the performance
of green binary search trees and AVL trees.  */

// TREES.C 

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include "stack.h"
#include "queue.h"
#include "bst.h"
#include "integer.h"
#include "real.h"
#include "string.h"
#include "scanner.h"
#include "gst.h"
#include "avl.h"

/* option flags */
int Author = 0;							// option -v
int treeType = 1;						// tree type
int treeTypeBoth = 0;						// both trees
char* Name = 0;							// File name

#define AVLTREE 1						// AVL macro
#define GREENSEARCHTREE 2					// GST macro

/*************** Private Methods ***************/
static int processOptions(int,char **);
static void processCommands(FILE *,AVL *,GST *);
void Fatal(char *,...);
//static void (*reader)(FILE *,HEAP *);				// reader function pointer
//static int (*compare)(void *,void *);				// compare function pointer
//static void (*display)(void *,FILE *);				// display function pointer
//static void (*release)(void *);					// free function pointer
//static void (*reader)(FILE *,HEAP *);
//static void readReals(FILE *,HEAP *);
static AVL *readStringsAndTokensA(FILE *);
static GST *readStringsAndTokensG(FILE *);
static char *cleanPhrase(char *);
static char *cleanWord(char *);
//static void readIntegers(FILE *,HEAP *);

/*************** Main ***************/

int main(int argc, char **argv) {
	AVL *tree = NULL;
	GST *tree2 = NULL;	
/*	QUEUE *final = 0;					// readIn QUEUE
	STACK *readOut = 0;					// readOut STACK
	HEAP *heap = 0;						// HEAP
	int x = 0;
	double y = 0;
	char *z;*/
	
	if (argc == 1) Fatal("%d arguments!\n", argc-1);	// Error argc=1

	if (argc < 2) {
		fprintf(stderr, "usage: %s\n", argv[0]);	// Error argc<2
		exit(1);
	}

	processOptions(argc,argv);

	if ((argc == 3) && !(Author)) {				// default AVL
		FILE *fp = fopen(Name, "r");

		if (fp == NULL) {
			fprintf(stderr, "Error. File %s does not exist.\n", Name);
			exit(1);
		}
		
		tree = readStringsAndTokensA(fp);
//		tree2 = readStringsAndTokensG(fp);	

		fclose(fp);

		FILE *fp1 = fopen(argv[2], "r");

		if (fp1 == NULL) {
			fprintf(stderr, "Error. File %s does not exist.\n", argv[2]);
			exit(1);
		}

		processCommands(fp1, tree, NULL);

		fclose(fp1);


/*		displayGST(tree2, stdout);
		statisticsGST(tree2, stdout);*/

		return 0;
	}

	else if (((argc == 4) || (argc == 5)) && !(Author)) {
		FILE *fp = fopen(Name, "r");

		if (fp == NULL) {
			fprintf(stderr, "Error. File %s does not exist.\n", Name);
			exit(1);
		}
	
		if ((treeType == GREENSEARCHTREE) && (argc == 4)) {	
								// Read into GST
			tree2 = readStringsAndTokensG(fp);
			FILE *fp2 = NULL;
			fclose(fp);

			fp2 = fopen(argv[3], "r");

			if (fp2 == NULL) {
				fprintf(stderr, "Error. File %s does not exist.\n", argv[3]);
				exit(1);
			}

			processCommands(fp2, NULL, tree2);

			fclose(fp2);
		}

		else if ((treeType == AVLTREE) && (argc == 4)) {
								// Read into AVL
			tree = readStringsAndTokensA(fp);
			FILE *fp3 = NULL;
			fclose(fp);

			fp3 = fopen(argv[3], "r");

			if (fp3 == NULL) {
				fprintf(stderr, "Error. File %s does not exist.\n", argv[3]);
				exit(1);
			}

			processCommands(fp3, tree, NULL);

			fclose(fp3);
		}

		else if ((treeTypeBoth == 2) && (argc == 5)) {
			tree = readStringsAndTokensA(fp);
			
			rewind(fp);	
	
			tree2 = readStringsAndTokensG(fp);
			
			fclose(fp);

			FILE *fp4 = NULL;

			fp4 = fopen(argv[4], "r");

			if (fp4 == NULL) {
				fprintf(stderr, "Error. File %s does not exist.\n", argv[4]);
				exit(1);
			}

			processCommands(fp4, tree, NULL);

			rewind(fp4);

			processCommands(fp4, NULL, tree2);
	
			fclose(fp4);
		}
	}

	// Print following if appropriate flags are set

	if (Author) {
		printf("Alan Manning\n");
		exit(0);
	}

	return 0;
}

void Fatal(char *fmt, ...) {
	va_list ap;

	fprintf(stderr, "An error occured: ");
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	exit(-1);

	return;
}

/* only -oXXX  or -o XXX options */

static int processOptions(int argc, char **argv) {
        int argIndex;
        int argUsed;
        int separateArg;
        int printArg;

        argIndex = 1;

        while (argIndex < argc && *argv[argIndex] == '-') {
                /* check if stdin, represented by "-" is an argument */
                /* if so, the end of options has been reached */
                if (argv[argIndex][1] == '\0') {
                        return argIndex;
                }

                separateArg = 0;
                argUsed = 0;
                printArg = 0;

                if (argv[argIndex][2] == '\0') {
                        separateArg = 1;
                }

                switch (argv[argIndex][1]) {
                        /*
                        * when option has an argument, do this
                        *
                        *     examples are -m4096 or -m 4096
                        *
                        *     case 'm':
                        *         MemorySize = atol(arg);
                        *         argUsed = 1;
                        *         break;
                        *
                        *
                        * when option does not have an argument, do this
                        *
                        *     example is -a
                        *
                        *     case 'a':
                        *         PrintActions = 1;
                        *         break;
                        */
			case 'r':
				if ((treeType == AVLTREE) && (treeTypeBoth == 1)) {
					fprintf(stderr, "Error. Option -r set twice.\n");
					exit(1);
				}
				treeType = AVLTREE;
				++treeTypeBoth;
				argUsed = 1;
				break;
			case 'g':
				if ((treeType == GREENSEARCHTREE) && (treeTypeBoth == 1)) {
					fprintf(stderr, "Error. Option -g set twice.\n");
					exit(1);
				}
				treeType = GREENSEARCHTREE;
				++treeTypeBoth;
				argUsed = 1;
				break;
                        case 'v':
                                Author = 1;
                                printArg = 1;
                                break;
                        default:
                                Fatal("option %s not understood\n",argv[argIndex]);
                }

                if (printArg) {
                        ++argIndex;
                }

                if (separateArg && argUsed) {
                        ++argIndex;
                }

        }

        if (argIndex == argc) {
                Name = malloc(sizeof(char) * strlen(argv[argIndex - 1]));
                strcpy(Name, argv[argIndex - 1]);
        }

        else {
                Name = malloc(sizeof(char) *strlen(argv[argIndex]));
                strcpy(Name, argv[argIndex]);
        }

        return argIndex;
}

static void processCommands(FILE *fp, AVL *at, GST *gt) {
	char *x = 0;
	int count = 0;
	int freq = 0;

	if (gt == NULL) {					// AVL case
		if (stringPending(fp)) {
			x = readString(fp);			// Phrase case
			x = cleanPhrase(x);
		}

		else {
			x = readToken(fp);			// Word case
			x = cleanWord(x);
		}

		while (!feof(fp)) {
			if (x[0] != '\0') {
				if (((x[0] == 's') || (x[0] == 'r') || (x[0] == 'v')) && (count % 2 == 0)) {
					switch(x[0]) {
						case 's':
							displayAVL(at, stdout);
							break;
						case 'r':
							statisticsAVL(at, stdout);
							break;
						default:
							printf("Alan Manning\n");
							exit(0);
					}
					
					count += 2;
				}

				else {
					char temp = x[0];

					if (stringPending(fp)) {
						x = readString(fp);
						x = cleanPhrase(x);
					}

					else {
						x = readToken(fp);
						x = cleanWord(x);
					}
					
					++count;
					if (x[0] != '\0') {
						switch(temp) {
							case 'i':
								insertAVL(at, newSTRING(x));
								break;
							case 'd':
								if (findAVL(at, newSTRING(x)) != NULL) {
									deleteAVL(at, newSTRING(x));
								}

								else {
									printf("Value %s not found.\n", x);
								}
								break;
							case 'f':
								freq = findAVLcount(at, newSTRING(x));
								printf("Frequency of %s: %d\n", x, freq);
								break;
							default:
								printf("Command not found.\n");
								exit(0);
						}
					}
				
					++count;	
				}
			}
	
			if ((stringPending(fp)) && !feof(fp)) {
				x = readString(fp);
				x = cleanPhrase(x);
			}
	
			else if (!feof(fp)) {
				x = readToken(fp);
				x = cleanWord(x);
			}
		}

		freeAVL(at);
	}

	else if (at == NULL) {					// GST case
		if (stringPending(fp)) {
			x = readString(fp);			// Phrase case
			x = cleanPhrase(x);
		}

		else {
			x = readToken(fp);			// Word case
			x = cleanWord(x);
		}

		while (!feof(fp)) {
			if (x[0] != '\0') {
				if (((x[0] == 's') || (x[0] == 'r') || (x[0] == 'v')) && (count % 2 == 0)) {
					switch(x[0]) {
						case 's':
							displayGST(gt, stdout);
							break;
						case 'r':
							statisticsGST(gt, stdout);
							break;
						default:
							printf("Alan Manning\n");
							exit(0);
					}
					
					count += 2;
				}

				else {
					char temp = x[0];

					if (stringPending(fp)) {
						x = readString(fp);
						x = cleanPhrase(x);
					}

					else {
						x = readToken(fp);
						x = cleanWord(x);
					}
					
					++count;
					if (x[0] != '\0') {
						switch(temp) {
							case 'i':
								insertGST(gt, newSTRING(x));
								break;
							case 'd':
								if (findGST(gt, newSTRING(x)) != NULL) {
									deleteGST(gt, newSTRING(x));
								}

								else {
									printf("Value %s not found.\n", x);
								}
								break;
							case 'f':
								freq = findGSTcount(gt, newSTRING(x));
								printf("Frequency of %s: %d\n", x, freq);
								break;
							default:
								printf("Command not found.\n");
								exit(0);
						}
					}
				
					++count;	
				}
			}
	
			if ((stringPending(fp)) && !feof(fp)) {
				x = readString(fp);
				x = cleanPhrase(x);
			}
	
			else if (!feof(fp)) {
				x = readToken(fp);
				x = cleanWord(x);
			}
		}
	
		freeGST(gt);
	}	

	return;
}

/* readStringsAndTokensA Method */
/* Reads strings or tokens from a given fp to an AVL tree. */

static AVL *readStringsAndTokensA(FILE *fp) {
	char *x = 0;
	AVL *tree = newAVL(displaySTRING, compareSTRING, freeSTRING);


	if (stringPending(fp)) {				// Phrase case
		x = readString(fp);
		x = cleanPhrase(x);	
	}
	
	else {							// Word	case
		x = readToken(fp);
		x = cleanWord(x);
	}

	while (!feof(fp)) {
		if (x[0] != '\0') {
			insertAVL(tree, newSTRING(x));		// Insert to AVL
		}

		if ((stringPending(fp)) && !feof(fp)) {
			x = readString(fp);
			x = cleanPhrase(x);
		}
	
		else if (!feof(fp)) {
			x = readToken(fp);
			x = cleanWord(x);
		}
	}

	return tree;
}

/* readStringsAndTokensG Method */
/* Reads strings or tokens from a given fp to a GST tree. */

static GST *readStringsAndTokensG(FILE *fp) {
	char *x = 0;
	GST *tree = newGST(displaySTRING, compareSTRING, freeSTRING);

	if (stringPending(fp)) {				// Phrase case
		x = readString(fp);
		x = cleanPhrase(x);	
	}
	
	else {							// Word case
		x = readToken(fp);
		x = cleanWord(x);
	}

	while (!feof(fp)) {
		if (x[0] != '\0') {
			insertGST(tree, newSTRING(x));		// Insert to GST
		}

		if ((stringPending(fp)) && !feof(fp)) {
			x = readString(fp);
			x = cleanPhrase(x);
		}
	
		else if (!feof(fp)) {
			x = readToken(fp);
			x = cleanWord(x);
		}
	}	

	return tree;
}

static char *cleanPhrase(char *x) {
	int length = strlen(x);
	char charArray[length];
	strcpy(charArray, x);
	char cleaned[length];
	int i = 0;
	int j = 0;

	// Skips leading whitespace
	while (isspace(charArray[i])) ++i;

	while (i < length) {
		if (isalpha(charArray[i])) {
			cleaned[j] = tolower(charArray[i]);
			++j;
		}

		else if (isspace(charArray[i]) && (j >= 1)) {
			if (!(isspace(cleaned[j - 1])) && (charArray[i + 1] != 0)) {
				cleaned[j] = ' ';
				++j;
			}
		}

		++i;
	}

	if (cleaned[j - 1] == ' ') --j;

	cleaned[j] = 0;

	int length2 = j;

	char *z = malloc(sizeof(char *) * length2);

	strcpy(z, cleaned);

	return z;
}

static char *cleanWord(char *x) {
	int length = strlen(x); 
	char charArray[length];
	strcpy(charArray, x);
	char cleaned[length];
	int i = 0;
	int j = 0;
	int length2 = 0;

	while (i < length) {
		if (isalpha(charArray[i])) {
			cleaned[j] = tolower(charArray[i]);
			++j;
		}

		++i;
	}

	cleaned[j] = 0;

	length2 = j;

	char *z = malloc(sizeof(char *) * length2);

	strcpy(z, cleaned);

	return z;
}
