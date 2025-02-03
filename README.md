# linked_list

## Create and manipulate singly and doubly linked lists.

- Dynamic allocation is used to create list objects that contain node objects. 
- Data is stored in the nodes as void pointers to objects managed by the user.
- Contains tests for each function and for memory management (memory leaks, double-free).
##


Run these commands from the linked_list folder to get started (gcc must be installed):

## Linux
   - Simple example:
      `$ gcc -I . -o ex.out example/example.c linked_list.c`
      `$ ./ex.out`

   - Tests without memory management:
      `$ gcc -I . -o test.out tests/tests.c linked_list.c`
      `$ ./test.out`

   - Tests including memory management:
      1. add `#include "mem_test_enab.h"` in **linked_list.h** after '#define LINKED_LIST_H'
      2. run test:
         `$ gcc -I . -I mem_test/ -o test.out tests/tests.c linked_list.c mem_test/mem_test.c`
         `$ ./test.out`
      3. remove `#include "mem_test_enab.h"`

## Windows
   - Simple example:
      `gcc -o ex.exe -I . .\example\example.c .\linked_list.c`
      `.\ex.exe`

   - Tests without memory management:
      `gcc -o test.exe -I . .\tests\tests.c .\linked_list.c`
      `.\test.exe`

   - Tests including memory management:
   1. add `#include "mem_test_enab.h"` in **linked_list.h** after '#define LINKED_LIST_H'
   2. run test:
      `gcc -o test.exe -I . -I .\mem_test\ .\tests\tests.c .\linked_list.c .\mem_test\mem_test.c`
      `.\test.exe`
   3. remove `#include "mem_test_enab.h"`
