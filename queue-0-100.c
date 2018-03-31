#include <stdio.h>
#include <stdlib.h>
#include "integer.h"
#include "real.h"
#include "string.h"
#include "sll.h"
#include "queue.h"

extern void srandom(unsigned int);
extern long int random(void);

int
main(void)
    {
    srandom(3932);
    QUEUE *p = newQUEUE(displayINTEGER,freeINTEGER);
    int i;
    int j;
    for (j = 0; j < 5; ++j)
        {
        for (i = 0; i < 750000; ++i)
            {
            int j = random() % 100;
            enqueue(p,newINTEGER(j));
            }
        if (sizeQUEUE(p) < 100)
            {
            displayQUEUE(p,stdout);
            printf("\n");
            displayQUEUEdebug(p,stdout);
            printf("\n");
            }
        for (i = 0; i < 375000; ++i)
            {
            freeINTEGER(dequeue(p));
            }
        if (sizeQUEUE(p) < 100)
            {
            displayQUEUE(p,stdout);
            printf("\n");
            displayQUEUEdebug(p,stdout);
            printf("\n");
            }
        while (sizeQUEUE(p) != 0)
            {
            freeINTEGER(dequeue(p));
            }
        if (sizeQUEUE(p) < 100)
            {
            displayQUEUE(p,stdout);
            printf("\n");
            displayQUEUEdebug(p,stdout);
            printf("\n");
            }
        }
    printf("size is %d\n",sizeQUEUE(p));
    freeQUEUE(p);
    return 0;
    }
