#include "woody.h"

/* 
	return the offset of a cave in the binary,
	the first cave bigger than minimal size, or the bigest if minimal_size = 0;

*/

off_t get_cave_size(char *file, off_t offset, off_t file_size)
{
	off_t index;

	index = 0;
	while (index + offset < file_size && !file[offset + index])
		index++;
	return (index);
}

off_t find_cave(char *file, off_t file_size, off_t asked_length, off_t *cave_size)
{
	off_t index;
	off_t tmp_size;
	off_t result;

	result = 0;
	*cave_size = 0;
	tmp_size = 0;
	index = 0;
	while (index < file_size)
	{
		if (!file[index])
		{
			tmp_size = get_cave_size(file, index, file_size);
			if (asked_length && tmp_size > asked_length)
			{
				*cave_size = tmp_size;
				return (index);
			}
			if (tmp_size > *cave_size)
			{
				*cave_size = tmp_size;
				result = index;
			}
			index += tmp_size;
		}
		else
			index += 1;
	}
	return (result);
}