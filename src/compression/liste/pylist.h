#ifndef _SRC_COMPRESSION_LISTE_PYLIST_H_
#define _SRC_COMPRESSION_LISTE_PYLIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "../struct.h"
#include "pylist.h"

struct pylist *new_py(void);

void addpy(struct pylist *py, size_t element);

void freepy(struct pylist *py);
