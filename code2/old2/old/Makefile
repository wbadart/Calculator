hotfix:
	gcc main.c eval.c plot.c gfx5.o -lm -lX11

exec: main.o plot.o eval.o gfx5.o
	gcc object_main.o object_eval.o object_plot.o gfx5.o

main.o: main.c head.h
	gcc main.c -c -o object_main.o

plot.o: plot.c eval.o head.h eval.h gfx5.o
	gcc plot.c object_object.o -c -o gfx5.o object_plot.o -lX11 -lm

eval.o: eval.c eval.h
	gcc eval.c -c -o object_eval.o

clean:
	rm object_* *.out
