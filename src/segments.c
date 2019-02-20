#include "woody.h"


off_t make_place(char **new_file, off_t *file_size, off_t code_size)
{
	off_t		offset;
	char		*tmp_file;

	code_size += 5; // jmp
	offset = ((Elf64_Ehdr *)*new_file)->e_shoff + ((Elf64_Ehdr *)*new_file)->e_shentsize *
		((Elf64_Ehdr *)*new_file)->e_shnum;
	offset += offset % 4;
	printf("h_offset: 0x%lx\n", offset);
	printf("code size: %lx\n", code_size);
	if (!(tmp_file = realloc(*new_file, offset + code_size)))
	{
		dprintf(2, "realloc error\n");
		return (0);
	}
	*new_file = tmp_file;
	*file_size = offset + code_size;
	return (offset);
}

off_t metamorph_segment(char *file, off_t file_size, off_t wanted_address, off_t wanted_size, off_t virt_addr)
{
	Elf64_Phdr	*h_table;
	size_t		table_length;
	size_t		index;
	size_t		entry_size;

	h_table = (void*)(((Elf64_Ehdr*)file)->e_phoff + file);
	table_length = ((Elf64_Ehdr*)file)->e_phnum;
	entry_size = ((Elf64_Ehdr*)file)->e_phentsize;
	index = 0;
	while (index < table_length)
	{
		if (h_table->p_type == PT_NOTE)
		{
			h_table->p_type = PT_LOAD;
			h_table->p_offset = wanted_address;
			h_table->p_vaddr = wanted_address + virt_addr;
			h_table->p_filesz = wanted_size + 5;
			h_table->p_memsz = wanted_size + 5;
			h_table->p_flags = PF_X | PF_R;
			return (1);
		}
		h_table = (void*)(((char*)h_table) + entry_size);
		index += 1;
	}
	return (0);
}
