MAIN = main
LIB = evaluation
HEAD = evaluation
LIB2 = plot
HEAD2 = head
EXT = c
COMP = gcc
FLAGS = -lm -lX11

hotfix:
	gcc main.c evaluation.c plot.c gfx5.o -lm -lX11

exec: $(MAIN).o $(LIB).o $(LIB2).o
	$(COMP) object_$(MAIN).o object_$(LIB).o object_$(LIB2).o $(FLAGS)

$(MAIN).o: $(MAIN).$(EXT) $(HEAD).h $(HEAD2).h
	$(COMP) $(MAIN).$(EXT) -c -o object_$(MAIN).o

$(LIB2).o: $(LIB2).$(EXT) $(LIB).o $(HEAD2).h gfx5.h
	$(COMP) $(LIB2).$(EXT) gfx5.o -lX11

$(LIB).o: $(LIB).$(EXT) $(HEAD).h
	$(COMP) $(LIB).$(EXT) -c -o object_$(LIB).o

clean:
	rm *.out object*
