CC=clang

all: elfParsing

clean:
	rm -rf *.o elfParsing

etape2-Principal.o: etape2-Principal.c etape2-Principal.h etape2-6.c \
	etape2-6.h etape2-7.c etape2-7.h etape2-8.c etape2-8.h
	$(CC) -c $<

elfParsing: etape2-Principal.o
	$(CC) -o $@ $^

