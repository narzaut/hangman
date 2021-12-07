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
#include <time.h>

#include <llist.h>

#include <dictionary.h>

struct word_s
{
	char * expression;
	char * definition;
};

struct dictionary_s
{
	 char * name;
	llist_t content;
	    int entries;
};
char  * file2str(char * file_name);
llist_t parse( char * buffer );
void removeChar(char *str, char garbage);

// assumption: line = "\" ... \", \" ... \"\n"
word_t line2word( char * line )
{
	word_t word = malloc(sizeof(word_t));
	char * nextLine;
	char * curLine = line;

	nextLine = strchr(curLine, ',');
	if (nextLine) *nextLine = '\0';
	word->expression = line;
	word->definition = nextLine+1;

	return word;
}

// parse the buffer line by line converting the lines
// read into the words to be inserted in the dictionary
llist_t parse( char * buffer )
{
	llist_t lista = make();
	char * curLine = buffer;
	char * nextLine;
		while(curLine){
			nextLine = strchr(curLine, '\n');
			if (nextLine) *nextLine = '\0';
		  add(lista, curLine);
		  curLine = nextLine ? nextLine + 1 : NULL;
	  }
	return lista;

}

dictionary_t load( char * file_name )
{

	dictionary_t dictionary = malloc(sizeof(dictionary_t));
	char * buffer = file2str(file_name);

	dictionary->name = "dic";
	dictionary->content = parse(buffer);
	dictionary->entries = dictionary->content->size;

	return dictionary;
}

char  * file2str(char * file_name){
	char * buff = 0;
	long length;
	FILE * f = fopen (file_name, "r");
	if (f){
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buff = malloc(length);
		if (buff){
			fread(buff, 1, length, f);
		}
		fclose (f);
	}

	return buff;
}

word_t random_word(dictionary_t dictionary){
	srand(time(0));
  return  line2word(at(dictionary->content, (rand() % (dictionary->entries-1))+1));
}

char * expression( word_t word )
{
	char * result = malloc(255);
	result = word->expression;
	removeChar(result, '"');

	return result;
}


char * definition( word_t word )
{
	char * result = malloc(255);

	result = word->definition;
	removeChar(result, '"');

	return result;
}

void removeChar(char *str, char garbage){

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}

void print_word( word_t word )
{
	printf( "  %s:", word->expression );
	printf( "\n  %s \n", word->definition );
}
