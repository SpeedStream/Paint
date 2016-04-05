all: compilar run

compilar:
	g++ -w -o paint paints2.c -lglut -lGL -lGLU

run:
	./paint

git:
	$ git add .
	$ git add -A
	$ git commit -m "last"
	$ git push origin master

clean:
	rm -rf *o paint