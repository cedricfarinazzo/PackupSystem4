#ifndef _SRC_COMPRESSION_LZ78_DICO_H_
#define _SRC_COMPRESSOPM_LZ78_DICO_H_

struct dico *new_dico(int len);

void transfert(unsigned char *src, unsigned char *dst, int len);

void extension_dico(struct dico *Dico);

void free_dico(struct dico *Dico);

#endif /*_SRC_COMPRESSION_LZ78_DICO_H_*/
