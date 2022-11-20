
#ifndef MEM_TEST_H
#define MEM_TEST_H

#include <stdlib.h>

#define MAX_NUM_ALLOCS  1000

typedef enum
{
    MT_PASSED = 0,             // allocated memory was correctly freed
    MT_ALLOCS_NOT_FREED,       // there is allocated memory that was not freed
    MT_INVALID_FREE_ATTEMPTS,  // there were attempts to free unallocated memory
}mTestStatus_t;

void* mtMalloc(size_t size);
void mtFree(void* ptr);
mTestStatus_t mtGetStatus(void);
char* mTestStatusToString(mTestStatus_t status);


#endif /* MEM_TEST_H */