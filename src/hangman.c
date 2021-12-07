/*
 * Copyright (c) 2018 Valentin Cassano.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <input.h>
#include <dictionary.h>
#include <time.h>
#include <hangman.h>
#define FALSE 0
#define TRUE !FALSE
#define ESC 27
#define GAME_OVER 7 // max number of misses
#define PENALTY   2 // penalty for incorrect guess

// state of hangman
typedef	struct state_s* state_t;

char * line2asterisk(char * line);
char * lowerCase(char * str);

struct state_s
{
	char * help;
	char * password;
	char * secret;
	   int length;
	char * misses;
	   int missed;
};

static void welcome( void )
{
	system("clear");
	printf( "\n        **********************************          " );
	printf( "\n        *             HANGMAN            *          " );
	printf( "\n        **********************************          " );
	printf( "\n                                                    " );
	printf( "\n ( press ESC to exit or any other key to continue ) \n" );
}

static void endgame_screen(state_t game)
{
	system("clear");
	if (strcmp(game->secret, game->password)){
		printf( "\n * HAS PERDIDO *\n" );
	}
	else{
		printf( "\n * HAS GANADO *\n" );
		printf( "\n Number of misses: %d" ,game->missed);
		printf( "\n Words missed: %s" ,game->misses);
	}
	printf( "\n La palabra correcta era: %s\n" ,game->password);
	printf( "\n ( press ESC to exit or any other key to play again ) \n" );
}

void draw_hangman( state_t game )
{
	switch ( game->missed )
	{
		case 0 :
		{
			printf( "\n  ___________ " );
			printf( "\n |/           " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |___         " );
		};
		break;

		case 1 :
		{
			printf( "\n  ___________ " );
			printf( "\n |/     |     " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |___         " );
		};
		break;

		case 2 :
		{
			printf( "\n  ___________ " );
			printf( "\n |/     |     " );
			printf( "\n |     (_)    " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |___         " );
		};
		break;

		case 3 :
		{
			printf( "\n  ___________ " );
			printf( "\n |/     |     " );
			printf( "\n |     (_)    " );
			printf( "\n |      |     " );
			printf( "\n |      |     " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |___         " );
		};
		break;

		case 4 :
		{
			printf( "\n  ___________ " );
			printf( "\n |/     |     " );
			printf( "\n |     (_)    " );
			printf( "\n |   >--|     " );
			printf( "\n |      |     " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |___         " );
		};
		break;

		case 5 :
		{
			printf( "\n  ___________ " );
			printf( "\n |/     |     " );
			printf( "\n |     (_)    " );
			printf( "\n |   >--|--<  " );
			printf( "\n |      |     " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |            " );
			printf( "\n |___         " );
		};
		break;

		case 6 :
		{
			printf( "\n  ___________ " );
			printf( "\n |/     |     " );
			printf( "\n |     (_)    " );
			printf( "\n |   >--|--<  " );
			printf( "\n |      |     " );
			printf( "\n |     /      " );
			printf( "\n |   _/       " );
			printf( "\n |            " );
			printf( "\n |___         " );
		};
		break;

		default :
		{
			printf( "\n  ___________ " );
			printf( "\n |/     |     " );
			printf( "\n |     (_)    " );
			printf( "\n |   >--|--<  " );
			printf( "\n |      |     " );
			printf( "\n |     / \\    " );
			printf( "\n |   _/   \\_  " );
			printf( "\n |            " );
			printf( "\n |___         " );
		};
		break;
	}
}

void display( state_t game )
{
	system("clear");
	draw_hangman( game );
	printf( "\n\n   *Disclaimer*");
	printf( "\n   help: %s", game->password   );
	printf( "\n   Press enter to switch between guess modes\n");
	printf( "\n   help: %s", game->help   );
	printf( "\n   secret: %s", game->secret );
	printf( "\n   misses: %s\n", game->misses );
}

char * lowerCase(char * str){
	for(int i = 0; str[i]; i++){
	  str[i] = tolower(str[i]);
	}
	return str;
}
// new game
state_t init( void )
{
	flush();
	state_t game = ( state_t ) malloc ( sizeof( struct state_s ) );
	word_t word = malloc(sizeof(word_t));
	word = random_word(dictionary);

 	game->help = definition(word);
	game->password = lowerCase(expression(word));
	game->secret = line2asterisk(game->password);
	game->length = strlen(expression(word));
	game->misses = malloc(sizeof(GAME_OVER * 2));
	game->missed = 0;

	return game;
}

char * line2asterisk(char * line){
	int i = 0;
	char * ret;
	ret = malloc(sizeof(strlen(line)));
	while ( line[i] != '\0'	 ){
		if (isalpha(line[i])){
			ret[i] = '*';
		}
		else{
			ret[i] = line[i];
		}
		i++;
	}
	return ret;
}
// update current game
int update( state_t game, char guess )
{
	int i = 0;
	char guess2 = guess;
	flush();
	size_t len;
	//UPDATE SECRET
		while (i <= game->length && guess2 != '\n'){
			if (game->password[i] == guess2){
				game->secret[i] = guess2;
			}
			if (i == game->length){
				if (!strchr(game->secret, guess2)){
					game->missed++;
					len = strlen(game->misses);
					game->misses[len] = guess2;
					game->misses[len+1] = ',';
					game->misses[len+2] = ' ';
					game->misses[len+3] = '\0';
				}
			}
			i++;
		}
	return 1;
}

int play( void )
{
	state_t game;
	char guess;
	char enter;
	char * whole;

	welcome();
	game = init();
	enter = getch();
	if (enter != ESC){
		while (game->missed < GAME_OVER && strcmp(game->secret, game->password) ){
			display(game);
			printf("\n\n   Guess a character: ");
			guess = getch();
			update(game, guess);
			display( game );
			//SWITCHES GUESSING MODE AND READS A STRING INSTEAD OF A CHAR
			if (guess == '\n'){
				printf("\n\n   Guess a word: ");
				whole = getstr(strlen(game->password));
				//IF INPUT IS CORRECT
				if (!strcmp(whole, game->password)){
					game->secret = game->password;
					break;;
				}
				//IF INPUT IN INCORRECT
				else{
					game->missed = game->missed + PENALTY;
					display(game);
				}
			}
		}
		//SHOWS END GAME SCREEN AND ASKS FOR A NEW GAME
		if (strcmp(game->secret, game->password)){
			endgame_screen(game);
		}
		else{
			endgame_screen(game);
		}
		enter = getch();
		if (enter != ESC){
			play();
		}
	}
	else{
		//SHOWS END GAME SCREEN
		printf("\n   Juego terminado\n");
	}
	return 0;
}
