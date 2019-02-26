TARGET = mcar


OPT = -g -Wall -Os
LDLIBS = -lm -lpthread
INCLUDES = -I ./header -I ./solver
CFLAGS = $(OPT) $(INCLUDES)
CC = gcc

$(TARGET): obj/mcar.o $(patsubst src/%.c,obj/%.o, $(wildcard src/*.c)) $(patsubst %.c,%.o, $(wildcard solver/*.c))
	$(CC) $^ -o $(TARGET) $(OPT) $(LDLIBS)
obj/mcar.o: mcar.c
	make -C obj mcar.o
obj/%.o: src/%.c
	make -C obj $(patsubst %.c,%.o, $(notdir $<))
solver/%.o: solver/%.c
	make -C solver $(patsubst %.c,%.o, $(notdir $<))

all:	$(TARGET)
print:  
	@echo $(wildcard solver/*.c) 

clean:
	rm -f $(TARGET) obj/*.o
rebuild:clean all
