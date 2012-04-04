target:
	gcc -o mandelbrot mandel.c -lGL -lglut -lGL -std=c99 -lm

clean:
	/bin/rm mandelbrot
