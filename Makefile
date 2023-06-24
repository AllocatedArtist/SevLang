
CC = zig cc

FLAGS = -Wall \
-O0 \
-g \
-std=c99 \

build_path = build

OBJ = $(build_path)/main.o \
$(build_path)/cli.o \
$(build_path)/lexer.o \
$(build_path)/utility.o \
$(build_path)/ast.o \


all: build

build: $(OBJ)
	$(CC) $(OBJ) $(FLAGS) -o sev.exe

$(build_path)/%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f $(build_path)/*.o *.exe
