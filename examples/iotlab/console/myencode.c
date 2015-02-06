#include "myencode.h"

#include <string.h>

int
encode(uint8_t* txt, int len, char* encoded)
{
	int i, j, nb;
	j = 0;
	nb = 0;
	for (i = 0 ; i < len ; i++) {
		uint8_t c = txt[i];
		uint8_t tmp = c & 0x0f; // same as c % 16 
		encoded[j++] = (tmp + 'A');
		c = c >> 4; // same as c / 16
		encoded[j++] = (c + 'A');
		nb+=2;
		if ((nb % 80) == 0)  { 
			encoded[j++] = '\n';
		}
	}
	return j;
}


int
decode(char* encoded, int len,  uint8_t* txt)
{
	int i, j;
	j = 0;
	for (i = 0 ; i < len ; i++) {
		if (encoded[i] == '\n') continue; 		
		uint8_t c1 = encoded[i++] - 'A';
		uint8_t c2 = encoded[i] - 'A';
		txt[j++] = (c2 << 4) | c1;
	}
	txt[j] = 0;
	return j;
}
