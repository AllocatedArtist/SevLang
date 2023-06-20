

all: build

build:
	zig cc main.c -o sev.exe


clean:
	rm -f *.exe
