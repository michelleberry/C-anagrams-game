#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "game_functions.h"

/* 
 * Program Name: anagramGame.c
 * Author: Michelle Berry
 * Date: April 25 2020
 * Purpose: Simple word game where users enter words that can be spelled using 
 *          the letters of a given word.
 */ 
int main(){
	/*declarations*/
	char inputStr[100], gameInput[100];
	char fileName[30];
	char line[100];
	char ** usedWords;
	
	bool stLoop = true;
	bool play = true;
	bool secondGuessed = false;

	int guessCt = 0;
	int scoreCt = 0;
	int uWsize = 0;
	int countDown = 5;  
	int lvlInt; 

	time_t t; 
	srand((unsigned) time(&t)); 

	printf("Welcome to Anagrams! Type 'start' to begin.\nAlternatively, type 'scores' to view highscores.\n");

	while (stLoop == true){
		
		fgets(inputStr, 100, stdin);
		char uInput1[100];
		sscanf(inputStr, "%s", uInput1);

		int k;
		for (k=0 ; k<strlen(uInput1); k++){
			uInput1[k] = tolower(uInput1[k]);
		}

		if(feof(stdin)){
			exit(1); 
		}

		if( strcmp(uInput1, "scores") == 0){
			print_scoreboard(); 
		}

		if( (strcmp(uInput1, "start") == 0) || (strcmp(uInput1, "Start") == 0)){
			stLoop = false;
		}  

	}

	while(countDown != 0){
		system("clear"); 
		printf("In this game, you will be given a string of letters. \nEnter valid words that can be spelled using this string \nof letters to earn points. \n");
		printf("Game starting in %d seconds ...\n", countDown);
		sleep(1);
		countDown--; 
	}


	lvlInt = (rand() % 4);

	switch(lvlInt){
		case 0:
			strcpy(fileName,"sandwich.txt");
			strcpy(game.set, "sandwich");
			break; 
		case 1:
			strcpy(fileName,"computer.txt");
			strcpy(game.set, "computer");
			break; 
		case 2:
			strcpy(fileName,"hamburger.txt");
			strcpy(game.set, "hamburger");
			break;
		case 3:
			strcpy(fileName,"painting.txt");
			strcpy(game.set, "painting");
			break;
		default:
			printf("error\n");
	}

	FILE *fp;
	if((fp = fopen(fileName, "r")) == NULL){
		printf("error opening file\n");
		exit(1);
	} 

	game.subsets = malloc( sizeof(char *) * file_size(fp)); 
	usedWords = malloc( sizeof(char *) * file_size(fp));

	game.maxLength = strlen(game.set); 

	int i = 0; 
	/*read the file into the array of all possible words*/
	while(fscanf(fp, "%s", line) != EOF){
		game.subsets[i] = malloc(sizeof(char) * (game.maxLength));
		if(game.subsets[i] == NULL){
			printf("ERROR - could not allocate memory\n");
		}
		strcpy(game.subsets[i], line);
		i++;
	}

	game.maxScore = i;
	
	/*game begins for the user*/
	while(play == true){

		secondGuessed = false; 
		/*refresh the ui display*/
		system("clear");
		ui_display( game.set, guessCt, scoreCt, game.maxScore);
		if (scoreCt > 0){ wordBankDisplay( usedWords, uWsize ); }
		
		printf("Type here:");
		
		if(feof(stdin)){
			exit(1); 
		}

		/*take user input rapidly (make it dissapear after typed!!)*/
		fgets(gameInput, 100, stdin);
		char inpWrd[100];
		sscanf(gameInput, "%s", inpWrd);
		guessCt++;

		if(gameInput[0] != ' ' && gameInput[0] != '\n' ){
			
			if(strcmp(gameInput, "exit_game\n") == 0){
				score_prompt(scoreCt, game.maxScore); 
				printf("Goodbye ... thank you for playing!\n");
				play = false;
				break;  
			}

			/*ensure a word being entered twice does not gain points for the user-
	 	 	we have another array of pre used words */
			if(uWsize > 0){
				int b;
				for(b = 0; b < (uWsize); b++){
					if(strcmp(inpWrd, usedWords[b]) == 0){
						secondGuessed = true;
					}
				}
			}
			
			/*compare user input to list of words. if matches count goes up*/
			int a;
			for (a = 0; a < game.maxScore ; a++){
				/*check if the word entered is a valid answer;*/
				/*if it is a new correct guess, add to list of guessed words*/
				if (secondGuessed == false && (strcmp(inpWrd, game.subsets[a])==0) ){
					usedWords[uWsize] = malloc(sizeof(char) * (strlen(inpWrd)+1));
					if( usedWords[uWsize] == NULL ){
						printf("Error - Failed to allocate memory. Program will exit.\n");
						exit(1);
					}
					strcpy(usedWords[uWsize], inpWrd);
					uWsize++; 
					scoreCt++; 
					a+= 1000; 
				}

			}

			if(scoreCt == game.maxScore){
				/*create winner sequence*/
				printf("CONGRATULATIONS, YOU'VE WON!!\n");
				score_prompt(scoreCt, game.maxScore);
				printf("Goodbye ... thank you for playing!\n");
				play = false;
				break;  
			}

		}	
		gameInput[0] = '\0';
		inpWrd[0] = '\0';
	}

	fclose(fp);
	int x; 
	for(x=0; x<(uWsize+1); x++){
		free(usedWords[x]);
	}
	free(usedWords);
	int sd;
	for(sd=0; sd<(game.maxScore+1); sd++){
		free(game.subsets[sd]);
	}
	free(game.subsets);
	return 0; 
}
