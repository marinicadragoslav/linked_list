
#ifndef MEM_MANAGEMENT_TEST_H
#define MEM_MANAGEMENT_TEST_H

#include <stdlib.h>

#define MAX_NUM_ALLOCS  1000

typedef enum
{
    MMT_PASSED = 0,             // allocated memory was correctly freed
    MMT_ALLOCS_NOT_FREED,       // there is allocated memory that was not freed
    MMT_INVALID_FREE_ATTEMPTS,  // there were attempts to free unallocated memory
}mmTestStatus_t;

void* mmtMalloc(size_t size);
void mmtFree(void* ptr);
mmTestStatus_t mmtGetStatus(void);
char* mmTestStatusToString(mmTestStatus_t status);


#endif /* MEM_MANAGEMENT_TEST_H */