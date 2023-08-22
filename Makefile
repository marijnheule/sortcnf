all: sortcnf

sortcnf: sortcnf.c
	gcc sortcnf.c -std=c99 -O2 -o sortcnf

clean:
	rm sortcnf
