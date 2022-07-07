CC = gcc 

Flag = -Wall -Wextra -Werror

all: main

test:
	$(CC) -c $(Flag) -D=TEST refactor_jpeg.c refactor_jpeg.h
	$(CC) main.c refactor_jpeg.o -ljpeg -o out_tets

refator_jpeg.o:
	$(CC) -c $(Flag) refactor_jpeg.c refactor_jpeg.h 

main: refator_jpeg.o
	$(CC) main.c refactor_jpeg.o -ljpeg -o out
