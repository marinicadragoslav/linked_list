# linked_list

## Create and manipulate singly and doubly linked lists.

- Dynamic allocation is used to create list objects that contain node objects. 
- Data is stored in the nodes as void pointers to objects managed by the user.  
- Contains tests for each function and for memory management (memory leaks, double-free).
##  

  
Run these commands from the linked_list folder to get started (gcc must be installed):

## Linux
   - Simple example:  
   `$ gcc -I . -o example.out example/example.c linked_list.c`  
   `$ ./example.out`  
     
   - Tests without memory management:  
    `$ gcc -I . -o tests.out tests/tests.c linked_list.c`  
    `$ ./tests.out`  
    
   - Tests including memory management: (first set this to 1: `#define MEM_MANAGEMENT_TEST 0` in **linked_list.h**)  
   `$ gcc -I . -I mem_management_test/ -o tests.out tests/tests.c linked_list.c mem_management_test/mem_management_test.c`  
   `$ ./tests.out`  
      *Don't forget to set MEM_MANAGEMENT_TEST back to 0 when done!*  
    
## Windows
   - Simple example:  
   `gcc -o example.exe -I . .\example\example.c .\linked_list.c`  
   `.\example.exe`  
     
   - Tests without memory management:  
    `gcc -o tests.exe -I . .\tests\tests.c .\linked_list.c`  
    `.\tests.exe`  
    
   - Tests including memory management: (first set this to 1: `#define MEM_MANAGEMENT_TEST 0` in **linked_list.h**)  
   `gcc -o tests.exe -I . -I .\mem_management_test\ .\tests\tests.c .\linked_list.c .\mem_management_test\mem_management_test.c`  
   `.\tests.exe`  
     *Don't forget to set MEM_MANAGEMENT_TEST back to 0 when done!*  
