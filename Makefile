

all:
	gcc -o wp main.c


push:
	git add .
	git commit -m "initial"
	git push