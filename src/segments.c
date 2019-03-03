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

int is_sect_exec(char *file, off_t file_size, off_t entry_point, int len)
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
	index = 0;
	if (!headers_length)
	{
		dprintf(2, "There is no program header\n");
		return (0);
	}
	printf("len : %lx\n", len);
	while (index < headers_length)
	{
//		printf (" index : %d   p_paddr %lx   p_filesz %lx   entry_p =  %lx    sum= %lx \n  flag : %d \n", index, phdr[index].p_paddr, phdr[index].p_filesz , entry_point, phdr[index].p_paddr+ phdr[index].p_filesz, phdr[index].p_flags);
		//printf("addr: 0x%lx, e flag: %d, r flag: %d, mask: %lx\n", phdr[index].p_vaddr, phdr[index].p_flags & 1, phdr[index].p_flags & 4, phdr[index].p_flags);
		if (!(phdr[index].p_flags & 1) || !(phdr[index].p_flags & 4))	 // flag for PF_R & PF_X
		{
		//	printf("invalid mask\n");
			index += 1;
			continue ;
		}
		printf("0x%lx < 0x%lx\n", phdr[index].p_offset, entry_point);
		printf("last offset acceptable: 0x%lx\n", phdr[index].p_offset + phdr[index].p_filesz);
		if (phdr[index].p_offset < entry_point &&
			phdr[index].p_offset + phdr[index].p_filesz + phdr[index].p_align >
			entry_point + len) // check if entry point is in the section
			return 1;
		index += 1;
	}
	printf("no section exe\n");
	return 0;
}
