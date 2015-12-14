hotfix:
	gcc final.c eval.c plot.c gfx5.o -lm -lX11

#couldn't really get these rules working. Use hotfix to compile.
exec: final.o plot.o eval.o gfx5.o
	gcc object_final.o object_eval.o object_plot.o gfx5.o

final.o: final.c plot.h
	gcc final.c -c -o object_final.o

plot.o: plot.c eval.o plot.h eval.h gfx5.o
	gcc plot.c object_object.o -c -o gfx5.o object_plot.o -lX11 -lm

eval.o: eval.c eval.h
	gcc eval.c -c -o object_eval.o

clean:
	rm object_* *.out
