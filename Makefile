CC=g++
CFLAGS=-I.
DEPS = AVL.h
OBJ = main.o AVL.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

avl: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o avl
