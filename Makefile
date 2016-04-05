all: compilar run

compilar:
	g++ -w -o paint paints2.c -lglut -lGL -lGLU

run:
	./paint

git:
	git add .
	git add --all
	git commit -m "last"
	git push

clean:
	rm -rf *o paint