#ifndef VHM_MALLOC_H
#define VHM_MALLOC_H

#include <stdlib.h>

// Keeps allocation methods relatively similar, but removes the need for the developer to explicitly cast to fit the C-style.

#define vhm_mnew(type) (type*) malloc(sizeof(type))
#define vhm_cnew(type, size) (type*) calloc(sizeof(type), size)
#define vhm_mfree(pointer) free((void*) pointer)

#endif // VHM_MALLOC_H