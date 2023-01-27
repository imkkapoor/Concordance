
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emalloc.h"
#include "seng265-list.h"


// created a new structure so that I can return multiple items from a function 

struct multiple{
	node_t *a,*b;
};

typedef struct multiple Multiple;


//creating function prototypes

void print_with_indent(node_t * , int);
int find_words_position(node_t *line_tracker,node_t *word_tracker);
void remove_right_white_space(node_t* line_tracker);
node_t* capitalize_found_word(node_t *line_tracker,node_t *word_tracker);



//This function reads each line for exclusion words and makes a lower case linked list out of it

node_t* read_file(){
	char each_line[100];
	int num_exwords = 0;
		
	fgets(each_line,6,stdin);	
	if(strncmp(strtok(each_line,"\n"),"1",2)==0){
		printf("Input is version 1, concord3 expected version 2");
		exit(1);
	}
	
	fgets(each_line,6,stdin);
	node_t *head = NULL;
	node_t *ele = NULL;
	
	//This loop will go untill """" is reached
	
	while(strncmp(fgets(each_line,42,stdin),"\"\"\"\"",4)!=0){		
		char *token;
		token = strtok(each_line,"\n");
		size_t n= 0;	

		//here I convert all the words to lowe case
	
		while(token[n]){
			token[n]=tolower(token[n]);
			n++;
		}
		ele = new_node(token);
		head = add_end(head,ele);	
		num_exwords++;
		

	}
	ele = NULL;
	
	return head;
}


//This function will go through each line and find all the words to be included
//and stores all the lines as well

Multiple make_sorted_words(node_t* exwords){
	char *token;
	char each_line[100]; 
        int flag =0;
        int total_words = 0;

	node_t *sorted_words = NULL;
	node_t *in_place = NULL;
	node_t *lines = NULL;
	
	while(fgets(each_line,100,stdin)){
		
		char each_line_copy[100];
		strncpy(each_line_copy,each_line,100);
		//just skipping an iteration if fuction sees an empty line
		
		if (strcmp(each_line,"\n")==0){continue;}

		token = strtok(each_line,"\n");
		
		node_t *line = new_node(each_line);
		lines = add_end(lines,line);

		node_t *cur = exwords;
                token = strtok(each_line," ");
                while(token!=NULL){
			char token_lower[40];
			cur = exwords;
			
			// convert token to lower case to check
			strcpy(token_lower,token);
			size_t n = 0;
			while (token_lower[n]){
				token_lower[n]=tolower(token_lower[n]);
				n++;
			}

			//this loop will check that if the word in line is in exclusion list or not
			while(cur!=NULL){

				if(strncmp(cur->value,token_lower,42)==0){
					flag=1;
				}
				cur = cur->next;
			}
				
			//if flag is set to 1 then it means, the word is in the sorted list
	                if(flag==0){

				char cur_line[100];
				strncpy(cur_line,each_line_copy,100);
				char *word = strstr(cur_line, token);
							
				size_t j = 0;
				while (word[j] && j<strlen(token)){
					word[j] = toupper(word[j]);
					token[j] = toupper(token[j]);
					j++;

				} 
				
				in_place = new_node(token);	
				
				//I made a new function add_alpha_order in the linked list file, and this adds 
				//all the words in order
				sorted_words = add_alpha_order(sorted_words,in_place);	
				total_words++;
                        }


                        flag=0;
                        token = strtok(NULL," ");

                }
        }

	token = NULL;
	in_place = NULL;
	Multiple both_linked_lists;
	both_linked_lists.a = sorted_words;
	both_linked_lists.b = lines;	
	return both_linked_lists;	

}


//This function sorts all the lines according to the sorted words

node_t* sorting_lines(node_t *sorted_words, node_t *lines){

	node_t *word_tracker = sorted_words;
	node_t *line_tracker = lines;
	node_t *sorted_lines = NULL;
	

	while (word_tracker!=NULL){

		line_tracker = lines;
		
		//this will ignore any duplicate entries for the words
		//since I track all the lines in one go
		
		if(word_tracker->next){
			if (strncmp((word_tracker->next)->value,word_tracker->value,42)==0){
				word_tracker= word_tracker->next;
				continue;	
			}
		}
		
		//this eill go through all the lines in from the list of lines we made above
		//and then add the repective line to a linked list in order
		
		while(line_tracker!=NULL){
			char check[100];
			char *token;	
				
			strncpy(check,line_tracker->value,100);
			
			token = strtok(check,"\n");
			token = strtok(check, " ");
		
			while (token!=NULL){
				size_t j = 0;
				while (token[j] && token[j]!=' '){
                                        token[j] = toupper(token[j]);
                                        j++;

                                }
				//Now I add the line to sorted_line linked list
				//while captializing the repective found word  
				if (strncmp(word_tracker->value,token,42)==0){
						node_t *temp_node = capitalize_found_word(line_tracker,word_tracker);
						sorted_lines = add_end(sorted_lines,temp_node);
					}
			

				token = strtok(NULL," ");
			}	
			
		
			check[0]='\0';
			token = NULL;
			line_tracker = line_tracker->next;
		}
		
		
		word_tracker = word_tracker->next;
		
			
	}
	
	
	word_tracker = NULL;
	line_tracker = NULL;
	return sorted_lines;	
}


