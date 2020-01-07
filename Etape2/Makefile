CC=clang

all: elfParsing2

clean:
	rm -rf *.o elfParsing2

etape2-Principal.o: etape2-Principal.c etape2-Principal.h etape2-7.c etape2-7.h
	$(CC) -c $<

elfParsing2: etape2-Principal.o
	$(CC) -o $@ $^ 
