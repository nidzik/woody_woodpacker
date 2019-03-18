#include <stdio.h>

void encrypt(char *key, char *text);
void decrypt(char *key, char *text);

void print_value(char *value)
{
	printf("%2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x\n",
		   (unsigned char)value[0], (unsigned char)value[1], (unsigned char)value[2], (unsigned char)value[3], (unsigned char)value[4],
		   (unsigned char)value[5], (unsigned char)value[6], (unsigned char)value[7], (unsigned char)value[8], (unsigned char)value[9],
		   (unsigned char)value[10], (unsigned char)value[11], (unsigned char)value[12], (unsigned char)value[13], (unsigned char)value[14],
		   (unsigned char)value[15]);
}

int main()
{
	char value[] = "0123456789123456";
	char key[] = "0123456789123456";

	printf("KEY       : %s\n", key);
	printf("value     : ");
	print_value(value);
	encrypt(key, value);
	printf("KEY       : %s\n", key);
	printf("encrypted : ");
	print_value(value);
	decrypt(key, value);
	printf("KEY       : %s\n", key);
	printf("decrypted : ");
	print_value(value);
	return (0);
}
