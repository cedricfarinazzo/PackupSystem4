#include <stdlib.h>
#include <stdio.h>

#include "../src/tools/base62.h"

#include <criterion/criterion.h>

Test(Base62, Encode)
{

}

Test(Base62, Decode)
{

    char text[] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
    
    size_t len = strlen(text);
    
    size_t elen;
    char *encode = base62_encode(text, len, &elen);
    
    cr_expect_not_null(encode);
    cr_expect_neq(elen, 0);
    cr_expect_str_not_empty(encode);
        
    size_t dlen;
    char *decode = base62_decode(encode, elen, &dlen);
    
    cr_expect_not_null(decode);
    cr_expect_neq(dlen, 0);
    cr_expect_str_not_empty(decode);
    cr_expect_str_eq(decode, text);
    cr_expect_eq(strlen(decode), len);

    if (encode != NULL)
        free(encode);
    if (decode != NULL)
        free(decode);
    
}
