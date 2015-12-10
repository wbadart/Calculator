MAIN = main
LIB = lib
HEAD = head
EXT = c
COMP = gcc
FLAGS = -lm

exec: $(MAIN).o $(LIB).o
	$(COMP) object_$(MAIN).o object_$(LIB).o $(FLAGS);
	echo
	./a.out sin\(-23/3.02\);
	

$(MAIN).o: $(MAIN).$(EXT) $(HEAD).h
	$(COMP) $(MAIN).$(EXT) -c -o object_$(MAIN).o

$(LIB).o: $(LIB).$(EXT) $(HEAD).h
	$(COMP) $(LIB).$(EXT) -c -o object_$(LIB).o

clean:
	rm *.out object*
