#include "woody.h"

int build_payload_pe(char *new_file, char *code, off_t code_len, off_t virt_addr, off_t cave_entry, p_pack *pp)
{
	off_t offset;
	QWORD va_cave_entry, va_text;

	offset = cave_entry;
	va_cave_entry = pp->rva + pp->virtual_address;
	va_text = pp->va_text + pp->virtual_address;

	memcpy(new_file + pp->offset_tls_callback, &(va_cave_entry), sizeof(QWORD));

	va_cave_entry += 8;
	memcpy(new_file + pp->offset_permissions_text, &(pp->value_permissions_text),4);
	memcpy(new_file + pp->offset_permissions, &(pp->value_permissions),4);
	memcpy(code, &(va_cave_entry), 8);
	memcpy(code + TEXT_LENGTH_OFFSET_PE + 8, &(pp->size_section_text), sizeof(int));
	memcpy(code + TEXT_OFFSET_OFFSET_PE + 8, &(va_text), sizeof(int));

	memcpy(new_file + offset - virt_addr, code, code_len);

	return 1;
}


char *inject_code_pe(char *file, off_t *file_size, p_pack *pp)
{
	off_t virt_addr;
	char *new_file;
	char code[] = PAYLOAD_PE;
	off_t cave_size;
	off_t cave_entry=0;
	off_t offset_max;

	virt_addr = pp->virtual_address;
	offset_max = pp->size_section_text + pp->offset_section_text;

	new_file = get_new_file(file, *file_size);

	cave_entry = find_cave_pe(file, *file_size, sizeof(code) , &cave_size, &offset_max);
	find_section_of_cave(file, cave_entry, pp);
	printf(" * code cave finded, offset: 0x%jx, size: 0x%jx\n", cave_entry, cave_size);

	cave_entry += virt_addr;

	build_payload_pe(new_file, code, sizeof(code) - 1, virt_addr, cave_entry, pp);

	return (new_file);
}
