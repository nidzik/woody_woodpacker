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

static int write_to_file(char *file_name, char *content, off_t content_size)
{
	int fd;
	size_t writed;

	fd = open(file_name, O_WRONLY | O_CREAT);
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
	return (fd);
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
	while (index < headers_length)
	{
		if (phdr[index].p_type == 1) // check the p_type for 1 which is PT_LOAD
		{
			*error = 0;
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
	int error;
	char *new_file;
	int fd;

	new_entry = get_virt_addr(file, file_size, &error);
	if (error)
	{
		dprintf(2, "Nothing founded\n");
		return (0);
	}
	printf("Virtual address offset: 0x%jx\n", new_entry);
	old_entry = ((Elf64_Ehdr *)file)->e_entry;
	new_entry += cave_entry;
	printf("Old entry: 0x%jx\nNew entry: 0x%jx\n", old_entry, new_entry);
	new_file = get_new_file(file, file_size);

	/* alter entry point and 1st cave   (with lseek)*/

	// change the entry point
	((Elf64_Ehdr *)new_file)->e_entry = cave_entry;
	memcpy(new_file + cave_entry, "\xcc\xcc", 2);

	// copy entire binary
	fd = write_to_file(FILE_NAME, new_file, file_size);

	// fill the 1st cave code with 2  sigtrap
	return (1);
}
