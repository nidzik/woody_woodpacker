#include "woody.h"

/* 
	return the offset of a cave in the binary,
	the first cave bigger than minimal size, or the bigest if minimal_size = 0;

*/

static off_t get_cave_size(char *file, off_t offset, off_t file_size)
{
	off_t index;

	index = 0;
	while (index + offset < file_size && !file[offset + index])
		index++;
	return (index);
}

off_t find_cave_pe(char *file, off_t file_size, off_t asked_length, off_t *cave_size, off_t *offset_max)
{
	off_t index;
	off_t tmp_size;

	*cave_size = 0;
	tmp_size = 0;
	index = 0;
	while (index < file_size)
	{
		if (!file[index] && index % 4 == 0 && index > *offset_max)
		{
			tmp_size = get_cave_size(file, index, file_size);
			if (tmp_size > asked_length && index != 0)
			{
				*cave_size = tmp_size;
				return (index);
			}
			index += tmp_size;
		}
		else
			index += 1;
	}
	return (0);
}

off_t find_cave(char *file, off_t file_size, off_t asked_length, off_t *cave_size)
{
	off_t index;
	off_t tmp_size;

	tmp_size = 0;
	index = 0;
	while (index < file_size)
	{
		if (!file[index] && index % 4 == 0)
		{
			tmp_size = get_cave_size(file, index, file_size);
			if (tmp_size > asked_length && index != 0 && (is_sect_exec(file, file_size, index , asked_length)))
			{
				*cave_size = tmp_size;
				return (index);
			}
			index += tmp_size;
		}
		else
			index += 1;
	}
	return (0);
}
