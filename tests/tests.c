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
static void ExpectEqual(unsigned int PtrA, unsigned int PtrB);
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
        /* Create valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);
        ExpectEqual(DList->Linkage, LL_DOUBLE);

        /* Create valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);
        ExpectEqual(SList->Linkage, LL_SINGLE);

        /* Create an invalid list -> should return NULL */
        List_t* InvList = NewList(5); /* argument out of range  */
        ExpectPtrNull(InvList);

        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
        ExpectResponse(DeleteList(SList), LL_OK);
        ExpectResponse(DeleteList(InvList), LL_NOT_OK); /* can't delete a NULL list */
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 2: NULL list argument => return NULL node");
    {
        List_t* NullList = NULL;
        ExpectPtrNull(GetHead(NullList));
        ExpectPtrNull(GetTail(NullList));
        ExpectPtrNull(GetNodeByData(NullList, &DummyData));
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 3: NULL list argument => return LL_NOT_OK");
    {
        List_t* NullList = NULL;
        unsigned int Count;
        ExpectResponse(AddToFront(NullList, &DummyData), LL_NOT_OK);
        ExpectResponse(AddToBack(NullList, &DummyData), LL_NOT_OK);
        ExpectResponse(InsertAfterData(NullList, &DummyData, &DummyData), LL_NOT_OK);
        ExpectResponse(RemoveHead(NullList), LL_NOT_OK);
        ExpectResponse(RemoveTail(NullList), LL_NOT_OK);
        ExpectResponse(RemoveNodeByData(NullList, &DummyData), LL_NOT_OK);
        ExpectResponse(GetCount(NullList, &Count), LL_NOT_OK);
        ExpectResponse(DeleteList(NullList), LL_NOT_OK);
    }
    TestEnd();
    
    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 4: NULL node argument => return NULL Ptr");
    {
        ListNode_t* NullNode = NULL;
        ExpectPtrNull(GetNext(NullNode));
        ExpectPtrNull(GetPrev(NullNode));
        ExpectPtrNull(GetData(NullNode));
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 5: NULL node argument => return LL_NOT_OK");
    {
        ListNode_t* NullNode = NULL;
        ExpectResponse(InsertAfterNode(NullNode, &DummyData), LL_NOT_OK);
        ExpectResponse(RemoveNode(NullNode), LL_NOT_OK);
        ExpectResponse(SetData(NullNode, &DummyData), LL_NOT_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 6: NULL data argument => return NULL node");
    {
        void* NullData = NULL;

        /* Create a valid empty list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: add at least one node, otherwise GetNodeByData will  */
        /* return NULL because of the empty list rather than the NULL data. */
        ExpectResponse(AddToFront(SList, &DummyData), LL_OK);
        ExpectListWith1Node(SList, DummyData.Id);
        
        /* Test */
        ExpectPtrNull(GetNodeByData(SList, NullData));

        /* Cleanup */
        ExpectResponse(DeleteList(SList), LL_OK);
    }
    TestEnd();
    
    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 7: NULL data argument => return LL_NOT_OK");
    {
        void* NullData = NULL;

        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Try inserting NULL data into an empty list */
        ExpectResponse(AddToFront(SList, NullData), LL_NOT_OK);
        ExpectResponse(AddToBack(SList, NullData), LL_NOT_OK);

        /* Insert one node */
        ExpectResponse(AddToFront(SList, &DummyData), LL_OK);
        ExpectListWith1Node(SList, DummyData.Id);

        /* Try inserting NULL data after a valid node (list must have at least one node) */
        ExpectResponse(InsertAfterNode(GetHead(SList), NullData), LL_NOT_OK);

        /* Try inserting NULL data after valid data (list must have at least one node) */
        ExpectResponse(InsertAfterData(SList, GetData(GetHead(SList)), NullData), LL_NOT_OK);

        /* Try inserting valid data after NULL data (list must have at least one node) */
        ExpectResponse(InsertAfterData(SList, NullData, GetData(GetHead(SList))), LL_NOT_OK);

        /* Try removing NULL data from a list that has at least one node. */
        ExpectResponse(RemoveNodeByData(SList, NullData), LL_NOT_OK);

        /* Try setting the data of an existing node to NULL */
        ExpectResponse(SetData(GetHead(SList), NullData), LL_NOT_OK);

        /* Cleanup */
        ExpectResponse(DeleteList(SList), LL_OK);
    }    
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 8: NULL Counter argument => return LL_NOT_OK");
    {
        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Test */
        unsigned int* CountPtr = NULL;
        ExpectResponse(GetCount(SList, CountPtr), LL_NOT_OK);

        /* Cleanup */
        ExpectResponse(DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 9: Empty list argument => return NULL node");
    {
        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Test */
        ExpectPtrNull(GetHead(SList));
        ExpectPtrNull(GetTail(SList));
        ExpectPtrNull(GetNodeByData(SList, &DummyData));

        /* Cleanup */
        ExpectResponse(DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 10: Remove from empty list => return LL_NOT_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Test */
        ExpectResponse(RemoveHead(DList), LL_NOT_OK);
        ExpectResponse(RemoveTail(DList), LL_NOT_OK);
        ExpectResponse(RemoveNodeByData(DList, &DummyData), LL_NOT_OK);
        
        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 11: Insert data after existing data in an empty list => return LL_NOT_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Test */
        ExpectResponse(InsertAfterData(DList, &DummyData, &DummyData), LL_NOT_OK);

        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
    }    
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 12: Insert data to the front/back of an empty list => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList); 

        /* Insert to front */
        ExpectResponse(AddToFront(DList, &DummyData), LL_OK);
        ExpectListWith1Node(DList, 0);

        /* Remove node */
        ExpectResponse(RemoveHead(DList), LL_OK);
        ExpectEmptyList(DList);

        /* Insert to back */
        ExpectResponse(AddToBack(DList, &DummyData), LL_OK);
        ExpectListWith1Node(DList, 0);

        /* Remove node */
        ExpectResponse(RemoveTail(DList), LL_OK);
        ExpectEmptyList(DList);

        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList); 

         /* Insert to front */
        ExpectResponse(AddToFront(SList, &DummyData), LL_OK);
        ExpectListWith1Node(SList, 0);

        /* Remove node */
        ExpectResponse(RemoveHead(SList), LL_OK);
        ExpectEmptyList(SList);

        /* Insert to back */
        ExpectResponse(AddToBack(SList, &DummyData), LL_OK);
        ExpectListWith1Node(SList, 0);

        /* Remove node */
        ExpectResponse(RemoveTail(SList), LL_OK);
        ExpectEmptyList(SList);
        
        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
        ExpectResponse(DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 13: Insert a node to the front of a non-empty list => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(AddToFront(DList, &TestData[0]), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);

        /* Test: Insert a node */
        ExpectResponse(AddToFront(DList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[1].Id, TestData[0].Id); 

        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(AddToFront(SList, &TestData[0]), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);

        /* Test: Insert a node */
        ExpectResponse(AddToFront(SList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[1].Id, TestData[0].Id); 

        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
        ExpectResponse(DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 14: Remove nodes from the back of a non-empty list => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert two nodes */
        ExpectResponse(AddToFront(DList, &TestData[0]), LL_OK);
        ExpectResponse(AddToFront(DList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[1].Id, TestData[0].Id); 

        /* Test */
        ExpectResponse(RemoveTail(DList), LL_OK);
        ExpectListWith1Node(DList, TestData[1].Id);
        ExpectResponse(RemoveTail(DList), LL_OK);
        ExpectEmptyList(DList);

        /* Attempt to remove from empty list (already tested - see test 10) */
        ExpectResponse(RemoveTail(DList), LL_NOT_OK); 

        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert two nodes */
        ExpectResponse(AddToFront(SList, &TestData[0]), LL_OK);
        ExpectResponse(AddToFront(SList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[1].Id, TestData[0].Id);

        /* Test */
        ExpectResponse(RemoveTail(SList), LL_OK);
        ExpectListWith1Node(SList, TestData[1].Id);
        ExpectResponse(RemoveTail(SList), LL_OK);
        ExpectEmptyList(SList);

        /* Attempt to remove from empty list (already tested - see test 10) */
        ExpectResponse(RemoveTail(SList), LL_NOT_OK); 

        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
        ExpectResponse(DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 15: Insert a node to the back of a non-empty list => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(AddToBack(DList, &TestData[0]), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);

        /* Test: Insert another node */
        ExpectResponse(AddToBack(DList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);

        /* Test: Insert a third node */
        ExpectResponse(AddToBack(DList, &TestData[0]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[0].Id);

        /* Test: Insert a fourth node */
        ExpectResponse(AddToBack(DList, &TestData[4]), LL_OK);
        ExpectListWith4Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[0].Id, TestData[4].Id);

        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(AddToBack(SList, &TestData[0]), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);

        /* Test: Insert another node */
        ExpectResponse(AddToBack(SList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);

        /* Test: Insert a third node */
        ExpectResponse(AddToBack(SList, &TestData[0]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[0].Id);

        /* Test: Insert a fourth node */
        ExpectResponse(AddToBack(SList, &TestData[4]), LL_OK);
        ExpectListWith4Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[0].Id, TestData[4].Id);

        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
        ExpectResponse(DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 16: Remove nodes from the front of a non-empty list => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert two nodes */
        ExpectResponse(AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(AddToBack(DList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);

        /* Test */
        ExpectResponse(RemoveHead(DList), LL_OK);
        ExpectListWith1Node(DList, TestData[1].Id);
        ExpectResponse(RemoveHead(DList), LL_OK);
        ExpectEmptyList(DList);

        /* Attempt to remove from empty list (already tested - see test 10) */
        ExpectResponse(RemoveHead(DList), LL_NOT_OK);

        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert two nodes */
        ExpectResponse(AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(AddToBack(SList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);

        /* Test */
        ExpectResponse(RemoveHead(SList), LL_OK);
        ExpectListWith1Node(SList, TestData[1].Id);
        ExpectResponse(RemoveHead(SList), LL_OK);
        ExpectEmptyList(SList);

        /* Attempt to remove from empty list (already tested - see test 10) */
        ExpectResponse(RemoveHead(SList), LL_NOT_OK);

        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
        ExpectResponse(DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 17: Insert a node after the head node => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(AddToBack(DList, &TestData[0]), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);

        /* Test: Insert a node after head */
        ExpectResponse(InsertAfterNode(GetHead(DList), &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);

        /* Test: Insert another node after head */
        ExpectResponse(InsertAfterNode(GetHead(DList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[2].Id, TestData[1].Id);

        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(AddToBack(SList, &TestData[0]), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);

        /* Test: Insert a node after head */
        ExpectResponse(InsertAfterNode(GetHead(SList), &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);

        /* Test: Insert another node after head */
        ExpectResponse(InsertAfterNode(GetHead(SList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[2].Id, TestData[1].Id);

        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
        ExpectResponse(DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 18: Insert a node after the tail node => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(AddToBack(DList, &TestData[0]), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);

        /* Test: Insert a node after tail */
        ExpectResponse(InsertAfterNode(GetTail(DList), &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);

        /* Test: Insert another node after tail */
        ExpectResponse(InsertAfterNode(GetTail(DList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[2].Id);

        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(AddToBack(SList, &TestData[0]), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);

        /* Test: Insert a node after tail */
        ExpectResponse(InsertAfterNode(GetTail(SList), &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);

        /* Test: Insert another node after tail */
        ExpectResponse(InsertAfterNode(GetTail(SList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id);

        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
        ExpectResponse(DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 19: Insert a node after any middle node => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(DList), &TestData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(DList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Test: Add 104 after 102 */
        ExpectResponse(InsertAfterNode(GetNext(GetHead(DList)), &TestData[3]), LL_OK);
        ExpectListWith4Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[3].Id, TestData[2].Id);  
        /* List is now: 101, 102, 104, 103 */

        /* Test: Add 105 after 104 */
        ExpectResponse(InsertAfterNode(GetPrev(GetTail(DList)), &TestData[4]), LL_OK);
        ExpectListWith5Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[3].Id, TestData[4].Id, TestData[2].Id);
        /* List is now: 101, 102, 104, 105, 103 */

        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(SList), &TestData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(SList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Test: Add 104 after 102 */
        ExpectResponse(InsertAfterNode(GetNext(GetHead(SList)), &TestData[3]), LL_OK);
        ExpectListWith4Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[3].Id, TestData[2].Id);
        /* List is now: 101, 102, 104, 103 */

        /* Test: Add 105 after 104 */
        ExpectResponse(InsertAfterNode(GetNext(GetNext(GetHead(SList))), &TestData[4]), LL_OK);
        ExpectListWith5Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[3].Id, TestData[4].Id, TestData[2].Id);
        /* List is now: 101, 102, 104, 105, 103 */

        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
        ExpectResponse(DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 20: Insert data after inexistent data, when list is non-empty => return LL_NOT_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(AddToBack(DList, &TestData[0]), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);

        /* Save head & tail */
        ListNode_t* HeadDList1 = GetHead(DList);
        ListNode_t* TailDList1 = GetTail(DList);

        /* Test: Try inserting a node after inexistent data: */
        ExpectResponse(InsertAfterData(DList, &DummyData, &TestData[0]), LL_NOT_OK);
        ExpectListWith1Node(DList, TestData[0].Id);

        /* Head and tail didn't change */
        ExpectEqualPtr(GetHead(DList), HeadDList1);
        ExpectEqualPtr(GetTail(DList), TailDList1);

        /* Insert another node: */
        ExpectResponse(AddToBack(DList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);

        /* Save head & tail */
        ListNode_t* HeadDList2 = GetHead(DList);
        ListNode_t* TailDList2 = GetTail(DList);

        /* Test: Try inserting another node after inexistent data: */
        ExpectResponse(InsertAfterData(DList, &TestData[2], &TestData[3]), LL_NOT_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);

        /* Head and tail didn't change */
        ExpectEqualPtr(GetHead(DList), HeadDList2);
        ExpectEqualPtr(GetTail(DList), TailDList2);

        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(AddToBack(SList, &TestData[0]), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);

        /* Save head & tail */
        ListNode_t* HeadSList1 = GetHead(SList);
        ListNode_t* TailSList1 = GetTail(SList);

        /* Test: Try inserting a node after inexistent data: */
        ExpectResponse(InsertAfterData(SList, &DummyData, &TestData[0]), LL_NOT_OK);
        ExpectListWith1Node(SList, TestData[0].Id);

        /* Head and tail didn't change */
        ExpectEqualPtr(GetHead(SList), HeadSList1);
        ExpectEqualPtr(GetTail(SList), TailSList1);

        /* Insert another node: */
        ExpectResponse(AddToBack(SList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);

        /* Save head & tail */
        ListNode_t* HeadSList2 = GetHead(SList);
        ListNode_t* TailSList2 = GetTail(SList);

        /* Test: Try inserting another node after inexistent data: */
        ExpectResponse(InsertAfterData(SList, &TestData[2], &TestData[3]), LL_NOT_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);

        /* Head and tail didn't change */
        ExpectEqualPtr(GetHead(SList), HeadSList2);
        ExpectEqualPtr(GetTail(SList), TailSList2);

        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
        ExpectResponse(DeleteList(SList), LL_OK);
    }    
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 21: Insert data after existing head data => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(AddToFront(DList, &TestData[0]), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);

        /* Save head & tail */
        ListNode_t* HeadDList1 = GetHead(DList);
        ListNode_t* TailDList1 = GetTail(DList);

        /* Test: Try inserting some data after the head data: */
        ExpectResponse(InsertAfterData(DList, &TestData[0], &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);

        /* Check that the tail node has changed (head node is the same) */
        ListNode_t* HeadDList2 = GetHead(DList);
        ListNode_t* TailDList2 = GetTail(DList);
        ExpectEqualPtr(HeadDList1, HeadDList2);
        ExpectNotEqualPtr(TailDList1, TailDList2);

        /* Test: Try inserting some data after the head data: */
        ExpectResponse(InsertAfterData(DList, &TestData[0], &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[2].Id, TestData[1].Id);

        /* Check that both head and tail nodes are unchanged */
        ListNode_t* HeadDList3 = GetHead(DList);
        ListNode_t* TailDList3 = GetTail(DList);
        ExpectEqualPtr(HeadDList2, HeadDList3);
        ExpectEqualPtr(TailDList2, TailDList3);

        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(AddToBack(SList, &TestData[0]), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);

        /* Save head & tail */
        ListNode_t* HeadSList1 = GetHead(SList);
        ListNode_t* TailSList1 = GetTail(SList);

        /* Test: Try inserting some data after the head data: */
        ExpectResponse(InsertAfterData(SList, &TestData[0], &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);

        /* Check that the tail node has changed (head node is the same) */
        ListNode_t* HeadSList2 = GetHead(SList);
        ListNode_t* TailSList2 = GetTail(SList);
        ExpectEqualPtr(HeadSList1, HeadSList2);
        ExpectNotEqualPtr(TailSList1, TailSList2);

        /* Test: Try inserting some data after the head data: */
        ExpectResponse(InsertAfterData(SList, &TestData[0], &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id,  TestData[2].Id, TestData[1].Id);

        /* Check that both head and tail nodes are unchanged */
        ListNode_t* HeadSList3 = GetHead(SList);
        ListNode_t* TailSList3 = GetTail(SList);
        ExpectEqualPtr(HeadSList2, HeadSList3);
        ExpectEqualPtr(TailSList2, TailSList3);

        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
        ExpectResponse(DeleteList(SList), LL_OK);
    }
    TestEnd();    

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 22: Insert data after existing data when existing data is in the tail => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(AddToFront(DList, &TestData[0]), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);

        /* Save head & tail */
        ListNode_t* HeadDList1 = GetHead(DList);
        ListNode_t* TailDList1 = GetTail(DList);

        /* Test: Try inserting some data after the tail data: */
        ExpectResponse(InsertAfterData(DList, &TestData[0], &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);

        /* Check that the tail node has changed (head is the same) */
        ListNode_t* HeadDList2 = GetHead(DList);
        ListNode_t* TailDList2 = GetTail(DList);
        ExpectEqualPtr(HeadDList1, HeadDList2);
        ExpectNotEqualPtr(TailDList1, TailDList2);

        /* Test: Try inserting some more data after the tail data: */
        ExpectResponse(InsertAfterData(DList, &TestData[1], &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[2].Id);

        /* Check that the tail node has changed (head is the same) */
        ListNode_t* HeadDList3 = GetHead(DList);
        ListNode_t* TailDList3 = GetTail(DList);
        ExpectEqualPtr(HeadDList2, HeadDList3);
        ExpectNotEqualPtr(TailDList2, TailDList3);

        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(AddToBack(SList, &TestData[0]), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);

        /* Save head & tail */
        ListNode_t* HeadSList1 = GetHead(SList);
        ListNode_t* TailSList1 = GetTail(SList);

        /* Test: Try inserting some data after the tail data: */
        ExpectResponse(InsertAfterData(SList, &TestData[0], &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);

        /* Check that the tail node has changed (head is the same) */
        ListNode_t* HeadSList2 = GetHead(SList);
        ListNode_t* TailSList2 = GetTail(SList);
        ExpectEqualPtr(HeadSList1, HeadSList2);
        ExpectNotEqualPtr(TailSList1, TailSList2);

        /* Test: Try inserting some data after the tail data: */
        ExpectResponse(InsertAfterData(SList, &TestData[1], &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id);

        /* Check that the tail node has changed (head is the same) */
        ListNode_t* HeadSList3 = GetHead(SList);
        ListNode_t* TailSList3 = GetTail(SList);
        ExpectEqualPtr(HeadSList2, HeadSList3);
        ExpectNotEqualPtr(TailSList2, TailSList3);

        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
        ExpectResponse(DeleteList(SList), LL_OK);
    }
    TestEnd();    

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 23: Insert data after existing data when existing data is in the middle => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(DList), &TestData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(DList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Save head and tail */
        ListNode_t* HeadDList1 = GetHead(DList);
        ListNode_t* TailDList1 = GetTail(DList);

        /* Test: Try inserting some data after the data in the middle node: */
        ExpectResponse(InsertAfterData(DList, &TestData[1], &TestData[3]), LL_OK);
        ExpectListWith4Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[3].Id, TestData[2].Id);
        /* List is now: 101, 102, 104, 103 */

        /* Check that the head & tail nodes didn't change */
        ListNode_t* HeadDList2 = GetHead(DList);
        ListNode_t* TailDList2 = GetTail(DList);
        ExpectEqualPtr(HeadDList1, HeadDList2);
        ExpectEqualPtr(TailDList1, TailDList2);

        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(SList), &TestData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(SList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Save head and tail */
        ListNode_t* HeadSList1 = GetHead(SList);
        ListNode_t* TailSList1 = GetTail(SList);

        /* Test: Try inserting some data after the data in the middle node: */
        ExpectResponse(InsertAfterData(SList, &TestData[1], &TestData[3]), LL_OK);
        ExpectListWith4Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[3].Id, TestData[2].Id);
        /* List is now: 101, 102, 104, 103 */

        /* Check that the head & tail nodes didn't change */
        ListNode_t* HeadSList2 = GetHead(SList);
        ListNode_t* TailSList2 = GetTail(SList);
        ExpectEqualPtr(HeadSList1, HeadSList2);
        ExpectEqualPtr(TailSList1, TailSList2);
        
        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
        ExpectResponse(DeleteList(SList), LL_OK);
    }    
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 24: Remove node from the front/back of a non-empty list => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(AddToFront(DList, &DummyData), LL_OK);
        ExpectListWith1Node(DList, DummyData.Id);

        /* Test: List with one node -> remove from front */
        ExpectResponse(RemoveHead(DList), LL_OK);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(AddToBack(DList, &DummyData), LL_OK);
        ExpectListWith1Node(DList, DummyData.Id);

        /* Test: List with one node -> remove from back */
        ExpectResponse(RemoveTail(DList), LL_OK);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(AddToFront(DList, &DummyData), LL_OK);
        ExpectListWith1Node(DList, DummyData.Id);

        /* Test: List with one node -> remove from back */
        ExpectResponse(RemoveTail(DList), LL_OK);
        ExpectEmptyList(DList);
        
        /* Precondition: insert one node */
        ExpectResponse(AddToBack(DList, &DummyData), LL_OK);
        ExpectListWith1Node(DList, DummyData.Id);

        /* Test: List with one node -> remove from front */
        ExpectResponse(RemoveHead(DList), LL_OK);
        ExpectEmptyList(DList);

        /* Precondition: Add 2 nodes */
        ExpectResponse(AddToFront(DList, &TestData[4]), LL_OK);
        ExpectResponse(AddToFront(DList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[3].Id, TestData[4].Id);

        /* Test: List with 2 nodes -> remove from front */
        ExpectResponse(RemoveHead(DList), LL_OK);
        ExpectListWith1Node(DList, TestData[4].Id);

        /* Precondition: Add one more node */
        ExpectResponse(AddToBack(DList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[4].Id, TestData[3].Id);

        /* Test: List with 2 nodes -> remove from back */
        ExpectResponse(RemoveTail(DList), LL_OK);
        ExpectListWith1Node(DList, TestData[4].Id);

        /* Precondition: Add one more node */
        ExpectResponse(AddToFront(DList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[3].Id, TestData[4].Id);

        /* Test: List with 2 nodes -> remove from back */
        ExpectResponse(RemoveTail(DList), LL_OK);
        ExpectListWith1Node(DList, TestData[3].Id);

        /* Precondition: Add one more node */
        ExpectResponse(AddToBack(DList, &TestData[4]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[3].Id, TestData[4].Id);

        /* Test: List with 2 nodes -> remove from front */
        ExpectResponse(RemoveHead(DList), LL_OK);
        ExpectListWith1Node(DList, TestData[4].Id);

        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(AddToFront(SList, &DummyData), LL_OK);
        ExpectListWith1Node(SList, DummyData.Id);

        /* Test: List with one node -> remove from front */
        ExpectResponse(RemoveHead(SList), LL_OK);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(AddToBack(SList, &DummyData), LL_OK);
        ExpectListWith1Node(SList, DummyData.Id);

        /* Test: List with one node -> remove from back */
        ExpectResponse(RemoveTail(SList), LL_OK);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(AddToFront(SList, &DummyData), LL_OK);
        ExpectListWith1Node(SList, DummyData.Id);

        /* Test: List with one node -> remove from back */
        ExpectResponse(RemoveTail(SList), LL_OK);
        ExpectEmptyList(SList);
        
        /* Precondition: insert one node */
        ExpectResponse(AddToBack(SList, &DummyData), LL_OK);
        ExpectListWith1Node(SList, DummyData.Id);

        /* Test: List with one node -> remove from front */
        ExpectResponse(RemoveHead(SList), LL_OK);
        ExpectEmptyList(SList);

        /* Precondition: Add 2 nodes */
        ExpectResponse(AddToFront(SList, &TestData[4]), LL_OK);
        ExpectResponse(AddToFront(SList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[3].Id, TestData[4].Id);

        /* Test: List with 2 nodes -> remove from front */
        ExpectResponse(RemoveHead(SList), LL_OK);
        ExpectListWith1Node(SList, TestData[4].Id);

        /* Precondition: Add one more node */
        ExpectResponse(AddToBack(SList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[4].Id, TestData[3].Id);

        /* Test: List with 2 nodes -> remove from back */
        ExpectResponse(RemoveTail(SList), LL_OK);
        ExpectListWith1Node(SList, TestData[4].Id);

        /* Precondition: Add one more node */
        ExpectResponse(AddToFront(SList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[3].Id, TestData[4].Id);

        /* Test: List with 2 nodes -> remove from back */
        ExpectResponse(RemoveTail(SList), LL_OK);
        ExpectListWith1Node(SList, TestData[3].Id);

        /* Precondition: Add one more node */
        ExpectResponse(AddToBack(SList, &TestData[4]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[3].Id, TestData[4].Id);

        /* Test: List with 2 nodes -> remove from front */
        ExpectResponse(RemoveHead(SList), LL_OK);
        ExpectListWith1Node(SList, TestData[4].Id);

        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
        ExpectResponse(DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 25: Remove node from the middle of a non-empty list => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(DList), &TestData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(DList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Save head and tail */
        ListNode_t* HeadDList1 = GetHead(DList);
        ListNode_t* TailDList1 = GetTail(DList);

        /* Test: Try to remove the node from the middle */
        ExpectResponse(RemoveNode(GetNext(GetHead(DList))), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[2].Id);
        /* List is now: 101, 103 */

        /* Head and tail are unchanged */
        ListNode_t* HeadDList2 = GetHead(DList);
        ListNode_t* TailDList2 = GetTail(DList);
        ExpectEqualPtr(HeadDList1, HeadDList2);
        ExpectEqualPtr(TailDList1, TailDList2);

        /* Precondition: Add 2 more nodes */
        ExpectResponse(AddToFront(DList, &TestData[4]), LL_OK);
        ExpectResponse(AddToFront(DList, &TestData[3]), LL_OK);
        ExpectListWith4Nodes(DList, TestData[3].Id, TestData[4].Id, TestData[0].Id, TestData[2].Id);
        /* List is now: 104, 105, 101, 103 */

        /* Save head & tail */
        ListNode_t* HeadDList3 = GetHead(DList);
        ListNode_t* TailDList3 = GetTail(DList);

        /* Try to remove the nodes from the middle */
        ExpectResponse(RemoveNode(GetNext(GetNext(GetHead(DList)))), LL_OK);
        ExpectListWith3Nodes(DList, TestData[3].Id, TestData[4].Id, TestData[2].Id);
        ExpectResponse(RemoveNode(GetNext(GetHead(DList))), LL_OK);
        ExpectListWith2Nodes(DList, TestData[3].Id, TestData[2].Id);
        /* List is now: 104, 103 */

        /* Head and tail are not changed */
        ListNode_t* HeadDList4 = GetHead(DList);
        ListNode_t* TailDList4 = GetTail(DList);
        ExpectEqualPtr(HeadDList3, HeadDList4);
        ExpectEqualPtr(TailDList3, TailDList4);

        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(SList), &TestData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(SList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Save head and tail */
        ListNode_t* HeadSList1 = GetHead(SList);
        ListNode_t* TailSList1 = GetTail(SList);

        /* Test: Try to remove the node from the middle */
        ExpectResponse(RemoveNode(GetNext(GetHead(SList))), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[2].Id);
        /* List is now: 101, 103 */

        /* Head and tail are unchanged */
        ListNode_t* HeadSList2 = GetHead(SList);
        ListNode_t* TailSList2 = GetTail(SList);
        ExpectEqualPtr(HeadSList1, HeadSList2);
        ExpectEqualPtr(TailSList1, TailSList2);

        /* Precondition: Add 2 more nodes */
        ExpectResponse(AddToFront(SList, &TestData[4]), LL_OK);
        ExpectResponse(AddToFront(SList, &TestData[3]), LL_OK);
        ExpectListWith4Nodes(SList, TestData[3].Id, TestData[4].Id, TestData[0].Id, TestData[2].Id);
        /* List is now: 104, 105, 101, 103 */

        /* Save head & tail */
        ListNode_t* HeadSList3 = GetHead(SList);
        ListNode_t* TailSList3 = GetTail(SList);

        /* Try to remove the nodes from the middle */
        ExpectResponse(RemoveNode(GetNext(GetNext(GetHead(SList)))), LL_OK);
        ExpectListWith3Nodes(SList, TestData[3].Id, TestData[4].Id, TestData[2].Id);
        ExpectResponse(RemoveNode(GetNext(GetHead(SList))), LL_OK);
        ExpectListWith2Nodes(SList, TestData[3].Id, TestData[2].Id);
        /* List is now: 104, 103 */

        /* Head and tail are not changed */
        ListNode_t* HeadSList4 = GetHead(SList);
        ListNode_t* TailSList4 = GetTail(SList);
        ExpectEqualPtr(HeadSList3, HeadSList4);
        ExpectEqualPtr(TailSList3, TailSList4);

        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
        ExpectResponse(DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 26: Remove inexistent data from a non-empty list => return LL_NOT_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(AddToFront(DList, &DummyData), LL_OK);
        ExpectListWith1Node(DList, DummyData.Id);

        /* Test: Remove inexistent data from a list with one node */
        ExpectResponse(RemoveNodeByData(DList, &TestData[3]), LL_NOT_OK);
        ExpectListWith1Node(DList, DummyData.Id);

        /* Precondition: insert one more node */
        ExpectResponse(AddToFront(DList, &TestData[4]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[4].Id, DummyData.Id);

        /* Test: Remove inexistent data from a list with two nodes */
        ExpectResponse(RemoveNodeByData(DList, &TestData[3]), LL_NOT_OK);
        ExpectListWith2Nodes(DList, TestData[4].Id, DummyData.Id);

        /* Precondition: insert one more node */
        ExpectResponse(AddToFront(DList, &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[2].Id, TestData[4].Id, DummyData.Id);

        /* Test: Remove inexistent data from a list with three nodes */
        ExpectResponse(RemoveNodeByData(DList, &TestData[0]), LL_NOT_OK);
        ExpectResponse(RemoveNodeByData(DList, &TestData[3]), LL_NOT_OK);

        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(AddToFront(SList, &DummyData), LL_OK);
        ExpectListWith1Node(SList, DummyData.Id);

        /* Test: Remove inexistent data from a list with one node */
        ExpectResponse(RemoveNodeByData(SList, &TestData[3]), LL_NOT_OK);
        ExpectListWith1Node(SList, DummyData.Id);

        /* Precondition: insert one more node */
        ExpectResponse(AddToFront(SList, &TestData[4]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[4].Id, DummyData.Id);

        /* Test: Remove inexistent data from a list with two nodes */
        ExpectResponse(RemoveNodeByData(SList, &TestData[3]), LL_NOT_OK);
        ExpectListWith2Nodes(SList, TestData[4].Id, DummyData.Id);

        /* Precondition: insert one more node */
        ExpectResponse(AddToFront(SList, &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[2].Id, TestData[4].Id, DummyData.Id);

        /* Test: Remove inexistent data from a list with three nodes */
        ExpectResponse(RemoveNodeByData(SList, &TestData[0]), LL_NOT_OK);
        ExpectResponse(RemoveNodeByData(SList, &TestData[3]), LL_NOT_OK);

        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
        ExpectResponse(DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 27: Remove node by data from the front/back of a non-empty list => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(AddToFront(DList, &DummyData), LL_OK);
        ExpectListWith1Node(DList, DummyData.Id);

        /* Test: Remove node by data from a list with 1 node */
        ExpectResponse(RemoveNodeByData(DList, &DummyData), LL_OK);
        ExpectEmptyList(DList);

        /* Precondition: insert one node */
        ExpectResponse(AddToFront(DList, &TestData[3]), LL_OK);
        ExpectListWith1Node(DList, TestData[3].Id);

        /* Test: Remove node by data from a list with 1 node */
        ExpectResponse(RemoveNodeByData(DList, &TestData[3]), LL_OK);
        ExpectEmptyList(DList);

        /* Precondition: insert two nodes */
        ExpectResponse(AddToFront(DList, &TestData[4]), LL_OK);
        ExpectResponse(AddToFront(DList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[3].Id, TestData[4].Id);

        /* Test: Remove node by data from a list with 2 nodes */
        ExpectResponse(RemoveNodeByData(DList, &TestData[4]), LL_OK);
        ExpectListWith1Node(DList, TestData[3].Id);

        /* Precondition: insert two nodes */
        ExpectResponse(AddToFront(DList, &TestData[2]), LL_OK);
        ExpectResponse(AddToFront(DList, &TestData[1]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[1].Id, TestData[2].Id, TestData[3].Id);

        /* Test: Remove node by data from a list with 3 nodes */
        ExpectResponse(RemoveNodeByData(DList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[2].Id, TestData[3].Id);

        /* Precondition: insert one node */
        ExpectResponse(AddToFront(DList, &TestData[1]), LL_OK);
        
        /* Test: Remove node by data from a list with 3 nodes */
        ExpectResponse(RemoveNodeByData(DList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[1].Id, TestData[2].Id);

        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(AddToFront(SList, &DummyData), LL_OK);
        ExpectListWith1Node(SList, DummyData.Id);

        /* Test: Remove node by data from a list with 1 node */
        ExpectResponse(RemoveNodeByData(SList, &DummyData), LL_OK);
        ExpectEmptyList(SList);

        /* Precondition: insert one node */
        ExpectResponse(AddToFront(SList, &TestData[3]), LL_OK);
        ExpectListWith1Node(SList, TestData[3].Id);

        /* Test: Remove node by data from a list with 1 node */
        ExpectResponse(RemoveNodeByData(SList, &TestData[3]), LL_OK);
        ExpectEmptyList(SList);

        /* Precondition: insert two nodes */
        ExpectResponse(AddToFront(SList, &TestData[4]), LL_OK);
        ExpectResponse(AddToFront(SList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[3].Id, TestData[4].Id);

        /* Test: Remove node by data from a list with 2 nodes */
        ExpectResponse(RemoveNodeByData(SList, &TestData[4]), LL_OK);
        ExpectListWith1Node(SList, TestData[3].Id);

        /* Precondition: insert two nodes */
        ExpectResponse(AddToFront(SList, &TestData[2]), LL_OK);
        ExpectResponse(AddToFront(SList, &TestData[1]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[1].Id, TestData[2].Id, TestData[3].Id);

        /* Test: Remove node by data from a list with 3 nodes */
        ExpectResponse(RemoveNodeByData(SList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[2].Id, TestData[3].Id);

        /* Precondition: insert one node */
        ExpectResponse(AddToFront(SList, &TestData[1]), LL_OK);
        
        /* Test: Remove node by data from a list with 3 nodes */
        ExpectResponse(RemoveNodeByData(SList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[1].Id, TestData[2].Id);

        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
        ExpectResponse(DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 28: Remove node by data from the middle of a list => return LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(DList), &TestData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(DList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Save head and tail */
        ListNode_t* HeadDList1 = GetHead(DList);
        ListNode_t* TailDList1 = GetTail(DList);

        /* Test: Remove the node from the middle (by data) */
        ExpectResponse(RemoveNodeByData(DList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[2].Id);
        /* List is now: 101, 103 */

        /* Head and tail are unchanged */
        ListNode_t* HeadDList2 = GetHead(DList);
        ListNode_t* TailDList2 = GetTail(DList);
        ExpectEqualPtr(HeadDList1, HeadDList2);
        ExpectEqualPtr(TailDList1, TailDList2);

        /* Precondition: Add 2 more nodes */
        ExpectResponse(AddToFront(DList, &TestData[4]), LL_OK);
        ExpectResponse(AddToFront(DList, &TestData[3]), LL_OK);
        ExpectListWith4Nodes(DList, TestData[3].Id, TestData[4].Id, TestData[0].Id, TestData[2].Id);
        /* List is now: 104, 105, 101, 103 */

        /* Save head and tail */
        ListNode_t* HeadDList3 = GetHead(DList);
        ListNode_t* TailDList3 = GetTail(DList);

        /* Remove the nodes from the middle */
        ExpectResponse(RemoveNodeByData(DList, &TestData[0]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[3].Id, TestData[4].Id, TestData[2].Id);
        ExpectResponse(RemoveNodeByData(DList, &TestData[4]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[3].Id, TestData[2].Id);
        /* List is now: 104, 103 */

        /* Head and tail are not changed */
        ListNode_t* HeadDList4 = GetHead(DList);
        ListNode_t* TailDList4 = GetTail(DList);
        ExpectEqualPtr(HeadDList3, HeadDList4);
        ExpectEqualPtr(TailDList3, TailDList4);

        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(SList), &TestData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(SList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Save head and tail */
        ListNode_t* HeadSList1 = GetHead(SList);
        ListNode_t* TailSList1 = GetTail(SList);

        /* Test: Remove the node from the middle (by data) */
        ExpectResponse(RemoveNodeByData(SList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[2].Id);
        /* List is now: 101, 103 */

        /* Head and tail are unchanged */
        ListNode_t* HeadSList2 = GetHead(SList);
        ListNode_t* TailSList2 = GetTail(SList);
        ExpectEqualPtr(HeadSList1, HeadSList2);
        ExpectEqualPtr(TailSList1, TailSList2);

        /* Precondition: Add 2 more nodes */
        ExpectResponse(AddToFront(SList, &TestData[4]), LL_OK);
        ExpectResponse(AddToFront(SList, &TestData[3]), LL_OK);
        ExpectListWith4Nodes(SList, TestData[3].Id, TestData[4].Id, TestData[0].Id, TestData[2].Id);
        /* List is now: 104, 105, 101, 103 */

        /* Save head and tail */
        ListNode_t* HeadSList3 = GetHead(SList);
        ListNode_t* TailSList3 = GetTail(SList);

        /* Remove the nodes from the middle */
        ExpectResponse(RemoveNodeByData(SList, &TestData[0]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[3].Id, TestData[4].Id, TestData[2].Id);
        ExpectResponse(RemoveNodeByData(SList, &TestData[4]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[3].Id, TestData[2].Id);
        /* List is now: 104, 103 */

        /* Head and tail are not changed */
        ListNode_t* HeadSList4 = GetHead(SList);
        ListNode_t* TailSList4 = GetTail(SList);
        ExpectEqualPtr(HeadSList3, HeadSList4);
        ExpectEqualPtr(TailSList3, TailSList4);

        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
        ExpectResponse(DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 29: GetPrev in a singly-linked list always return NULL");
    {
        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(SList), &TestData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(SList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Test: */
        ExpectPtrNull(GetPrev(GetHead(SList)));
        ExpectPtrNull(GetPrev(GetNext(GetHead(SList))));
        ExpectPtrNull(GetPrev(GetNext(GetNext(GetHead(SList)))));

        /* Cleanup */
        ExpectResponse(DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 30: Get node by data from the head/tail/middle of a list");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(DList), &TestData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(DList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Test: */
        ExpectEqualPtr(GetNodeByData(DList, &TestData[0]), GetHead(DList));
        ExpectEqualPtr(GetNodeByData(DList, &TestData[1]), GetNext(GetHead(DList)));
        ExpectEqualPtr(GetNodeByData(DList, &TestData[2]), GetTail(DList));

        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(SList), &TestData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(SList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Test: */
        ExpectEqualPtr(GetNodeByData(SList, &TestData[0]), GetHead(SList));
        ExpectEqualPtr(GetNodeByData(SList, &TestData[1]), GetNext(GetHead(SList)));
        ExpectEqualPtr(GetNodeByData(SList, &TestData[2]), GetTail(SList));

        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
        ExpectResponse(DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 31: Remove Node from head/tail of a list");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert 1 node */
        ExpectResponse(AddToFront(DList, &TestData[3]), LL_OK);
        ExpectListWith1Node(DList, TestData[3].Id);

        /* Test: Remove head node from a list with 1 node */
        ExpectResponse(RemoveNode(GetHead(DList)), LL_OK);
        ExpectEmptyList(DList);

        /* Precondition: insert 1 node */
        ExpectResponse(AddToFront(DList, &TestData[4]), LL_OK);
        ExpectListWith1Node(DList, TestData[4].Id);

        /* Test: Remove tail node from a list with 1 node */
        ExpectResponse(RemoveNode(GetTail(DList)), LL_OK);
        ExpectEmptyList(DList);

        /* Precondition: insert 2 nodes */
        ExpectResponse(AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(DList), &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);
        /* List is now: 101, 102 */

        /* Test: Remove head node from a list with 2 nodes */
        ExpectResponse(RemoveNode(GetHead(DList)), LL_OK);
        ExpectListWith1Node(DList, TestData[1].Id);
        /* List is now: 102 */

        /* Precondition: insert 1 more node */
        ExpectResponse(AddToFront(DList, &TestData[4]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[4].Id, TestData[1].Id);
        /* List is now: 105, 102 */

        /* Test: Remove tail node from a list with 2 nodes */
        ExpectResponse(RemoveNode(GetTail(DList)), LL_OK);
        ExpectListWith1Node(DList, TestData[4].Id);
        /* List is now: 105 */

        /* Test: Remove tail node from a list with 1 node */
        ExpectResponse(RemoveNode(GetTail(DList)), LL_OK);
        ExpectEmptyList(DList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(DList), &TestData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(DList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Test: Remove head node */
        ExpectResponse(RemoveNode(GetHead(DList)), LL_OK);
        ExpectListWith2Nodes(DList, TestData[1].Id, TestData[2].Id);
        /* List is now: 102, 103 */

        /* Precondition: insert 1 more node */
        ExpectResponse(AddToFront(DList, &TestData[4]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[4].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 105, 102, 103 */

        /* Test: Remove tail node from a list with 3 nodes */
        ExpectResponse(RemoveNode(GetTail(DList)), LL_OK);
        ExpectListWith2Nodes(DList, TestData[4].Id, TestData[1].Id);
        /* List is now: 105, 102 */

        /* Create a valid empty singly-linked list */
        List_t* SList = NewList(LL_SINGLE);
        ExpectEmptyList(SList);

        /* Precondition: insert 1 node */
        ExpectResponse(AddToFront(SList, &TestData[3]), LL_OK);
        ExpectListWith1Node(SList, TestData[3].Id);

        /* Test: Remove head node from a list with 1 node */
        ExpectResponse(RemoveNode(GetHead(SList)), LL_OK);
        ExpectEmptyList(SList);

        /* Precondition: insert 1 node */
        ExpectResponse(AddToFront(SList, &TestData[4]), LL_OK);
        ExpectListWith1Node(SList, TestData[4].Id);

        /* Test: Remove tail node from a list with 1 node */
        ExpectResponse(RemoveNode(GetTail(SList)), LL_OK);
        ExpectEmptyList(SList);

        /* Precondition: insert 2 nodes */
        ExpectResponse(AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(SList), &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);
        /* List is now: 101, 102 */

        /* Test: Remove head node from a list with 2 nodes */
        ExpectResponse(RemoveNode(GetHead(SList)), LL_OK);
        ExpectListWith1Node(SList, TestData[1].Id);
        /* List is now: 102 */

        /* Precondition: insert 1 more node */
        ExpectResponse(AddToFront(SList, &TestData[4]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[4].Id, TestData[1].Id);
        /* List is now: 105, 102 */

        /* Test: Remove tail node from a list with 2 nodes */
        ExpectResponse(RemoveNode(GetTail(SList)), LL_OK);
        ExpectListWith1Node(SList, TestData[4].Id);
        /* List is now: 105 */

        /* Test: Remove tail node from a list with 1 node */
        ExpectResponse(RemoveNode(GetTail(SList)), LL_OK);
        ExpectEmptyList(SList);

        /* Precondition: insert 3 nodes */
        ExpectResponse(AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(SList), &TestData[1]), LL_OK);
        ExpectResponse(InsertAfterNode(GetTail(SList), &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 101, 102, 103 */

        /* Test: Remove head node from a list with 3 nodes */
        ExpectResponse(RemoveNode(GetHead(SList)), LL_OK);
        ExpectListWith2Nodes(SList, TestData[1].Id, TestData[2].Id);
        /* List is now: 102, 103 */

        /* Precondition: insert 1 more node */
        ExpectResponse(AddToFront(SList, &TestData[4]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[4].Id, TestData[1].Id, TestData[2].Id);
        /* List is now: 105, 102, 103 */

        /* Test: Remove tail node from a list with 3 nodes */
        ExpectResponse(RemoveNode(GetTail(SList)), LL_OK);
        ExpectListWith2Nodes(SList, TestData[4].Id, TestData[1].Id);
        /* List is now: 105, 102 */

        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
        ExpectResponse(DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 32: Set node data for a valid node returns LL_OK");
    {
        /* Create a valid empty doubly-linked list */
        List_t* DList = NewList(LL_DOUBLE);
        ExpectEmptyList(DList);

        /* Precondition: insert 1 node */
        ExpectResponse(AddToFront(DList, &TestData[3]), LL_OK);
        ExpectListWith1Node(DList, TestData[3].Id);
        ExpectEqualPtr(GetData(GetHead(DList)), &TestData[3]);

        /* Change the head node data in a list with one node */
        ExpectResponse(SetData(GetHead(DList), &TestData[4]), LL_OK);
        ExpectListWith1Node(DList, TestData[4].Id);
        ExpectEqualPtr(GetData(GetHead(DList)), &TestData[4]);

        /* Precondition: insert 1 more node */
        ExpectResponse(AddToFront(DList, &TestData[0]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[4].Id);
        ExpectEqualPtr(GetData(GetHead(DList)), &TestData[0]);
        ExpectEqualPtr(GetData(GetTail(DList)), &TestData[4]);

        /* Change the head and tail data in a list with two nodes */
        ExpectResponse(SetData(GetHead(DList), &TestData[1]), LL_OK);
        ExpectResponse(SetData(GetTail(DList), &TestData[2]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[1].Id, TestData[2].Id);
        ExpectEqualPtr(GetData(GetHead(DList)), &TestData[1]);
        ExpectEqualPtr(GetData(GetTail(DList)), &TestData[2]);

        /* Cleanup */
        ExpectResponse(DeleteList(DList), LL_OK);
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

static void ExpectEqual(unsigned int PtrA, unsigned int PtrB)
{
    if (PtrA != PtrB)
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
    ExpectPtrNull(GetHead(List));
    ExpectPtrNull(GetTail(List));

    unsigned int NodeCount;
    ExpectResponse(GetCount(List, &NodeCount), LL_OK);
    ExpectEqual(NodeCount, 0);
}

static void ExpectListWith1Node(List_t* List, int Id1)
{
    unsigned int NodeCount;
    ExpectPtrNotNull(List);
    ListNode_t* Head = GetHead(List);
    ListNode_t* Tail = GetTail(List);

    ExpectPtrNotNull(Head);
    ExpectPtrNotNull(Tail);
    ExpectEqualPtr(Head, Tail);
    ExpectPtrNull(GetNext(Head));
    ExpectPtrNull(GetNext(Tail));

    if(List->Linkage == LL_DOUBLE)
    {
        ExpectPtrNull(GetPrev(Head));
        ExpectPtrNull(GetPrev(Tail));
    }

    ExpectResponse(GetCount(List, &NodeCount), LL_OK);
    ExpectEqual(NodeCount, 1);
    ExpectEqual(((TestData_t*)GetData(Head))->Id, Id1);
    ExpectEqual(((TestData_t*)GetData(Tail))->Id, Id1);

}

static void ExpectListWith2Nodes(List_t* List, int Id1, int Id2)
{
    unsigned int NodeCount;
    ExpectPtrNotNull(List);
    ListNode_t* Head = GetHead(List);
    ListNode_t* Tail = GetTail(List);
    ExpectPtrNotNull(Head);
    ExpectPtrNotNull(Tail);
    ExpectNotEqualPtr(Head, Tail);
    ExpectPtrNull(GetNext(Tail));
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectPtrNull(GetPrev(Head));
    }
    ExpectResponse(GetCount(List, &NodeCount), LL_OK);
    ExpectEqual(NodeCount, 2);
    ExpectEqual(((TestData_t*)GetData(Head))->Id, Id1);
    ExpectEqual(((TestData_t*)GetData(Tail))->Id, Id2);
    ExpectEqual(((TestData_t*)GetData(GetNext(Head)))->Id, Id2);
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectEqual(((TestData_t*)GetData(GetPrev(Tail)))->Id, Id1);
    }
}

static void ExpectListWith3Nodes(List_t* List, int Id1, int Id2, int Id3)
{
    unsigned int NodeCount;
    ExpectPtrNotNull(List);
    ListNode_t* Head = GetHead(List);
    ListNode_t* Tail = GetTail(List);
    ExpectPtrNotNull(Head);
    ExpectPtrNotNull(Tail);
    ExpectNotEqualPtr(Head, Tail);
    ExpectPtrNull(GetNext(Tail));
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectPtrNull(GetPrev(Head));
    }
    ExpectResponse(GetCount(List, &NodeCount), LL_OK);
    ExpectEqual(NodeCount, 3);
    ExpectEqual(((TestData_t*)GetData(Head))->Id, Id1);
    ExpectEqual(((TestData_t*)GetData(Tail))->Id, Id3);
    ExpectEqual(((TestData_t*)GetData(GetNext(Head)))->Id, Id2);
    ExpectEqual(((TestData_t*)GetData(GetNext(GetNext(Head))))->Id, Id3);
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectEqual(((TestData_t*)GetData(GetPrev(Tail)))->Id, Id2);
        ExpectEqual(((TestData_t*)GetData(GetPrev(GetPrev(Tail))))->Id, Id1);
    }
}

static void ExpectListWith4Nodes(List_t* List, int Id1, int Id2, int Id3, int Id4)
{
    unsigned int NodeCount;
    ExpectPtrNotNull(List);
    ListNode_t* Head = GetHead(List);
    ListNode_t* Tail = GetTail(List);
    ExpectPtrNotNull(Head);
    ExpectPtrNotNull(Tail);
    ExpectNotEqualPtr(Head, Tail);
    ExpectPtrNull(GetNext(Tail));
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectPtrNull(GetPrev(Head));
    }
    ExpectResponse(GetCount(List, &NodeCount), LL_OK);
    ExpectEqual(NodeCount, 4);
    ExpectEqual(((TestData_t*)GetData(Head))->Id, Id1);
    ExpectEqual(((TestData_t*)GetData(Tail))->Id, Id4);
    ExpectEqual(((TestData_t*)GetData(GetNext(Head)))->Id, Id2);
    ExpectEqual(((TestData_t*)GetData(GetNext(GetNext(Head))))->Id, Id3);
    ExpectEqual(((TestData_t*)GetData(GetNext(GetNext(GetNext(Head)))))->Id, Id4);
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectEqual(((TestData_t*)GetData(GetPrev(Tail)))->Id, Id3);
        ExpectEqual(((TestData_t*)GetData(GetPrev(GetPrev(Tail))))->Id, Id2);
        ExpectEqual(((TestData_t*)GetData(GetPrev(GetPrev(GetPrev(Tail)))))->Id, Id1);
    }
}

static void ExpectListWith5Nodes(List_t* List, int Id1, int Id2, int Id3, int Id4, int Id5)
{
    unsigned int NodeCount;
    ExpectPtrNotNull(List);
    ListNode_t* Head = GetHead(List);
    ListNode_t* Tail = GetTail(List);
    ExpectPtrNotNull(Head);
    ExpectPtrNotNull(Tail);
    ExpectNotEqualPtr(Head, Tail);
    ExpectPtrNull(GetNext(Tail));
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectPtrNull(GetPrev(Head));
    }
    ExpectResponse(GetCount(List, &NodeCount), LL_OK);
    ExpectEqual(NodeCount, 5);
    ExpectEqual(((TestData_t*)GetData(Head))->Id, Id1);
    ExpectEqual(((TestData_t*)GetData(Tail))->Id, Id5);
    ExpectEqual(((TestData_t*)GetData(GetNext(Head)))->Id, Id2);
    ExpectEqual(((TestData_t*)GetData(GetNext(GetNext(Head))))->Id, Id3);
    ExpectEqual(((TestData_t*)GetData(GetNext(GetNext(GetNext(Head)))))->Id, Id4);
    ExpectEqual(((TestData_t*)GetData(GetNext(GetNext(GetNext(GetNext(Head))))))->Id, Id5);
    if (List->Linkage == LL_DOUBLE)
    {
        ExpectEqual(((TestData_t*)GetData(GetPrev(Tail)))->Id, Id4);
        ExpectEqual(((TestData_t*)GetData(GetPrev(GetPrev(Tail))))->Id, Id3);
        ExpectEqual(((TestData_t*)GetData(GetPrev(GetPrev(GetPrev(Tail)))))->Id, Id2);
        ExpectEqual(((TestData_t*)GetData(GetPrev(GetPrev(GetPrev(GetPrev(Tail))))))->Id, Id1);
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
