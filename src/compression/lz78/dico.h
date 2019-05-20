#ifndef _SRC_COMPRESSION_LZ78_DICO_H_
#define _SRC_COMPRESSOPM_LZ78_DICO_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../struct.h"
#include "../liste/liste.h"
#include "../liste/pylist.h"

struct dico *new_dico(size_t len, size_t taux);

void transfert_vector(size_t *src, size_t *dst, size_t len);

void transfert_letter(unsigned char *src, unsigned char *dst, size_t len);

void extension_dico(struct dico *Dico);

void free_dico(struct dico *Dico);

#endif /*_SRC_COMPRESSION_LZ78_DICO_H_*/
