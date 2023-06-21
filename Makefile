
CC = zig cc


all: build

build:
	$(CC) main.c -o sev.exe


clean:
	rm -f *.exe
