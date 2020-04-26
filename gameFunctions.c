#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* 
 * Program Name: gameFunctions.c
 * Author: Michelle Berry
 * Date: April 25 2020
 * Purpose: contains support functions for anagramGame.c
 */ 

/*the scores kept in the scores file will be read into a linked list
with this structure, to be sorted from highest to lowest scores
before printing.*/
typedef struct score {
	char * name; 
	int value; 
	struct score * next; 
	struct score * sort;  
} Node; 

typedef struct list{
	int size; 
	Node * next; 
	Node * sort; 
} List; 

/*this is a struct used to streamline reading & writing to score file*/
struct profile { 
	char name[30]; 
	int score; 
} score_template; 


/*simply changes any newline characters in string to blank spaces*/
void get_rid_of_newline_char(char * string){
	int i; 
	for(i=0; i<strlen(string); i++){
		if(string[i] == '\n'){
			string[i] = ' '; 
		}
	}
}

/*writes player's score to a textfile along with nickname*/
void write_score(int points){
	printf("Please enter your name (30 character limit):\n");
	char input[100]; 
	fgets(input, 100, stdin);

	strncpy( score_template.name, input, 30);
	score_template.score = points; 

	FILE * fp;
	if((fp = fopen("scores.bin", "a")) == NULL){
		printf("error opening file\n");
		exit(1);
	} 

	get_rid_of_newline_char(score_template.name); 
	
	fwrite(&score_template, sizeof(score_template), 1, fp); 
	printf("Your score has been recorded.\n"); 

}

/*calculates player score and asks them if they would like to record it*/
void score_prompt( int score, int maxscore ){

	bool unanswered = true; 
	char gameInput[100];

	double dblpoints = ((double) score/ (double)maxscore) * 1000; 
	int points = (int) dblpoints;

	printf("You found %d/%d anagrams, giving you a score of %d.\n Would you like to record this score? [yes/no]: ", score, maxscore, points);
	 
	while(unanswered == true){
		fgets(gameInput, 100, stdin);
		char inpWrd[100];
		sscanf(gameInput, "%s", inpWrd);

		int k; /*convert the string to all lowercase for easier comparison*/
		for (k=0 ; k<strlen(inpWrd); k++){
			inpWrd[k] = tolower(inpWrd[k]); 
		}

		if(strcmp(inpWrd, "yes") == 0){
			write_score(points); 
			unanswered = false; 

		} else if(strcmp(inpWrd, "no") == 0){
			printf("Understandable, you did not do so well.\n");
			unanswered = false; 
		}
	}
}

/*creates + returns the head of a linked list (empty)*/
List * create_list(){
	List * head = malloc(sizeof(List));
	if(head != NULL){
		head->next = NULL; 
		head->sort = NULL;
		head->size = 0;  
	} else {
		printf("Could not allocate memory - program exiting.\n");
		exit(1); 
	}
	return head; 
}

/*finds last node in a linked list*/
Node * find_last(List * list){
	Node * node = list->next; 
	if(node != NULL){
		while(node->next != NULL){
			node = node->next; 
		}
	}
	return node; 
}

/*inserts given node to sort links of List in descending order*/
void insert_sorted( List * list, Node * new_node){
	int search; 
	if(list->sort == NULL){
		list->sort = new_node;
		new_node->sort = NULL;   
	} else {
		search = new_node->value; 
		
		Node * node = list->sort; 
		Node * insert_node = NULL; 
		/*look for the last node less than the value of the new node - we will insert new node after that*/
		while(node != NULL && (node->value != search) && (node->value > search)){
			insert_node = node; 
			node = node->sort; 
		} 

		if(insert_node == NULL){ /*if you must push the new node*/
			new_node->sort = list->sort; 
			list->sort = new_node; 
		} else if(insert_node->sort == NULL){ /*if you are appending the new node*/
			insert_node->sort = new_node; 
			new_node->sort = NULL; 
		} else {/*insert between insert node and insert_node->sort ( after insert node)*/
			new_node->sort = insert_node->sort;
			insert_node->sort = new_node; 
		}	
	}
}

