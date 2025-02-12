# linked_list

## Create and manipulate singly and doubly linked lists.

- Dynamic allocation is used to create list objects that contain node objects. 
- Data is stored as void pointers to objects managed by the user.
- Contains tests for each function and for memory management (memory leaks, double-free).
- Contains a simple usage example
##

Run these commands from the linked_list folder to get started (gcc must be installed):

## Simple example
   - Linux:<br />
      `$ gcc -I . -o ex.out example/example.c linked_list.c`<br />
      `$ ./ex.out`<br />
   - Windows:<br />
      `gcc -o ex.exe -I . .\example\example.c .\linked_list.c`<br />
      `.\ex.exe`<br />

## Tests without memory management:
   - Linux:<br />
      `$ gcc -I . -o test.out tests/tests.c linked_list.c`<br />
      `$ ./test.out`<br />
   - Windows:<br />
      `gcc -o test.exe -I . .\tests\tests.c .\linked_list.c`<br />
      `.\test.exe`<br />

## Tests with memory management:
   - add `#include "mem_test_enab.h"` in <ins>linked_list.h</ins> after `#define LINKED_LIST_H`<br />
   - run test:<br />
        - Linux:<br />
              `$ gcc -I . -I mem_test/ -o test.out tests/tests.c linked_list.c mem_test/mem_test.c`<br />
              `$ ./test.out`<br />
        - Windows:<br />
              `gcc -o test.exe -I . -I .\mem_test\ .\tests\tests.c .\linked_list.c .\mem_test\mem_test.c`<br />
              `.\test.exe`<br />   
   - remove `#include "mem_test_enab.h"`<br />   
<br />
