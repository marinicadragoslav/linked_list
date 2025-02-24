#include "mem_test.h"

#define MAX_NUM_ALLOCS  1000

static void* RemainingAllocs[MAX_NUM_ALLOCS];
static unsigned int NumRemainingAllocs = 0;
static unsigned int InvalidFreeAttempts = 0;

static void _AddAlloc(void* Ptr);
static void _RemoveAlloc(void* Ptr);

void* MtMalloc(size_t Size)
{
    void* Ret = malloc(Size);

    if (Ret)
    {
        _AddAlloc(Ret);
    }

    return Ret;
}

void MtFree(void* Ptr)
{
    _RemoveAlloc(Ptr);
    free(Ptr);
}

MtStatus_t MtGetStatus(void)
{
    if (NumRemainingAllocs == 0 && InvalidFreeAttempts == 0)
    {
        return MT_PASSED;
    }

    if (InvalidFreeAttempts > 0)
    {
        return MT_INVALID_FREE_ATTEMPTS;
    }

    return MT_ALLOCS_NOT_FREED;
}

char* MtStatusToString(MtStatus_t Status)
{
    switch(Status)
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

static void _AddAlloc(void* Ptr)
{
    RemainingAllocs[NumRemainingAllocs++] = Ptr;
}

static void _RemoveAlloc(void *Ptr)
{
    if ((Ptr == NULL) || (NumRemainingAllocs == 0))
    {
        /* This pointer was not allocated, or it was already deallocated */
        InvalidFreeAttempts++;
        return;
    }

    unsigned int i, j;
    
    /* Search for Ptr in the array of allocations */
    for (i = 0; i < NumRemainingAllocs; i++)
    {
        if (RemainingAllocs[i] == Ptr)
        {
            /* Remove the first match of Ptr from the array and exit function */
            for (j = i; j < (NumRemainingAllocs - 1); j++)
            {
                RemainingAllocs[j] = RemainingAllocs[j + 1];
            }
            NumRemainingAllocs--;
            return;
        }
    }

    /* Ptr not found */
    InvalidFreeAttempts++;
}

