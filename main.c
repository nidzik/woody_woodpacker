#include <sys/mman.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <stdlib.h>

long getImageBase(int fd)
{
	Elf64_Ehdr ehdr;
	Elf64_Phdr phdr;

	lseek(fd, 0, SEEK_SET);

	read(fd, &ehdr, sizeof(ehdr)); // read Elf Header
	int numheaders = ehdr.e_phnum; // how many program headers are there?
	printf ("e_phunum = %d (number of headers)\n", numheaders) ;
	lseek(fd, ehdr.e_phoff, SEEK_SET); // set file offset to first program header as provided by elf.e_phoff

	for(int i=0;i<numheaders;i++) // loop through all program headers
	{
		read(fd, &phdr, sizeof(phdr));
		if (phdr.p_type == 1) // check the p_type for 1 which is PT_LOAD
		{
			printf("ImageBase: %x\n", phdr.p_vaddr); // first header that is a PT_LOAD type contains image base
			break;
		}
	}

	return phdr.p_vaddr;
}

int main(int ac, char **av)
{
	void *ptr = NULL;
	Elf64_Ehdr elf;
	Elf32_Shdr shdr;
	int fd = 0;
	int lenght = 10;

	int i = 0;
	
	if ((fd = open(av[1], O_RDWR)) == -1 )
		printf("error open ");
	if ((ptr = mmap(0, lenght, PROT_READ,\
					MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	printf ("mapping failed");

	read(fd, &elf, sizeof(elf));
	

	printf("Program Entry: %x\n",  elf.e_entry);

	getImageBase(fd);
	
	/* replace Program entry */ 
	elf.e_entry = 0x41414141;


	lseek(fd, 0, SEEK_SET);
	write(fd, &elf, sizeof(elf));
	printf("Program entry: %x\nPhdr entry : %d\nShdr entry : %d \nSize of ELF header : %d \n",  elf.e_entry,elf.e_phoff,elf.e_shoff, elf.e_ehsize );

	
	lseek(fd, 6654, SEEK_SET);
	write(fd, &shdr, sizeof(shdr));
	printf("section name : %s \n",shdr.sh_name);
//	printf();
	
		close(fd);
		printf ("\nclosing fd, exiting...\n");
		return 0 ;
}
