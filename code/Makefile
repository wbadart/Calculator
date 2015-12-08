MAIN = main
LIB = lib
HEAD = head
EXT = c
COMP = gcc
FLAGS = -lm

exec: $(MAIN).o $(LIB).o
	$(COMP) object_$(MAIN).o object_$(LIB).o $(FLAGS)

test: test.c $(LIB).o
	$(COMP) test.c object_$(LIB).o $(FLAGS)

$(MAIN).o: $(MAIN).$(EXT) $(HEAD).h
	$(COMP) $(MAIN).$(EXT) -c -o object_$(MAIN).o

$(LIB).o: $(LIB).$(EXT) $(HEAD).h
	$(COMP) $(LIB).$(EXT) -c -o object_$(LIB).o

clean:
	rm *.out object*
