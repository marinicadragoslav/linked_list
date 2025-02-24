#ifndef MEM_TEST_H
#define MEM_TEST_H

#include <stdlib.h>

typedef enum
{
    MT_PASSED = 0,             /* allocated memory was correctly freed */
    MT_ALLOCS_NOT_FREED,       /* there is allocated memory that was not freed */
    MT_INVALID_FREE_ATTEMPTS,  /* there were attempts to free unallocated memory */
}MtStatus_t;

void* MtMalloc(size_t size);
void MtFree(void* ptr);
MtStatus_t MtGetStatus(void);
char* MtStatusToString(MtStatus_t status);


#endif /* MEM_TEST_H */