#include "woody.h"

static char *get_new_file(char *old_file, off_t file_size)
{
	char *result;

	if (!(result = malloc(file_size)))
	{
		dprintf(2, "malloc failed\n");
		return (NULL);
	}
	memcpy(result, old_file, (size_t)file_size);
	return (result);
}

int write_to_file(char *file_name, char *content, off_t content_size)
{
	int fd;
	size_t writed;

	fd = open(file_name, O_WRONLY | O_CREAT, S_IRWXU);
	if (fd == -1)
	{
		dprintf(2, "Cannot create the new binary \"%s\":(\n", file_name);
		return (0);
	}

	writed = write(fd, content, content_size);
	if (writed != content_size)
	{
		dprintf(2, "incomplete write to the new binary\n");
		close(fd);
		unlink(file_name);
		return (0);
	}
	close(fd);
	return (1);
}

static Elf64_Addr get_virt_addr(char *file, off_t file_size, int *error)
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
	//	printf("file pt : %p, phdr pt : %p\n", file, phdr);
	//	printf("headers length: %ld\n", headers_length);
	index = 0;
	*error = 1;
	if (!headers_length)
	{
		dprintf(2, "There is no program header\n");
		return (0);
	}
	while (index < headers_length)
	{
		if (phdr[index].p_type == 1) // check the p_type for 1 which is PT_LOAD
		{
			*error = 0;
			return (phdr[index].p_vaddr);
		}
		index += 1;
	}
	dprintf(2, "unamble to find the PT_LOAD program header\n");
	return (0);
}

int build_payload(char *file, char *new_file, char *code, off_t code_len, Elf64_Shdr *section, off_t virt_addr)
{
	char *jump;
	off_t offset;
	int jmp_adr;
	char jmp_code[] = "\xe9\xff\xff\xff\xff";

	offset = ((Elf64_Ehdr *)new_file)->e_entry;
	jmp_adr = ((Elf64_Ehdr *)file)->e_entry - (offset + code_len);
	jmp_adr -= 5; // wtf
	printf("offset: 0x%lx\n", offset);
	printf("offset of the jump: %d\n", jmp_adr);
	printf("size of the code: 0x%lx\n", code_len);
	memcpy(jmp_code + 1, &jmp_adr, sizeof(int));
	offset += (WOODY_DEBUG ? 4 : 0);
	memcpy(code + NEW_EP_OFFSET, &offset, sizeof(int));
	memcpy(code + TEXT_LENGTH_OFFSET, &(section->sh_size), sizeof(int));
	memcpy(code + TEXT_OFFSET_OFFSET, &(section->sh_offset), sizeof(int));
	offset -= (WOODY_DEBUG ? 4 : 0);
	memcpy(new_file + offset - virt_addr, code, code_len);
	offset += code_len;
	// write(2, code, code_len);
	memcpy(new_file + offset - virt_addr, jmp_code, sizeof(jmp_code) - 1);
	return 1;
}

char *inject_code(char *file, off_t *file_size, Elf64_Shdr *section)
{
	off_t virt_addr;
	int error;
	char *new_file;
	char code[] = PAYLOAD;
	off_t cave_size;
	off_t cave_entry;

	new_file = get_new_file(file, *file_size);
	cave_entry = new_section(&new_file, file_size, sizeof(code) - 1);
	//cave_entry = find_cave(file, *file_size, 0, &cave_size);
	//printf("bigest cave entry: 0x%jx, cave size: 0x%jx\n", cave_entry, cave_size);
	virt_addr = get_virt_addr(file, *file_size, &error);
	if (error)
		return (NULL);
	printf("Virtual address offset: 0x%jx\n", virt_addr);
	cave_entry += virt_addr;
	printf("Old entry: 0x%jx\nNew entry: 0x%jx\n", ((Elf64_Ehdr *)file)->e_entry, cave_entry);

	// change the entry point
	((Elf64_Ehdr *)new_file)->e_entry = cave_entry;
	// copy the code to the cave and jump to the normal entry point
	// Example with shellcode
	// build_payload(file, new_file, shellcode, sizeof(shellcode));

	// without shellcode (just a jump to the begining)
	build_payload(file, new_file, code, sizeof(code) - 1, section, virt_addr);

	// fill the 1st cave code with 2  sigtrap
	return (new_file);
}
