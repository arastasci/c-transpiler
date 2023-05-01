advcalc2ir: main.o hash.o tokenizer.o parser.o reg.o file.o
	gcc main.o hash.o tokenizer.o parser.o reg.o file.o -o advcalc2ir

main.o: main.c file.h
	gcc -c main.c

hash.o: hash.c hash.h
	gcc -c hash.c

tokenizer.o: tokenizer.c tokenizer.h
	gcc -c tokenizer.c

parser.o: parser.c parser.h file.h
	gcc -c parser.c



reg.o: reg.c reg.h
	gcc -c reg.c

file.o: file.c file.h
	gcc -c file.c
clean:
	rm *.o advcalc2ir
