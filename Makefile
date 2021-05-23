CC=gcc
CFLAGS=-I.

hellomake: test.o LCD_lib.o mzapo_parlcd.o
	$(CC) -o test test.o LCD_lib.o mzapo_parlcd.o -lm
