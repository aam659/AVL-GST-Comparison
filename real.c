#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "real.h"
#include "integer.h"

struct real
    {
    double value;
    };

REAL *
newREAL(double x)
    {
    REAL *p = malloc(sizeof(REAL));
    if (p == 0) { fprintf(stderr,"out of memory\n"); exit(1); }
    p->value = x;
    return p;
    }

double 
getREAL(REAL *v)
    {
    return v->value;
    }

double 
setREAL(REAL *v,double x)
    {
    double old = v->value;
    v->value = x;
    return old;
    }

int
compareREAL(void *v,void *w)
    {
    double result = getREAL(v) - getREAL(w);
    if (result > 0) return 1;
    else if (result < 0) return -1;
    return 0;
    }

int
rcompareREAL(void *v,void *w)
    {
    double result = getREAL(w) - getREAL(v);
    if (result > 0) return 1;
    else if (result < 0) return -1;
    return 0;
    }

void 
displayREAL(void *v, FILE *fp)
    {
    fprintf(fp,"%lf", getREAL((REAL *) v));
    }

void
freeREAL(void *v)
    {
    free((REAL *) v);
    }
