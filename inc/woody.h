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
// payload/print_woody.s
// nasm -f bin payload/print_woody.s
# define PRINT_WOODY_PAYLOAD	"\xeb\x14\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\x5e\xba\x0e"\
				"\x00\x00\x00\x0f\x05\xeb\x13\xe8\xe7\xff\xff\xff\x2e\x2e\x2e"\
				"\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a\x48\x31\xc0\x48"\
				"\x31\xff\x48\x31\xd2\x48\x31\xf6"

off_t	find_cave(char *file, off_t file_size, off_t asked_length, off_t *cave_size);
int	inject_code(char *file, off_t file_size, off_t cave_entry, off_t cave_size);

#endif
