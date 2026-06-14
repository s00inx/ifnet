TARGET = ifct

all: 
	gcc -Wall -Iinclude -Wextra src/main.c src/nl.c -o $(TARGET) 