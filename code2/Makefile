MAIN = main
LIB = lib
HEAD = head
EXT = c
COMP = gcc
FLAGS = -lm

exec: $(MAIN).o $(LIB).o utils.o
	$(COMP) object_$(MAIN).o object_$(LIB).o object_utils.o $(FLAGS)

$(MAIN).o: $(MAIN).$(EXT) $(HEAD).h
	$(COMP) $(MAIN).$(EXT) -c -o object_$(MAIN).o

$(LIB).o: $(LIB).$(EXT) $(HEAD).h
	$(COMP) $(LIB).$(EXT) -c -o object_$(LIB).o

utils.o: utils.c head.h
	$(COMP) utils.c -c -o object_utils.o

clean:
	rm *.out object*
