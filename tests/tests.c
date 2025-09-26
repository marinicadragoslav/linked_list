#include <stdio.h>
#include "linked_list.h"

typedef struct TestData
{
    int Id;
    float Age;
    char* Name[10];
}TestData_t;

/* Test helper functions */
static void TestStart(const char* const Header);
static void TestEnd(void);
static void ExpectResponse(ListStatus_t Actual, ListStatus_t Expected);
static void ExpectPtrNull(void* Ptr);
static void ExpectPtrNotNull(void* Ptr);
static void ExpectEqual(unsigned int A, unsigned int B);
static void ExpectEqualPtr(void* PtrA, void* PtrB);
static void ExpectNotEqualPtr(void* PtrA, void* PtrB);
static void ExpectEmptyList(List_t* List);
static void ExpectListWith1Node(List_t* List, int Id1);
static void ExpectListWith2Nodes(List_t* List, int Id1, int Id2);
static void ExpectListWith3Nodes(List_t* List, int Id1, int Id2, int Id3);
static void ExpectListWith4Nodes(List_t* List, int Id1, int Id2, int Id3, int Id4);
static void ExpectListWith5Nodes(List_t* List, int Id1, int Id2, int Id3, int Id4, int Id5);

/* Test report variables */
unsigned int NumFailedSubpoints = 0;
unsigned int NumFailedTests = 0;
int TestStartEndBalance = 0;

/* Test data to insert in lists */
TestData_t DummyData = {0};
TestData_t TestData[5] = {{.Id = 101}, {.Id = 102}, {.Id = 103}, {.Id = 104}, {.Id = 105}};

