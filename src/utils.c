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