CC = gcc 

Flag = -Wall -Wextra -Werror

all: refator_jpeg.o main


refator_jpeg.o:
	$(CC) -c $(Flag) refactor_jpeg.c refactor_jpeg.h 

main:
	$(CC) main.c refactor_jpeg.o -ljpeg -o out
