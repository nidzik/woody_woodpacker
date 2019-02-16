#ifndef WOODY_H
#define WOODY_H

#include <sys/mman.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>

#define FILE_NAME "woody"
#define KEY "super_key"
// payload/print_woody.s
// nasm -f bin payload/print_woody.s

#define WOODY_DEBUG 0

#define DEBUG_PAYLOAD "\xcc\xcc\xcc\xcc"

#define PRINT_WOODY_PAYLOAD "\xeb\x14\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\x5e\xba\x0e" \
							"\x00\x00\x00\x0f\x05\xeb\x13\xe8\xe7\xff\xff\xff\x2e\x2e\x2e" \
							"\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a\x48\x31\xc0\x48" \
							"\x31\xff\x48\x31\xd2\x48\x31\xf6"

#define DECRYPT_TEXT "\x41\xbd\x30\x08\x00\x00\x41\xbc\xa2\x01\x00\x00\x41\xbb\x30\x05\x00\x00\x48\x8d\x3d\xe7\xff\xff\xff\x49\xf7\xdd\x4c\x01\xef\x4c\x01\xdf\x49\x89\xfa\x48\x81\xe7\x00\xf0\xff\xff\x48\xf7\xdf\x49\x01\xfa\x48\xf7\xdf\x4d\x01\xd4\xb8\x0a\x00\x00\x00\x4c\x89\xe6\xba\x07\x00\x00\x00\x0f\x05\x4c\x01\xd7\x49\x89\xfe\x49\xf7\xda\x4d\x01\xd4\x48\x31\xc9\x48\xff\xc1\x41\x80\x36\x42\x4d\x8d\x76\x01\x48\x81\xf9\xd2\x01\x00\x00\x7e\xec\x48\x31\xc0\x48\x31\xff\x48\x31\xf6\x48\x31\xd2\x4d\x31\xf6\x48\x31\xc9"

#if WOODY_DEBUG == 1
#define NEW_EP_OFFSET (2 + 4)
#define TEXT_LENGTH_OFFSET (8 + 4)
#define TEXT_OFFSET_OFFSET (14 + 4)
#define PAYLOAD DEBUG_PAYLOAD DECRYPT_TEXT PRINT_WOODY_PAYLOAD
#else
#define NEW_EP_OFFSET 2
#define TEXT_LENGTH_OFFSET 8
#define TEXT_OFFSET_OFFSET 14
#define PAYLOAD DECRYPT_TEXT PRINT_WOODY_PAYLOAD
#endif
Elf64_Shdr *find_sect(char *elf, const char *sect, off_t file_size);
void encrypt_section(char *file, Elf64_Shdr *header);
void print_section(char *file, Elf64_Shdr *header);
off_t find_cave(char *file, off_t file_size, off_t asked_length, off_t *cave_size);
char *inject_code(char *file, off_t *file_size, Elf64_Shdr *section);
int write_to_file(char *file_name, char *content, off_t content_size);

#endif
