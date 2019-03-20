#include "woody.h"

int verif_header(char *file, off_t size)
{
	if (strncmp(file, ELFMAG, sizeof(ELFMAG) - 1) || size < EI_CLASS)
	{
		dprintf(2, "the file is not an elf\n");
		return (0);
	}
	if ((file[EI_CLASS] == ELFCLASS32 && size < sizeof(Elf32_Ehdr)) || (file[EI_CLASS] == ELFCLASS64 && size < sizeof(Elf64_Ehdr)))
	{
		dprintf(2, "The file is too small\n");
		return (0);
	}
	if (file[EI_CLASS] == ELFCLASS32)
	{
		if (((Elf32_Ehdr *)file)->e_type != ET_EXEC && ((Elf32_Ehdr *)file)->e_type != ET_DYN)
		{
			dprintf(2, "the file is not an executable\n");
			return (0);
		}
		return (32);
	}
	else if (file[EI_CLASS] == ELFCLASS64)
	{
		if (((Elf64_Ehdr *)file)->e_type != ET_EXEC && ((Elf64_Ehdr *)file)->e_type != ET_DYN)
		{
			dprintf(2, "the file is not an executable\n");
			return (0);
		}
		return (64);
	}
	else
	{
		dprintf(2, "not a valid format\n");
		return (0);
	}
}

char *get_file(char *name, off_t *file_size)
{
	struct stat metadata;
	int fd;
	char *file;

	if ((fd = open(name, O_RDONLY)) == -1)
	{
		dprintf(2, "error during the open\n");
		return (NULL);
	}
	if (fstat(fd, &metadata) == -1)
	{
		dprintf(2, "ftsat failed\n");
		return (NULL);
	}
	*file_size = metadata.st_size;
	if ((file = mmap(0, (size_t)*file_size, PROT_READ,
					 MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		dprintf(2, "mmap failed\n");
		return (NULL);
	}
	close(fd);
	return (file);
}

int main(int ac, char **av)
{
	char *file;
	char *new_file;
	off_t file_size;
	void *ptr = NULL;
	Elf32_Shdr *shdr;
	Elf64_Shdr *text;
	int arch;
	char	key[16];

    srand(time(NULL));
	if (ac < 2)
	{
		dprintf(2, "Usage: %s [FILENAME]\n", av[0]);
		return (1);
	}
	file_size = 0;
	file = get_file(av[1], &file_size);
	if (!file || !(arch = verif_header(file, file_size)))
		return (1);
	printf("Sucess retreiving %s, file size: %zu bytes.\n", av[1], file_size);
	// Step 1 : generate our code with the good address
	// char *code = get_our_code(file, file_size)

	// Step 2 : find where to place our code
	// off_t entry = find_place(char *bin, off_t code_size);

	// Step 3 : copy our code (if we found a place, else we )
	// memcpy(bin + entry, our_code, code_length)

	text = find_sect(file, ".text", file_size);
	generate_key(key);
	new_file = inject_code(file, &file_size, text, key);
	if (!text || text->sh_offset + text->sh_size > file_size)
	{
		dprintf(2, text ? "No text section\n" : "Invalid file\n");
		return (1);
	}
	//print_section(new_file, text);
	encrypt_section(new_file, text, key);
	//print_section(new_file, text);

	printf("exiting...\n");

	return write_to_file(FILE_NAME, new_file, file_size);
}
