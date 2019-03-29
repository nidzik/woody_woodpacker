#include "woody.h"

#define DODGE_HARD_CODED (KEY2_OFFSET_OFFSET + 10)
#define SIZE_OF_JUMP 5

int is_infected(char *file, size_t file_size)
{
    off_t   ep;
    char    payload[] = PAYLOAD;
    int     error;
    off_t   virt_addr;

    virt_addr = get_virt_addr(file, file_size, &error, &ep);
    if (error)
        return 1;
    ep = ((Elf64_Ehdr *)file)->e_entry;
    ep -= virt_addr;
    if (ep + sizeof(payload) > file_size)
    {
        dprintf(2, "[ KO ] Invalid file\n");
        return 1;
    }
    if (!memcmp(file + ep + DODGE_HARD_CODED, payload + DODGE_HARD_CODED, sizeof(payload)
     - DODGE_HARD_CODED - SIZE_OF_JUMP))
    {
		dprintf(2, "[ KO ] Binary already infected\n");
        return (1);
    }
    return (0);
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

p_pack *init_struct(void)
{
	p_pack *pp;

	if (!(pp = (p_pack *)malloc(sizeof(p_pack))))
		return NULL;
	pp->arch =  0;
	pp->offset_tls_callback = 0;
	pp->size_section_text = 0;
	pp->offset_section_text = 0;
	pp->offset_permissions = 0;
	pp->value_permissions_text= 0;
	pp->offset_permissions_text = 0;
	pp->value_permissions = 0;
	pp->rva = 0;
	pp->va_text = 0;
	pp->virtual_address = 0;
	return pp;
}