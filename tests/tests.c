#include <stdio.h>
#include "linked_list.h"

typedef struct TestData
{
    int id;
    float age;
    char* name[10];
}TestData_t;

// Test helper functions
static void TestStart(const char* const header);
static void TestEnd(void);
static void ExpectResponse(ListStatus_t actual, ListStatus_t expected);
static void ExpectPointerNull(void* pointer);
static void ExpectPointerNotNull(void* pointer);
static void ExpectEqual(unsigned int this, unsigned int toThis);
static void ExpectEqualPtr(void* this, void* toThis);
static void ExpectNotEqualPtr(void* this, void* toThis);
static void ExpectEmptyList(List_t* List);
static void ExpectListWith1Node(List_t* List, int id1);
static void ExpectListWith2Nodes(List_t* List, int id1, int id2);
static void ExpectListWith3Nodes(List_t* List, int id1, int id2, int id3);
static void ExpectListWith4Nodes(List_t* List, int id1, int id2, int id3, int id4);
static void ExpectListWith5Nodes(List_t* List, int id1, int id2, int id3, int id4, int id5);

// Test report variables
unsigned int numFailedSubpoints = 0;
unsigned int numFailedTests = 0;
int testStartEndBalance = 0;

// Test data to insert in lists
TestData_t dummyData = {0};
TestData_t testData[5] = {{.id = 101}, {.id = 102}, {.id = 103}, {.id = 104}, {.id = 105}};

