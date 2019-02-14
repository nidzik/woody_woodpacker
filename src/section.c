#include "woody.h"

void	encrypt_section(char *file, Elf64_Shdr *header)
{
	size_t	key_len;
	char key[] = KEY;
	off_t offset;
	off_t size;
	off_t index;

	offset = header->sh_offset;
	size = header->sh_size;
	key_len = strlen(key);
	index = 0;	
	while (index < size)
	{
		//file[offset + index] = file[offset + index] ^ key[index % key_len];
		file[offset + index] = file[offset + index] ^ 0x42;
		//file[offset + index] = file[offset + index] ^ key[(index + offset) % key_len];
		index += 1;
	}
	return ;
}


void	print_section(char *file, Elf64_Shdr *header)
{
	off_t offset;
	off_t size;
	off_t index;

	offset = header->sh_offset;
	size = header->sh_size;
	index = 0;
	while (index < size)
	{
		printf(" %02x%02x%02x%02x ", *((unsigned char *)file + offset + index),
				*((unsigned char *)file + offset + index + 1),
				*((unsigned char *)file + offset + index + 2),
				*((unsigned char *)file + offset + index + 3));
		if ( index == 12 || (index && !((index + 4) % 16)))
			printf("\n");
		fflush(stdout);
		index += 4;
	}
	printf("\n");
}


Elf64_Shdr *find_sect(char *file, const char *sect, off_t file_size)
{

	Elf64_Shdr *shdr;
	Elf64_Shdr stable;
	char *sname;
	int shnum;

	shdr = (Elf64_Shdr *)(file + ((Elf64_Ehdr *)file)->e_shoff);
	stable = shdr[((Elf64_Ehdr *)file)->e_shstrndx];
	sname = (char *)(file + stable.sh_offset);
	shnum = 0;
	while (shnum < ((Elf64_Ehdr *)file)->e_shnum)
	{
		stable = shdr[shnum];
		if (!strcmp(sname + stable.sh_name, sect))
		{
			printf("\nname sect : %s \t| offset : 0x%lx \t| size: 0x%lx \n", sname + stable.sh_name, stable.sh_offset, stable.sh_size);
			return ((Elf64_Shdr*)((char*)shdr + sizeof(Elf64_Shdr) * shnum));
		}
		shnum += 1;
	}
	return (NULL);
}
