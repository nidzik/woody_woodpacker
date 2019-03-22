#include "woody.h"

void	encrypt_section(char *file, Elf64_Shdr *header, char *key)
{
	off_t offset;
	off_t size;

	offset = header->sh_offset;
	size = header->sh_size;
	encrypt(key, file + offset, size);
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
			printf(" * .text section finded, offset: 0x%lx, size: 0x%lx\n", stable.sh_offset, stable.sh_size);
			return ((Elf64_Shdr*)((char*)shdr + sizeof(Elf64_Shdr) * shnum));
		}
		shnum += 1;
	}
	return (NULL);
}