#include "woody.h"

// long getImageBase(int fd)
// {
// 	Elf64_Ehdr ehdr;
// 	Elf64_Phdr phdr;

// 	lseek(fd, 0, SEEK_SET);

// 	read(fd, &ehdr, sizeof(ehdr)); // read Elf Header
// 	int numheaders = ehdr.e_phnum; // how many program headers are there?
// 	printf("e_phunum = %d (number of headers)\n", numheaders);
// 	lseek(fd, ehdr.e_phoff, SEEK_SET); // set file offset to first program header as provided by elf.e_phoff

// 	for (int i = 0; i < numheaders; i++) // loop through all program headers
// 	{
// 		read(fd, &phdr, sizeof(phdr));
// 		if (phdr.p_type == 1) // check the p_type for 1 which is PT_LOAD
// 		{
// 			printf("ImageBase: %x\n", phdr.p_vaddr); // first header that is a PT_LOAD type contains image base
// 			break;
// 		}
// 	}

// 	return phdr.p_vaddr;
// }

char *get_file(char *name, off_t *file_size)
{
	struct stat metadata;
	int fd;
	char *file;

	if ((fd = open(name, O_RDONLY)) == -1)
	{
		dprintf(2, "error open ");
		return (NULL);
	}
	if (fstat(fd, &metadata) == -1)
	{
		dprintf(2, "ftsat failed");
		return (NULL);
	}
	*file_size = metadata.st_size;
	if ((file = mmap(0, (size_t)*file_size, PROT_READ,
					 MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		dprintf(2, "mmap failed");
		return (NULL);
	}
	close(fd);
	return (file);
}

int main(int ac, char **av)
{
	char *file;
	off_t file_size;
	void *ptr = NULL;
	Elf64_Ehdr *elf;
	Elf32_Shdr *shdr;

	if (ac < 2)
	{
		dprintf(2, "Usage: %s [FILENAME]\n", av[0]);
		return (1);
	}
	file_size = 0;
	file = get_file(av[1], &file_size);
	if (!file)
		return (1);
	printf("Sucess retreiving %s, file size: %zu bytes.\n", av[1], file_size);
	// read(fd, &elf, sizeof(elf));
	elf = (void *)file;

	// Step 1 : generate our code with the good address
	// char *code = get_our_code(file, file_size)

	// Step 2 : find where to place our code
	// off_t entry = find_place(char *bin, off_t code_size);
	off_t cave_size;
	off_t entry;
	entry = find_cave(file, file_size, 0, &cave_size);
	printf("bigest cave entry: %jd, cave size: %jd\n", entry, cave_size);
	// Step 3 : copy our code (if we found a place, else we )
	// memcpy(bin + entry, our_code, code_length)

	// getImageBase(fd);

	/* replace Program entry */
	// elf->e_entry = 0x41414141;

	printf("Program entry: %lx\nPhdr entry : %ld\nShdr entry : %ld \nSize of ELF header : %d \n", elf->e_entry, elf->e_phoff, elf->e_shoff, elf->e_ehsize);

	// lseek(fd, 6654, SEEK_SET);
	// write(fd, &shdr, sizeof(shdr));
	// printf("section name : %s \n", shdr.sh_name);
	//	printf();

	printf("\nclosing fd, exiting...\n");
	return 0;
}
