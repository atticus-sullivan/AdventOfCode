#ifndef _READ_FILE_H_
#define _READ_FILE_H_

#define read_file(path, type) ((_Generic((type), int: read_file_int, char*: read_file_string))(path))
#define print_file(structure) ((_Generic((structure), struct int_content: print_file_int, struct string_content: print_file_string))(structure))
#define free_file(structure) ((_Generic((structure), struct int_content*: free_file_int, struct string_content*: free_file_string))(structure))

struct string_content {
	int length;
	char** content;
};

struct int_content {
	int length;
	int* content;
};


struct int_content      *read_file_int(   char* path);
struct string_content   *read_file_string(char* path);

void                     print_file_int(struct int_content *output);
void                     free_file_int( struct int_content *output);

void                     print_file_string(struct string_content *output);
void                     free_file_string( struct string_content *output);

#endif
