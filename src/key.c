#include "woody.h"

void generate_key(char *key)
{
    unsigned int first;
    unsigned int second;
    unsigned int third;
    unsigned int fourth;

    first = rand();
    second = rand();
    third = rand();
    fourth = rand();
    memcpy(key, &first, sizeof(int));
    memcpy(key + 4, &second, sizeof(int));
    memcpy(key + 8, &third, sizeof(int));
    memcpy(key + 12, &fourth, sizeof(int));
    printf(" * Key: 0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n",
        (unsigned char)key[0],
        (unsigned char)key[1],
        (unsigned char)key[2],
        (unsigned char)key[3],
        (unsigned char)key[4],
        (unsigned char)key[5],
        (unsigned char)key[6],
        (unsigned char)key[7],
        (unsigned char)key[8],
        (unsigned char)key[9],
        (unsigned char)key[10],
        (unsigned char)key[11],
        (unsigned char)key[12],
        (unsigned char)key[13],
        (unsigned char)key[14],
        (unsigned char)key[15]
    );
    return ;
}