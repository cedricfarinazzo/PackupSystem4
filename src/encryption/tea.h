#ifndef _SRC_ENCRYPTION_TEA_H_
#define _SRC_ENCRYPTION_TEA_H_

#include <stddef.h>
#include <stdint.h>

#define BUFFER_SIZE 512

void TEA_encrypt(uint32_t* v, uint32_t* k);

void TEA_encrypt(uint32_t* v, uint32_t* k);

#endif /* _SRC_ENCRYPTION_ROTN_H_ */
