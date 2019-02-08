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

off_t	find_cave(char *file, off_t file_size, off_t asked_length, off_t *cave_size);
int	inject_code(char *file, off_t file_size, off_t cave_entry, off_t cave_size);

#endif
