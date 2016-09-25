# -*- Makefile -*-
CC=mpicc
CFLAGS=-Wall -O3
LIBS=

EXEC=mandelbrot-display
OBJS=mandelbrot-display.o profile.o

all : $(EXEC)

$(EXEC) : $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS) $(LIBS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

.PHONY : clean
clean :
	@rm $(EXEC) $(OBJS) 2>/dev/null || true
