#include "woody.h"

void find_sect(void *elf, const char * sect)
{
	void *elfsave; 
	Elf64_Shdr *shdr;
	Elf64_Shdr stable;
	char *sname;
	int shnum;


//	printf("Program entry: %lx\nPhdr entry : %ld\nShdr entry : %ld \nSize of ELF header : %d \n", elf->e_entry, elf->e_phoff, elf->e_shoff, elf->e_ehsize);
	elfsave = elf; 
	shdr = (Elf64_Shdr *)(elf + ((Elf64_Ehdr *)elf)->e_shoff);
	stable = shdr[((Elf64_Ehdr *)elf)->e_shstrndx];
	sname = (char *)(elf + stable.sh_offset);
	
	for (shnum = 0 ; shnum < ((Elf64_Ehdr *)elf)->e_shnum ; shnum++)
	{
		stable = shdr[shnum];
//		printf ("name sect : %s \t\t|offset : %d \n", sname + stable.sh_name, stable.sh_offset);
		if (!strcmp(sname + stable.sh_name, sect))
		{
			printf ("\nname sect : %s \t| offset : %d \n", sname + stable.sh_name, stable.sh_offset);
			for (int j = 0; j < stable.sh_size ; j+=4)
			{
				printf(" %02x%02x%02x%02x ", *((unsigned char*)elfsave + stable.sh_offset + j),\
					   *((unsigned char*)elfsave + stable.sh_offset + j+1),\
					   *((unsigned char*)elfsave + stable.sh_offset + j+2),\
					   *((unsigned char*)elfsave + stable.sh_offset + j+3));
				if (!(j % 16) && j != 0)
					printf("\n");
				fflush (stdout);
			}
		}
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
	if ((file = mmap(0, (size_t)*file_size, PROT_READ | PROT_WRITE,
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

	/* replace Program entry  (+1 to 'align the shit')*/
/*
	((Elf64_Ehdr *)elf)->e_entry = entry + 1;

	int fdd = open(av[1], O_RDWR );
	lseek(fdd,0, SEEK_SET);
	write(fdd, elf, sizeof(Elf64_Ehdr));
	lseek(fdd, entry+1, SEEK_SET);
	char *strr = "\xcc\x80";
	printf("strr == %s ", strr);
	write(fdd,strr,3); 
	close(fdd);
*/		
	find_sect(elf,".text");
	
	inject_code(file, file_size, entry, cave_size);

	printf("exiting...\n");

	return 0;
}
