#include "woody.h"

static Elf64_Addr get_virt_addr(char *file, off_t file_size)
{
	Elf64_Ehdr *ehdr;
	Elf64_Phdr *phdr;
	size_t headers_length;
	off_t index;

	ehdr = (void *)file;
	headers_length = ehdr->e_phnum;
	if (headers_length * sizeof(Elf64_Phdr) + ehdr->e_phoff > file_size)
	{
		dprintf(2, "corrupted binary\n");
		return 0;
	}
	phdr = (void *)(file + ehdr->e_phoff);
	printf("file pt : %p, phdr pt : %p\n", file, phdr);
	printf("headers length: %ld\n", headers_length);
	index = 0;
	while (index < headers_length)
	{
		if (phdr[index].p_type == 1) // check the p_type for 1 which is PT_LOAD
		{
			printf("hey we found it \n");
			return (phdr[index].p_vaddr);
		}
		index += 1;
	}
	printf("found nothig :(");
	return (0);
}

int inject_code(char *file, off_t file_size, off_t cave_entry, off_t cave_size)
{
	off_t new_entry;
	off_t old_entry;

	if (!(new_entry = get_virt_addr(file, file_size)))
		return (0);
	printf("Virtual address offset: 0x%jx\n", new_entry);
	old_entry = ((Elf64_Ehdr *)file)->e_entry;
	new_entry += cave_entry;
	printf("Old entry: 0x%jx\nNew entry: 0x%jx\n", old_entry, new_entry);
	return (1);
}