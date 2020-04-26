struct word_data{
	char set[50];    /*the original word*/
	int maxLength;   /*holds length of the original word*/
	char ** subsets; /*holds all the possible anagrams (subsets of the set)*/
	int maxScore;    /*holds number of subsets (i.e. maximum score that can be acheived)*/	
};
struct word_data game;

/*public functions*/
void wordBankDisplay( char ** wordList, int amount );

void ui_display( char * gameWord, int guessCt, int scoreCt, int maxscore);

void print_scoreboard();

void score_prompt( int score, int maxscore ); 

int file_size(FILE *fp);