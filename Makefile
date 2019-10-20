#Makefile for Assignment1 for mac 


build:
	clang++ `fltk-config --use-gl --ldflags` MyGLCanvas.cpp main.cpp -o fp

clean:
	rm a1 

