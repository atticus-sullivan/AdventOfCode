#ifndef _READ_FILE_H_
#define _READ_FILE_H_

/*
Advent Of Code 2020
Copyright (C) 2024  Lukas Heindl

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define read_file(path, type) ((_Generic((type),	\
	int: read_file_int,								\
	double: read_file_double,						\
	long: read_file_long,							\
	char*: read_file_string							\
	))(path))

#define print_file(structure) ((_Generic((structure),		\
				struct int_content*: print_file_int,		\
				struct double_content*: print_file_double,	\
				struct long_content*: print_file_long,		\
				struct string_content*: print_file_string	\
				))(structure))

#define free_file(structure) ((_Generic((structure),		\
				struct int_content*: free_file_int,			\
				struct double_content*: free_file_double,	\
				struct long_content*: free_file_long,		\
				struct string_content*: free_file_string	\
				))(structure))

struct string_content {
	int length;
	char** content;
};

struct int_content {
	int length;
	int* content;
};

struct double_content {
	int length;
	double* content;
};

struct long_content {
	int length;
	long* content;
};


struct int_content      *read_file_int( char* path);
void                     print_file_int(struct int_content *output);
void                     free_file_int( struct int_content *output);

struct double_content   *read_file_double( char* path);
void                     print_file_double(struct double_content *output);
void                     free_file_double( struct double_content *output);

struct long_content     *read_file_long( char* path);
void                     print_file_long(struct long_content *output);
void                     free_file_long( struct long_content *output);

struct string_content   *read_file_string( char* path);
void                     print_file_string(struct string_content *output);
void                     free_file_string( struct string_content *output);

struct int_content *read_line_int(char* path);

#endif
