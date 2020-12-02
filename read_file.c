#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./read_file.h"

void free_nested_string_file(struct string_content *output){
	for(int i=0; i < output->length; i++){
		free(output->content[i]);
	}
	free(output->content);
	free(output);
}

void free_nested_int_file(struct int_content *output){
	free(output->content);
	free(output);
}

void print_file_string(struct string_content *output){
	for(int i=0; i < output->length; i++){
		printf("%s\n", output->content[i]);
	}
}

void print_file_int(struct int_content *output){
	for(int i=0; i < output->length; i++){
		printf("%d\n", output->content[i]);
	}
}

int wc_l(char* path) {
   FILE *fp;
   char ch;
   int linesCount = 0;
   //open file in read more
   fp=fopen(path,"r");
   if(fp == NULL) {
      printf("File \"%s\" does not exist!!!\n",path);
      return -1;
   }
   //read character by character and check for new line
   while((ch = fgetc(fp))!=EOF) {
      if(ch == '\n')
         linesCount++;
   }
   //close the file
   fclose(fp);
   return linesCount;
}

struct string_content *read_file_string(char* path){
	int lines = wc_l(path);
	FILE* input = fopen(path, "r");
	if(input == NULL) {
		printf("File \"%s\" does not exist!!!\n",path);
		return NULL;
	}

	struct string_content *output = malloc(sizeof(struct string_content));
	output->content = malloc(lines*sizeof(char*));
	output->length = lines;
	
	size_t size;
	char* line;
	ssize_t length;
	for(int i=0; i < output->length; i++){
		size=0;
		line = NULL;
		length = getline(&line, &size, input);
		line[length-1] = '\0';
		output->content[i] = line;
	}
	fclose(input);
	return output;
}

struct int_content *read_file_int(char* path){
	int lines = wc_l(path);
	FILE* input = fopen(path, "r");
	if(input == NULL) {
		printf("File \"%s\" does not exist!!!\n",path);
		return NULL;
	}

	struct int_content *output = malloc(sizeof(struct int_content));
	output->content = malloc(lines*sizeof(char*));
	output->length = lines;
	
	size_t size;
	char* line;
	ssize_t length;
	for(int i=0; i < output->length; i++){
		size=0;
		line = NULL;
		length = getline(&line, &size, input);
		line[length-1] = '\0';
		output->content[i] = atoi(line);
		free(line);
	}
	fclose(input);
	return output;
}
