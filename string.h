#ifndef __STRING_INCLUDED__
#define __STRING_INCLUDED__

#include <stdio.h>

typedef struct string STRING;

extern STRING *newSTRING(char *);
extern char *getSTRING(STRING *);
extern char *setSTRING(STRING *,char *);
extern int compareSTRING(void *,void *);
extern int rcompareSTRING(void *,void *);
extern void displaySTRING(void *,FILE *);
extern void freeSTRING(void *);

#endif