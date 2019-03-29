#include "woody.h"

void    print_pe_section(char *file, DWORD offset, DWORD size)
{
	off_t index;

	index = 0;
	while (index < size)
	{
		printf(" %02x%02x%02x%02x ", *((unsigned char *)file + offset + index),
			   *((unsigned char *)file + offset + index + 1),
			   *((unsigned char *)file + offset + index + 2),
			   *((unsigned char *)file + offset + index + 3));
		if ( index == 12 || (index && !((index + 4) % 16)))
			printf("\n");
		fflush(stdout);
		index += 4;
	}
	printf("\n");
}

off_t get_tls_callback(char *file, const char *sect, off_t file_size, p_pack *pp)
{
	WORD numSections, optHeaderSize;
	unsigned const char *opt_header, *secTable, *tlsDir, *dataDir;
	unsigned int  i;
	char *sec; 
	char secName[9] = { };
	off_t offCallBack, voffSect, vsizeSect, ra_off_tls, offSect, tls_directory_rva;
	
	i = 0;
	numSections = READ_WORD(file + READ_DWORD(file + 0x3c) + 4  +2);
	optHeaderSize = READ_WORD(file + READ_DWORD(file + 0x3c) + 4  +16);
	opt_header = file + READ_DWORD(file + 0x3c) + 4 + 20;

	if (numSections == 0 || optHeaderSize == 0)
		return 0;
	secTable = opt_header + optHeaderSize; 
	dataDir = (opt_header + 96);

	tls_directory_rva = READ_DWORD(dataDir + 0x58) ; 

	while (i < numSections)
	{
		sec = (unsigned char *)(secTable + 40 * i);
		memcpy(secName, sec, 8);
		secName[8] = 0;
		offSect = READ_DWORD(sec + 20);
		voffSect = READ_DWORD(sec + 12);
		vsizeSect = READ_DWORD(sec + 8);
			
		if (tls_directory_rva > voffSect && tls_directory_rva < voffSect + vsizeSect)
		{
			ra_off_tls =  tls_directory_rva - voffSect + offSect;
			break;
		}
			i++;
	}


	tlsDir = file +READ_DWORD (sec + 20) + 0x1c ;  
	offCallBack = ra_off_tls + 24;
	pp->offset_tls_callback = offCallBack;
	return 0;
}

int get_info_text(char *file, const char *sect, off_t file_size, p_pack *pp)
{
	WORD numSections, optHeaderSize;
	unsigned const char *opt_header, *secTable;
	unsigned int sizeSectText, i;
	char *sec; 
	char secName[9] = { };
	off_t offSectText;
	
	i = 0;
	numSections = READ_WORD(file + READ_DWORD(file + 0x3c) + 4  +2);
	optHeaderSize = READ_WORD(file + READ_DWORD(file + 0x3c) + 4  +16);
	opt_header = file + READ_DWORD(file + 0x3c) + 4 + 20;

	pp->virtual_address = READ_QWORD(opt_header + 24); 

	if (numSections == 0 || optHeaderSize == 0)
		return -1;
	secTable = opt_header + optHeaderSize; 

	while (i < numSections)
	{
		sec = (unsigned char *)(secTable + 40 * i);
		memcpy(secName, sec, 8);
		secName[8] = 0;

		if (strcmp(secName, sect) == 0 )
			break;
		i++;
	}

	sizeSectText = READ_DWORD(sec + 16);
	offSectText = READ_DWORD(sec + 20);

	pp->offset_permissions_text = READ_DWORD(file + 0x3C) + 4 + 20 + optHeaderSize + 40 * i  + 36;
	pp->value_permissions_text = READ_DWORD(sec+ 36) | 0x80000000 | 0x20000000;
	pp->offset_section_text = offSectText;
	pp->size_section_text = sizeSectText;
	pp->va_text = READ_DWORD(sec + 12);
	return 0;
}

int find_section_of_cave(char *file, off_t cave_entry, p_pack *pp)
{
	WORD numSections, optHeaderSize;
	unsigned const char *opt_header, *secTable;
	unsigned int sizeSectText, i;
	char *sec; 
	off_t offSect, sizeSect, va;
	
	i = 0;
	numSections = READ_WORD(file + READ_DWORD(file + 0x3c) + 4  +2);
	optHeaderSize = READ_WORD(file + READ_DWORD(file + 0x3c) + 4  +16);
	opt_header = file + READ_DWORD(file + 0x3c) + 4 + 20;

	if (numSections == 0 || optHeaderSize == 0)
		return -1;
	secTable = opt_header + optHeaderSize; 

	while (i < numSections)
	{
		sec = (unsigned char *)(secTable + 40 * i);
		offSect = READ_DWORD(sec + 20);
		sizeSect = READ_DWORD(sec + 16);
		va = READ_DWORD(sec + 12);
		if (cave_entry < offSect + sizeSect)
			break;
		i++;
	}
	printf ("\033[32;1mcave entry is in %s   rva : %lx\n\033[0m",sec,cave_entry - offSect + va);
	pp->offset_permissions = READ_DWORD(file + 0x3C) + 4 + 20 + optHeaderSize + 40 * i  + 36;
	pp->value_permissions = READ_DWORD(sec+ 36) | 0x80000000 | 0x20000000;
	pp->rva = cave_entry - offSect + va;
	return 0;
}

Elf64_Shdr *find_sect_pe(char *file, const char *sect, off_t file_size, p_pack *pp)
{
	Elf64_Shdr *header;
	p_pack ppp;
	unsigned const char *pe, *coff, *opt_header, *dataDir, *secTable;
	WORD numSections, optHeaderSize;
	DWORD vaRes;
	int i = 0;
	char *sec;
	char secName[9];
	char secName1[9];
	off_t entry_point;
	int offset_tls_callback;

	if (get_tls_callback(file, ".tls\0", file_size, pp ) == -1) 
		return NULL;
	
	if (get_info_text(file, ".text", file_size, pp) == -1) 
		return NULL;
	printf ("\033[32;1moffset section text : %x  size : %x\noffset tls callback : %x value : %016x \n\033[0m", pp->offset_section_text, pp->size_section_text, pp->offset_tls_callback, READ_QWORD(file + pp->offset_tls_callback));
	return header;
}