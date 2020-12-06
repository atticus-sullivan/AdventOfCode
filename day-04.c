#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <regex.h>

#include "./read_file.h"

struct boolean {
	unsigned int birth_year : 1;
	unsigned int issue_year : 1;
	unsigned int expir_year : 1;
	unsigned int height     : 1;
	unsigned int hair_color : 1;
	unsigned int eye_color  : 1;
	unsigned int pass_id    : 1;
	unsigned int count_id   : 1;
};

int check_boolean(struct boolean *input){
	return
		input->birth_year &&
		input->issue_year &&
		input->expir_year &&
		input->height     &&
		input->hair_color &&
		input->eye_color  &&
		/* input->count_id   && */
		input->pass_id;
}


regex_t height_r;
regmatch_t height_match[1];

regex_t hair_r;
regmatch_t hair_match[1];

regex_t eye_r;
regmatch_t eye_match[1];

regex_t pass_r;
regmatch_t pass_match[1];
int set_boolean2(struct boolean *input, char* cat, char* value){
	if(!strcmp(cat, "byr")){
		int birth = atoi(value);
		if(birth <= 2002 && birth >= 1920){
			input->birth_year       = 1;
			return 1;
		}
	}
	else if (!strcmp(cat, "iyr")){
		int issue = atoi(value);
		if(issue <= 2020 && issue >= 2010){
			input->issue_year = 1;
			return 1;
		}
	}
	else if (!strcmp(cat, "eyr")){
		int exp = atoi(value);
		if(exp <= 2030 && exp >= 2020){
			input->expir_year = 1;
			return 1;
		}
	}
	else if (!strcmp(cat, "hgt")){
		// <digit>in/cm
		if(!regexec(&height_r, value, 1, height_match, 0)){
			int length = strlen(value);
			if(!strcmp((value+length-2), "in")){
				value[length-2] = '\0';
				int num = atoi(value);
				if(num <= 76 && num >= 56){
					input->height     = 1;
			return 1;
				}
			} else {
				value[length-2] = '\0';
				int num = atoi(value);
				if(num <= 193 && num >= 150){
					input->height     = 1;
			return 1;
				}
			}
		}
	}
	else if (!strcmp(cat, "hcl")){
		if(!regexec(&hair_r, value, 1, hair_match, 0)){
			input->hair_color = 1;
			return 1;
		}
	}
	else if (!strcmp(cat, "ecl")){
		if(!strcmp(value,"amb") || !strcmp(value,"blu") || !strcmp(value,"brn") || !strcmp(value,"gry") || !strcmp(value,"grn") || !strcmp(value,"hzl") || !strcmp(value,"oth")){
			input->eye_color  = 1;
			return 1;
		}
	}
	else if (!strcmp(cat, "pid")){
		if(!regexec(&pass_r, value, 1, pass_match, 0)){
			input->pass_id    = 1;
			return 1;
		}
	}
	else if (!strcmp(cat, "cid")){
		input->count_id   = 1;
			return 1;
	}
	return 0;
}

void set_boolean(struct boolean *input, char* cat){
	if(!strcmp(cat, "byr")) input->birth_year       = 1;
	else if (!strcmp(cat, "iyr")) input->issue_year = 1;
	else if (!strcmp(cat, "eyr")) input->expir_year = 1;
	else if (!strcmp(cat, "hgt")) input->height     = 1;
	else if (!strcmp(cat, "hcl")) input->hair_color = 1;
	else if (!strcmp(cat, "ecl")) input->eye_color  = 1;
	else if (!strcmp(cat, "pid")) input->pass_id    = 1;
	else if (!strcmp(cat, "cid")) input->count_id   = 1;
}

int part1(struct string_content* input){
	int counter = 0;
	for(int i = 0; i < input->length; i++){
		struct boolean check = {0,0,0,0,0,0,0,0};
		while(strcmp(input->content[i], "")){
			char* line = input->content[i];
			while(*line != '\0'){
				char* cat   = strtok_r(line, ":", &line);
				char* value = strtok_r(NULL, " ", &line);
				(void)value;
				/* printf("cat: %s\t value: %s\n", cat, value); */
				set_boolean(&check, cat);
			}
			i++;
		}
		if(check_boolean(&check)){
			counter++;
			/* printf("Passport is valid\n\n"); */
		} else{
			/* printf("Passport is INvalid\n\n"); */
		}
	}
	return counter;
}

int part2(struct string_content* input){
	int counter = 0;
	for(int i = 0; i < input->length; i++){
		struct boolean check = {0,0,0,0,0,0,0,0};
		while(strcmp(input->content[i], "")){
			char* line = input->content[i];
			while(*line != '\0'){
				char* cat   = strtok_r(line, ":", &line);
				char* value = strtok_r(NULL, " ", &line);
				/* printf("cat: %s\t value: %s\n", cat, value); */
				set_boolean2(&check, cat, value);
			}
			i++;
		}
		if(check_boolean(&check)){
			counter++;
			/* printf("Passport is valid\n\n"); */
		} else{
			/* printf("Passport is INvalid\n\n"); */
		}
	}
	return counter;
}

int main() {
	char* type;
	(void)type;

	struct string_content *input = read_file("./day-04.dat",type);
	printf("Part1\nValid Passports: %d\n", part1(input));
	free_file(input);

	regcomp(&height_r, "^[0-9]+(in|cm)$", REG_EXTENDED);
	regcomp(&hair_r, "^#[a-f0-9]{6}$", REG_EXTENDED);
	regcomp(&eye_r, "amb blu brn gry grn hzl oth", REG_EXTENDED);
	regcomp(&pass_r, "^[0-9]{9}$", REG_EXTENDED);

	input = read_file("./day-04.dat",type);
	printf("\nPart2\nValid Passports: %d\n", part2(input));
	free_file(input);

	regfree(&height_r);
	regfree(&hair_r);
	regfree(&eye_r);
	regfree(&pass_r);
	return EXIT_SUCCESS;
}
