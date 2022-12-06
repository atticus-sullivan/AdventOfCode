#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <regex.h>

#include "./read_file.h"

// variables for regex matching // are compiled in main()
regex_t height_r;
regmatch_t height_match[3];
regex_t hair_r;
regex_t eye_r;
regex_t pass_r;

struct pass_is_valid {
	unsigned int birth_year : 1;
	unsigned int issue_year : 1;
	unsigned int expir_year : 1;
	unsigned int height     : 1;
	unsigned int hair_color : 1;
	unsigned int eye_color  : 1;
	unsigned int pass_id    : 1;
	unsigned int count_id   : 1;
};

int check_pass_is_valid(struct pass_is_valid *input){
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

void set_boolean_ele2(struct pass_is_valid *input, char* cat, char* value){
	if(!strcmp(cat, "byr")){
		int birth = atoi(value);
		if(birth <= 2002 && birth >= 1920){
			input->birth_year = 1;
			return;
		}
	}
	else if (!strcmp(cat, "iyr")){
		int issue = atoi(value);
		if(issue <= 2020 && issue >= 2010){
			input->issue_year = 1;
			return;
		}
	}
	else if (!strcmp(cat, "eyr")){
		int exp = atoi(value);
		if(exp <= 2030 && exp >= 2020){
			input->expir_year = 1;
			return;
		}
	}
	else if (!strcmp(cat, "hgt")){
		// <digit>in/cm
		if(!regexec(&height_r, value, 3, height_match, 0)){ // capture groups: digits, unit
			if(!strncmp((value+height_match[2].rm_so), "in", height_match[2].rm_eo - height_match[2].rm_so)){
				value[height_match[2].rm_so] = '\0';
				int num = atoi(value);
				if(num <= 76 && num >= 56){
					input->height = 1;
			return;
				}
			} else {
				value[height_match[2].rm_so] = '\0';
				int num = atoi(value);
				if(num <= 193 && num >= 150){
					input->height = 1;
			return;
				}
			}
		}
	}
	else if (!strcmp(cat, "hcl")){
		if(!regexec(&hair_r, value, 1, NULL, 0)){
			input->hair_color = 1;
			return;
		}
	}
	else if (!strcmp(cat, "ecl")){
		if(!regexec(&eye_r, value, 1, NULL, 0)){
			input->eye_color = 1;
			return;
		}
	}
	else if (!strcmp(cat, "pid")){
		if(!regexec(&pass_r, value, 1, NULL, 0)){
			input->pass_id = 1;
			return;
		}
	}
	else if (!strcmp(cat, "cid")){
		input->count_id = 1;
			return;
	}
	return;
}

void set_boolean_ele(struct pass_is_valid *input, char* cat, char* value){
	(void) value; // value is needed to pass this as function pointer
	if(!strcmp(cat, "byr")) input->birth_year       = 1;
	else if (!strcmp(cat, "iyr")) input->issue_year = 1;
	else if (!strcmp(cat, "eyr")) input->expir_year = 1;
	else if (!strcmp(cat, "hgt")) input->height     = 1;
	else if (!strcmp(cat, "hcl")) input->hair_color = 1;
	else if (!strcmp(cat, "ecl")) input->eye_color  = 1;
	else if (!strcmp(cat, "pid")) input->pass_id    = 1;
	else if (!strcmp(cat, "cid")) input->count_id   = 1;
}

int part1_2(struct string_content* input, void (*set_bool)(struct pass_is_valid*, char*, char*)){
	int counter = 0;
	for(int i = 0; i < input->length; i++){
		struct pass_is_valid check = {0,0,0,0,0,0,0,0};
		while(strcmp(input->content[i], "") && i < input->length){ // while current passport is not over
			char* line = input->content[i];
			while(*line != '\0'){ // while current line is not over
				char* category = strtok_r(line, ":", &line);
				char* value    = strtok_r(NULL, " ", &line);
				/* printf("cat: %s\t value: %s\n", cat, value); */
				set_bool(&check, category, value);
			}
			i++;
		}
		if(check_pass_is_valid(&check)){
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

	regcomp(&height_r, "^([0-9]+)(in|cm)$",            REG_EXTENDED);
	regcomp(&hair_r, "^#[a-f0-9]{6}$",                 REG_EXTENDED | REG_NOSUB);
	regcomp(&eye_r, "^(amb|blu|brn|gry|grn|hzl|oth)$", REG_EXTENDED | REG_NOSUB);
	regcomp(&pass_r, "^[0-9]{9}$",                     REG_EXTENDED | REG_NOSUB);

	struct string_content *input = read_file("./problems/day-04.dat",type);
	printf("Part1\nValid Passports: %d\n", part1_2(input, set_boolean_ele));
	free_file(input);

	input = read_file("./problems/day-04.dat",type);
	printf("\nPart2\nValid Passports: %d\n", part1_2(input, set_boolean_ele2));
	free_file(input);

	regfree(&height_r);
	regfree(&hair_r);
	regfree(&eye_r);
	regfree(&pass_r);
	return EXIT_SUCCESS;
}
