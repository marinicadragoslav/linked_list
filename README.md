# linked_list

## Create and manipulate singly and doubly linked lists.

- Dynamic allocation is used to create list objects that contain node objects. 
- Data is stored in the nodes as void pointers to objects managed by the user.
- Contains tests for each function and for memory management (memory leaks, double-free).
##

Run these commands from the linked_list folder to get started (gcc must be installed):

## Linux
   - Simple example:<br />
      `$ gcc -I . -o ex.out example/example.c linked_list.c`<br />
      `$ ./ex.out`<br />

   - Tests without memory management:<br />
      `$ gcc -I . -o test.out tests/tests.c linked_list.c`<br />
      `$ ./test.out`<br />

   - Tests including memory management:<br />
   1. add `#include "mem_test_enab.h"` in **linked_list.h** after `#define LINKED_LIST_H`<br />
   2. run test:<br />
      `$ gcc -I . -I mem_test/ -o test.out tests/tests.c linked_list.c mem_test/mem_test.c`<br />
      `$ ./test.out`<br />
   3. remove `#include "mem_test_enab.h"`<br />

## Windows
   - Simple example:<br />
      `gcc -o ex.exe -I . .\example\example.c .\linked_list.c`<br />
      `.\ex.exe`<br />

   - Tests without memory management:<br />
      `gcc -o test.exe -I . .\tests\tests.c .\linked_list.c`<br />
      `.\test.exe`<br />

   - Tests including memory management:<br />
   1. add `#include "mem_test_enab.h"` in **linked_list.h** after `#define LINKED_LIST_H`<br />
   2. run test:<br />
      `gcc -o test.exe -I . -I .\mem_test\ .\tests\tests.c .\linked_list.c .\mem_test\mem_test.c`<br />
      `.\test.exe`<br />
   3. remove `#include "mem_test_enab.h"`<br />
<br />
