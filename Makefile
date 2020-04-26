all: anagramGame

anagramGame: anagramGame.o gameFunctions.o 
	gcc -ansi -Wall -o anagramGame anagramGame.o gameFunctions.o

anagramGame.o: anagramGame.c
	gcc -ansi -Wall -c anagramGame.c

gameFunctions.o: gameFunctions.c
	gcc -ansi -Wall -c gameFunctions.c 