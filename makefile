TOBJS = integer.o real.o string.o dll.o sll.o stack.o queue.o bst.o scanner.o gst.o avl.o trees.o
BSTOBJS = integer.o real.o string.o sll.o queue.o bst.o test-bst.o
OOPTS = -Wall -Wextra -std=c99 -c -g
EOPTS = -Wall -Wextra -std=c99 -g
VOPTS = -v
DOPTS = data commands
AOPTS = -r data commands
GOPTS = -g data commands
AGOPTS = -r -g data

all : test-bst trees 

trees : $(TOBJS)
	gcc $(EOPTS) -o trees $(TOBJS)

test-bst : $(BSTOBJS)
	gcc $(EOPTS) -o test-bst $(BSTOBJS)

dll.o : dll.c dll.h
	gcc $(OOPTS) dll.c

sll.o : sll.c sll.h
	gcc $(OOPTS) sll.c

stack.o : stack.c stack.h dll.h
	gcc $(OOPTS) stack.c

queue.o : queue.c queue.h sll.h
	gcc $(OOPTS) queue.c

integer.o : integer.c integer.h
	gcc $(OOPTS) integer.c

real.o : real.c real.h
	gcc $(OOPTS) real.c

string.o : string.c string.h
	gcc $(OOPTS) string.c

scanner.o : scanner.c scanner.h
	gcc $(OOPTS) scanner.c

bst.o : bst.c bst.h
	gcc $(OOPTS) bst.c

trees.o : trees.c integer.h real.h string.h sll.h queue.h bst.h scanner.h avl.h gst.h 
	gcc $(OOPTS) trees.c

avl.o : avl.c integer.h real.h string.h sll.h queue.h bst.h avl.h
	gcc $(OOPTS) avl.c

gst.o : gst.c integer.h real.h string.h sll.h queue.h bst.h gst.h
	gcc $(OOPTS) gst.c

test-bst.o : test-bst.c integer.h real.h string.h sll.h queue.h
	gcc $(OOPTS) test-bst.c

valgrind : test-bst
	@echo testing bst
	valgrind ./test-bst

test : all
	@echo running: trees -v
	./trees $(VOPTS)
	@echo running: trees -r data commands
	./trees $(AOPTS)
	@echo running: trees -g data commands
	./trees $(GOPTS)
	@echo running: trees -r -g data commands
	./trees $(AGOPTS)
	@echo running: trees data commands
	./trees $(DOPTS)

clean   :
	rm -f *.o trees test-bst


