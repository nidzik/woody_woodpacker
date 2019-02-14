#ifndef WOODY_H
#define WOODY_H

# include <sys/mman.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <elf.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <string.h>

# define  FILE_NAME "woody"
# define KEY "super_key"
// payload/print_woody.s
// nasm -f bin payload/print_woody.s
# define PRINT_WOODY_PAYLOAD	"\xeb\x14\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\x5e\xba\x0e"\
				"\x00\x00\x00\x0f\x05\xeb\x13\xe8\xe7\xff\xff\xff\x2e\x2e\x2e"\
				"\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a\x48\x31\xc0\x48"\
				"\x31\xff\x48\x31\xd2\x48\x31\xf6"
# define DECRYPT_TEXT "\x90\x90\x90\x90\x48\xbf\x00\x40\x55\x55\x55\x55\x00\x00" \
	"\xbe\x00\x10\x00\x00\xba\x07\x00\x00\x00\xb8\x0a\x00\x00\x00\x0f\x05\x49\xbe\x80\x45\x55\x55\x55\x55\x00\x00"\
	"\x48\x31\xc9\x48\x81\xf9\xd2\x01\x00\x00\x7f\x11\x48\xff\xc1\x41\x80\x36\x42\x4d\x8d\x76\x01"\
	"\x48\x81\xf9\xd2\x01\x00\x00\x7e\xec"
# define PAYLOAD DECRYPT_TEXT PRINT_WOODY_PAYLOAD
Elf64_Shdr	*find_sect(char *elf, const char *sect, off_t file_size);
void		encrypt_section(char *file, Elf64_Shdr *header);
void		print_section(char *file, Elf64_Shdr *header);
off_t		find_cave(char *file, off_t file_size, off_t asked_length, off_t *cave_size);
char		*inject_code(char *file, off_t *file_size);
int		write_to_file(char *file_name, char *content, off_t content_size);

#endif
