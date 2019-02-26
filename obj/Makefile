TARGET = mcar

OPT = -g -Wall -Os
LDLIBS = -lm -lpthread
INCLUDES = -I ../header -I ../solver
CFLAGS = $(OPT) $(INCLUDES)
CC = gcc

%.o: ../src/%.c
	gcc -c $< -o $@ $(LDLIBS) $(CFLAGS) 
	
mcar.o: ../mcar.c 
	gcc -c $^ $(LDLIBS) $(CFLAGS)
matrix.o: ../src/matrix.c
	gcc -c $^ $(LDLIBS) $(CFLAGS) 
spi.o: ../src/spi.c
	gcc -c $^  $(LDLIBS) $(CFLAGS) 

