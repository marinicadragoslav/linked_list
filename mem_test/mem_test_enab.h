#ifndef MEM_TEST_ENAB_H
#define MEM_TEST_ENAB_H

/* Override standard library functions for testing */
#define malloc  MtMalloc
#define free    MtFree

#include "mem_test.h"

#endif /* MEM_TEST_ENAB_H */
