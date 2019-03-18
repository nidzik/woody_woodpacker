#include <stdio.h>

void encrypt(char *key, char *text, size_t size);
void decrypt(char *key, char *text, size_t size);

void print_value(char *value, size_t size)
{
	size_t index;

	index = 0;
	while (index < size)
	{
		printf("%2x ", (unsigned char)value[index]);
		index += 1;
	}
	printf("\n");
}

int main()
{
	char value[] = "0123456789123456012345678912345";
	char key[] = "0123456789123456012345678912345";

	printf("KEY       : %s\n", key);
	printf("value     : ");
	print_value(value, sizeof(value));
	encrypt(key, value, sizeof(value));
	printf("KEY       : %s\n", key);
	printf("encrypted : ");
	print_value(value, sizeof(value));
	decrypt(key, value, sizeof(value));
	printf("KEY       : %s\n", key);
	printf("decrypted : ");
	print_value(value, sizeof(value));
	return (0);
}
