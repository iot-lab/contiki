#ifndef __MY_ENCODE__
#define __MY_ENCODE__

//#ifndef COPY
#include <stdint.h>
//#endif

int
encode(uint8_t* txt, int len, char* encoded);

int
decode(char* encoded, int len,  uint8_t* txt);

#endif