//Designed this function in order to help to capitalize the word found in the line
//so that it can be reached easily when formatting

node_t* capitalize_found_word(node_t *line_tracker,node_t *word_tracker){
	int i = find_words_position(line_tracker, word_tracker);	
	char new_val[100];

	strncpy(new_val,line_tracker->value,100);

	while(new_val[i] && new_val[i]!=' '){
		new_val[i] = toupper(new_val[i]);
		i++;
	}	

	node_t *temp_node = new_node(new_val);
	return temp_node;
	
}



//This function will format all the lines i.e cut from right and left side,
//then center align the aligns and finaaly print them

void formatting_lines(node_t *sorted_words, node_t *sorted_lines){
	node_t *word_tracker=sorted_words;
	node_t *line_tracker=sorted_lines;
	
	//this loop will go through each line
	while(word_tracker!=NULL){	
		
		// here I find position of the capitalized word
		int position = find_words_position(line_tracker,word_tracker);
	
	
		// this translates to that there are too many words on left side
		// so I cut the left side
		if(position >20){
			for(int i = 0; i<strlen(line_tracker->value);i++){
				line_tracker->value[i]=line_tracker->value[position -21 +i];	
			}
	
			if(line_tracker->value[0]!=' '){
				int i = 0;
				while (line_tracker->value[i]!=' '){
					line_tracker->value[i]=' ';
					i++;
				}
			}
		}
		

		//getting the position again	
		position = find_words_position(line_tracker,word_tracker);
		
		// now working on right side of the line and cutting any excess words
		if(strlen(line_tracker->value)-position>31){
			int j = position+31;
			while (line_tracker->value[j]!=' '){

				j--;
			}

			line_tracker->value[j] = '\0';
			
			while(line_tracker->value[j-1]==' '){
				j--;
			}
			line_tracker->value[j] = '\0';
			
		}
	
		//removing any white space at the end of the line
		remove_right_white_space(line_tracker);	
		position = find_words_position(line_tracker,word_tracker);
		
		//finally sending the line to be printed to helper function 
		print_with_indent(line_tracker,position);
	

		word_tracker=word_tracker->next;
		line_tracker=line_tracker->next;
	
	}	
	word_tracker = NULL;
	line_tracker = NULL;

}



// this funtion helps to remove any white space at the end of a line 

void remove_right_white_space(node_t* line_tracker){

	int len = strlen(line_tracker->value);
	while(line_tracker->value[len-1]==' '){
		len--;

	}
	line_tracker->value[len] = '\0';

}


// designed this functio to get accurate position of the 
// sorted word in the line as i was having problems with strcspn() function 

int find_words_position(node_t *line_tracker,node_t *word_tracker){

	char *token;
	int word_position=0;
	char check[100];
	
	strncpy(check,line_tracker->value,100);

	
	token = strtok(check," ");
	while(token!=NULL){
		size_t j =0;
		while(token[j] && token[j]!=' '){
			token[j] = toupper(token[j]);	
			j++;
		}
		if (strncmp(word_tracker->value,token,42)==0){
			word_position = token - check;
			break;
		}
	token = strtok(NULL , " ");
	}
	check[0]='\0';
	token = NULL;
	return word_position;
}


//this function helps to print all the lines with appropriate indent 
// in the starting

void print_with_indent(node_t *to_print, int position){
	for(int i = 0; i<29 - position;i++){
		printf(" ");
	}
	char *printing  = strtok(to_print->value,"\n");
	printf(printing);
	printf("\n");	

}


// this function will free all the assigned memory with linked lists
// and a parameter which I created as it was using strdup()

void free_all(node_t *head){
	node_t *temp_node;
	temp_node = head;

	while (temp_node != NULL) {
		assert(temp_node != NULL);
		head = remove_front(head);
		free(temp_node->value);
		free(temp_node);
		temp_node = head;
    	}
	assert(head == NULL);
}



// main function
// basically has calls to all the functions and calls free_all
// at the end to free all heap memory

int main(int argc, char *argv[])
{
	
	node_t *exwords;	
	exwords = read_file();

	node_t *lines;
	node_t *sorted_words;

	Multiple to_sort_lines = make_sorted_words(exwords);
	lines = to_sort_lines.b;
	sorted_words = to_sort_lines.a;	
	
	node_t *sorted_lines;
	sorted_lines =  sorting_lines(sorted_words,lines);	

	formatting_lines(sorted_words,sorted_lines);	
	

	// free calls
	free_all(exwords);
	free_all(lines);
	free_all(sorted_words);
	free_all(sorted_lines);


    exit(0);
}