/*adds a node to the end and fills with func. arguments*/
void append(List * head, char * name, int value){
	if(head->next == NULL){ /*if first node is nonexistent, starts by creating it*/
		Node * first = malloc(sizeof(Node)); 

		first->value = value; 
		first->name = malloc(sizeof(char) * strlen(name)+1);
		strcpy(first->name, name);

		head->next = first; 
		head->sort = NULL; 
		first->next = NULL; 
		first->sort = NULL; 
		insert_sorted(head, first); 
	} else { /*otherwise, find last node in list and append to it*/
		Node * last = find_last(head); 
		Node * new_node = malloc(sizeof(Node));
		
		new_node->value = value;
		new_node->name = malloc(sizeof(char) * strlen(name)+1);
		strcpy(new_node->name, name);

		last->next = new_node; 
		new_node->next = NULL;
		new_node->sort = NULL;
		insert_sorted(head, new_node);
	}

}

/*prints TOP 5 of list in descending order (according to scores).*/
void print_sorted_list(List * list){
	Node * node = list->sort; 
	int num = 1;
	if(node != NULL){
		while( (node->sort != NULL) && (num < 5)){
			printf("%d. %s || %d ||\n", num, node->name, node->value);
			node = node->sort; 
			num++; 
		}
		printf("%d. %s || %d ||\n", num, node->name, node->value);
	}	
}

/*frees each node from head to tail, as well as string inside each node*/
void free_list(List * list){
	Node * new_head = list->next;
	Node * old_node = NULL;

	while(new_head != NULL){
		old_node = new_head;
		new_head = new_head->next;
		free(old_node->name); 
		free(old_node);
	}
}

/*gives size of the file (Mark Wineberg's Function from CIS*2500 Assignment 1)*/
int file_size(FILE *fp){
	int sz = 0;
	fseek(fp, 0L, SEEK_END);
	sz = ftell(fp);
	rewind(fp);
	return sz;
}

/*reads textfile of scores and prints top 5 after sorting into descending order*/
void print_scoreboard(){
	
	printf("\n");

	List * list = create_list(); 
	FILE *fp; 

	if((fp = fopen("scores.bin", "r")) == NULL){
		printf("error opening file\n");
		exit(1);
	} 
	
	/*read each name and score into a linked list. i decided to encrypt the file (binary)
	 so users cannot easily cheat by altering score records*/
	while(fread(&score_template, sizeof(score_template), 1, fp) != 0){
		append(list, score_template.name, score_template.score);
		list->size++; 
	}

	print_sorted_list(list); 

	printf("\nEnter 'start' to begin playing.\n");

	/*free allocated memory before function exits!*/
	fclose(fp);  
	free_list(list);  
}

/*prints words that were already correctly submitted by player*/
void wordBankDisplay( char ** wordList , int amount ){

	printf("Guessed words: \t \t To give up, type 'exit_game'.\n");
	printf("--------------------\n");

	int p;
	for(p=0; p < amount; p++){
	 	if((p+1) < amount){ /* prints side by side if enough words (even amount) */
	 		printf(" %s \t \t %s\n", wordList[p], wordList[p+1]);
	 		p++;
	 	} else {
	 		printf(" %s \n", wordList[p]);
	 	}		 
	}
}

/*display game information like the word, score, and guesses made. (my makeshift GUI).*/
void ui_display( char * gameWord, int guessCt, int scoreCt, int maxscore){
	
	printf("What words can be made using the letters in the word....\n");
	printf(" ____________________________________\n");
	printf("|                                    |\n");

	int lef = (35 - strlen(gameWord))/2;
	int right = lef + 2;

	int l,r;
	printf("|");
	for(l = 0; l < lef; l++){
		printf(" ");
	}

	printf("%s", gameWord);
	for(r = 0; r < right; r++){
		printf(" ");
	}

	printf("|\n");
	printf("|____________________________________|\n");
	printf("  Guessed:%d       Score:%d/%d\n", guessCt, scoreCt, maxscore );
		
}


