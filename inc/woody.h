#ifndef WOODY_H
# define WOODY_H

# include <sys/mman.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <elf.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <string.h>
# include <time.h>

typedef uint64_t QWORD;
typedef uint32_t DWORD;
typedef uint16_t WORD;
typedef uint8_t BYTE;

typedef struct 		s_pack
{
	unsigned int	arch;
	unsigned int 	offset_tls_callback;
	unsigned int 	size_section_text;
	unsigned int 	offset_section_text;
	unsigned int 	offset_permissions;
	unsigned int 	value_permissions_text;
	unsigned int 	offset_permissions_text;
	unsigned int 	value_permissions;
	unsigned int 	rva;
	unsigned int 	va_text;
	unsigned int	tls_protect;
	unsigned int	offset_entry_point;
	unsigned int	value_entry_point;
	QWORD 			virtual_address;

} p_pack;

#define READ_BYTE(p) (((unsigned char*)(p))[0])
#define READ_WORD(p) ((((unsigned char*)(p))[0]) | ((((unsigned char*)(p))[1]) << 8))
#define READ_DWORD(p) ((((unsigned char*)(p))[0]) | ((((unsigned char*)(p))[1]) << 8) | \
					   ((((unsigned char*)(p))[2]) << 16) | ((((unsigned char*)(p))[3]) << 24))

#define READ_QWORDD(p) ((((unsigned char*)(p+4))[0]) | ((((unsigned char*)(p+4))[1]) << 8) | \
					   ((((unsigned char*)(p+4))[2]) << 16) | ((((unsigned char*)(p+4))[3]) << 24))

#define READ_QWORD(p) READ_DWORD(p) | READ_QWORDD(p)

#define PAD(x) (((x) + 3) & 0xFFFFFFFC)


# define FILE_NAME "woody"

# define WOODY_DEBUG 0

# define DEBUG_PAYLOAD "\xcc\xcc\xcc\xcc"

# define ADDR_PAYLOAD "\x00\x00\x00\x00\x00\x00\x00\x00"

# define PRINT_WOODY_PAYLOAD	"\xeb\x14\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\x5e\xba\x0e" \
								"\x00\x00\x00\x0f\x05\xeb\x13\xe8\xe7\xff\xff\xff\x2e\x2e\x2e" \
								"\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a\x48\x31\xc0\x48" \
								"\x31\xff\x48\x31\xd2\x48\x31\xf6"

# define DECRYPT_TEXT			"\xeb\x25\x66\x0f\x70\xd2\xff\xc5\xe1\x73\xf9\x04\x66\x0f\xef\xcb\xc5\xe1\x73\xf9\x04\x66\x0f\xef\xcb\xc5\xe1\x73\xf9\x04\x66\x0f\xef\xcb\x66\x0f\xef\xca\xc3\x41\xbd\xef\xbe\xad\xde\x41\xbc\xef\xbe\xad\xde\x41\xbb\xef\xbe\xad\xde\x49\xb9\xef\xbe\xad\xde\xef\xbe\xad\xde\x49\xb8\xef\xbe\xad\xde\xef\xbe\xad\xde\x66\x49\x0f\x6e\xc9\x66\x49\x0f\x6e\xd0\x0f\x16\xca\x48\x8d\x3d\x9f\xff\xff\xff\x49\xf7\xdd\x4c\x01\xef\x4c\x01\xdf\x49\x89\xfa\x48\x81\xe7\x00\xf0\xff\xff\x48\xf7\xdf\x49\x01\xfa\x48\xf7\xdf\x4d\x01\xd4\xb8\x0a\x00\x00\x00\x4c\x89\xe6\xba\x07\x00\x00\x00\x0f\x05\x4c\x01\xd7\x49\xf7\xda\x4d\x01\xd4\x48\x89\xfe\x4c\x89\xe2\xf3\x0f\x6f\xc1\x66\x0f\x3a\xdf\xd1\x01\xe8\x55\xff\xff\xff\xf3\x0f\x6f\xe1\x66\x0f\x3a\xdf\xd1\x02\xe8\x46\xff\xff\xff\xf3\x0f\x6f\xe9\x66\x0f\x3a\xdf\xd1\x04\xe8\x37\xff\xff\xff\xf3\x0f\x6f\xf1\x66\x0f\x3a\xdf\xd1\x08\xe8\x28\xff\xff\xff\xf3\x0f\x6f\xf9\x66\x0f\x3a\xdf\xd1\x10\xe8\x19\xff\xff\xff\xf3\x44\x0f\x6f\xc1\x66\x0f\x3a\xdf\xd1\x20\xe8\x09\xff\xff\xff\xf3\x44\x0f\x6f\xc9\x66\x0f\x3a\xdf\xd1\x40\xe8\xf9\xfe\xff\xff\xf3\x44\x0f\x6f\xd1\x66\x0f\x3a\xdf\xd1\x80\xe8\xe9\xfe\xff\xff\xf3\x44\x0f\x6f\xd9\x66\x0f\x3a\xdf\xd1\x1b\xe8\xd9\xfe\xff\xff\xf3\x44\x0f\x6f\xe1\x66\x0f\x3a\xdf\xd1\x36\xe8\xc9\xfe\xff\xff\xf3\x44\x0f\x6f\xe9\x66\x0f\x38\xdb\xe4\x66\x0f\x38\xdb\xed\x66\x0f\x38\xdb\xf6\x66\x0f\x38\xdb\xff\x66\x45\x0f\x38\xdb\xc0\x66\x45\x0f\x38\xdb\xc9\x66\x45\x0f\x38\xdb\xd2\x66\x45\x0f\x38\xdb\xdb\x66\x45\x0f\x38\xdb\xe4\x48\x31\xff\x48\x39\xfa\x7e\x53\xf3\x44\x0f\x6f\x3c\x3e\x66\x45\x0f\xef\xfd\x66\x45\x0f\x38\xde\xfc\x66\x45\x0f\x38\xde\xfb\x66\x45\x0f\x38\xde\xfa\x66\x45\x0f\x38\xde\xf9\x66\x45\x0f\x38\xde\xf8\x66\x44\x0f\x38\xde\xff\x66\x44\x0f\x38\xde\xfe\x66\x44\x0f\x38\xde\xfd\x66\x44\x0f\x38\xde\xfc\x66\x44\x0f\x38\xdf\xf8\xf3\x44\x0f\x7f\x3c\x3e\x48\x83\xc7\x10\xeb\xa8\x48\x31\xc0\x48\x31\xff\x48\x31\xf6\x48\x31\xd2\x4d\x31\xf6\x48\x31\xc9"

