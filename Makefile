all: compilar run

compilar:
	g++ -w -o paint paints2.c -lglut -lGL -lGLU

run:
	./paint

clean:
	rm -rf *o