#include "mem_test.h"

typedef enum
{
    FALSE = 0,
    TRUE
}Bool_t;

void* remainingAllocs[MAX_NUM_ALLOCS];
unsigned int numRemainingAllocs = 0;
unsigned int invalidFreeAttempts = 0;

static void AddAlloc(void* ptr);
static void RemoveAlloc(void* ptr);

void* mtMalloc(size_t size)
{
    void* ret = malloc(size);

    if (ret)
    {
        AddAlloc(ret);
    }

    return ret;
}

void mtFree(void* ptr)
{
    RemoveAlloc(ptr);
    free(ptr);
}

mTestStatus_t mtGetStatus(void)
{
    if (numRemainingAllocs == 0 && invalidFreeAttempts == 0)
    {
        return MT_PASSED;
    }

    if (invalidFreeAttempts > 0)
    {
        return MT_INVALID_FREE_ATTEMPTS;
    }

    return MT_ALLOCS_NOT_FREED;
}

char* mTestStatusToString(mTestStatus_t status)
{
    switch(status)
    {
        case MT_PASSED:
            return "MT_PASSED";
        case MT_ALLOCS_NOT_FREED:
            return "MT_ALLOCS_NOT_FREED";
        case MT_INVALID_FREE_ATTEMPTS:
            return "MT_INVALID_FREE_ATTEMPTS";
        default:
            return "NO_STATUS";
    }
}

static void AddAlloc(void* ptr)
{
    remainingAllocs[numRemainingAllocs++] = ptr;
}

static void RemoveAlloc(void *ptr)
{
    if ((ptr == NULL) || (numRemainingAllocs == 0))
    {
        // This pointer was not allocated, or it was already deallocated
        invalidFreeAttempts++;
        return;
    }

    // Search for ptr in the array of allocations
    for (unsigned int i = 0; i < numRemainingAllocs; i++)
    {
        if (remainingAllocs[i] == ptr)
        {
            // Remove the first match of ptr from the array and exit function
            for (unsigned int j = i; j < (numRemainingAllocs - 1); j++)
            {
                remainingAllocs[j] = remainingAllocs[j + 1];
            }
            numRemainingAllocs--;
            return;
        }
    }

    // ptr not found
    invalidFreeAttempts++;
}

