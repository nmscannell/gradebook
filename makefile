grades: main.o book.o commands.o
	gcc -o grades main.o book.o commands.o

main.o: main.c book.h commands.h
	gcc -c main.c

book.o: book.c book.h student.h
	gcc -c book.c

commands.o: commands.c commands.h book.h
	gcc -c commands.c

clean:
	rm *.o