int main(void)
{
    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 1: Create lists with different types of linkage");
    {
        /* Positive test: Create valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);
        ExpectEqual((unsigned int)DList->Linkage, (unsigned int)LL_DOUBLE);

        /* Positive test: Create valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);
        ExpectEqual(SList->Linkage, LL_SINGLE);

        /* Create an invalid list -> should return NULL */
        List_t* InvList = LL_NewList(5); /* argument out of range  */
        ExpectPtrNull(InvList);

        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
        ExpectResponse(LL_DeleteList(InvList), LL_NOT_OK); /* can't delete a NULL list */
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 2: NULL list argument => return NULL node");
    {
        List_t* NullList = NULL;
        ExpectPtrNull(LL_GetHead(NullList));
        ExpectPtrNull(LL_GetTail(NullList));
        ExpectPtrNull(LL_GetNodeByData(NullList, &DummyData));
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 3: NULL list argument => return LL_NOT_OK");
    {
        List_t* NullList = NULL;
        unsigned int Count;
        ExpectResponse(LL_AddToFront(NullList, &DummyData), LL_NOT_OK);
        ExpectResponse(LL_AddToBack(NullList, &DummyData), LL_NOT_OK);
        ExpectResponse(LL_InsertAfterData(NullList, &DummyData, &DummyData), LL_NOT_OK);
        ExpectResponse(LL_RemoveHead(NullList), LL_NOT_OK);
        ExpectResponse(LL_RemoveTail(NullList), LL_NOT_OK);
        ExpectResponse(LL_RemoveNodeByData(NullList, &DummyData), LL_NOT_OK);
        ExpectResponse(LL_GetCount(NullList, &Count), LL_NOT_OK);
        ExpectResponse(LL_DeleteList(NullList), LL_NOT_OK);
    }
    TestEnd();
    
    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 4: NULL node argument => return NULL Ptr");
    {
        ListNode_t* NullNode = NULL;
        ExpectPtrNull(LL_GetNext(NullNode));
        ExpectPtrNull(LL_GetPrev(NullNode));
        ExpectPtrNull(LL_GetData(NullNode));
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 5: NULL node argument => return LL_NOT_OK");
    {
        ListNode_t* NullNode = NULL;
        ExpectResponse(LL_InsertAfterNode(NullNode, &DummyData), LL_NOT_OK);
        ExpectResponse(LL_RemoveNode(NullNode), LL_NOT_OK);
        ExpectResponse(LL_SetData(NullNode, &DummyData), LL_NOT_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 6: NULL data argument => return NULL node");
    {
        void* NullData = NULL;

        /* Create a valid empty list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: add at least one node, otherwise GetNodeByData will  */
        /* return NULL because of the empty list rather than the NULL data. */
        ExpectResponse(LL_AddToFront(SList, &DummyData), LL_OK);
        ExpectListWith1Node(SList, DummyData.Id);
        
        /* Test */
        ExpectPtrNull(LL_GetNodeByData(SList, NullData));

        /* Cleanup */
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();
    
    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 7: NULL data argument => return LL_NOT_OK");
    {
        void* NullData = NULL;

        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Try inserting NULL data into an empty list */
        ExpectResponse(LL_AddToFront(SList, NullData), LL_NOT_OK);
        ExpectResponse(LL_AddToBack(SList, NullData), LL_NOT_OK);

        /* Insert one node */
        ExpectResponse(LL_AddToFront(SList, &DummyData), LL_OK);
        ExpectListWith1Node(SList, DummyData.Id);

        /* Try inserting NULL data after a valid node (list must have at least one node) */
        ExpectResponse(LL_InsertAfterNode(LL_GetHead(SList), NullData), LL_NOT_OK);

        /* Try inserting NULL data after valid data (list must have at least one node) */
        ExpectResponse(LL_InsertAfterData(SList, LL_GetData(LL_GetHead(SList)), NullData), LL_NOT_OK);

        /* Try inserting valid data after NULL data (list must have at least one node) */
        ExpectResponse(LL_InsertAfterData(SList, NullData, LL_GetData(LL_GetHead(SList))), LL_NOT_OK);

        /* Try removing NULL data from a list that has at least one node. */
        ExpectResponse(LL_RemoveNodeByData(SList, NullData), LL_NOT_OK);

        /* Try setting the data of an existing node to NULL */
        ExpectResponse(LL_SetData(LL_GetHead(SList), NullData), LL_NOT_OK);

        /* Cleanup */
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }    
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 8: NULL Counter argument => return LL_NOT_OK");
    {
        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Test */
        unsigned int* CountPtr = NULL;
        ExpectResponse(LL_GetCount(SList, CountPtr), LL_NOT_OK);

        /* Cleanup */
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 9: Empty list argument => return NULL node");
    {
        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Test */
        ExpectPtrNull(LL_GetHead(SList));
        ExpectPtrNull(LL_GetTail(SList));
        ExpectPtrNull(LL_GetNodeByData(SList, &DummyData));

        /* Cleanup */
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 10: Remove from empty list => return LL_NOT_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Test */
        ExpectResponse(LL_RemoveHead(DList), LL_NOT_OK);
        ExpectResponse(LL_RemoveTail(DList), LL_NOT_OK);
        ExpectResponse(LL_RemoveNodeByData(DList, &DummyData), LL_NOT_OK);
        
        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 11: Insert data after existing data in an empty list => return LL_NOT_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Test */
        ExpectResponse(LL_InsertAfterData(DList, &DummyData, &DummyData), LL_NOT_OK);

        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
    }    
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 12: Insert data to the front/back of an empty list => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList); 

        /* Insert to front */
        ExpectResponse(LL_AddToFront(DList, &DummyData), LL_OK);
        ExpectListWith1Node(DList, 0);

        /* Remove node */
        ExpectResponse(LL_RemoveHead(DList), LL_OK);
        ExpectEmptyList(DList);

        /* Insert to back */
        ExpectResponse(LL_AddToBack(DList, &DummyData), LL_OK);
        ExpectListWith1Node(DList, 0);

        /* Remove node */
        ExpectResponse(LL_RemoveTail(DList), LL_OK);
        ExpectEmptyList(DList);

        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList); 

         /* Insert to front */
        ExpectResponse(LL_AddToFront(SList, &DummyData), LL_OK);
        ExpectListWith1Node(SList, 0);

        /* Remove node */
        ExpectResponse(LL_RemoveHead(SList), LL_OK);
        ExpectEmptyList(SList);

        /* Insert to back */
        ExpectResponse(LL_AddToBack(SList, &DummyData), LL_OK);
        ExpectListWith1Node(SList, 0);

        /* Remove node */
        ExpectResponse(LL_RemoveTail(SList), LL_OK);
        ExpectEmptyList(SList);
        
        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 13: Insert a node to the front of a non-empty list => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToFront(DList, &TestData[0]), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);

        /* Test: Insert a node */
        ExpectResponse(LL_AddToFront(DList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[1].Id, TestData[0].Id); 

        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToFront(SList, &TestData[0]), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);

        /* Test: Insert a node */
        ExpectResponse(LL_AddToFront(SList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[1].Id, TestData[0].Id); 

        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 14: Remove nodes from the back of a non-empty list => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert two nodes */
        ExpectResponse(LL_AddToFront(DList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToFront(DList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[1].Id, TestData[0].Id); 

        /* Test */
        ExpectResponse(LL_RemoveTail(DList), LL_OK);
        ExpectListWith1Node(DList, TestData[1].Id);
        ExpectResponse(LL_RemoveTail(DList), LL_OK);
        ExpectEmptyList(DList);

        /* Attempt to remove from empty list (already tested - see test 10) */
        ExpectResponse(LL_RemoveTail(DList), LL_NOT_OK); 

        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert two nodes */
        ExpectResponse(LL_AddToFront(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToFront(SList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[1].Id, TestData[0].Id);

        /* Test */
        ExpectResponse(LL_RemoveTail(SList), LL_OK);
        ExpectListWith1Node(SList, TestData[1].Id);
        ExpectResponse(LL_RemoveTail(SList), LL_OK);
        ExpectEmptyList(SList);

        /* Attempt to remove from empty list (already tested - see test 10) */
        ExpectResponse(LL_RemoveTail(SList), LL_NOT_OK); 

        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 15: Insert a node to the back of a non-empty list => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);

        /* Test: Insert another node */
        ExpectResponse(LL_AddToBack(DList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);

        /* Test: Insert a third node */
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[0].Id);

        /* Test: Insert a fourth node */
        ExpectResponse(LL_AddToBack(DList, &TestData[4]), LL_OK);
        ExpectListWith4Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[0].Id, TestData[4].Id);

        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);

        /* Test: Insert another node */
        ExpectResponse(LL_AddToBack(SList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);

        /* Test: Insert a third node */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[0].Id);

        /* Test: Insert a fourth node */
        ExpectResponse(LL_AddToBack(SList, &TestData[4]), LL_OK);
        ExpectListWith4Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[0].Id, TestData[4].Id);

        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 16: Remove nodes from the front of a non-empty list => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert two nodes */
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToBack(DList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);

        /* Test */
        ExpectResponse(LL_RemoveHead(DList), LL_OK);
        ExpectListWith1Node(DList, TestData[1].Id);
        ExpectResponse(LL_RemoveHead(DList), LL_OK);
        ExpectEmptyList(DList);

        /* Attempt to remove from empty list (already tested - see test 10) */
        ExpectResponse(LL_RemoveHead(DList), LL_NOT_OK);

        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert two nodes */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToBack(SList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);

        /* Test */
        ExpectResponse(LL_RemoveHead(SList), LL_OK);
        ExpectListWith1Node(SList, TestData[1].Id);
        ExpectResponse(LL_RemoveHead(SList), LL_OK);
        ExpectEmptyList(SList);

        /* Attempt to remove from empty list (already tested - see test 10) */
        ExpectResponse(LL_RemoveHead(SList), LL_NOT_OK);

        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 17: Insert a node after the head node => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);

        /* Test: Insert a node after head */
        ExpectResponse(LL_InsertAfterNode(LL_GetHead(DList), &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);

        /* Test: Insert another node after head */
        ExpectResponse(LL_InsertAfterNode(LL_GetHead(DList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[2].Id, TestData[1].Id);

        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);

        /* Test: Insert a node after head */
        ExpectResponse(LL_InsertAfterNode(LL_GetHead(SList), &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);

        /* Test: Insert another node after head */
        ExpectResponse(LL_InsertAfterNode(LL_GetHead(SList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[2].Id, TestData[1].Id);

        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 18: Insert a node after the tail node => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);

        /* Test: Insert a node after tail */
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(DList), &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);

        /* Test: Insert another node after tail */
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(DList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[2].Id);

        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);

        /* Test: Insert a node after tail */
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(SList), &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);

        /* Test: Insert another node after tail */
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(SList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id);

        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 19: Insert a node after any middle node => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(DList), &TestData[1]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(DList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Test: Add 104 after 102 */
        ExpectResponse(LL_InsertAfterNode(LL_GetNext(LL_GetHead(DList)), &TestData[3]), LL_OK);
        ExpectListWith4Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[3].Id, TestData[2].Id);  
        /* List is now: 101, 102, 104, 103 */

        /* Test: Add 105 after 104 */
        ExpectResponse(LL_InsertAfterNode(LL_GetPrev(LL_GetTail(DList)), &TestData[4]), LL_OK);
        ExpectListWith5Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[3].Id, TestData[4].Id, TestData[2].Id);
        /* List is now: 101, 102, 104, 105, 103 */

        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(SList), &TestData[1]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(SList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Test: Add 104 after 102 */
        ExpectResponse(LL_InsertAfterNode(LL_GetNext(LL_GetHead(SList)), &TestData[3]), LL_OK);
        ExpectListWith4Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[3].Id, TestData[2].Id);
        /* List is now: 101, 102, 104, 103 */

        /* Test: Add 105 after 104 */
        ExpectResponse(LL_InsertAfterNode(LL_GetNext(LL_GetNext(LL_GetHead(SList))), &TestData[4]), LL_OK);
        ExpectListWith5Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[3].Id, TestData[4].Id, TestData[2].Id);
        /* List is now: 101, 102, 104, 105, 103 */

        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 20: Insert data after inexistent data, when list is non-empty => return LL_NOT_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);

        /* Save head & tail */
        ListNode_t* HeadDList1 = LL_GetHead(DList);
        ListNode_t* TailDList1 = LL_GetTail(DList);

        /* Test: Try inserting a node after inexistent data: */
        ExpectResponse(LL_InsertAfterData(DList, &DummyData, &TestData[0]), LL_NOT_OK);
        ExpectListWith1Node(DList, TestData[0].Id);

        /* Head and tail didn't change */
        ExpectEqualPtr(LL_GetHead(DList), HeadDList1);
        ExpectEqualPtr(LL_GetTail(DList), TailDList1);

        /* Insert another node: */
        ExpectResponse(LL_AddToBack(DList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);

        /* Save head & tail */
        ListNode_t* HeadDList2 = LL_GetHead(DList);
        ListNode_t* TailDList2 = LL_GetTail(DList);

        /* Test: Try inserting another node after inexistent data: */
        ExpectResponse(LL_InsertAfterData(DList, &TestData[2], &TestData[3]), LL_NOT_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);

        /* Head and tail didn't change */
        ExpectEqualPtr(LL_GetHead(DList), HeadDList2);
        ExpectEqualPtr(LL_GetTail(DList), TailDList2);

        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);

        /* Save head & tail */
        ListNode_t* HeadSList1 = LL_GetHead(SList);
        ListNode_t* TailSList1 = LL_GetTail(SList);

        /* Test: Try inserting a node after inexistent data: */
        ExpectResponse(LL_InsertAfterData(SList, &DummyData, &TestData[0]), LL_NOT_OK);
        ExpectListWith1Node(SList, TestData[0].Id);

        /* Head and tail didn't change */
        ExpectEqualPtr(LL_GetHead(SList), HeadSList1);
        ExpectEqualPtr(LL_GetTail(SList), TailSList1);

        /* Insert another node: */
        ExpectResponse(LL_AddToBack(SList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);

        /* Save head & tail */
        ListNode_t* HeadSList2 = LL_GetHead(SList);
        ListNode_t* TailSList2 = LL_GetTail(SList);

        /* Test: Try inserting another node after inexistent data: */
        ExpectResponse(LL_InsertAfterData(SList, &TestData[2], &TestData[3]), LL_NOT_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);

        /* Head and tail didn't change */
        ExpectEqualPtr(LL_GetHead(SList), HeadSList2);
        ExpectEqualPtr(LL_GetTail(SList), TailSList2);

        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }    
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 21: Insert data after existing head data => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToFront(DList, &TestData[0]), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);

        /* Save head & tail */
        ListNode_t* HeadDList1 = LL_GetHead(DList);
        ListNode_t* TailDList1 = LL_GetTail(DList);

        /* Test: Try inserting some data after the head data: */
        ExpectResponse(LL_InsertAfterData(DList, &TestData[0], &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);

        /* Check that the tail node has changed (head node is the same) */
        ListNode_t* HeadDList2 = LL_GetHead(DList);
        ListNode_t* TailDList2 = LL_GetTail(DList);
        ExpectEqualPtr(HeadDList1, HeadDList2);
        ExpectNotEqualPtr(TailDList1, TailDList2);

        /* Test: Try inserting some data after the head data: */
        ExpectResponse(LL_InsertAfterData(DList, &TestData[0], &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[2].Id, TestData[1].Id);

        /* Check that both head and tail nodes are unchanged */
        ListNode_t* HeadDList3 = LL_GetHead(DList);
        ListNode_t* TailDList3 = LL_GetTail(DList);
        ExpectEqualPtr(HeadDList2, HeadDList3);
        ExpectEqualPtr(TailDList2, TailDList3);

        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);

        /* Save head & tail */
        ListNode_t* HeadSList1 = LL_GetHead(SList);
        ListNode_t* TailSList1 = LL_GetTail(SList);

        /* Test: Try inserting some data after the head data: */
        ExpectResponse(LL_InsertAfterData(SList, &TestData[0], &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);

        /* Check that the tail node has changed (head node is the same) */
        ListNode_t* HeadSList2 = LL_GetHead(SList);
        ListNode_t* TailSList2 = LL_GetTail(SList);
        ExpectEqualPtr(HeadSList1, HeadSList2);
        ExpectNotEqualPtr(TailSList1, TailSList2);

        /* Test: Try inserting some data after the head data: */
        ExpectResponse(LL_InsertAfterData(SList, &TestData[0], &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id,  TestData[2].Id, TestData[1].Id);

        /* Check that both head and tail nodes are unchanged */
        ListNode_t* HeadSList3 = LL_GetHead(SList);
        ListNode_t* TailSList3 = LL_GetTail(SList);
        ExpectEqualPtr(HeadSList2, HeadSList3);
        ExpectEqualPtr(TailSList2, TailSList3);

        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();    

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 22: Insert data after existing data when existing data is in the tail => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToFront(DList, &TestData[0]), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);

        /* Save head & tail */
        ListNode_t* HeadDList1 = LL_GetHead(DList);
        ListNode_t* TailDList1 = LL_GetTail(DList);

        /* Test: Try inserting some data after the tail data: */
        ExpectResponse(LL_InsertAfterData(DList, &TestData[0], &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);

        /* Check that the tail node has changed (head is the same) */
        ListNode_t* HeadDList2 = LL_GetHead(DList);
        ListNode_t* TailDList2 = LL_GetTail(DList);
        ExpectEqualPtr(HeadDList1, HeadDList2);
        ExpectNotEqualPtr(TailDList1, TailDList2);

        /* Test: Try inserting some more data after the tail data: */
        ExpectResponse(LL_InsertAfterData(DList, &TestData[1], &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[2].Id);

        /* Check that the tail node has changed (head is the same) */
        ListNode_t* HeadDList3 = LL_GetHead(DList);
        ListNode_t* TailDList3 = LL_GetTail(DList);
        ExpectEqualPtr(HeadDList2, HeadDList3);
        ExpectNotEqualPtr(TailDList2, TailDList3);

        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);

        /* Save head & tail */
        ListNode_t* HeadSList1 = LL_GetHead(SList);
        ListNode_t* TailSList1 = LL_GetTail(SList);

        /* Test: Try inserting some data after the tail data: */
        ExpectResponse(LL_InsertAfterData(SList, &TestData[0], &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);

        /* Check that the tail node has changed (head is the same) */
        ListNode_t* HeadSList2 = LL_GetHead(SList);
        ListNode_t* TailSList2 = LL_GetTail(SList);
        ExpectEqualPtr(HeadSList1, HeadSList2);
        ExpectNotEqualPtr(TailSList1, TailSList2);

        /* Test: Try inserting some data after the tail data: */
        ExpectResponse(LL_InsertAfterData(SList, &TestData[1], &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id);

        /* Check that the tail node has changed (head is the same) */
        ListNode_t* HeadSList3 = LL_GetHead(SList);
        ListNode_t* TailSList3 = LL_GetTail(SList);
        ExpectEqualPtr(HeadSList2, HeadSList3);
        ExpectNotEqualPtr(TailSList2, TailSList3);

        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();    

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 23: Insert data after existing data when existing data is in the middle => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(DList), &TestData[1]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(DList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Save head and tail */
        ListNode_t* HeadDList1 = LL_GetHead(DList);
        ListNode_t* TailDList1 = LL_GetTail(DList);

        /* Test: Try inserting some data after the data in the middle node: */
        ExpectResponse(LL_InsertAfterData(DList, &TestData[1], &TestData[3]), LL_OK);
        ExpectListWith4Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[3].Id, TestData[2].Id);
        /* List is now: 101, 102, 104, 103 */

        /* Check that the head & tail nodes didn't change */
        ListNode_t* HeadDList2 = LL_GetHead(DList);
        ListNode_t* TailDList2 = LL_GetTail(DList);
        ExpectEqualPtr(HeadDList1, HeadDList2);
        ExpectEqualPtr(TailDList1, TailDList2);

        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(SList), &TestData[1]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(SList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Save head and tail */
        ListNode_t* HeadSList1 = LL_GetHead(SList);
        ListNode_t* TailSList1 = LL_GetTail(SList);

        /* Test: Try inserting some data after the data in the middle node: */
        ExpectResponse(LL_InsertAfterData(SList, &TestData[1], &TestData[3]), LL_OK);
        ExpectListWith4Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[3].Id, TestData[2].Id);
        /* List is now: 101, 102, 104, 103 */

        /* Check that the head & tail nodes didn't change */
        ListNode_t* HeadSList2 = LL_GetHead(SList);
        ListNode_t* TailSList2 = LL_GetTail(SList);
        ExpectEqualPtr(HeadSList1, HeadSList2);
        ExpectEqualPtr(TailSList1, TailSList2);
        
        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }    
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 24: Remove node from the front/back of a non-empty list => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToFront(DList, &DummyData), LL_OK);
        ExpectListWith1Node(DList, DummyData.Id);

        /* Test: List with one node -> remove from front */
        ExpectResponse(LL_RemoveHead(DList), LL_OK);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToBack(DList, &DummyData), LL_OK);
        ExpectListWith1Node(DList, DummyData.Id);

        /* Test: List with one node -> remove from back */
        ExpectResponse(LL_RemoveTail(DList), LL_OK);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToFront(DList, &DummyData), LL_OK);
        ExpectListWith1Node(DList, DummyData.Id);

        /* Test: List with one node -> remove from back */
        ExpectResponse(LL_RemoveTail(DList), LL_OK);
        ExpectEmptyList(DList);
        
        /* Precondition: insert one node */
        ExpectResponse(LL_AddToBack(DList, &DummyData), LL_OK);
        ExpectListWith1Node(DList, DummyData.Id);

        /* Test: List with one node -> remove from front */
        ExpectResponse(LL_RemoveHead(DList), LL_OK);
        ExpectEmptyList(DList);

        /* Precondition: Add 2 nodes */
        ExpectResponse(LL_AddToFront(DList, &TestData[4]), LL_OK);
        ExpectResponse(LL_AddToFront(DList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[3].Id, TestData[4].Id);

        /* Test: List with 2 nodes -> remove from front */
        ExpectResponse(LL_RemoveHead(DList), LL_OK);
        ExpectListWith1Node(DList, TestData[4].Id);

        /* Precondition: Add one more node */
        ExpectResponse(LL_AddToBack(DList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[4].Id, TestData[3].Id);

        /* Test: List with 2 nodes -> remove from back */
        ExpectResponse(LL_RemoveTail(DList), LL_OK);
        ExpectListWith1Node(DList, TestData[4].Id);

        /* Precondition: Add one more node */
        ExpectResponse(LL_AddToFront(DList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[3].Id, TestData[4].Id);

        /* Test: List with 2 nodes -> remove from back */
        ExpectResponse(LL_RemoveTail(DList), LL_OK);
        ExpectListWith1Node(DList, TestData[3].Id);

        /* Precondition: Add one more node */
        ExpectResponse(LL_AddToBack(DList, &TestData[4]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[3].Id, TestData[4].Id);

        /* Test: List with 2 nodes -> remove from front */
        ExpectResponse(LL_RemoveHead(DList), LL_OK);
        ExpectListWith1Node(DList, TestData[4].Id);

        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToFront(SList, &DummyData), LL_OK);
        ExpectListWith1Node(SList, DummyData.Id);

        /* Test: List with one node -> remove from front */
        ExpectResponse(LL_RemoveHead(SList), LL_OK);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToBack(SList, &DummyData), LL_OK);
        ExpectListWith1Node(SList, DummyData.Id);

        /* Test: List with one node -> remove from back */
        ExpectResponse(LL_RemoveTail(SList), LL_OK);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToFront(SList, &DummyData), LL_OK);
        ExpectListWith1Node(SList, DummyData.Id);

        /* Test: List with one node -> remove from back */
        ExpectResponse(LL_RemoveTail(SList), LL_OK);
        ExpectEmptyList(SList);
        
        /* Precondition: insert one node */
        ExpectResponse(LL_AddToBack(SList, &DummyData), LL_OK);
        ExpectListWith1Node(SList, DummyData.Id);

        /* Test: List with one node -> remove from front */
        ExpectResponse(LL_RemoveHead(SList), LL_OK);
        ExpectEmptyList(SList);

        /* Precondition: Add 2 nodes */
        ExpectResponse(LL_AddToFront(SList, &TestData[4]), LL_OK);
        ExpectResponse(LL_AddToFront(SList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[3].Id, TestData[4].Id);

        /* Test: List with 2 nodes -> remove from front */
        ExpectResponse(LL_RemoveHead(SList), LL_OK);
        ExpectListWith1Node(SList, TestData[4].Id);

        /* Precondition: Add one more node */
        ExpectResponse(LL_AddToBack(SList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[4].Id, TestData[3].Id);

        /* Test: List with 2 nodes -> remove from back */
        ExpectResponse(LL_RemoveTail(SList), LL_OK);
        ExpectListWith1Node(SList, TestData[4].Id);

        /* Precondition: Add one more node */
        ExpectResponse(LL_AddToFront(SList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[3].Id, TestData[4].Id);

        /* Test: List with 2 nodes -> remove from back */
        ExpectResponse(LL_RemoveTail(SList), LL_OK);
        ExpectListWith1Node(SList, TestData[3].Id);

        /* Precondition: Add one more node */
        ExpectResponse(LL_AddToBack(SList, &TestData[4]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[3].Id, TestData[4].Id);

        /* Test: List with 2 nodes -> remove from front */
        ExpectResponse(LL_RemoveHead(SList), LL_OK);
        ExpectListWith1Node(SList, TestData[4].Id);

        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 25: Remove node from the middle of a non-empty list => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(DList), &TestData[1]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(DList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Save head and tail */
        ListNode_t* HeadDList1 = LL_GetHead(DList);
        ListNode_t* TailDList1 = LL_GetTail(DList);

        /* Test: Try to remove the node from the middle */
        ExpectResponse(LL_RemoveNode(LL_GetNext(LL_GetHead(DList))), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[2].Id);
        /* List is now: 101, 103 */

        /* Head and tail are unchanged */
        ListNode_t* HeadDList2 = LL_GetHead(DList);
        ListNode_t* TailDList2 = LL_GetTail(DList);
        ExpectEqualPtr(HeadDList1, HeadDList2);
        ExpectEqualPtr(TailDList1, TailDList2);

        /* Precondition: Add 2 more nodes */
        ExpectResponse(LL_AddToFront(DList, &TestData[4]), LL_OK);
        ExpectResponse(LL_AddToFront(DList, &TestData[3]), LL_OK);
        ExpectListWith4Nodes(DList, TestData[3].Id, TestData[4].Id, TestData[0].Id, TestData[2].Id);
        /* List is now: 104, 105, 101, 103 */

        /* Save head & tail */
        ListNode_t* HeadDList3 = LL_GetHead(DList);
        ListNode_t* TailDList3 = LL_GetTail(DList);

        /* Try to remove the nodes from the middle */
        ExpectResponse(LL_RemoveNode(LL_GetNext(LL_GetNext(LL_GetHead(DList)))), LL_OK);
        ExpectListWith3Nodes(DList, TestData[3].Id, TestData[4].Id, TestData[2].Id);
        ExpectResponse(LL_RemoveNode(LL_GetNext(LL_GetHead(DList))), LL_OK);
        ExpectListWith2Nodes(DList, TestData[3].Id, TestData[2].Id);
        /* List is now: 104, 103 */

        /* Head and tail are not changed */
        ListNode_t* HeadDList4 = LL_GetHead(DList);
        ListNode_t* TailDList4 = LL_GetTail(DList);
        ExpectEqualPtr(HeadDList3, HeadDList4);
        ExpectEqualPtr(TailDList3, TailDList4);

        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(SList), &TestData[1]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(SList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Save head and tail */
        ListNode_t* HeadSList1 = LL_GetHead(SList);
        ListNode_t* TailSList1 = LL_GetTail(SList);

        /* Test: Try to remove the node from the middle */
        ExpectResponse(LL_RemoveNode(LL_GetNext(LL_GetHead(SList))), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[2].Id);
        /* List is now: 101, 103 */

        /* Head and tail are unchanged */
        ListNode_t* HeadSList2 = LL_GetHead(SList);
        ListNode_t* TailSList2 = LL_GetTail(SList);
        ExpectEqualPtr(HeadSList1, HeadSList2);
        ExpectEqualPtr(TailSList1, TailSList2);

        /* Precondition: Add 2 more nodes */
        ExpectResponse(LL_AddToFront(SList, &TestData[4]), LL_OK);
        ExpectResponse(LL_AddToFront(SList, &TestData[3]), LL_OK);
        ExpectListWith4Nodes(SList, TestData[3].Id, TestData[4].Id, TestData[0].Id, TestData[2].Id);
        /* List is now: 104, 105, 101, 103 */

        /* Save head & tail */
        ListNode_t* HeadSList3 = LL_GetHead(SList);
        ListNode_t* TailSList3 = LL_GetTail(SList);

        /* Try to remove the nodes from the middle */
        ExpectResponse(LL_RemoveNode(LL_GetNext(LL_GetNext(LL_GetHead(SList)))), LL_OK);
        ExpectListWith3Nodes(SList, TestData[3].Id, TestData[4].Id, TestData[2].Id);
        ExpectResponse(LL_RemoveNode(LL_GetNext(LL_GetHead(SList))), LL_OK);
        ExpectListWith2Nodes(SList, TestData[3].Id, TestData[2].Id);
        /* List is now: 104, 103 */

        /* Head and tail are not changed */
        ListNode_t* HeadSList4 = LL_GetHead(SList);
        ListNode_t* TailSList4 = LL_GetTail(SList);
        ExpectEqualPtr(HeadSList3, HeadSList4);
        ExpectEqualPtr(TailSList3, TailSList4);

        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 26: Remove inexistent data from a non-empty list => return LL_NOT_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToFront(DList, &DummyData), LL_OK);
        ExpectListWith1Node(DList, DummyData.Id);

        /* Test: Remove inexistent data from a list with one node */
        ExpectResponse(LL_RemoveNodeByData(DList, &TestData[3]), LL_NOT_OK);
        ExpectListWith1Node(DList, DummyData.Id);

        /* Precondition: insert one more node */
        ExpectResponse(LL_AddToFront(DList, &TestData[4]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[4].Id, DummyData.Id);

        /* Test: Remove inexistent data from a list with two nodes */
        ExpectResponse(LL_RemoveNodeByData(DList, &TestData[3]), LL_NOT_OK);
        ExpectListWith2Nodes(DList, TestData[4].Id, DummyData.Id);

        /* Precondition: insert one more node */
        ExpectResponse(LL_AddToFront(DList, &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[2].Id, TestData[4].Id, DummyData.Id);

        /* Test: Remove inexistent data from a list with three nodes */
        ExpectResponse(LL_RemoveNodeByData(DList, &TestData[0]), LL_NOT_OK);
        ExpectResponse(LL_RemoveNodeByData(DList, &TestData[3]), LL_NOT_OK);

        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToFront(SList, &DummyData), LL_OK);
        ExpectListWith1Node(SList, DummyData.Id);

        /* Test: Remove inexistent data from a list with one node */
        ExpectResponse(LL_RemoveNodeByData(SList, &TestData[3]), LL_NOT_OK);
        ExpectListWith1Node(SList, DummyData.Id);

        /* Precondition: insert one more node */
        ExpectResponse(LL_AddToFront(SList, &TestData[4]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[4].Id, DummyData.Id);

        /* Test: Remove inexistent data from a list with two nodes */
        ExpectResponse(LL_RemoveNodeByData(SList, &TestData[3]), LL_NOT_OK);
        ExpectListWith2Nodes(SList, TestData[4].Id, DummyData.Id);

        /* Precondition: insert one more node */
        ExpectResponse(LL_AddToFront(SList, &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[2].Id, TestData[4].Id, DummyData.Id);

        /* Test: Remove inexistent data from a list with three nodes */
        ExpectResponse(LL_RemoveNodeByData(SList, &TestData[0]), LL_NOT_OK);
        ExpectResponse(LL_RemoveNodeByData(SList, &TestData[3]), LL_NOT_OK);

        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 27: Remove node by data from the front/back of a non-empty list => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToFront(DList, &DummyData), LL_OK);
        ExpectListWith1Node(DList, DummyData.Id);

        /* Test: Remove node by data from a list with 1 node */
        ExpectResponse(LL_RemoveNodeByData(DList, &DummyData), LL_OK);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToFront(DList, &TestData[3]), LL_OK);
        ExpectListWith1Node(DList, TestData[3].Id);

        /* Test: Remove node by data from a list with 1 node */
        ExpectResponse(LL_RemoveNodeByData(DList, &TestData[3]), LL_OK);
        ExpectEmptyList(DList);

        /* Precondition: insert two nodes */
        ExpectResponse(LL_AddToFront(DList, &TestData[4]), LL_OK);
        ExpectResponse(LL_AddToFront(DList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[3].Id, TestData[4].Id);

        /* Test: Remove node by data from a list with 2 nodes */
        ExpectResponse(LL_RemoveNodeByData(DList, &TestData[4]), LL_OK);
        ExpectListWith1Node(DList, TestData[3].Id);

        /* Precondition: insert two nodes */
        ExpectResponse(LL_AddToFront(DList, &TestData[2]), LL_OK);
        ExpectResponse(LL_AddToFront(DList, &TestData[1]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[1].Id, TestData[2].Id, TestData[3].Id);

        /* Test: Remove node by data from a list with 3 nodes */
        ExpectResponse(LL_RemoveNodeByData(DList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[2].Id, TestData[3].Id);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToFront(DList, &TestData[1]), LL_OK);
        
        /* Test: Remove node by data from a list with 3 nodes */
        ExpectResponse(LL_RemoveNodeByData(DList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[1].Id, TestData[2].Id);

        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToFront(SList, &DummyData), LL_OK);
        ExpectListWith1Node(SList, DummyData.Id);

        /* Test: Remove node by data from a list with 1 node */
        ExpectResponse(LL_RemoveNodeByData(SList, &DummyData), LL_OK);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToFront(SList, &TestData[3]), LL_OK);
        ExpectListWith1Node(SList, TestData[3].Id);

        /* Test: Remove node by data from a list with 1 node */
        ExpectResponse(LL_RemoveNodeByData(SList, &TestData[3]), LL_OK);
        ExpectEmptyList(SList);

        /* Precondition: insert two nodes */
        ExpectResponse(LL_AddToFront(SList, &TestData[4]), LL_OK);
        ExpectResponse(LL_AddToFront(SList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[3].Id, TestData[4].Id);

        /* Test: Remove node by data from a list with 2 nodes */
        ExpectResponse(LL_RemoveNodeByData(SList, &TestData[4]), LL_OK);
        ExpectListWith1Node(SList, TestData[3].Id);

        /* Precondition: insert two nodes */
        ExpectResponse(LL_AddToFront(SList, &TestData[2]), LL_OK);
        ExpectResponse(LL_AddToFront(SList, &TestData[1]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[1].Id, TestData[2].Id, TestData[3].Id);

        /* Test: Remove node by data from a list with 3 nodes */
        ExpectResponse(LL_RemoveNodeByData(SList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[2].Id, TestData[3].Id);

        /* Precondition: insert one node */
        ExpectResponse(LL_AddToFront(SList, &TestData[1]), LL_OK);
        
        /* Test: Remove node by data from a list with 3 nodes */
        ExpectResponse(LL_RemoveNodeByData(SList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[1].Id, TestData[2].Id);

        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 28: Remove node by data from the middle of a list => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(DList), &TestData[1]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(DList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Save head and tail */
        ListNode_t* HeadDList1 = LL_GetHead(DList);
        ListNode_t* TailDList1 = LL_GetTail(DList);

        /* Test: Remove the node from the middle (by data) */
        ExpectResponse(LL_RemoveNodeByData(DList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[2].Id);
        /* List is now: 101, 103 */

        /* Head and tail are unchanged */
        ListNode_t* HeadDList2 = LL_GetHead(DList);
        ListNode_t* TailDList2 = LL_GetTail(DList);
        ExpectEqualPtr(HeadDList1, HeadDList2);
        ExpectEqualPtr(TailDList1, TailDList2);

        /* Precondition: Add 2 more nodes */
        ExpectResponse(LL_AddToFront(DList, &TestData[4]), LL_OK);
        ExpectResponse(LL_AddToFront(DList, &TestData[3]), LL_OK);
        ExpectListWith4Nodes(DList, TestData[3].Id, TestData[4].Id, TestData[0].Id, TestData[2].Id);
        /* List is now: 104, 105, 101, 103 */

        /* Save head and tail */
        ListNode_t* HeadDList3 = LL_GetHead(DList);
        ListNode_t* TailDList3 = LL_GetTail(DList);

        /* Remove the nodes from the middle */
        ExpectResponse(LL_RemoveNodeByData(DList, &TestData[0]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[3].Id, TestData[4].Id, TestData[2].Id);
        ExpectResponse(LL_RemoveNodeByData(DList, &TestData[4]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[3].Id, TestData[2].Id);
        /* List is now: 104, 103 */

        /* Head and tail are not changed */
        ListNode_t* HeadDList4 = LL_GetHead(DList);
        ListNode_t* TailDList4 = LL_GetTail(DList);
        ExpectEqualPtr(HeadDList3, HeadDList4);
        ExpectEqualPtr(TailDList3, TailDList4);

        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(SList), &TestData[1]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(SList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Save head and tail */
        ListNode_t* HeadSList1 = LL_GetHead(SList);
        ListNode_t* TailSList1 = LL_GetTail(SList);

        /* Test: Remove the node from the middle (by data) */
        ExpectResponse(LL_RemoveNodeByData(SList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[2].Id);
        /* List is now: 101, 103 */

        /* Head and tail are unchanged */
        ListNode_t* HeadSList2 = LL_GetHead(SList);
        ListNode_t* TailSList2 = LL_GetTail(SList);
        ExpectEqualPtr(HeadSList1, HeadSList2);
        ExpectEqualPtr(TailSList1, TailSList2);

        /* Precondition: Add 2 more nodes */
        ExpectResponse(LL_AddToFront(SList, &TestData[4]), LL_OK);
        ExpectResponse(LL_AddToFront(SList, &TestData[3]), LL_OK);
        ExpectListWith4Nodes(SList, TestData[3].Id, TestData[4].Id, TestData[0].Id, TestData[2].Id);
        /* List is now: 104, 105, 101, 103 */

        /* Save head and tail */
        ListNode_t* HeadSList3 = LL_GetHead(SList);
        ListNode_t* TailSList3 = LL_GetTail(SList);

        /* Remove the nodes from the middle */
        ExpectResponse(LL_RemoveNodeByData(SList, &TestData[0]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[3].Id, TestData[4].Id, TestData[2].Id);
        ExpectResponse(LL_RemoveNodeByData(SList, &TestData[4]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[3].Id, TestData[2].Id);
        /* List is now: 104, 103 */

        /* Head and tail are not changed */
        ListNode_t* HeadSList4 = LL_GetHead(SList);
        ListNode_t* TailSList4 = LL_GetTail(SList);
        ExpectEqualPtr(HeadSList3, HeadSList4);
        ExpectEqualPtr(TailSList3, TailSList4);

        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 29: GetPrev in a singly-linked list always return NULL");
    {
        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(SList), &TestData[1]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(SList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Test: */
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetPrev(LL_GetNext(LL_GetHead(SList))));
        ExpectPtrNull(LL_GetPrev(LL_GetNext(LL_GetNext(LL_GetHead(SList)))));

        /* Cleanup */
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 30: Get node by data from the head/tail/middle of a list");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(DList), &TestData[1]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(DList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Test: */
        ExpectEqualPtr(LL_GetNodeByData(DList, &TestData[0]), LL_GetHead(DList));
        ExpectEqualPtr(LL_GetNodeByData(DList, &TestData[1]), LL_GetNext(LL_GetHead(DList)));
        ExpectEqualPtr(LL_GetNodeByData(DList, &TestData[2]), LL_GetTail(DList));

        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(SList), &TestData[1]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(SList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Test: */
        ExpectEqualPtr(LL_GetNodeByData(SList, &TestData[0]), LL_GetHead(SList));
        ExpectEqualPtr(LL_GetNodeByData(SList, &TestData[1]), LL_GetNext(LL_GetHead(SList)));
        ExpectEqualPtr(LL_GetNodeByData(SList, &TestData[2]), LL_GetTail(SList));

        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 31: Remove Node from head/tail of a list");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert 1 node */
        ExpectResponse(LL_AddToFront(DList, &TestData[3]), LL_OK);
        ExpectListWith1Node(DList, TestData[3].Id);

        /* Test: Remove head node from a list with 1 node */
        ExpectResponse(LL_RemoveNode(LL_GetHead(DList)), LL_OK);
        ExpectEmptyList(DList);

        /* Precondition: insert 1 node */
        ExpectResponse(LL_AddToFront(DList, &TestData[4]), LL_OK);
        ExpectListWith1Node(DList, TestData[4].Id);

        /* Test: Remove tail node from a list with 1 node */
        ExpectResponse(LL_RemoveNode(LL_GetTail(DList)), LL_OK);
        ExpectEmptyList(DList);

        /* Precondition: insert 2 nodes */
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(DList), &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);
        /* List is now: 101, 102 */

        /* Test: Remove head node from a list with 2 nodes */
        ExpectResponse(LL_RemoveNode(LL_GetHead(DList)), LL_OK);
        ExpectListWith1Node(DList, TestData[1].Id);
        /* List is now: 102 */

        /* Precondition: insert 1 more node */
        ExpectResponse(LL_AddToFront(DList, &TestData[4]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[4].Id, TestData[1].Id);
        /* List is now: 105, 102 */

        /* Test: Remove tail node from a list with 2 nodes */
        ExpectResponse(LL_RemoveNode(LL_GetTail(DList)), LL_OK);
        ExpectListWith1Node(DList, TestData[4].Id);
        /* List is now: 105 */

        /* Test: Remove tail node from a list with 1 node */
        ExpectResponse(LL_RemoveNode(LL_GetTail(DList)), LL_OK);
        ExpectEmptyList(DList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(DList), &TestData[1]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(DList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Test: Remove head node */
        ExpectResponse(LL_RemoveNode(LL_GetHead(DList)), LL_OK);
        ExpectListWith2Nodes(DList, TestData[1].Id, TestData[2].Id);
        /* List is now: 102, 103 */

        /* Precondition: insert 1 more node */
        ExpectResponse(LL_AddToFront(DList, &TestData[4]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[4].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 105, 102, 103 */

        /* Test: Remove tail node from a list with 3 nodes */
        ExpectResponse(LL_RemoveNode(LL_GetTail(DList)), LL_OK);
        ExpectListWith2Nodes(DList, TestData[4].Id, TestData[1].Id);
        /* List is now: 105, 102 */

        /* Create a valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert 1 node */
        ExpectResponse(LL_AddToFront(SList, &TestData[3]), LL_OK);
        ExpectListWith1Node(SList, TestData[3].Id);

        /* Test: Remove head node from a list with 1 node */
        ExpectResponse(LL_RemoveNode(LL_GetHead(SList)), LL_OK);
        ExpectEmptyList(SList);

        /* Precondition: insert 1 node */
        ExpectResponse(LL_AddToFront(SList, &TestData[4]), LL_OK);
        ExpectListWith1Node(SList, TestData[4].Id);

        /* Test: Remove tail node from a list with 1 node */
        ExpectResponse(LL_RemoveNode(LL_GetTail(SList)), LL_OK);
        ExpectEmptyList(SList);

        /* Precondition: insert 2 nodes */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(SList), &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);
        /* List is now: 101, 102 */

        /* Test: Remove head node from a list with 2 nodes */
        ExpectResponse(LL_RemoveNode(LL_GetHead(SList)), LL_OK);
        ExpectListWith1Node(SList, TestData[1].Id);
        /* List is now: 102 */

        /* Precondition: insert 1 more node */
        ExpectResponse(LL_AddToFront(SList, &TestData[4]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[4].Id, TestData[1].Id);
        /* List is now: 105, 102 */

        /* Test: Remove tail node from a list with 2 nodes */
        ExpectResponse(LL_RemoveNode(LL_GetTail(SList)), LL_OK);
        ExpectListWith1Node(SList, TestData[4].Id);
        /* List is now: 105 */

        /* Test: Remove tail node from a list with 1 node */
        ExpectResponse(LL_RemoveNode(LL_GetTail(SList)), LL_OK);
        ExpectEmptyList(SList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(SList), &TestData[1]), LL_OK);
        ExpectResponse(LL_InsertAfterNode(LL_GetTail(SList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Test: Remove head node from a list with 3 nodes */
        ExpectResponse(LL_RemoveNode(LL_GetHead(SList)), LL_OK);
        ExpectListWith2Nodes(SList, TestData[1].Id, TestData[2].Id);
        /* List is now: 102, 103 */

        /* Precondition: insert 1 more node */
        ExpectResponse(LL_AddToFront(SList, &TestData[4]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[4].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 105, 102, 103 */

        /* Test: Remove tail node from a list with 3 nodes */
        ExpectResponse(LL_RemoveNode(LL_GetTail(SList)), LL_OK);
        ExpectListWith2Nodes(SList, TestData[4].Id, TestData[1].Id);
        /* List is now: 105, 102 */

        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 32: Set node data for a valid node returns LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert 1 node */
        ExpectResponse(LL_AddToFront(DList, &TestData[3]), LL_OK);
        ExpectListWith1Node(DList, TestData[3].Id);
        ExpectEqualPtr(LL_GetData(LL_GetHead(DList)), &TestData[3]);

        /* Change the head node data in a list with one node */
        ExpectResponse(LL_SetData(LL_GetHead(DList), &TestData[4]), LL_OK);
        ExpectListWith1Node(DList, TestData[4].Id);
        ExpectEqualPtr(LL_GetData(LL_GetHead(DList)), &TestData[4]);

        /* Precondition: insert 1 more node */
        ExpectResponse(LL_AddToFront(DList, &TestData[0]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[4].Id);
        ExpectEqualPtr(LL_GetData(LL_GetHead(DList)), &TestData[0]);
        ExpectEqualPtr(LL_GetData(LL_GetTail(DList)), &TestData[4]);

        /* Change the head and tail data in a list with two nodes */
        ExpectResponse(LL_SetData(LL_GetHead(DList), &TestData[1]), LL_OK);
        ExpectResponse(LL_SetData(LL_GetTail(DList), &TestData[2]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[1].Id, TestData[2].Id);
        ExpectEqualPtr(LL_GetData(LL_GetHead(DList)), &TestData[1]);
        ExpectEqualPtr(LL_GetData(LL_GetTail(DList)), &TestData[2]);

        /* Cleanup */
        ExpectResponse(LL_DeleteList(DList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 33: LL_NewList Tests");
    {
        /* Test 1: Create valid doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectPtrNotNull(DList);
        if (DList) {
            ExpectEmptyList(DList);
            ExpectEqual((unsigned int)DList->Linkage, (unsigned int)LL_DOUBLE);
            ExpectResponse(LL_DeleteList(DList), LL_OK);
        }

        /* Test 2: Create valid singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectPtrNotNull(SList);
        if (SList) {
            ExpectEmptyList(SList);
            ExpectEqual(SList->Linkage, LL_SINGLE);
            ExpectResponse(LL_DeleteList(SList), LL_OK);
        }

        /* Test 3: Create list with invalid linkage */
        List_t* InvList = LL_NewList(5); /* argument out of range */
        ExpectPtrNull(InvList);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 34: LL_GetHead Tests");
    {
        /* Test 1: Get head from NULL list */
        List_t* NullList = NULL;
        ExpectPtrNull(LL_GetHead(NullList));

        /* Test 2: Get head from empty list */
        List_t* EmptyList = LL_NewList(LL_DOUBLE);
        ExpectPtrNull(LL_GetHead(EmptyList));

        /* Test 3: Get head from list with one node */
        ExpectResponse(LL_AddToFront(EmptyList, &TestData[0]), LL_OK);
        ListNode_t* Head = LL_GetHead(EmptyList);
        ExpectPtrNotNull(Head);
        ExpectEqualPtr(LL_GetData(Head), &TestData[0]);

        /* Test 4: Get head from list with multiple nodes */
        ExpectResponse(LL_AddToFront(EmptyList, &TestData[1]), LL_OK);
        Head = LL_GetHead(EmptyList);
        ExpectPtrNotNull(Head);
        ExpectEqualPtr(LL_GetData(Head), &TestData[1]);

        ExpectResponse(LL_DeleteList(EmptyList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 35: LL_GetTail Tests");
    {
        /* Test 1: Get tail from NULL list */
        List_t* NullList = NULL;
        ExpectPtrNull(LL_GetTail(NullList));

        /* Test 2: Get tail from empty list */
        List_t* EmptyList = LL_NewList(LL_SINGLE);
        ExpectPtrNull(LL_GetTail(EmptyList));

        /* Test 3: Get tail from list with one node */
        ExpectResponse(LL_AddToBack(EmptyList, &TestData[0]), LL_OK);
        ListNode_t* Tail = LL_GetTail(EmptyList);
        ExpectPtrNotNull(Tail);
        ExpectEqualPtr(LL_GetData(Tail), &TestData[0]);

        /* Test 4: Get tail from list with multiple nodes */
        ExpectResponse(LL_AddToBack(EmptyList, &TestData[1]), LL_OK);
        Tail = LL_GetTail(EmptyList);
        ExpectPtrNotNull(Tail);
        ExpectEqualPtr(LL_GetData(Tail), &TestData[1]);

        ExpectResponse(LL_DeleteList(EmptyList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 36: LL_GetNext Tests");
    {
        /* Test 1: Get next from NULL node */
        ListNode_t* NullNode = NULL;
        ExpectPtrNull(LL_GetNext(NullNode));

        /* Test 2: Get next from single node list */
        List_t* List = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_AddToFront(List, &TestData[0]), LL_OK);
        ListNode_t* Head = LL_GetHead(List);
        ExpectPtrNull(LL_GetNext(Head));

        /* Test 3: Get next from multi-node list */
        ExpectResponse(LL_AddToBack(List, &TestData[1]), LL_OK);
        ExpectResponse(LL_AddToBack(List, &TestData[2]), LL_OK);
        Head = LL_GetHead(List);
        ListNode_t* Second = LL_GetNext(Head);
        ExpectPtrNotNull(Second);
        ExpectEqualPtr(LL_GetData(Second), &TestData[1]);
        
        ListNode_t* Third = LL_GetNext(Second);
        ExpectPtrNotNull(Third);
        ExpectEqualPtr(LL_GetData(Third), &TestData[2]);
        
        ExpectPtrNull(LL_GetNext(Third)); /* tail node */

        ExpectResponse(LL_DeleteList(List), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 37: LL_GetPrev Tests");
    {
        /* Test 1: Get prev from NULL node */
        ListNode_t* NullNode = NULL;
        ExpectPtrNull(LL_GetPrev(NullNode));

        /* Test 2: Get prev from singly-linked list (should always return NULL) */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_AddToFront(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToBack(SList, &TestData[1]), LL_OK);
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetPrev(LL_GetTail(SList)));

        /* Test 3: Get prev from doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_AddToFront(DList, &TestData[0]), LL_OK);
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList))); /* head has no prev */

        ExpectResponse(LL_AddToBack(DList, &TestData[1]), LL_OK);
        ExpectResponse(LL_AddToBack(DList, &TestData[2]), LL_OK);
        
        ListNode_t* Tail = LL_GetTail(DList);
        ListNode_t* Middle = LL_GetPrev(Tail);
        ExpectPtrNotNull(Middle);
        ExpectEqualPtr(LL_GetData(Middle), &TestData[1]);
        
        ListNode_t* Head = LL_GetPrev(Middle);
        ExpectPtrNotNull(Head);
        ExpectEqualPtr(LL_GetData(Head), &TestData[0]);

        ExpectResponse(LL_DeleteList(SList), LL_OK);
        ExpectResponse(LL_DeleteList(DList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 38: LL_GetData Tests");
    {
        /* Test 1: Get data from NULL node */
        ListNode_t* NullNode = NULL;
        ExpectPtrNull(LL_GetData(NullNode));

        /* Test 2: Get data from valid nodes */
        List_t* List = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_AddToFront(List, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToBack(List, &TestData[1]), LL_OK);
        
        ListNode_t* Head = LL_GetHead(List);
        void* HeadData = LL_GetData(Head);
        ExpectPtrNotNull(HeadData);
        ExpectEqualPtr(HeadData, &TestData[0]);
        
        ListNode_t* Tail = LL_GetTail(List);
        void* TailData = LL_GetData(Tail);
        ExpectPtrNotNull(TailData);
        ExpectEqualPtr(TailData, &TestData[1]);

        ExpectResponse(LL_DeleteList(List), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 39: LL_GetNodeByData Tests");
    {
        /* Test 1: Get node from NULL list */
        List_t* NullList = NULL;
        ExpectPtrNull(LL_GetNodeByData(NullList, &TestData[0]));

        /* Test 2: Get node with NULL data */
        List_t* List = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_AddToFront(List, &TestData[0]), LL_OK);
        ExpectPtrNull(LL_GetNodeByData(List, NULL));

        /* Test 3: Get node from empty list */
        List_t* EmptyList = LL_NewList(LL_DOUBLE);
        ExpectPtrNull(LL_GetNodeByData(EmptyList, &TestData[0]));

        /* Test 4: Get existing node */
        ExpectResponse(LL_AddToFront(List, &TestData[1]), LL_OK);
        ExpectResponse(LL_AddToBack(List, &TestData[2]), LL_OK);
        
        ListNode_t* FoundNode = LL_GetNodeByData(List, &TestData[1]);
        ExpectPtrNotNull(FoundNode);
        ExpectEqualPtr(LL_GetData(FoundNode), &TestData[1]);

        /* Test 5: Get non-existing node */
        ExpectPtrNull(LL_GetNodeByData(List, &TestData[3]));

        ExpectResponse(LL_DeleteList(List), LL_OK);
        ExpectResponse(LL_DeleteList(EmptyList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 40: LL_SetData Tests");
    {
        /* Test 1: Set data on NULL node */
        ListNode_t* NullNode = NULL;
        ExpectResponse(LL_SetData(NullNode, &TestData[0]), LL_NOT_OK);

        /* Test 2: Set NULL data */
        List_t* List = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_AddToFront(List, &TestData[0]), LL_OK);
        ListNode_t* Head = LL_GetHead(List);
        ExpectResponse(LL_SetData(Head, NULL), LL_NOT_OK);

        /* Test 3: Set valid data */
        ExpectResponse(LL_SetData(Head, &TestData[1]), LL_OK);
        ExpectEqualPtr(LL_GetData(Head), &TestData[1]);

        /* Test 4: Change data multiple times */
        ExpectResponse(LL_SetData(Head, &TestData[2]), LL_OK);
        ExpectEqualPtr(LL_GetData(Head), &TestData[2]);

        ExpectResponse(LL_DeleteList(List), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 41: LL_AddToFront Tests");
    {
        /* Test 1: Add to NULL list */
        List_t* NullList = NULL;
        ExpectResponse(LL_AddToFront(NullList, &TestData[0]), LL_NOT_OK);

        /* Test 2: Add NULL data */
        List_t* List = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_AddToFront(List, NULL), LL_NOT_OK);

        /* Test 3: Add to empty list */
        ExpectResponse(LL_AddToFront(List, &TestData[0]), LL_OK);
        ExpectListWith1Node(List, TestData[0].Id);

        /* Test 4: Add to non-empty list */
        ExpectResponse(LL_AddToFront(List, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(List, TestData[1].Id, TestData[0].Id);

        /* Test 5: Add multiple nodes */
        ExpectResponse(LL_AddToFront(List, &TestData[2]), LL_OK);
        ExpectListWith3Nodes(List, TestData[2].Id, TestData[1].Id, TestData[0].Id);

        ExpectResponse(LL_DeleteList(List), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 42: LL_AddToBack Tests");
    {
        /* Test 1: Add to NULL list */
        List_t* NullList = NULL;
        ExpectResponse(LL_AddToBack(NullList, &TestData[0]), LL_NOT_OK);

        /* Test 2: Add NULL data */
        List_t* List = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_AddToBack(List, NULL), LL_NOT_OK);

        /* Test 3: Add to empty list */
        ExpectResponse(LL_AddToBack(List, &TestData[0]), LL_OK);
        ExpectListWith1Node(List, TestData[0].Id);

        /* Test 4: Add to non-empty list */
        ExpectResponse(LL_AddToBack(List, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(List, TestData[0].Id, TestData[1].Id);

        /* Test 5: Add multiple nodes */
        ExpectResponse(LL_AddToBack(List, &TestData[2]), LL_OK);
        ExpectListWith3Nodes(List, TestData[0].Id, TestData[1].Id, TestData[2].Id);

        ExpectResponse(LL_DeleteList(List), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 43: LL_InsertAfterNode Tests");
    {
        /* Test 1: Insert after NULL node */
        ListNode_t* NullNode = NULL;
        ExpectResponse(LL_InsertAfterNode(NullNode, &TestData[0]), LL_NOT_OK);

        /* Test 2: Insert NULL data */
        List_t* List = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_AddToFront(List, &TestData[0]), LL_OK);
        ListNode_t* Head = LL_GetHead(List);
        ExpectResponse(LL_InsertAfterNode(Head, NULL), LL_NOT_OK);

        /* Test 3: Insert after head node */
        ExpectResponse(LL_InsertAfterNode(Head, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(List, TestData[0].Id, TestData[1].Id);

        /* Test 4: Insert after tail node */
        ListNode_t* Tail = LL_GetTail(List);
        ExpectResponse(LL_InsertAfterNode(Tail, &TestData[2]), LL_OK);
        ExpectListWith3Nodes(List, TestData[0].Id, TestData[1].Id, TestData[2].Id);

        /* Test 5: Insert after middle node */
        ListNode_t* Middle = LL_GetNext(Head);
        ExpectResponse(LL_InsertAfterNode(Middle, &TestData[3]), LL_OK);
        unsigned int Count;
        ExpectResponse(LL_GetCount(List, &Count), LL_OK);
        ExpectEqual(Count, 4);

        ExpectResponse(LL_DeleteList(List), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 44: LL_InsertAfterData Tests");
    {
        /* Test 1: Insert in NULL list */
        List_t* NullList = NULL;
        ExpectResponse(LL_InsertAfterData(NullList, &TestData[0], &TestData[1]), LL_NOT_OK);

        /* Test 2: Insert with NULL existing data */
        List_t* List = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_AddToFront(List, &TestData[0]), LL_OK);
        ExpectResponse(LL_InsertAfterData(List, NULL, &TestData[1]), LL_NOT_OK);

        /* Test 3: Insert with NULL new data */
        ExpectResponse(LL_InsertAfterData(List, &TestData[0], NULL), LL_NOT_OK);

        /* Test 4: Insert in empty list */
        List_t* EmptyList = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_InsertAfterData(EmptyList, &TestData[0], &TestData[1]), LL_NOT_OK);

        /* Test 5: Insert after non-existing data */
        ExpectResponse(LL_InsertAfterData(List, &TestData[3], &TestData[1]), LL_NOT_OK);

        /* Test 6: Insert after existing data */
        ExpectResponse(LL_InsertAfterData(List, &TestData[0], &TestData[1]), LL_OK);
        ExpectListWith2Nodes(List, TestData[0].Id, TestData[1].Id);

        /* Test 7: Insert after head data in multi-node list */
        ExpectResponse(LL_InsertAfterData(List, &TestData[0], &TestData[2]), LL_OK);
        ExpectListWith3Nodes(List, TestData[0].Id, TestData[2].Id, TestData[1].Id);

        ExpectResponse(LL_DeleteList(List), LL_OK);
        ExpectResponse(LL_DeleteList(EmptyList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 45: LL_RemoveHead Tests");
    {
        /* Test 1: Remove from NULL list */
        List_t* NullList = NULL;
        ExpectResponse(LL_RemoveHead(NullList), LL_NOT_OK);

        /* Test 2: Remove from empty list */
        List_t* EmptyList = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_RemoveHead(EmptyList), LL_NOT_OK);

        /* Test 3: Remove from single-node list */
        List_t* List = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_AddToFront(List, &TestData[0]), LL_OK);
        ExpectResponse(LL_RemoveHead(List), LL_OK);
        ExpectEmptyList(List);

        /* Test 4: Remove from multi-node list */
        ExpectResponse(LL_AddToBack(List, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToBack(List, &TestData[1]), LL_OK);
        ExpectResponse(LL_AddToBack(List, &TestData[2]), LL_OK);
        ExpectResponse(LL_RemoveHead(List), LL_OK);
        ExpectListWith2Nodes(List, TestData[1].Id, TestData[2].Id);

        ExpectResponse(LL_DeleteList(List), LL_OK);
        ExpectResponse(LL_DeleteList(EmptyList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 46: LL_RemoveTail Tests");
    {
        /* Test 1: Remove from NULL list */
        List_t* NullList = NULL;
        ExpectResponse(LL_RemoveTail(NullList), LL_NOT_OK);

        /* Test 2: Remove from empty s-list */
        List_t* EmptySList = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_RemoveTail(EmptySList), LL_NOT_OK);

        /* Test 3: Remove from empty d-list */
        List_t* EmptyDList = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_RemoveTail(EmptyDList), LL_NOT_OK);

        /* Test 4: Remove from single-node d-list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(LL_RemoveTail(DList), LL_OK);
        ExpectEmptyList(DList);

        /* Test 5: Remove from multi-node d-list */
        ExpectResponse(LL_AddToFront(DList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToFront(DList, &TestData[1]), LL_OK);
        ExpectResponse(LL_AddToFront(DList, &TestData[2]), LL_OK);
        ExpectResponse(LL_RemoveTail(DList), LL_OK);
        ExpectListWith2Nodes(DList, TestData[2].Id, TestData[1].Id);

        /* Test 6: Remove from single-node s-list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_RemoveTail(SList), LL_OK);
        ExpectEmptyList(SList);

        /* Test 7: Remove from multi-node s-list */
        ExpectResponse(LL_AddToFront(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToFront(SList, &TestData[1]), LL_OK);
        ExpectResponse(LL_AddToFront(SList, &TestData[2]), LL_OK);
        ExpectResponse(LL_RemoveTail(SList), LL_OK);
        ExpectListWith2Nodes(SList, TestData[2].Id, TestData[1].Id);

        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
        ExpectResponse(LL_DeleteList(EmptySList), LL_OK);
        ExpectResponse(LL_DeleteList(EmptyDList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 47: LL_RemoveNode Tests");
    {
        /* Test 1: Remove NULL node */
        ListNode_t* NullNode = NULL;
        ExpectResponse(LL_RemoveNode(NullNode), LL_NOT_OK);

        /* Test 2: Remove head node from single-node list */
        List_t* List = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_AddToFront(List, &TestData[0]), LL_OK);
        ListNode_t* Head = LL_GetHead(List);
        ExpectResponse(LL_RemoveNode(Head), LL_OK);
        ExpectEmptyList(List);

        /* Test 3: Remove head node from multi-node list */
        ExpectResponse(LL_AddToBack(List, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToBack(List, &TestData[1]), LL_OK);
        ExpectResponse(LL_AddToBack(List, &TestData[2]), LL_OK);
        Head = LL_GetHead(List);
        ExpectResponse(LL_RemoveNode(Head), LL_OK);
        ExpectListWith2Nodes(List, TestData[1].Id, TestData[2].Id);

        /* Test 4: Remove tail node */
        ListNode_t* Tail = LL_GetTail(List);
        ExpectResponse(LL_RemoveNode(Tail), LL_OK);
        ExpectListWith1Node(List, TestData[1].Id);

        /* Test 5: Remove middle node */
        ExpectResponse(LL_AddToBack(List, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToBack(List, &TestData[2]), LL_OK);
        ListNode_t* Middle = LL_GetNext(LL_GetHead(List));
        ExpectResponse(LL_RemoveNode(Middle), LL_OK);
        ExpectListWith2Nodes(List, TestData[1].Id, TestData[2].Id);

        ExpectResponse(LL_DeleteList(List), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 48: LL_RemoveNodeByData Tests");
    {
        /* Test 1: Remove from NULL list */
        List_t* NullList = NULL;
        ExpectResponse(LL_RemoveNodeByData(NullList, &TestData[0]), LL_NOT_OK);

        /* Test 2: Remove NULL data */
        List_t* List = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_AddToFront(List, &TestData[0]), LL_OK);
        ExpectResponse(LL_RemoveNodeByData(List, NULL), LL_NOT_OK);

        /* Test 3: Remove from empty list */
        List_t* EmptyList = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_RemoveNodeByData(EmptyList, &TestData[0]), LL_NOT_OK);

        /* Test 4: Remove non-existing data */
        ExpectResponse(LL_RemoveNodeByData(List, &TestData[3]), LL_NOT_OK);

        /* Test 5: Remove existing data from single-node list */
        ExpectResponse(LL_RemoveNodeByData(List, &TestData[0]), LL_OK);
        ExpectEmptyList(List);

        /* Test 6: Remove head data from multi-node list */
        ExpectResponse(LL_AddToFront(List, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToFront(List, &TestData[1]), LL_OK);
        ExpectResponse(LL_AddToFront(List, &TestData[2]), LL_OK);
        ExpectResponse(LL_RemoveNodeByData(List, &TestData[2]), LL_OK);
        ExpectListWith2Nodes(List, TestData[1].Id, TestData[0].Id);

        /* Test 7: Remove tail data */
        ExpectResponse(LL_RemoveNodeByData(List, &TestData[0]), LL_OK);
        ExpectListWith1Node(List, TestData[1].Id);

        ExpectResponse(LL_DeleteList(List), LL_OK);
        ExpectResponse(LL_DeleteList(EmptyList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 49: LL_GetCount Tests");
    {
        /* Test 1: Get count from NULL list */
        List_t* NullList = NULL;
        unsigned int Count;
        ExpectResponse(LL_GetCount(NullList, &Count), LL_NOT_OK);

        /* Test 2: Get count with NULL counter */
        List_t* List = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_GetCount(List, NULL), LL_NOT_OK);

        /* Test 3: Get count from empty list */
        ExpectResponse(LL_GetCount(List, &Count), LL_OK);
        ExpectEqual(Count, 0);

        /* Test 4: Get count from single-node list */
        ExpectResponse(LL_AddToFront(List, &TestData[0]), LL_OK);
        ExpectResponse(LL_GetCount(List, &Count), LL_OK);
        ExpectEqual(Count, 1);

        /* Test 5: Get count from multi-node list */
        ExpectResponse(LL_AddToBack(List, &TestData[1]), LL_OK);
        ExpectResponse(LL_AddToBack(List, &TestData[2]), LL_OK);
        ExpectResponse(LL_GetCount(List, &Count), LL_OK);
        ExpectEqual(Count, 3);

        /* Test 6: Count after removal */
        ExpectResponse(LL_RemoveHead(List), LL_OK);
        ExpectResponse(LL_GetCount(List, &Count), LL_OK);
        ExpectEqual(Count, 2);

        ExpectResponse(LL_DeleteList(List), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 48: LL_DeleteList Tests");
    {
        /* Test 1: Delete NULL list */
        List_t* NullList = NULL;
        ExpectResponse(LL_DeleteList(NullList), LL_NOT_OK);

        /* Test 2: Delete empty list */
        List_t* EmptyList = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_DeleteList(EmptyList), LL_OK);

        /* Test 3: Delete single-node list */
        List_t* SingleList = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_AddToFront(SingleList, &TestData[0]), LL_OK);
        ExpectResponse(LL_DeleteList(SingleList), LL_OK);

        /* Test 4: Delete multi-node list */
        List_t* MultiList = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_AddToFront(MultiList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToFront(MultiList, &TestData[1]), LL_OK);
        ExpectResponse(LL_AddToFront(MultiList, &TestData[2]), LL_OK);
        ExpectResponse(LL_DeleteList(MultiList), LL_OK);
    }
    TestEnd();


    /* ---------------------------------------------------------------------------------------------------------------- */
    /* Test report: */
    printf("---> Number of failed tests: %u\n\n", NumFailedTests);

    /* Warn if there is a mismatch between the number of TestStart and TestEnd functions: */
    if (TestStartEndBalance != 0)
    {
        printf("!!! Warning! the number of TestStart() functions should match the number of TestEnd() functions!\n\n");
    }

    /* ---------------------------------------------------------------------------------------------------------------- */
    /* Memory management test: */
    #ifdef MEM_TEST_ENAB_H
        printf("---> Memory management test status: %s\n\n", MtStatusToString(MtGetStatus()));
    #endif

    return 0;
}

static void ExpectResponse(ListStatus_t Actual, ListStatus_t Expected)
{
    if(Actual != Expected)
    {
        NumFailedSubpoints++;
    }
}

static void ExpectPtrNull(void* Ptr)
{
    if(Ptr != NULL)
    {
        NumFailedSubpoints++;
    }
}

static void ExpectPtrNotNull(void* Ptr)
{
    if(Ptr == NULL)
    {
        NumFailedSubpoints++;
    }
}

static void ExpectEqual(unsigned int A, unsigned int B)
{
    if (A != B)
    {
        NumFailedSubpoints++;
    }
}

static void ExpectEqualPtr(void* PtrA, void* PtrB)
{
    if (PtrA != PtrB)
    {
        NumFailedSubpoints++;
    }
}

static void ExpectNotEqualPtr(void* PtrA, void* PtrB)
{
    if (PtrA == PtrB)
    {
        NumFailedSubpoints++;
    }
}

static void ExpectEmptyList(List_t* List)
{
    ExpectPtrNotNull(List);
    ExpectPtrNull(LL_GetHead(List));
    ExpectPtrNull(LL_GetTail(List));

    unsigned int NodeCount;
    ExpectResponse(LL_GetCount(List, &NodeCount), LL_OK);
    ExpectEqual(NodeCount, 0);
}

static void ExpectListWith1Node(List_t* List, int Id1)
{
    unsigned int NodeCount;
    ExpectPtrNotNull(List);
    ListNode_t* Head = LL_GetHead(List);
    ListNode_t* Tail = LL_GetTail(List);

    ExpectPtrNotNull(Head);
    ExpectPtrNotNull(Tail);
    ExpectEqualPtr(Head, Tail);
    ExpectPtrNull(LL_GetNext(Head));
    ExpectPtrNull(LL_GetNext(Tail));

    if(List->Linkage == LL_DOUBLE)
    {
        ExpectPtrNull(LL_GetPrev(Head));
        ExpectPtrNull(LL_GetPrev(Tail));
    }

    ExpectResponse(LL_GetCount(List, &NodeCount), LL_OK);
    ExpectEqual(NodeCount, 1);
    ExpectEqual(((TestData_t*)LL_GetData(Head))->Id, Id1);
    ExpectEqual(((TestData_t*)LL_GetData(Tail))->Id, Id1);

}

static void ExpectListWith2Nodes(List_t* List, int Id1, int Id2)
{
    unsigned int NodeCount;
    ExpectPtrNotNull(List);
    ListNode_t* Head = LL_GetHead(List);
    ListNode_t* Tail = LL_GetTail(List);
    ExpectPtrNotNull(Head);
    ExpectPtrNotNull(Tail);
    ExpectNotEqualPtr(Head, Tail);
    ExpectEqualPtr(Head->Next, Tail);
    ExpectPtrNull(LL_GetNext(Tail));
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectPtrNull(LL_GetPrev(Head));
        ExpectEqualPtr(Tail->Prev, Head);
    }
    ExpectResponse(LL_GetCount(List, &NodeCount), LL_OK);
    ExpectEqual(NodeCount, 2);
    ExpectEqual(((TestData_t*)LL_GetData(Head))->Id, Id1);
    ExpectEqual(((TestData_t*)LL_GetData(Tail))->Id, Id2);
    ExpectEqual(((TestData_t*)LL_GetData(LL_GetNext(Head)))->Id, Id2);
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectEqual(((TestData_t*)LL_GetData(LL_GetPrev(Tail)))->Id, Id1);
    }
}

static void ExpectListWith3Nodes(List_t* List, int Id1, int Id2, int Id3)
{
    unsigned int NodeCount;
    ExpectPtrNotNull(List);
    ListNode_t* Head = LL_GetHead(List);
    ListNode_t* Tail = LL_GetTail(List);
    ExpectPtrNotNull(Head);
    ExpectPtrNotNull(Tail);
    ExpectNotEqualPtr(Head, Tail);
    ExpectPtrNull(LL_GetNext(Tail));
    ExpectEqualPtr(Head->Next->Next, Tail);
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectPtrNull(LL_GetPrev(Head));
        ExpectEqualPtr(Tail->Prev->Prev, Head);
    }
    ExpectResponse(LL_GetCount(List, &NodeCount), LL_OK);
    ExpectEqual(NodeCount, 3);
    ExpectEqual(((TestData_t*)LL_GetData(Head))->Id, Id1);
    ExpectEqual(((TestData_t*)LL_GetData(Tail))->Id, Id3);
    ExpectEqual(((TestData_t*)LL_GetData(LL_GetNext(Head)))->Id, Id2);
    ExpectEqual(((TestData_t*)LL_GetData(LL_GetNext(LL_GetNext(Head))))->Id, Id3);
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectEqual(((TestData_t*)LL_GetData(LL_GetPrev(Tail)))->Id, Id2);
        ExpectEqual(((TestData_t*)LL_GetData(LL_GetPrev(LL_GetPrev(Tail))))->Id, Id1);
    }
}

static void ExpectListWith4Nodes(List_t* List, int Id1, int Id2, int Id3, int Id4)
{
    unsigned int NodeCount;
    ExpectPtrNotNull(List);
    ListNode_t* Head = LL_GetHead(List);
    ListNode_t* Tail = LL_GetTail(List);
    ExpectPtrNotNull(Head);
    ExpectPtrNotNull(Tail);
    ExpectNotEqualPtr(Head, Tail);
    ExpectPtrNull(LL_GetNext(Tail));
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectPtrNull(LL_GetPrev(Head));
    }
    ExpectResponse(LL_GetCount(List, &NodeCount), LL_OK);
    ExpectEqual(NodeCount, 4);
    ExpectEqual(((TestData_t*)LL_GetData(Head))->Id, Id1);
    ExpectEqual(((TestData_t*)LL_GetData(Tail))->Id, Id4);
    ExpectEqual(((TestData_t*)LL_GetData(LL_GetNext(Head)))->Id, Id2);
    ExpectEqual(((TestData_t*)LL_GetData(LL_GetNext(LL_GetNext(Head))))->Id, Id3);
    ExpectEqual(((TestData_t*)LL_GetData(LL_GetNext(LL_GetNext(LL_GetNext(Head)))))->Id, Id4);
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectEqual(((TestData_t*)LL_GetData(LL_GetPrev(Tail)))->Id, Id3);
        ExpectEqual(((TestData_t*)LL_GetData(LL_GetPrev(LL_GetPrev(Tail))))->Id, Id2);
        ExpectEqual(((TestData_t*)LL_GetData(LL_GetPrev(LL_GetPrev(LL_GetPrev(Tail)))))->Id, Id1);
    }
}

static void ExpectListWith5Nodes(List_t* List, int Id1, int Id2, int Id3, int Id4, int Id5)
{
    unsigned int NodeCount;
    ExpectPtrNotNull(List);
    ListNode_t* Head = LL_GetHead(List);
    ListNode_t* Tail = LL_GetTail(List);
    ExpectPtrNotNull(Head);
    ExpectPtrNotNull(Tail);
    ExpectNotEqualPtr(Head, Tail);
    ExpectPtrNull(LL_GetNext(Tail));
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectPtrNull(LL_GetPrev(Head));
    }
    ExpectResponse(LL_GetCount(List, &NodeCount), LL_OK);
    ExpectEqual(NodeCount, 5);
    ExpectEqual(((TestData_t*)LL_GetData(Head))->Id, Id1);
    ExpectEqual(((TestData_t*)LL_GetData(Tail))->Id, Id5);
    ExpectEqual(((TestData_t*)LL_GetData(LL_GetNext(Head)))->Id, Id2);
    ExpectEqual(((TestData_t*)LL_GetData(LL_GetNext(LL_GetNext(Head))))->Id, Id3);
    ExpectEqual(((TestData_t*)LL_GetData(LL_GetNext(LL_GetNext(LL_GetNext(Head)))))->Id, Id4);
    ExpectEqual(((TestData_t*)LL_GetData(LL_GetNext(LL_GetNext(LL_GetNext(LL_GetNext(Head))))))->Id, Id5);
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectEqual(((TestData_t*)LL_GetData(LL_GetPrev(Tail)))->Id, Id4);
        ExpectEqual(((TestData_t*)LL_GetData(LL_GetPrev(LL_GetPrev(Tail))))->Id, Id3);
        ExpectEqual(((TestData_t*)LL_GetData(LL_GetPrev(LL_GetPrev(LL_GetPrev(Tail)))))->Id, Id2);
        ExpectEqual(((TestData_t*)LL_GetData(LL_GetPrev(LL_GetPrev(LL_GetPrev(LL_GetPrev(Tail))))))->Id, Id1);
    }
}

static void TestStart(const char* const Header)
{
    printf("%s\n", Header);
    NumFailedSubpoints = 0;
    TestStartEndBalance++;
}

static void TestEnd(void)
{
    if(NumFailedSubpoints > 0)
    {
        NumFailedTests++;
        printf("Failed!\n\n");
        NumFailedSubpoints = 0;
    }
    else
    {
        printf("Passed!\n\n");
    }

    TestStartEndBalance--;
}
