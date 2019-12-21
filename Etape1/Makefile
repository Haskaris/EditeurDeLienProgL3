CC=clang

all: elfParsing

clean:
	rm -rf *.o elfParsing

etape1-Principal.o: etape1-Principal.c etape1-Principal.h etape1-1.c etape1-1.h etape1-2.c etape1-2.h etape1-3.c etape1-3.h etape1-4.c etape1-4.h etape1-5.c etape1-5.h
	$(CC) -c $<

elfParsing: etape1-Principal.o
	$(CC) -o $@ $^ 
