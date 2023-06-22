
CC = zig cc

FLAGS = -Wall \
-O0 \
-g \
-std=c99 \


OBJ = main.o \
cli.o \
lexer.o \
utility.o \


all: build

build: $(OBJ)
	$(CC) $(OBJ) $(FLAGS) -o sev.exe

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f *.exe *.o
