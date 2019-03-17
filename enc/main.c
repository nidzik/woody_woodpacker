#include <stdio.h>

void encrypt();
void decrypt();

void print_value(char *value)
{
	printf("%2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x\n",
		value[0], value[1], value[2], value[3], value[4],
		value[5], value[6], value[7], value[8], value[9],
		value[10], value[11], value[12], value[13], value[14],
		value[15], value[16]);
}

int main()
{
	char value[16] = "0123456789123456";

	print_value(value);
	encrypt();
	print_value(value);
	decrypt();
	print_value(value);
	return (0);
}
