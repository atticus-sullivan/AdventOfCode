#ifndef _READ_FILE_H_
#define _READ_FILE_H_

struct string_content {
	int length;
	char** content;
};

struct int_content {
	int length;
	int* content;
};


struct int_content      *read_file_int(char* path);
struct string_content   *read_file_string(char* path);

void                     print_file_int(         struct int_content *output);
void                     free_nested_int_file(   struct int_content *output);

void                     print_file_string(         struct string_content *output);
void                     free_nested_string_file(   struct string_content *output);

#endif
