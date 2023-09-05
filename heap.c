#include <stdlib.h>
#include "variable.h"

#define sizeof_heap 255

unsigned char xdata arr_heap[sizeof_heap] = {0};

void init_heap(void)
{
    init_mempool((void _MALLOC_MEM_ *)arr_heap, sizeof_heap);
}

void _MALLOC_MEM_ *malloctest(size_t number)
{
    return malloc(number);
}