# define DECRYPT_TEXT_PE "\x41\xbd\xef\xbe\xad\xde\x41\xbc\xef\xbe\xad\xde\x41\xbb\xef\xbe\xad\xde\x48\x8d\x3d\xe7\xff\xff\xff\x49\xf7\xdd\x4c\x01\xef\x4c\x01\xdf\x4c\x01\xd7\x4d\x89\xde\x49\xf7\xda\x4d\x01\xd4\x48\x31\xc9\x48\xff\xc1\x41\x80\x36\x42\x4d\x8d\x76\x01\x4c\x39\xe1\x7e\xf0\x48\x31\xc0\x48\x31\xff\x48\x31\xf6\x48\x31\xd2\x4d\x31\xf6\x48\x31\xc9"


# if WOODY_DEBUG == 1
#  define NEW_EP_OFFSET (41 + 4)
#  define TEXT_LENGTH_OFFSET (47 + 4)
#  define TEXT_OFFSET_OFFSET (53 + 4)
#  define KEY1_OFFSET_OFFSET (59 + 4)
#  define KEY2_OFFSET_OFFSET (69 + 4)
#  define PAYLOAD DEBUG_PAYLOAD DECRYPT_TEXT PRINT_WOODY_PAYLOAD
# else
#  define NEW_EP_OFFSET 41
#  define TEXT_LENGTH_OFFSET 47
#  define TEXT_OFFSET_OFFSET 53
#  define KEY1_OFFSET_OFFSET 59
#  define KEY2_OFFSET_OFFSET 69
#  define PAYLOAD DECRYPT_TEXT PRINT_WOODY_PAYLOAD
# endif
#  define TEXT_LENGTH_OFFSET_PE 8
#  define TEXT_OFFSET_OFFSET_PE 14
#  define PAYLOAD_PE ADDR_PAYLOAD  DECRYPT_TEXT_PE

int			verif_header(char *file, off_t size);
char		*get_file(char *name, off_t *file_size);
off_t 		make_place(char **new_file, off_t *file_size, off_t code_size);
off_t 		metamorph_segment(char *file, off_t wanted_address, off_t wanted_size, off_t virt_addr);
off_t		new_section(char **new_file, off_t *file_size, off_t code_size);
int			is_sect_exec(char *file, off_t file_size, off_t entry_point, int len);
Elf64_Shdr	*find_sect(char *elf, const char *sect, off_t file_size);
void		encrypt_section(char *file, Elf64_Shdr *header, char *key);
void		print_section(char *file, Elf64_Shdr *header);
off_t		find_cave(char *file, off_t file_size, off_t asked_length, off_t *cave_size);
char		*inject_code(char *file, off_t *file_size, Elf64_Shdr *section, char *key);
int			write_to_file(char *file_name, char *content, off_t content_size);
void		generate_key(char *key);
int			is_infected(char *file, size_t file_size);
Elf64_Addr	get_virt_addr(char *file, off_t file_size, int *error, off_t *offset_max);
char 		*get_new_file(char *old_file, off_t file_size);
// ASM
void encrypt(char *key, char *value, size_t len);
 // PE
p_pack		*init_struct(void);
int 	 	find_sect_pe(char *file, const char *sect, p_pack *pp, off_t file_size);
int 		find_section_of_cave(char *file, off_t cave_entry, p_pack *pp);
void		encrypt_section_pe(char *file, p_pack *pp);
char		*inject_code_pe(char *file, off_t *file_size, p_pack *pp);
off_t		find_cave_pe(char *file, off_t file_size, off_t asked_length, off_t *cave_size, off_t *offset_max);
#endif
