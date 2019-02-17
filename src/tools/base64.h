#ifndef _SRC_TOUCH_BASE64_H_
#define _SRC_TOUCH_BASE64_H_

#include <stdlib.h>
#include <string.h>

unsigned char * base64_encode(unsigned char *src, size_t len,
			      size_t *out_len);

unsigned char * base64_decode(unsigned char *src, size_t len,
			      size_t *out_len);

#endif /* _SRC_TOUCH_BASE64_H_ */