int main(void)
{
    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 1: Create lists with different types of linkage");
    {
        // Create valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);
        ExpectEqual(dList->Linkage, LL_DOUBLE);

        // Create valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);
        ExpectEqual(sList->Linkage, LL_SINGLE);

        // Create an invalid list -> should return NULL
        List_t* invList = NewList(5); // argument out of range 
        ExpectPointerNull(invList);

        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
        ExpectResponse(DeleteList(sList), LL_OK);
        ExpectResponse(DeleteList(invList), LL_NOT_OK); // can't delete a NULL list
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 2: NULL list argument => return NULL node");
    {
        List_t* nullList = NULL;
        ExpectPointerNull(GetHead(nullList));
        ExpectPointerNull(GetTail(nullList));
        ExpectPointerNull(GetNodeByData(nullList, &dummyData));
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 3: NULL list argument => return LL_NOT_OK");
    {
        List_t* nullList = NULL;
        unsigned int count;
        ExpectResponse(AddToFront(nullList, &dummyData), LL_NOT_OK);
        ExpectResponse(AddToBack(nullList, &dummyData), LL_NOT_OK);
        ExpectResponse(InsertAfterData(nullList, &dummyData, &dummyData), LL_NOT_OK);
        ExpectResponse(RemoveHead(nullList), LL_NOT_OK);
        ExpectResponse(RemoveTail(nullList), LL_NOT_OK);
        ExpectResponse(RemoveNodeByData(nullList, &dummyData), LL_NOT_OK);
        ExpectResponse(GetCount(nullList, &count), LL_NOT_OK);
        ExpectResponse(DeleteList(nullList), LL_NOT_OK);
    }
    TestEnd();
    
    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 4: NULL node argument => return NULL pointer");
    {
        ListNode_t* nullNode = NULL;
        ExpectPointerNull(GetNext(nullNode));
        ExpectPointerNull(GetPrev(nullNode));
        ExpectPointerNull(GetData(nullNode));
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 5: NULL node argument => return LL_NOT_OK");
    {
        ListNode_t* nullNode = NULL;
        ExpectResponse(InsertAfterNode(nullNode, &dummyData), LL_NOT_OK);
        ExpectResponse(RemoveNode(nullNode), LL_NOT_OK);
        ExpectResponse(SetData(nullNode, &dummyData), LL_NOT_OK);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 6: NULL data argument => return NULL node");
    {
        void* nullData = NULL;

        // Create a valid empty list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: add at least one node, otherwise GetNodeByData will 
        // return NULL because of the empty list rather than the NULL data.
        ExpectResponse(AddToFront(sList, &dummyData), LL_OK);
        ExpectListWith1Node(sList, dummyData.id);
        
        // Test
        ExpectPointerNull(GetNodeByData(sList, nullData));

        // Cleanup
        ExpectResponse(DeleteList(sList), LL_OK);
    }
    TestEnd();
    
    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 7: NULL data argument => return LL_NOT_OK");
    {
        void* nullData = NULL;

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Try inserting NULL data into an empty list
        ExpectResponse(AddToFront(sList, nullData), LL_NOT_OK);
        ExpectResponse(AddToBack(sList, nullData), LL_NOT_OK);

        // Insert one node
        ExpectResponse(AddToFront(sList, &dummyData), LL_OK);
        ExpectListWith1Node(sList, dummyData.id);

        // Try inserting NULL data after a valid node (list must have at least one node)
        ExpectResponse(InsertAfterNode(GetHead(sList), nullData), LL_NOT_OK);

        // Try inserting NULL data after valid data (list must have at least one node)
        ExpectResponse(InsertAfterData(sList, GetData(GetHead(sList)), nullData), LL_NOT_OK);

        // Try inserting valid data after NULL data (list must have at least one node)
        ExpectResponse(InsertAfterData(sList, nullData, GetData(GetHead(sList))), LL_NOT_OK);

        // Try removing NULL data from a list that has at least one node.
        ExpectResponse(RemoveNodeByData(sList, nullData), LL_NOT_OK);

        // Try setting the data of an existing node to NULL
        ExpectResponse(SetData(GetHead(sList), nullData), LL_NOT_OK);

        // Cleanup
        ExpectResponse(DeleteList(sList), LL_OK);
    }    
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 8: NULL counter argument => return LL_NOT_OK");
    {
        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Test
        unsigned int* count_ptr = NULL;
        ExpectResponse(GetCount(sList, count_ptr), LL_NOT_OK);

        // Cleanup
        ExpectResponse(DeleteList(sList), LL_OK);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 9: Empty list argument => return NULL node");
    {
        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Test
        ExpectPointerNull(GetHead(sList));
        ExpectPointerNull(GetTail(sList));
        ExpectPointerNull(GetNodeByData(sList, &dummyData));

        // Cleanup
        ExpectResponse(DeleteList(sList), LL_OK);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 10: Remove from empty list => return LL_NOT_OK");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);

        // Test
        ExpectResponse(RemoveHead(dList), LL_NOT_OK);
        ExpectResponse(RemoveTail(dList), LL_NOT_OK);
        ExpectResponse(RemoveNodeByData(dList, &dummyData), LL_NOT_OK);
        
        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 11: Insert data after existing data in an empty list => return LL_NOT_OK");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);

        // Test
        ExpectResponse(InsertAfterData(dList, &dummyData, &dummyData), LL_NOT_OK);

        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
    }    
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 12: Insert data to the front/back of an empty list => return LL_OK");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList); 

        // Insert to front
        ExpectResponse(AddToFront(dList, &dummyData), LL_OK);
        ExpectListWith1Node(dList, 0);

        // Remove node
        ExpectResponse(RemoveHead(dList), LL_OK);
        ExpectEmptyList(dList);

        // Insert to back
        ExpectResponse(AddToBack(dList, &dummyData), LL_OK);
        ExpectListWith1Node(dList, 0);

        // Remove node
        ExpectResponse(RemoveTail(dList), LL_OK);
        ExpectEmptyList(dList);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList); 

         // Insert to front
        ExpectResponse(AddToFront(sList, &dummyData), LL_OK);
        ExpectListWith1Node(sList, 0);

        // Remove node
        ExpectResponse(RemoveHead(sList), LL_OK);
        ExpectEmptyList(sList);

        // Insert to back
        ExpectResponse(AddToBack(sList, &dummyData), LL_OK);
        ExpectListWith1Node(sList, 0);

        // Remove node
        ExpectResponse(RemoveTail(sList), LL_OK);
        ExpectEmptyList(sList);
        
        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
        ExpectResponse(DeleteList(sList), LL_OK);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 13: Insert a node to the front of a non-empty list => return LL_OK");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(AddToFront(dList, &testData[0]), LL_OK);
        ExpectListWith1Node(dList, testData[0].id);

        // Test: Insert a node
        ExpectResponse(AddToFront(dList, &testData[1]), LL_OK);
        ExpectListWith2Nodes(dList, testData[1].id, testData[0].id); 

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(AddToFront(sList, &testData[0]), LL_OK);
        ExpectListWith1Node(sList, testData[0].id);

        // Test: Insert a node
        ExpectResponse(AddToFront(sList, &testData[1]), LL_OK);
        ExpectListWith2Nodes(sList, testData[1].id, testData[0].id); 

        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
        ExpectResponse(DeleteList(sList), LL_OK);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 14: Remove nodes from the back of a non-empty list => return LL_OK");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert two nodes
        ExpectResponse(AddToFront(dList, &testData[0]), LL_OK);
        ExpectResponse(AddToFront(dList, &testData[1]), LL_OK);
        ExpectListWith2Nodes(dList, testData[1].id, testData[0].id); 

        // Test
        ExpectResponse(RemoveTail(dList), LL_OK);
        ExpectListWith1Node(dList, testData[1].id);
        ExpectResponse(RemoveTail(dList), LL_OK);
        ExpectEmptyList(dList);

        // Attempt to remove from empty list (already tested - see test 10)
        ExpectResponse(RemoveTail(dList), LL_NOT_OK); 

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert two nodes
        ExpectResponse(AddToFront(sList, &testData[0]), LL_OK);
        ExpectResponse(AddToFront(sList, &testData[1]), LL_OK);
        ExpectListWith2Nodes(sList, testData[1].id, testData[0].id);

        // Test
        ExpectResponse(RemoveTail(sList), LL_OK);
        ExpectListWith1Node(sList, testData[1].id);
        ExpectResponse(RemoveTail(sList), LL_OK);
        ExpectEmptyList(sList);

        // Attempt to remove from empty list (already tested - see test 10)
        ExpectResponse(RemoveTail(sList), LL_NOT_OK); 

        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
        ExpectResponse(DeleteList(sList), LL_OK);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 15: Insert a node to the back of a non-empty list => return LL_OK");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(AddToBack(dList, &testData[0]), LL_OK);
        ExpectListWith1Node(dList, testData[0].id);

        // Test: Insert another node
        ExpectResponse(AddToBack(dList, &testData[1]), LL_OK);
        ExpectListWith2Nodes(dList, testData[0].id, testData[1].id);

        // Test: Insert a third node
        ExpectResponse(AddToBack(dList, &testData[0]), LL_OK);
        ExpectListWith3Nodes(dList, testData[0].id, testData[1].id, testData[0].id);

        // Test: Insert a fourth node
        ExpectResponse(AddToBack(dList, &testData[4]), LL_OK);
        ExpectListWith4Nodes(dList, testData[0].id, testData[1].id, testData[0].id, testData[4].id);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(AddToBack(sList, &testData[0]), LL_OK);
        ExpectListWith1Node(sList, testData[0].id);

        // Test: Insert another node
        ExpectResponse(AddToBack(sList, &testData[1]), LL_OK);
        ExpectListWith2Nodes(sList, testData[0].id, testData[1].id);

        // Test: Insert a third node
        ExpectResponse(AddToBack(sList, &testData[0]), LL_OK);
        ExpectListWith3Nodes(sList, testData[0].id, testData[1].id, testData[0].id);

        // Test: Insert a fourth node
        ExpectResponse(AddToBack(sList, &testData[4]), LL_OK);
        ExpectListWith4Nodes(sList, testData[0].id, testData[1].id, testData[0].id, testData[4].id);

        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
        ExpectResponse(DeleteList(sList), LL_OK);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 16: Remove nodes from the front of a non-empty list => return LL_OK");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert two nodes
        ExpectResponse(AddToBack(dList, &testData[0]), LL_OK);
        ExpectResponse(AddToBack(dList, &testData[1]), LL_OK);
        ExpectListWith2Nodes(dList, testData[0].id, testData[1].id);

        // Test
        ExpectResponse(RemoveHead(dList), LL_OK);
        ExpectListWith1Node(dList, testData[1].id);
        ExpectResponse(RemoveHead(dList), LL_OK);
        ExpectEmptyList(dList);

        // Attempt to remove from empty list (already tested - see test 10)
        ExpectResponse(RemoveHead(dList), LL_NOT_OK);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert two nodes
        ExpectResponse(AddToBack(sList, &testData[0]), LL_OK);
        ExpectResponse(AddToBack(sList, &testData[1]), LL_OK);
        ExpectListWith2Nodes(sList, testData[0].id, testData[1].id);

        // Test
        ExpectResponse(RemoveHead(sList), LL_OK);
        ExpectListWith1Node(sList, testData[1].id);
        ExpectResponse(RemoveHead(sList), LL_OK);
        ExpectEmptyList(sList);

        // Attempt to remove from empty list (already tested - see test 10)
        ExpectResponse(RemoveHead(sList), LL_NOT_OK);

        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
        ExpectResponse(DeleteList(sList), LL_OK);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 17: Insert a node after the head node => return LL_OK");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(AddToBack(dList, &testData[0]), LL_OK);
        ExpectListWith1Node(dList, testData[0].id);

        // Test: Insert a node after head
        ExpectResponse(InsertAfterNode(GetHead(dList), &testData[1]), LL_OK);
        ExpectListWith2Nodes(dList, testData[0].id, testData[1].id);

        // Test: Insert another node after head
        ExpectResponse(InsertAfterNode(GetHead(dList), &testData[2]), LL_OK);
        ExpectListWith3Nodes(dList, testData[0].id, testData[2].id, testData[1].id);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(AddToBack(sList, &testData[0]), LL_OK);
        ExpectListWith1Node(sList, testData[0].id);

        // Test: Insert a node after head
        ExpectResponse(InsertAfterNode(GetHead(sList), &testData[1]), LL_OK);
        ExpectListWith2Nodes(sList, testData[0].id, testData[1].id);

        // Test: Insert another node after head
        ExpectResponse(InsertAfterNode(GetHead(sList), &testData[2]), LL_OK);
        ExpectListWith3Nodes(sList, testData[0].id, testData[2].id, testData[1].id);

        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
        ExpectResponse(DeleteList(sList), LL_OK);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 18: Insert a node after the tail node => return LL_OK");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(AddToBack(dList, &testData[0]), LL_OK);
        ExpectListWith1Node(dList, testData[0].id);

        // Test: Insert a node after tail
        ExpectResponse(InsertAfterNode(GetTail(dList), &testData[1]), LL_OK);
        ExpectListWith2Nodes(dList, testData[0].id, testData[1].id);

        // Test: Insert another node after tail
        ExpectResponse(InsertAfterNode(GetTail(dList), &testData[2]), LL_OK);
        ExpectListWith3Nodes(dList, testData[0].id, testData[1].id, testData[2].id);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(AddToBack(sList, &testData[0]), LL_OK);
        ExpectListWith1Node(sList, testData[0].id);

        // Test: Insert a node after tail
        ExpectResponse(InsertAfterNode(GetTail(sList), &testData[1]), LL_OK);
        ExpectListWith2Nodes(sList, testData[0].id, testData[1].id);

        // Test: Insert another node after tail
        ExpectResponse(InsertAfterNode(GetTail(sList), &testData[2]), LL_OK);
        ExpectListWith3Nodes(sList, testData[0].id, testData[1].id, testData[2].id);

        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
        ExpectResponse(DeleteList(sList), LL_OK);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 19: Insert a node after any middle node => return LL_OK");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert 3 nodes
        ExpectResponse(AddToBack(dList, &testData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(dList), &testData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(dList), &testData[2]), LL_OK);
        ExpectListWith3Nodes(dList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Test: Add 104 after 102
        ExpectResponse(InsertAfterNode(GetNext(GetHead(dList)), &testData[3]), LL_OK);
        ExpectListWith4Nodes(dList, testData[0].id, testData[1].id, testData[3].id, testData[2].id);  
        // List is now: 101, 102, 104, 103

        // Test: Add 105 after 104
        ExpectResponse(InsertAfterNode(GetPrev(GetTail(dList)), &testData[4]), LL_OK);
        ExpectListWith5Nodes(dList, testData[0].id, testData[1].id, testData[3].id, testData[4].id, testData[2].id);
        // List is now: 101, 102, 104, 105, 103

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert 3 nodes
        ExpectResponse(AddToBack(sList, &testData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(sList), &testData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(sList), &testData[2]), LL_OK);
        ExpectListWith3Nodes(sList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Test: Add 104 after 102
        ExpectResponse(InsertAfterNode(GetNext(GetHead(sList)), &testData[3]), LL_OK);
        ExpectListWith4Nodes(sList, testData[0].id, testData[1].id, testData[3].id, testData[2].id);
        // List is now: 101, 102, 104, 103

        // Test: Add 105 after 104
        ExpectResponse(InsertAfterNode(GetNext(GetNext(GetHead(sList))), &testData[4]), LL_OK);
        ExpectListWith5Nodes(sList, testData[0].id, testData[1].id, testData[3].id, testData[4].id, testData[2].id);
        // List is now: 101, 102, 104, 105, 103

        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
        ExpectResponse(DeleteList(sList), LL_OK);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 20: Insert data after inexistent data, when list is non-empty => return LL_NOT_OK");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(AddToBack(dList, &testData[0]), LL_OK);
        ExpectListWith1Node(dList, testData[0].id);

        // Save head & tail
        ListNode_t* head_dList_1 = GetHead(dList);
        ListNode_t* tail_dList_1 = GetTail(dList);

        // Test: Try inserting a node after inexistent data:
        ExpectResponse(InsertAfterData(dList, &dummyData, &testData[0]), LL_NOT_OK);
        ExpectListWith1Node(dList, testData[0].id);

        // Head and tail didn't change
        ExpectEqualPtr(GetHead(dList), head_dList_1);
        ExpectEqualPtr(GetTail(dList), tail_dList_1);

        // Insert another node:
        ExpectResponse(AddToBack(dList, &testData[1]), LL_OK);
        ExpectListWith2Nodes(dList, testData[0].id, testData[1].id);

        // Save head & tail
        ListNode_t* head_dList_2 = GetHead(dList);
        ListNode_t* tail_dList_2 = GetTail(dList);

        // Test: Try inserting another node after inexistent data:
        ExpectResponse(InsertAfterData(dList, &testData[2], &testData[3]), LL_NOT_OK);
        ExpectListWith2Nodes(dList, testData[0].id, testData[1].id);

        // Head and tail didn't change
        ExpectEqualPtr(GetHead(dList), head_dList_2);
        ExpectEqualPtr(GetTail(dList), tail_dList_2);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(AddToBack(sList, &testData[0]), LL_OK);
        ExpectListWith1Node(sList, testData[0].id);

        // Save head & tail
        ListNode_t* head_sList_1 = GetHead(sList);
        ListNode_t* tail_sList_1 = GetTail(sList);

        // Test: Try inserting a node after inexistent data:
        ExpectResponse(InsertAfterData(sList, &dummyData, &testData[0]), LL_NOT_OK);
        ExpectListWith1Node(sList, testData[0].id);

        // Head and tail didn't change
        ExpectEqualPtr(GetHead(sList), head_sList_1);
        ExpectEqualPtr(GetTail(sList), tail_sList_1);

        // Insert another node:
        ExpectResponse(AddToBack(sList, &testData[1]), LL_OK);
        ExpectListWith2Nodes(sList, testData[0].id, testData[1].id);

        // Save head & tail
        ListNode_t* head_sList_2 = GetHead(sList);
        ListNode_t* tail_sList_2 = GetTail(sList);

        // Test: Try inserting another node after inexistent data:
        ExpectResponse(InsertAfterData(sList, &testData[2], &testData[3]), LL_NOT_OK);
        ExpectListWith2Nodes(sList, testData[0].id, testData[1].id);

        // Head and tail didn't change
        ExpectEqualPtr(GetHead(sList), head_sList_2);
        ExpectEqualPtr(GetTail(sList), tail_sList_2);

        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
        ExpectResponse(DeleteList(sList), LL_OK);
    }    
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 21: Insert data after existing head data => return LL_OK");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(AddToFront(dList, &testData[0]), LL_OK);
        ExpectListWith1Node(dList, testData[0].id);

        // Save head & tail
        ListNode_t* head_dList_1 = GetHead(dList);
        ListNode_t* tail_dList_1 = GetTail(dList);

        // Test: Try inserting some data after the head data:
        ExpectResponse(InsertAfterData(dList, &testData[0], &testData[1]), LL_OK);
        ExpectListWith2Nodes(dList, testData[0].id, testData[1].id);

        // Check that the tail node has changed (head node is the same)
        ListNode_t* head_dList_2 = GetHead(dList);
        ListNode_t* tail_dList_2 = GetTail(dList);
        ExpectEqualPtr(head_dList_1, head_dList_2);
        ExpectNotEqualPtr(tail_dList_1, tail_dList_2);

        // Test: Try inserting some data after the head data:
        ExpectResponse(InsertAfterData(dList, &testData[0], &testData[2]), LL_OK);
        ExpectListWith3Nodes(dList, testData[0].id, testData[2].id, testData[1].id);

        // Check that both head and tail nodes are unchanged
        ListNode_t* head_dList_3 = GetHead(dList);
        ListNode_t* tail_dList_3 = GetTail(dList);
        ExpectEqualPtr(head_dList_2, head_dList_3);
        ExpectEqualPtr(tail_dList_2, tail_dList_3);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(AddToBack(sList, &testData[0]), LL_OK);
        ExpectListWith1Node(sList, testData[0].id);

        // Save head & tail
        ListNode_t* head_sList_1 = GetHead(sList);
        ListNode_t* tail_sList_1 = GetTail(sList);

        // Test: Try inserting some data after the head data:
        ExpectResponse(InsertAfterData(sList, &testData[0], &testData[1]), LL_OK);
        ExpectListWith2Nodes(sList, testData[0].id, testData[1].id);

        // Check that the tail node has changed (head node is the same)
        ListNode_t* head_sList_2 = GetHead(sList);
        ListNode_t* tail_sList_2 = GetTail(sList);
        ExpectEqualPtr(head_sList_1, head_sList_2);
        ExpectNotEqualPtr(tail_sList_1, tail_sList_2);

        // Test: Try inserting some data after the head data:
        ExpectResponse(InsertAfterData(sList, &testData[0], &testData[2]), LL_OK);
        ExpectListWith3Nodes(sList, testData[0].id,  testData[2].id, testData[1].id);

        // Check that both head and tail nodes are unchanged
        ListNode_t* head_sList_3 = GetHead(sList);
        ListNode_t* tail_sList_3 = GetTail(sList);
        ExpectEqualPtr(head_sList_2, head_sList_3);
        ExpectEqualPtr(tail_sList_2, tail_sList_3);

        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
        ExpectResponse(DeleteList(sList), LL_OK);
    }
    TestEnd();    

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 22: Insert data after existing data when existing data is in the tail => return LL_OK");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(AddToFront(dList, &testData[0]), LL_OK);
        ExpectListWith1Node(dList, testData[0].id);

        // Save head & tail
        ListNode_t* head_dList_1 = GetHead(dList);
        ListNode_t* tail_dList_1 = GetTail(dList);

        // Test: Try inserting some data after the tail data:
        ExpectResponse(InsertAfterData(dList, &testData[0], &testData[1]), LL_OK);
        ExpectListWith2Nodes(dList, testData[0].id, testData[1].id);

        // Check that the tail node has changed (head is the same)
        ListNode_t* head_dList_2 = GetHead(dList);
        ListNode_t* tail_dList_2 = GetTail(dList);
        ExpectEqualPtr(head_dList_1, head_dList_2);
        ExpectNotEqualPtr(tail_dList_1, tail_dList_2);

        // Test: Try inserting some more data after the tail data:
        ExpectResponse(InsertAfterData(dList, &testData[1], &testData[2]), LL_OK);
        ExpectListWith3Nodes(dList, testData[0].id, testData[1].id, testData[2].id);

        // Check that the tail node has changed (head is the same)
        ListNode_t* head_dList_3 = GetHead(dList);
        ListNode_t* tail_dList_3 = GetTail(dList);
        ExpectEqualPtr(head_dList_2, head_dList_3);
        ExpectNotEqualPtr(tail_dList_2, tail_dList_3);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(AddToBack(sList, &testData[0]), LL_OK);
        ExpectListWith1Node(sList, testData[0].id);

        // Save head & tail
        ListNode_t* head_sList_1 = GetHead(sList);
        ListNode_t* tail_sList_1 = GetTail(sList);

        // Test: Try inserting some data after the tail data:
        ExpectResponse(InsertAfterData(sList, &testData[0], &testData[1]), LL_OK);
        ExpectListWith2Nodes(sList, testData[0].id, testData[1].id);

        // Check that the tail node has changed (head is the same)
        ListNode_t* head_sList_2 = GetHead(sList);
        ListNode_t* tail_sList_2 = GetTail(sList);
        ExpectEqualPtr(head_sList_1, head_sList_2);
        ExpectNotEqualPtr(tail_sList_1, tail_sList_2);

        // Test: Try inserting some data after the tail data:
        ExpectResponse(InsertAfterData(sList, &testData[1], &testData[2]), LL_OK);
        ExpectListWith3Nodes(sList, testData[0].id, testData[1].id, testData[2].id);

        // Check that the tail node has changed (head is the same)
        ListNode_t* head_sList_3 = GetHead(sList);
        ListNode_t* tail_sList_3 = GetTail(sList);
        ExpectEqualPtr(head_sList_2, head_sList_3);
        ExpectNotEqualPtr(tail_sList_2, tail_sList_3);

        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
        ExpectResponse(DeleteList(sList), LL_OK);
    }
    TestEnd();    

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 23: Insert data after existing data when existing data is in the middle => return LL_OK");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert 3 nodes
        ExpectResponse(AddToBack(dList, &testData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(dList), &testData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(dList), &testData[2]), LL_OK);
        ExpectListWith3Nodes(dList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Save head and tail
        ListNode_t* head_dList_1 = GetHead(dList);
        ListNode_t* tail_dList_1 = GetTail(dList);

        // Test: Try inserting some data after the data in the middle node:
        ExpectResponse(InsertAfterData(dList, &testData[1], &testData[3]), LL_OK);
        ExpectListWith4Nodes(dList, testData[0].id, testData[1].id, testData[3].id, testData[2].id);
        // List is now: 101, 102, 104, 103

        // Check that the head & tail nodes didn't change
        ListNode_t* head_dList_2 = GetHead(dList);
        ListNode_t* tail_dList_2 = GetTail(dList);
        ExpectEqualPtr(head_dList_1, head_dList_2);
        ExpectEqualPtr(tail_dList_1, tail_dList_2);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert 3 nodes
        ExpectResponse(AddToBack(sList, &testData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(sList), &testData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(sList), &testData[2]), LL_OK);
        ExpectListWith3Nodes(sList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Save head and tail
        ListNode_t* head_sList_1 = GetHead(sList);
        ListNode_t* tail_sList_1 = GetTail(sList);

        // Test: Try inserting some data after the data in the middle node:
        ExpectResponse(InsertAfterData(sList, &testData[1], &testData[3]), LL_OK);
        ExpectListWith4Nodes(sList, testData[0].id, testData[1].id, testData[3].id, testData[2].id);
        // List is now: 101, 102, 104, 103

        // Check that the head & tail nodes didn't change
        ListNode_t* head_sList_2 = GetHead(sList);
        ListNode_t* tail_sList_2 = GetTail(sList);
        ExpectEqualPtr(head_sList_1, head_sList_2);
        ExpectEqualPtr(tail_sList_1, tail_sList_2);
        
        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
        ExpectResponse(DeleteList(sList), LL_OK);
    }    
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 24: Remove node from the front/back of a non-empty list => return LL_OK");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(AddToFront(dList, &dummyData), LL_OK);
        ExpectListWith1Node(dList, dummyData.id);

        // Test: List with one node -> remove from front
        ExpectResponse(RemoveHead(dList), LL_OK);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(AddToBack(dList, &dummyData), LL_OK);
        ExpectListWith1Node(dList, dummyData.id);

        // Test: List with one node -> remove from back
        ExpectResponse(RemoveTail(dList), LL_OK);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(AddToFront(dList, &dummyData), LL_OK);
        ExpectListWith1Node(dList, dummyData.id);

        // Test: List with one node -> remove from back
        ExpectResponse(RemoveTail(dList), LL_OK);
        ExpectEmptyList(dList);
        
        // Precondition: insert one node
        ExpectResponse(AddToBack(dList, &dummyData), LL_OK);
        ExpectListWith1Node(dList, dummyData.id);

        // Test: List with one node -> remove from front
        ExpectResponse(RemoveHead(dList), LL_OK);
        ExpectEmptyList(dList);

        // Precondition: Add 2 nodes
        ExpectResponse(AddToFront(dList, &testData[4]), LL_OK);
        ExpectResponse(AddToFront(dList, &testData[3]), LL_OK);
        ExpectListWith2Nodes(dList, testData[3].id, testData[4].id);

        // Test: List with 2 nodes -> remove from front
        ExpectResponse(RemoveHead(dList), LL_OK);
        ExpectListWith1Node(dList, testData[4].id);

        // Precondition: Add one more node
        ExpectResponse(AddToBack(dList, &testData[3]), LL_OK);
        ExpectListWith2Nodes(dList, testData[4].id, testData[3].id);

        // Test: List with 2 nodes -> remove from back
        ExpectResponse(RemoveTail(dList), LL_OK);
        ExpectListWith1Node(dList, testData[4].id);

        // Precondition: Add one more node
        ExpectResponse(AddToFront(dList, &testData[3]), LL_OK);
        ExpectListWith2Nodes(dList, testData[3].id, testData[4].id);

        // Test: List with 2 nodes -> remove from back
        ExpectResponse(RemoveTail(dList), LL_OK);
        ExpectListWith1Node(dList, testData[3].id);

        // Precondition: Add one more node
        ExpectResponse(AddToBack(dList, &testData[4]), LL_OK);
        ExpectListWith2Nodes(dList, testData[3].id, testData[4].id);

        // Test: List with 2 nodes -> remove from front
        ExpectResponse(RemoveHead(dList), LL_OK);
        ExpectListWith1Node(dList, testData[4].id);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(AddToFront(sList, &dummyData), LL_OK);
        ExpectListWith1Node(sList, dummyData.id);

        // Test: List with one node -> remove from front
        ExpectResponse(RemoveHead(sList), LL_OK);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(AddToBack(sList, &dummyData), LL_OK);
        ExpectListWith1Node(sList, dummyData.id);

        // Test: List with one node -> remove from back
        ExpectResponse(RemoveTail(sList), LL_OK);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(AddToFront(sList, &dummyData), LL_OK);
        ExpectListWith1Node(sList, dummyData.id);

        // Test: List with one node -> remove from back
        ExpectResponse(RemoveTail(sList), LL_OK);
        ExpectEmptyList(sList);
        
        // Precondition: insert one node
        ExpectResponse(AddToBack(sList, &dummyData), LL_OK);
        ExpectListWith1Node(sList, dummyData.id);

        // Test: List with one node -> remove from front
        ExpectResponse(RemoveHead(sList), LL_OK);
        ExpectEmptyList(sList);

        // Precondition: Add 2 nodes
        ExpectResponse(AddToFront(sList, &testData[4]), LL_OK);
        ExpectResponse(AddToFront(sList, &testData[3]), LL_OK);
        ExpectListWith2Nodes(sList, testData[3].id, testData[4].id);

        // Test: List with 2 nodes -> remove from front
        ExpectResponse(RemoveHead(sList), LL_OK);
        ExpectListWith1Node(sList, testData[4].id);

        // Precondition: Add one more node
        ExpectResponse(AddToBack(sList, &testData[3]), LL_OK);
        ExpectListWith2Nodes(sList, testData[4].id, testData[3].id);

        // Test: List with 2 nodes -> remove from back
        ExpectResponse(RemoveTail(sList), LL_OK);
        ExpectListWith1Node(sList, testData[4].id);

        // Precondition: Add one more node
        ExpectResponse(AddToFront(sList, &testData[3]), LL_OK);
        ExpectListWith2Nodes(sList, testData[3].id, testData[4].id);

        // Test: List with 2 nodes -> remove from back
        ExpectResponse(RemoveTail(sList), LL_OK);
        ExpectListWith1Node(sList, testData[3].id);

        // Precondition: Add one more node
        ExpectResponse(AddToBack(sList, &testData[4]), LL_OK);
        ExpectListWith2Nodes(sList, testData[3].id, testData[4].id);

        // Test: List with 2 nodes -> remove from front
        ExpectResponse(RemoveHead(sList), LL_OK);
        ExpectListWith1Node(sList, testData[4].id);

        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
        ExpectResponse(DeleteList(sList), LL_OK);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 25: Remove node from the middle of a non-empty list => return LL_OK");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert 3 nodes
        ExpectResponse(AddToBack(dList, &testData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(dList), &testData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(dList), &testData[2]), LL_OK);
        ExpectListWith3Nodes(dList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Save head and tail
        ListNode_t* head_dList_1 = GetHead(dList);
        ListNode_t* tail_dList_1 = GetTail(dList);

        // Test: Try to remove the node from the middle
        ExpectResponse(RemoveNode(GetNext(GetHead(dList))), LL_OK);
        ExpectListWith2Nodes(dList, testData[0].id, testData[2].id);
        // List is now: 101, 103

        // Head and tail are unchanged
        ListNode_t* head_dList_2 = GetHead(dList);
        ListNode_t* tail_dList_2 = GetTail(dList);
        ExpectEqualPtr(head_dList_1, head_dList_2);
        ExpectEqualPtr(tail_dList_1, tail_dList_2);

        // Precondition: Add 2 more nodes
        ExpectResponse(AddToFront(dList, &testData[4]), LL_OK);
        ExpectResponse(AddToFront(dList, &testData[3]), LL_OK);
        ExpectListWith4Nodes(dList, testData[3].id, testData[4].id, testData[0].id, testData[2].id);
        // List is now: 104, 105, 101, 103

        // Save head & tail
        ListNode_t* head_dList_3 = GetHead(dList);
        ListNode_t* tail_dList_3 = GetTail(dList);

        // Try to remove the nodes from the middle
        ExpectResponse(RemoveNode(GetNext(GetNext(GetHead(dList)))), LL_OK);
        ExpectListWith3Nodes(dList, testData[3].id, testData[4].id, testData[2].id);
        ExpectResponse(RemoveNode(GetNext(GetHead(dList))), LL_OK);
        ExpectListWith2Nodes(dList, testData[3].id, testData[2].id);
        // List is now: 104, 103

        // Head and tail are not changed
        ListNode_t* head_dList_4 = GetHead(dList);
        ListNode_t* tail_dList_4 = GetTail(dList);
        ExpectEqualPtr(head_dList_3, head_dList_4);
        ExpectEqualPtr(tail_dList_3, tail_dList_4);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert 3 nodes
        ExpectResponse(AddToBack(sList, &testData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(sList), &testData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(sList), &testData[2]), LL_OK);
        ExpectListWith3Nodes(sList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Save head and tail
        ListNode_t* head_sList_1 = GetHead(sList);
        ListNode_t* tail_sList_1 = GetTail(sList);

        // Test: Try to remove the node from the middle
        ExpectResponse(RemoveNode(GetNext(GetHead(sList))), LL_OK);
        ExpectListWith2Nodes(sList, testData[0].id, testData[2].id);
        // List is now: 101, 103

        // Head and tail are unchanged
        ListNode_t* head_sList_2 = GetHead(sList);
        ListNode_t* tail_sList_2 = GetTail(sList);
        ExpectEqualPtr(head_sList_1, head_sList_2);
        ExpectEqualPtr(tail_sList_1, tail_sList_2);

        // Precondition: Add 2 more nodes
        ExpectResponse(AddToFront(sList, &testData[4]), LL_OK);
        ExpectResponse(AddToFront(sList, &testData[3]), LL_OK);
        ExpectListWith4Nodes(sList, testData[3].id, testData[4].id, testData[0].id, testData[2].id);
        // List is now: 104, 105, 101, 103

        // Save head & tail
        ListNode_t* head_sList_3 = GetHead(sList);
        ListNode_t* tail_sList_3 = GetTail(sList);

        // Try to remove the nodes from the middle
        ExpectResponse(RemoveNode(GetNext(GetNext(GetHead(sList)))), LL_OK);
        ExpectListWith3Nodes(sList, testData[3].id, testData[4].id, testData[2].id);
        ExpectResponse(RemoveNode(GetNext(GetHead(sList))), LL_OK);
        ExpectListWith2Nodes(sList, testData[3].id, testData[2].id);
        // List is now: 104, 103

        // Head and tail are not changed
        ListNode_t* head_sList_4 = GetHead(sList);
        ListNode_t* tail_sList_4 = GetTail(sList);
        ExpectEqualPtr(head_sList_3, head_sList_4);
        ExpectEqualPtr(tail_sList_3, tail_sList_4);

        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
        ExpectResponse(DeleteList(sList), LL_OK);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 26: Remove inexistent data from a non-empty list => return LL_NOT_OK");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(AddToFront(dList, &dummyData), LL_OK);
        ExpectListWith1Node(dList, dummyData.id);

        // Test: Remove inexistent data from a list with one node
        ExpectResponse(RemoveNodeByData(dList, &testData[3]), LL_NOT_OK);
        ExpectListWith1Node(dList, dummyData.id);

        // Precondition: insert one more node
        ExpectResponse(AddToFront(dList, &testData[4]), LL_OK);
        ExpectListWith2Nodes(dList, testData[4].id, dummyData.id);

        // Test: Remove inexistent data from a list with two nodes
        ExpectResponse(RemoveNodeByData(dList, &testData[3]), LL_NOT_OK);
        ExpectListWith2Nodes(dList, testData[4].id, dummyData.id);

        // Precondition: insert one more node
        ExpectResponse(AddToFront(dList, &testData[2]), LL_OK);
        ExpectListWith3Nodes(dList, testData[2].id, testData[4].id, dummyData.id);

        // Test: Remove inexistent data from a list with three nodes
        ExpectResponse(RemoveNodeByData(dList, &testData[0]), LL_NOT_OK);
        ExpectResponse(RemoveNodeByData(dList, &testData[3]), LL_NOT_OK);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(AddToFront(sList, &dummyData), LL_OK);
        ExpectListWith1Node(sList, dummyData.id);

        // Test: Remove inexistent data from a list with one node
        ExpectResponse(RemoveNodeByData(sList, &testData[3]), LL_NOT_OK);
        ExpectListWith1Node(sList, dummyData.id);

        // Precondition: insert one more node
        ExpectResponse(AddToFront(sList, &testData[4]), LL_OK);
        ExpectListWith2Nodes(sList, testData[4].id, dummyData.id);

        // Test: Remove inexistent data from a list with two nodes
        ExpectResponse(RemoveNodeByData(sList, &testData[3]), LL_NOT_OK);
        ExpectListWith2Nodes(sList, testData[4].id, dummyData.id);

        // Precondition: insert one more node
        ExpectResponse(AddToFront(sList, &testData[2]), LL_OK);
        ExpectListWith3Nodes(sList, testData[2].id, testData[4].id, dummyData.id);

        // Test: Remove inexistent data from a list with three nodes
        ExpectResponse(RemoveNodeByData(sList, &testData[0]), LL_NOT_OK);
        ExpectResponse(RemoveNodeByData(sList, &testData[3]), LL_NOT_OK);

        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
        ExpectResponse(DeleteList(sList), LL_OK);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 27: Remove node by data from the front/back of a non-empty list => return LL_OK");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(AddToFront(dList, &dummyData), LL_OK);
        ExpectListWith1Node(dList, dummyData.id);

        // Test: Remove node by data from a list with 1 node
        ExpectResponse(RemoveNodeByData(dList, &dummyData), LL_OK);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(AddToFront(dList, &testData[3]), LL_OK);
        ExpectListWith1Node(dList, testData[3].id);

        // Test: Remove node by data from a list with 1 node
        ExpectResponse(RemoveNodeByData(dList, &testData[3]), LL_OK);
        ExpectEmptyList(dList);

        // Precondition: insert two nodes
        ExpectResponse(AddToFront(dList, &testData[4]), LL_OK);
        ExpectResponse(AddToFront(dList, &testData[3]), LL_OK);
        ExpectListWith2Nodes(dList, testData[3].id, testData[4].id);

        // Test: Remove node by data from a list with 2 nodes
        ExpectResponse(RemoveNodeByData(dList, &testData[4]), LL_OK);
        ExpectListWith1Node(dList, testData[3].id);

        // Precondition: insert two nodes
        ExpectResponse(AddToFront(dList, &testData[2]), LL_OK);
        ExpectResponse(AddToFront(dList, &testData[1]), LL_OK);
        ExpectListWith3Nodes(dList, testData[1].id, testData[2].id, testData[3].id);

        // Test: Remove node by data from a list with 3 nodes
        ExpectResponse(RemoveNodeByData(dList, &testData[1]), LL_OK);
        ExpectListWith2Nodes(dList, testData[2].id, testData[3].id);

        // Precondition: insert one node
        ExpectResponse(AddToFront(dList, &testData[1]), LL_OK);
        
        // Test: Remove node by data from a list with 3 nodes
        ExpectResponse(RemoveNodeByData(dList, &testData[3]), LL_OK);
        ExpectListWith2Nodes(dList, testData[1].id, testData[2].id);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(AddToFront(sList, &dummyData), LL_OK);
        ExpectListWith1Node(sList, dummyData.id);

        // Test: Remove node by data from a list with 1 node
        ExpectResponse(RemoveNodeByData(sList, &dummyData), LL_OK);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(AddToFront(sList, &testData[3]), LL_OK);
        ExpectListWith1Node(sList, testData[3].id);

        // Test: Remove node by data from a list with 1 node
        ExpectResponse(RemoveNodeByData(sList, &testData[3]), LL_OK);
        ExpectEmptyList(sList);

        // Precondition: insert two nodes
        ExpectResponse(AddToFront(sList, &testData[4]), LL_OK);
        ExpectResponse(AddToFront(sList, &testData[3]), LL_OK);
        ExpectListWith2Nodes(sList, testData[3].id, testData[4].id);

        // Test: Remove node by data from a list with 2 nodes
        ExpectResponse(RemoveNodeByData(sList, &testData[4]), LL_OK);
        ExpectListWith1Node(sList, testData[3].id);

        // Precondition: insert two nodes
        ExpectResponse(AddToFront(sList, &testData[2]), LL_OK);
        ExpectResponse(AddToFront(sList, &testData[1]), LL_OK);
        ExpectListWith3Nodes(sList, testData[1].id, testData[2].id, testData[3].id);

        // Test: Remove node by data from a list with 3 nodes
        ExpectResponse(RemoveNodeByData(sList, &testData[1]), LL_OK);
        ExpectListWith2Nodes(sList, testData[2].id, testData[3].id);

        // Precondition: insert one node
        ExpectResponse(AddToFront(sList, &testData[1]), LL_OK);
        
        // Test: Remove node by data from a list with 3 nodes
        ExpectResponse(RemoveNodeByData(sList, &testData[3]), LL_OK);
        ExpectListWith2Nodes(sList, testData[1].id, testData[2].id);

        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
        ExpectResponse(DeleteList(sList), LL_OK);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 28: Remove node by data from the middle of a list => return LL_OK");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert 3 nodes
        ExpectResponse(AddToBack(dList, &testData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(dList), &testData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(dList), &testData[2]), LL_OK);
        ExpectListWith3Nodes(dList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Save head and tail
        ListNode_t* head_dList_1 = GetHead(dList);
        ListNode_t* tail_dList_1 = GetTail(dList);

        // Test: Remove the node from the middle (by data)
        ExpectResponse(RemoveNodeByData(dList, &testData[1]), LL_OK);
        ExpectListWith2Nodes(dList, testData[0].id, testData[2].id);
        // List is now: 101, 103

        // Head and tail are unchanged
        ListNode_t* head_dList_2 = GetHead(dList);
        ListNode_t* tail_dList_2 = GetTail(dList);
        ExpectEqualPtr(head_dList_1, head_dList_2);
        ExpectEqualPtr(tail_dList_1, tail_dList_2);

        // Precondition: Add 2 more nodes
        ExpectResponse(AddToFront(dList, &testData[4]), LL_OK);
        ExpectResponse(AddToFront(dList, &testData[3]), LL_OK);
        ExpectListWith4Nodes(dList, testData[3].id, testData[4].id, testData[0].id, testData[2].id);
        // List is now: 104, 105, 101, 103

        // Save head and tail
        ListNode_t* head_dList_3 = GetHead(dList);
        ListNode_t* tail_dList_3 = GetTail(dList);

        // Remove the nodes from the middle
        ExpectResponse(RemoveNodeByData(dList, &testData[0]), LL_OK);
        ExpectListWith3Nodes(dList, testData[3].id, testData[4].id, testData[2].id);
        ExpectResponse(RemoveNodeByData(dList, &testData[4]), LL_OK);
        ExpectListWith2Nodes(dList, testData[3].id, testData[2].id);
        // List is now: 104, 103

        // Head and tail are not changed
        ListNode_t* head_dList_4 = GetHead(dList);
        ListNode_t* tail_dList_4 = GetTail(dList);
        ExpectEqualPtr(head_dList_3, head_dList_4);
        ExpectEqualPtr(tail_dList_3, tail_dList_4);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert 3 nodes
        ExpectResponse(AddToBack(sList, &testData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(sList), &testData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(sList), &testData[2]), LL_OK);
        ExpectListWith3Nodes(sList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Save head and tail
        ListNode_t* head_sList_1 = GetHead(sList);
        ListNode_t* tail_sList_1 = GetTail(sList);

        // Test: Remove the node from the middle (by data)
        ExpectResponse(RemoveNodeByData(sList, &testData[1]), LL_OK);
        ExpectListWith2Nodes(sList, testData[0].id, testData[2].id);
        // List is now: 101, 103

        // Head and tail are unchanged
        ListNode_t* head_sList_2 = GetHead(sList);
        ListNode_t* tail_sList_2 = GetTail(sList);
        ExpectEqualPtr(head_sList_1, head_sList_2);
        ExpectEqualPtr(tail_sList_1, tail_sList_2);

        // Precondition: Add 2 more nodes
        ExpectResponse(AddToFront(sList, &testData[4]), LL_OK);
        ExpectResponse(AddToFront(sList, &testData[3]), LL_OK);
        ExpectListWith4Nodes(sList, testData[3].id, testData[4].id, testData[0].id, testData[2].id);
        // List is now: 104, 105, 101, 103

        // Save head and tail
        ListNode_t* head_sList_3 = GetHead(sList);
        ListNode_t* tail_sList_3 = GetTail(sList);

        // Remove the nodes from the middle
        ExpectResponse(RemoveNodeByData(sList, &testData[0]), LL_OK);
        ExpectListWith3Nodes(sList, testData[3].id, testData[4].id, testData[2].id);
        ExpectResponse(RemoveNodeByData(sList, &testData[4]), LL_OK);
        ExpectListWith2Nodes(sList, testData[3].id, testData[2].id);
        // List is now: 104, 103

        // Head and tail are not changed
        ListNode_t* head_sList_4 = GetHead(sList);
        ListNode_t* tail_sList_4 = GetTail(sList);
        ExpectEqualPtr(head_sList_3, head_sList_4);
        ExpectEqualPtr(tail_sList_3, tail_sList_4);

        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
        ExpectResponse(DeleteList(sList), LL_OK);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 29: GetPrev in a singly-linked list always return NULL");
    {
        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert 3 nodes
        ExpectResponse(AddToBack(sList, &testData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(sList), &testData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(sList), &testData[2]), LL_OK);
        ExpectListWith3Nodes(sList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Test:
        ExpectPointerNull(GetPrev(GetHead(sList)));
        ExpectPointerNull(GetPrev(GetNext(GetHead(sList))));
        ExpectPointerNull(GetPrev(GetNext(GetNext(GetHead(sList)))));

        // Cleanup
        ExpectResponse(DeleteList(sList), LL_OK);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 30: Get node by data from the head/tail/middle of a list");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert 3 nodes
        ExpectResponse(AddToBack(dList, &testData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(dList), &testData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(dList), &testData[2]), LL_OK);
        ExpectListWith3Nodes(dList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Test:
        ExpectEqualPtr(GetNodeByData(dList, &testData[0]), GetHead(dList));
        ExpectEqualPtr(GetNodeByData(dList, &testData[1]), GetNext(GetHead(dList)));
        ExpectEqualPtr(GetNodeByData(dList, &testData[2]), GetTail(dList));

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert 3 nodes
        ExpectResponse(AddToBack(sList, &testData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(sList), &testData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(sList), &testData[2]), LL_OK);
        ExpectListWith3Nodes(sList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Test:
        ExpectEqualPtr(GetNodeByData(sList, &testData[0]), GetHead(sList));
        ExpectEqualPtr(GetNodeByData(sList, &testData[1]), GetNext(GetHead(sList)));
        ExpectEqualPtr(GetNodeByData(sList, &testData[2]), GetTail(sList));

        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
        ExpectResponse(DeleteList(sList), LL_OK);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 31: Remove Node from head/tail of a list");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert 1 node
        ExpectResponse(AddToFront(dList, &testData[3]), LL_OK);
        ExpectListWith1Node(dList, testData[3].id);

        // Test: Remove head node from a list with 1 node
        ExpectResponse(RemoveNode(GetHead(dList)), LL_OK);
        ExpectEmptyList(dList);

        // Precondition: insert 1 node
        ExpectResponse(AddToFront(dList, &testData[4]), LL_OK);
        ExpectListWith1Node(dList, testData[4].id);

        // Test: Remove tail node from a list with 1 node
        ExpectResponse(RemoveNode(GetTail(dList)), LL_OK);
        ExpectEmptyList(dList);

        // Precondition: insert 2 nodes
        ExpectResponse(AddToBack(dList, &testData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(dList), &testData[1]), LL_OK);
        ExpectListWith2Nodes(dList, testData[0].id, testData[1].id);
        // List is now: 101, 102

        // Test: Remove head node from a list with 2 nodes
        ExpectResponse(RemoveNode(GetHead(dList)), LL_OK);
        ExpectListWith1Node(dList, testData[1].id);
        // List is now: 102

        // Precondition: insert 1 more node
        ExpectResponse(AddToFront(dList, &testData[4]), LL_OK);
        ExpectListWith2Nodes(dList, testData[4].id, testData[1].id);
        // List is now: 105, 102

        // Test: Remove tail node from a list with 2 nodes
        ExpectResponse(RemoveNode(GetTail(dList)), LL_OK);
        ExpectListWith1Node(dList, testData[4].id);
        // List is now: 105

        // Test: Remove tail node from a list with 1 node
        ExpectResponse(RemoveNode(GetTail(dList)), LL_OK);
        ExpectEmptyList(dList);

        // Precondition: insert 3 nodes
        ExpectResponse(AddToBack(dList, &testData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(dList), &testData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(dList), &testData[2]), LL_OK);
        ExpectListWith3Nodes(dList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Test: Remove head node
        ExpectResponse(RemoveNode(GetHead(dList)), LL_OK);
        ExpectListWith2Nodes(dList, testData[1].id, testData[2].id);
        // List is now: 102, 103

        // Precondition: insert 1 more node
        ExpectResponse(AddToFront(dList, &testData[4]), LL_OK);
        ExpectListWith3Nodes(dList, testData[4].id, testData[1].id, testData[2].id);
        // List is now: 105, 102, 103

        // Test: Remove tail node from a list with 3 nodes
        ExpectResponse(RemoveNode(GetTail(dList)), LL_OK);
        ExpectListWith2Nodes(dList, testData[4].id, testData[1].id);
        // List is now: 105, 102

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LL_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert 1 node
        ExpectResponse(AddToFront(sList, &testData[3]), LL_OK);
        ExpectListWith1Node(sList, testData[3].id);

        // Test: Remove head node from a list with 1 node
        ExpectResponse(RemoveNode(GetHead(sList)), LL_OK);
        ExpectEmptyList(sList);

        // Precondition: insert 1 node
        ExpectResponse(AddToFront(sList, &testData[4]), LL_OK);
        ExpectListWith1Node(sList, testData[4].id);

        // Test: Remove tail node from a list with 1 node
        ExpectResponse(RemoveNode(GetTail(sList)), LL_OK);
        ExpectEmptyList(sList);

        // Precondition: insert 2 nodes
        ExpectResponse(AddToBack(sList, &testData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(sList), &testData[1]), LL_OK);
        ExpectListWith2Nodes(sList, testData[0].id, testData[1].id);
        // List is now: 101, 102

        // Test: Remove head node from a list with 2 nodes
        ExpectResponse(RemoveNode(GetHead(sList)), LL_OK);
        ExpectListWith1Node(sList, testData[1].id);
        // List is now: 102

        // Precondition: insert 1 more node
        ExpectResponse(AddToFront(sList, &testData[4]), LL_OK);
        ExpectListWith2Nodes(sList, testData[4].id, testData[1].id);
        // List is now: 105, 102

        // Test: Remove tail node from a list with 2 nodes
        ExpectResponse(RemoveNode(GetTail(sList)), LL_OK);
        ExpectListWith1Node(sList, testData[4].id);
        // List is now: 105

        // Test: Remove tail node from a list with 1 node
        ExpectResponse(RemoveNode(GetTail(sList)), LL_OK);
        ExpectEmptyList(sList);

        // Precondition: insert 3 nodes
        ExpectResponse(AddToBack(sList, &testData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(sList), &testData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(sList), &testData[2]), LL_OK);
        ExpectListWith3Nodes(sList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Test: Remove head node from a list with 3 nodes
        ExpectResponse(RemoveNode(GetHead(sList)), LL_OK);
        ExpectListWith2Nodes(sList, testData[1].id, testData[2].id);
        // List is now: 102, 103

        // Precondition: insert 1 more node
        ExpectResponse(AddToFront(sList, &testData[4]), LL_OK);
        ExpectListWith3Nodes(sList, testData[4].id, testData[1].id, testData[2].id);
        // List is now: 105, 102, 103

        // Test: Remove tail node from a list with 3 nodes
        ExpectResponse(RemoveNode(GetTail(sList)), LL_OK);
        ExpectListWith2Nodes(sList, testData[4].id, testData[1].id);
        // List is now: 105, 102

        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
        ExpectResponse(DeleteList(sList), LL_OK);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 32: Set node data for a valid node returns LL_OK");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LL_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert 1 node
        ExpectResponse(AddToFront(dList, &testData[3]), LL_OK);
        ExpectListWith1Node(dList, testData[3].id);
        ExpectEqualPtr(GetData(GetHead(dList)), &testData[3]);

        // Change the head node data in a list with one node
        ExpectResponse(SetData(GetHead(dList), &testData[4]), LL_OK);
        ExpectListWith1Node(dList, testData[4].id);
        ExpectEqualPtr(GetData(GetHead(dList)), &testData[4]);

        // Precondition: insert 1 more node
        ExpectResponse(AddToFront(dList, &testData[0]), LL_OK);
        ExpectListWith2Nodes(dList, testData[0].id, testData[4].id);
        ExpectEqualPtr(GetData(GetHead(dList)), &testData[0]);
        ExpectEqualPtr(GetData(GetTail(dList)), &testData[4]);

        // Change the head and tail data in a list with two nodes
        ExpectResponse(SetData(GetHead(dList), &testData[1]), LL_OK);
        ExpectResponse(SetData(GetTail(dList), &testData[2]), LL_OK);
        ExpectListWith2Nodes(dList, testData[1].id, testData[2].id);
        ExpectEqualPtr(GetData(GetHead(dList)), &testData[1]);
        ExpectEqualPtr(GetData(GetTail(dList)), &testData[2]);

        // Cleanup
        ExpectResponse(DeleteList(dList), LL_OK);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    // Test report:
    printf("---> Number of failed tests: %u\n\n", numFailedTests);

    // Warn if there is a mismatch between the number of TestStart and TestEnd functions:
    if (testStartEndBalance != 0)
    {
        printf("!!! Warning! the number of TestStart() functions should match the number of TestEnd() functions!\n\n");
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Memory management test:
    #ifdef MEM_TEST_ENAB_H
        printf("---> Memory management test status: %s\n\n", mTestStatusToString(mtGetStatus()));
    #endif

    return 0;
}

static void ExpectResponse(ListStatus_t actual, ListStatus_t expected)
{
    if(actual != expected)
    {
        numFailedSubpoints++;
    }
}

static void ExpectPointerNull(void* pointer)
{
    if(pointer != NULL)
    {
        numFailedSubpoints++;
    }
}

static void ExpectPointerNotNull(void* pointer)
{
    if(pointer == NULL)
    {
        numFailedSubpoints++;
    }
}

static void ExpectEqual(unsigned int this, unsigned int toThis)
{
    if (this != toThis)
    {
        numFailedSubpoints++;
    }
}

static void ExpectEqualPtr(void* this, void* toThis)
{
    if (this != toThis)
    {
        numFailedSubpoints++;
    }
}

static void ExpectNotEqualPtr(void* this, void* toThis)
{
    if (this == toThis)
    {
        numFailedSubpoints++;
    }
}

static void ExpectEmptyList(List_t* List)
{
    ExpectPointerNotNull(List);
    ExpectPointerNull(GetHead(List));
    ExpectPointerNull(GetTail(List));

    unsigned int nodeCount;
    ExpectResponse(GetCount(List, &nodeCount), LL_OK);
    ExpectEqual(nodeCount, 0);
}

static void ExpectListWith1Node(List_t* List, int id1)
{
    unsigned int nodeCount;
    ExpectPointerNotNull(List);
    ListNode_t* head = GetHead(List);
    ListNode_t* tail = GetTail(List);

    ExpectPointerNotNull(head);
    ExpectPointerNotNull(tail);
    ExpectEqualPtr(head, tail);
    ExpectPointerNull(GetNext(head));
    ExpectPointerNull(GetNext(tail));

    if(List->Linkage == LL_DOUBLE)
    {
        ExpectPointerNull(GetPrev(head));
        ExpectPointerNull(GetPrev(tail));
    }

    ExpectResponse(GetCount(List, &nodeCount), LL_OK);
    ExpectEqual(nodeCount, 1);
    ExpectEqual(((TestData_t*)GetData(head))->id, id1);
    ExpectEqual(((TestData_t*)GetData(tail))->id, id1);

}

static void ExpectListWith2Nodes(List_t* List, int id1, int id2)
{
    unsigned int nodeCount;
    ExpectPointerNotNull(List);
    ListNode_t* head = GetHead(List);
    ListNode_t* tail = GetTail(List);
    ExpectPointerNotNull(head);
    ExpectPointerNotNull(tail);
    ExpectNotEqualPtr(head, tail);
    ExpectPointerNull(GetNext(tail));
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectPointerNull(GetPrev(head));
    }
    ExpectResponse(GetCount(List, &nodeCount), LL_OK);
    ExpectEqual(nodeCount, 2);
    ExpectEqual(((TestData_t*)GetData(head))->id, id1);
    ExpectEqual(((TestData_t*)GetData(tail))->id, id2);
    ExpectEqual(((TestData_t*)GetData(GetNext(head)))->id, id2);
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectEqual(((TestData_t*)GetData(GetPrev(tail)))->id, id1);
    }
}

static void ExpectListWith3Nodes(List_t* List, int id1, int id2, int id3)
{
    unsigned int nodeCount;
    ExpectPointerNotNull(List);
    ListNode_t* head = GetHead(List);
    ListNode_t* tail = GetTail(List);
    ExpectPointerNotNull(head);
    ExpectPointerNotNull(tail);
    ExpectNotEqualPtr(head, tail);
    ExpectPointerNull(GetNext(tail));
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectPointerNull(GetPrev(head));
    }
    ExpectResponse(GetCount(List, &nodeCount), LL_OK);
    ExpectEqual(nodeCount, 3);
    ExpectEqual(((TestData_t*)GetData(head))->id, id1);
    ExpectEqual(((TestData_t*)GetData(tail))->id, id3);
    ExpectEqual(((TestData_t*)GetData(GetNext(head)))->id, id2);
    ExpectEqual(((TestData_t*)GetData(GetNext(GetNext(head))))->id, id3);
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectEqual(((TestData_t*)GetData(GetPrev(tail)))->id, id2);
        ExpectEqual(((TestData_t*)GetData(GetPrev(GetPrev(tail))))->id, id1);
    }
}

static void ExpectListWith4Nodes(List_t* List, int id1, int id2, int id3, int id4)
{
    unsigned int nodeCount;
    ExpectPointerNotNull(List);
    ListNode_t* head = GetHead(List);
    ListNode_t* tail = GetTail(List);
    ExpectPointerNotNull(head);
    ExpectPointerNotNull(tail);
    ExpectNotEqualPtr(head, tail);
    ExpectPointerNull(GetNext(tail));
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectPointerNull(GetPrev(head));
    }
    ExpectResponse(GetCount(List, &nodeCount), LL_OK);
    ExpectEqual(nodeCount, 4);
    ExpectEqual(((TestData_t*)GetData(head))->id, id1);
    ExpectEqual(((TestData_t*)GetData(tail))->id, id4);
    ExpectEqual(((TestData_t*)GetData(GetNext(head)))->id, id2);
    ExpectEqual(((TestData_t*)GetData(GetNext(GetNext(head))))->id, id3);
    ExpectEqual(((TestData_t*)GetData(GetNext(GetNext(GetNext(head)))))->id, id4);
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectEqual(((TestData_t*)GetData(GetPrev(tail)))->id, id3);
        ExpectEqual(((TestData_t*)GetData(GetPrev(GetPrev(tail))))->id, id2);
        ExpectEqual(((TestData_t*)GetData(GetPrev(GetPrev(GetPrev(tail)))))->id, id1);
    }
}

static void ExpectListWith5Nodes(List_t* List, int id1, int id2, int id3, int id4, int id5)
{
    unsigned int nodeCount;
    ExpectPointerNotNull(List);
    ListNode_t* head = GetHead(List);
    ListNode_t* tail = GetTail(List);
    ExpectPointerNotNull(head);
    ExpectPointerNotNull(tail);
    ExpectNotEqualPtr(head, tail);
    ExpectPointerNull(GetNext(tail));
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectPointerNull(GetPrev(head));
    }
    ExpectResponse(GetCount(List, &nodeCount), LL_OK);
    ExpectEqual(nodeCount, 5);
    ExpectEqual(((TestData_t*)GetData(head))->id, id1);
    ExpectEqual(((TestData_t*)GetData(tail))->id, id5);
    ExpectEqual(((TestData_t*)GetData(GetNext(head)))->id, id2);
    ExpectEqual(((TestData_t*)GetData(GetNext(GetNext(head))))->id, id3);
    ExpectEqual(((TestData_t*)GetData(GetNext(GetNext(GetNext(head)))))->id, id4);
    ExpectEqual(((TestData_t*)GetData(GetNext(GetNext(GetNext(GetNext(head))))))->id, id5);
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectEqual(((TestData_t*)GetData(GetPrev(tail)))->id, id4);
        ExpectEqual(((TestData_t*)GetData(GetPrev(GetPrev(tail))))->id, id3);
        ExpectEqual(((TestData_t*)GetData(GetPrev(GetPrev(GetPrev(tail)))))->id, id2);
        ExpectEqual(((TestData_t*)GetData(GetPrev(GetPrev(GetPrev(GetPrev(tail))))))->id, id1);
    }
}

static void TestStart(const char* const header)
{
    printf("%s\n", header);
    numFailedSubpoints = 0;
    testStartEndBalance++;
}

static void TestEnd(void)
{
    if(numFailedSubpoints > 0)
    {
        numFailedTests++;
        printf("Failed!\n\n");
        numFailedSubpoints = 0;
    }
    else
    {
        printf("Passed!\n\n");
    }

    testStartEndBalance--;
}