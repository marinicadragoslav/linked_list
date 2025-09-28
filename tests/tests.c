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
    TestStart("Test 1: LL_NewList with single, double and invalid linkage");
    {
        /* Positive test: Create valid empty doubly-linked list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);
        ExpectEqual((unsigned int)DList->Linkage, (unsigned int)LL_DOUBLE);

        /* Positive test: Create valid empty singly-linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);
        ExpectEqual((unsigned int)SList->Linkage, (unsigned int)LL_SINGLE);

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
    TestStart("Test 2: LL_GetHead with NULL, empty, one-node, multi-node lists");
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

        /* Test 4: Head should be equal to the tail for a one-node list */
        ExpectEqualPtr(Head, LL_GetTail(EmptyList));

        /* Test 5: Get head from list with multiple nodes */
        ExpectResponse(LL_AddToFront(EmptyList, &TestData[1]), LL_OK);
        Head = LL_GetHead(EmptyList);
        ExpectPtrNotNull(Head);
        ExpectEqualPtr(LL_GetData(Head), &TestData[1]);

        /* Test 6: Head should not be equal to the tail */
        ExpectNotEqualPtr(Head, LL_GetTail(EmptyList));

        ExpectResponse(LL_DeleteList(EmptyList), LL_OK);
    }
    TestEnd();

        /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 3: LL_GetTail with NULL, empty, one-node, multi-node lists");
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

        /* Test 4: Tail should be equal to head for a one-node list */
        ExpectEqualPtr(Tail, LL_GetHead(EmptyList));

        /* Test 5: Get tail from list with multiple nodes */
        ExpectResponse(LL_AddToBack(EmptyList, &TestData[1]), LL_OK);
        Tail = LL_GetTail(EmptyList);
        ExpectPtrNotNull(Tail);
        ExpectEqualPtr(LL_GetData(Tail), &TestData[1]);

        /* Test 6: Tail should not be equal to head */
        ExpectNotEqualPtr(Tail, LL_GetHead(EmptyList));

        ExpectResponse(LL_DeleteList(EmptyList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 4: LL_GetNext Tests");
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
        ExpectPtrNull(LL_GetNext(Third)); /* Third == Tail */
        ExpectEqualPtr(Third, LL_GetTail(List));/* Third == Tail */

        ExpectResponse(LL_DeleteList(List), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 5: LL_GetPrev Tests");
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
        ExpectEqualPtr(Middle, LL_GetNext(LL_GetHead(DList))); /* check that Middle is correct */
        
        ListNode_t* Head = LL_GetPrev(Middle);
        ExpectPtrNotNull(Head);
        ExpectEqualPtr(LL_GetData(Head), &TestData[0]);
        ExpectEqualPtr(Head, LL_GetHead(DList));

        ExpectResponse(LL_DeleteList(SList), LL_OK);
        ExpectResponse(LL_DeleteList(DList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 6: LL_GetData Tests");
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
    TestStart("Test 7: LL_GetNodeByData Tests");
    {
        /* Test 1: Get node from NULL list should return NULL */
        List_t* NullList = NULL;
        ExpectPtrNull(LL_GetNodeByData(NullList, &TestData[0]));

        /* Test 2: Get node with NULL data should return NULL */
        List_t* List = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_AddToFront(List, &TestData[0]), LL_OK);
        ExpectPtrNull(LL_GetNodeByData(List, NULL));

        /* Test 3: Get node from empty list should return NULL */
        List_t* EmptyList = LL_NewList(LL_DOUBLE);
        ExpectPtrNull(LL_GetNodeByData(EmptyList, &TestData[0]));

        /* Test 4: Get node from a one-node list */
        ListNode_t* FoundNode = LL_GetNodeByData(List, &TestData[0]);
        ExpectPtrNotNull(FoundNode);
        ExpectEqualPtr(LL_GetData(FoundNode), &TestData[0]);
        ExpectEqualPtr(FoundNode, LL_GetHead(List));

        /* Test 5: Get inexistent node from a one-node list */
        ListNode_t* NotFoundNode = LL_GetNodeByData(List, &TestData[1]);
        ExpectPtrNull(NotFoundNode);

        /* Test 6: Get node from a multi-node list */
        ExpectResponse(LL_AddToBack(List, &TestData[1]), LL_OK);
        FoundNode = LL_GetNodeByData(List, &TestData[0]);
        ExpectPtrNotNull(FoundNode);
        ExpectEqualPtr(LL_GetData(FoundNode), &TestData[0]);
        ExpectEqualPtr(FoundNode, LL_GetHead(List));
        FoundNode = LL_GetNodeByData(List, &TestData[1]);
        ExpectPtrNotNull(FoundNode);
        ExpectEqualPtr(LL_GetData(FoundNode), &TestData[1]);
        ExpectEqualPtr(FoundNode, LL_GetTail(List));

        /* Test 7: Get inexistent node from a multi-node list */
        NotFoundNode = LL_GetNodeByData(List, &TestData[3]);
        ExpectPtrNull(NotFoundNode);

        ExpectResponse(LL_DeleteList(List), LL_OK);
        ExpectResponse(LL_DeleteList(EmptyList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 8: LL_SetData Tests");
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

        /* Test 4: Set data multiple times */
        ExpectResponse(LL_SetData(Head, &TestData[2]), LL_OK);
        ExpectEqualPtr(LL_GetData(Head), &TestData[2]);

        ExpectResponse(LL_DeleteList(List), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 9: LL_AddToFront Tests");
    {
        /* Test 1: Add to NULL list should fail */
        List_t* NullList = NULL;
        ExpectResponse(LL_AddToFront(NullList, &TestData[0]), LL_NOT_OK);

        /* Test 2: Add NULL data to empty singly linked list should fail */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_AddToFront(SList, NULL), LL_NOT_OK);

        /* Test 3: Add to empty s-list should succeed */
        ExpectResponse(LL_AddToFront(SList, &TestData[0]), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);
        ExpectEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList)));

        /* Test 4: Add to one-node s-list should succeed */
        ExpectResponse(LL_AddToFront(SList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[1].Id, TestData[0].Id);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectEqualPtr(LL_GetNext(LL_GetHead(SList)), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList)));

        /* Test 5: Add to multi-node s-list should succeed */
        ExpectResponse(LL_AddToFront(SList, &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[2].Id, TestData[1].Id, TestData[0].Id);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectEqualPtr(LL_GetNext(LL_GetNext(LL_GetHead(SList))), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList)));

        /* Test 6: Add to empty d-list should succeed */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_AddToFront(DList, &TestData[0]), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);
        ExpectEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));

        /* Test 7: Add to one-node d-list should succeed */
        ExpectResponse(LL_AddToFront(DList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[1].Id, TestData[0].Id);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetNext(LL_GetHead(DList)), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetPrev(LL_GetTail(DList)), LL_GetHead(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));

        /* Test 8: Add to multi-node d-list should succeed */
        ExpectResponse(LL_AddToFront(DList, &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[2].Id, TestData[1].Id, TestData[0].Id);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetNext(LL_GetNext(LL_GetHead(DList))), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetPrev(LL_GetPrev(LL_GetTail(DList))), LL_GetHead(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));

        ExpectResponse(LL_DeleteList(SList), LL_OK);
        ExpectResponse(LL_DeleteList(DList), LL_OK);
    }
    TestEnd();

        /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 10: LL_AddToBack Tests");
    {
        /* Test 1: Add to NULL list should fail */
        List_t* NullList = NULL;
        ExpectResponse(LL_AddToBack(NullList, &TestData[0]), LL_NOT_OK);

        /* Test 2: Add NULL data should fail */
        List_t* SList = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_AddToBack(SList, NULL), LL_NOT_OK);

        /* Test 3: Add to empty singly linked list should succeed */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);
        ExpectEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList)));

        /* Test 4: Add to one-node s-list should succeed */
        ExpectResponse(LL_AddToBack(SList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectEqualPtr(LL_GetNext(LL_GetHead(SList)), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList)));

        /* Test 5: Add to multi-node s-list should succeed */
        ExpectResponse(LL_AddToBack(SList, &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectEqualPtr(LL_GetNext(LL_GetNext(LL_GetHead(SList))), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList)));

        /* Test 6: Add to empty d-list should succeed */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);
        ExpectEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));

        /* Test 7: Add to one-node d-list should succeed */
        ExpectResponse(LL_AddToBack(DList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetNext(LL_GetHead(DList)), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetPrev(LL_GetTail(DList)), LL_GetHead(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));

        /* Test 8: Add to multi-node d-list should succeed */
        ExpectResponse(LL_AddToBack(DList, &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetNext(LL_GetNext(LL_GetHead(DList))), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetPrev(LL_GetPrev(LL_GetTail(DList))), LL_GetHead(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));

        ExpectResponse(LL_DeleteList(SList), LL_OK);
        ExpectResponse(LL_DeleteList(DList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 11: LL_InsertAfterNode Tests");
    {
        /* Test 1: Insert after NULL node */
        ListNode_t* NullNode = NULL;
        ExpectResponse(LL_InsertAfterNode(NullNode, &TestData[0]), LL_NOT_OK);

        /* Test 2: Insert NULL data into singly linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_AddToFront(SList, &TestData[0]), LL_OK);
        ListNode_t* Head = LL_GetHead(SList);
        ExpectResponse(LL_InsertAfterNode(Head, NULL), LL_NOT_OK);

        /* Test 3: Insert after head node of a singly linked list */
        ExpectResponse(LL_InsertAfterNode(Head, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectEqualPtr(LL_GetNext(LL_GetHead(SList)), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList)));

        /* Test 4: Insert after tail node of a singly linked list */
         ListNode_t* Tail = LL_GetTail(SList);
        ExpectResponse(LL_InsertAfterNode(Tail, &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectEqualPtr(LL_GetNext(LL_GetNext(LL_GetHead(SList))), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList)));

        /* Test 5: Insert after middle node of an s-list */
         ListNode_t* Middle = LL_GetNext(Head);
        ExpectResponse(LL_InsertAfterNode(Middle, &TestData[3]), LL_OK);
        ExpectListWith4Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[3].Id, TestData[2].Id);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectEqualPtr(LL_GetNext(LL_GetNext(LL_GetNext(LL_GetHead(SList)))), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList))); 

        /* Test 6: Insert after head node of a doubly linked list */
         List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_AddToFront(DList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToBack(DList, &TestData[1]), LL_OK);
        Head = LL_GetHead(DList);
        Tail = LL_GetTail(DList);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);
        ExpectResponse(LL_InsertAfterNode(Head, &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[2].Id, TestData[1].Id);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetNext(LL_GetNext(LL_GetHead(DList))), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetPrev(LL_GetPrev(LL_GetTail(DList))), LL_GetHead(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList))); 

        /* Test 7: Insert after tail node of a doubly linked list */
         ExpectResponse(LL_InsertAfterNode(Tail, &TestData[3]), LL_OK);
        ExpectListWith4Nodes(DList, TestData[0].Id, TestData[2].Id, TestData[1].Id, TestData[3].Id);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetNext(LL_GetNext(LL_GetNext(LL_GetHead(DList)))), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetPrev(LL_GetPrev(LL_GetPrev(LL_GetTail(DList)))), LL_GetHead(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList))); 

        /* Test 8: Insert after middle node of a doubly linked list */
        Middle = LL_GetNext(LL_GetHead(DList));
        ExpectResponse(LL_InsertAfterNode(Middle, &TestData[4]), LL_OK);
        ExpectListWith5Nodes(DList, TestData[0].Id, TestData[2].Id, TestData[4].Id, TestData[1].Id, TestData[3].Id);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetNext(LL_GetNext(LL_GetNext(LL_GetNext(LL_GetHead(DList))))), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetPrev(LL_GetPrev(LL_GetPrev(LL_GetPrev(LL_GetTail(DList))))), LL_GetHead(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList))); 

        ExpectResponse(LL_DeleteList(SList), LL_OK);
        ExpectResponse(LL_DeleteList(DList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 12: LL_InsertAfterData Tests");
    {
        /* Test 1: Insert in NULL list should fail */
        List_t* NullList = NULL;
        ExpectResponse(LL_InsertAfterData(NullList, &TestData[0], &TestData[1]), LL_NOT_OK);

        /* Test 2: Insert with NULL existing data should fail */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_AddToFront(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_InsertAfterData(SList, NULL, &TestData[1]), LL_NOT_OK);

        /* Test 3: Insert with NULL new data should fail */
        ExpectResponse(LL_InsertAfterData(SList, &TestData[0], NULL), LL_NOT_OK);

        /* Test 4: Insert in empty singly linked list should fail */
        List_t* EmptyList = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_InsertAfterData(EmptyList, &TestData[0], &TestData[1]), LL_NOT_OK);

        /* Test 5: Insert after non-existing data in s-list should fail */
        ExpectResponse(LL_InsertAfterData(SList, &TestData[3], &TestData[1]), LL_NOT_OK);

        /* Test 6: Insert after existing data (in head) in s-list should succeed */
        ExpectResponse(LL_InsertAfterData(SList, &TestData[0], &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectEqualPtr(LL_GetNext(LL_GetHead(SList)), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList)));

        /* Test 7: Insert after existing data (in tail) in s-list should succeed */
        ExpectResponse(LL_InsertAfterData(SList, &TestData[1], &TestData[3]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[3].Id);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectEqualPtr(LL_GetNext(LL_GetNext(LL_GetHead(SList))), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList)));

        /* Test 8: Insert after middle existing node in s-list should succeed */
        ExpectResponse(LL_InsertAfterData(SList, &TestData[1], &TestData[2]), LL_OK);
        ExpectListWith4Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id, TestData[3].Id);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectEqualPtr(LL_GetNext(LL_GetNext(LL_GetNext(LL_GetHead(SList)))), LL_GetTail(SList));

        /* Test 9: Insert after the first out of two equal data nodes in s-list should succeed */
        ExpectResponse(LL_SetData(LL_GetNext(LL_GetHead(SList)), &TestData[2]), LL_OK); /* set second node data equal to third node data */
        ExpectListWith4Nodes(SList, TestData[0].Id, TestData[2].Id, TestData[2].Id, TestData[3].Id);
        ExpectResponse(LL_InsertAfterData(SList, &TestData[2], &TestData[4]), LL_OK);
        ExpectListWith5Nodes(SList, TestData[0].Id, TestData[2].Id, TestData[4].Id, TestData[2].Id, TestData[3].Id);

        /* Test 10: Insert after non-existing node of multi-node s-list should fail */
        ExpectResponse(LL_InsertAfterData(SList, &TestData[1], &TestData[3]), LL_NOT_OK);

        /* Test 11: Insert in empty doubly linked list should fail */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_InsertAfterData(DList, &TestData[0], &TestData[1]), LL_NOT_OK);

        /* Test 12: Insert after non-existing data in d-list should fail */
        ExpectResponse(LL_AddToFront(DList, &TestData[0]), LL_OK);
        ExpectResponse(LL_InsertAfterData(DList, &TestData[3], &TestData[1]), LL_NOT_OK);

        /* Test 13: Insert after existing data (in head) in d-list should succeed */
        ExpectResponse(LL_InsertAfterData(DList, &TestData[0], &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetNext(LL_GetHead(DList)), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetPrev(LL_GetTail(DList)), LL_GetHead(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));

        /* Test 14: Insert after existing data (in tail) in d-list should succeed */
        ExpectResponse(LL_InsertAfterData(DList, &TestData[1], &TestData[2]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetNext(LL_GetNext(LL_GetHead(DList))), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetPrev(LL_GetPrev(LL_GetTail(DList))), LL_GetHead(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));

        /* Test 15: Insert after existing data (in middle) in d-list should succeed */
        ExpectResponse(LL_InsertAfterData(DList, &TestData[1], &TestData[3]), LL_OK);
        ExpectListWith4Nodes(DList, TestData[0].Id, TestData[1].Id, TestData[3].Id, TestData[2].Id);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetNext(LL_GetNext(LL_GetNext(LL_GetHead(DList)))), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetPrev(LL_GetPrev(LL_GetPrev(LL_GetTail(DList)))), LL_GetHead(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));

        /* Test 16: Insert after the first out of two equal data nodes in d-list should succeed */
        ExpectResponse(LL_SetData(LL_GetNext(LL_GetHead(DList)), &TestData[3]), LL_OK); /* set second node data equal to third node data */
        ExpectListWith4Nodes(DList, TestData[0].Id, TestData[3].Id, TestData[3].Id, TestData[2].Id);
        ExpectResponse(LL_InsertAfterData(DList, &TestData[3], &TestData[4]), LL_OK);
        ExpectListWith5Nodes(DList, TestData[0].Id, TestData[3].Id, TestData[4].Id, TestData[3].Id, TestData[2].Id);

        /* Test 17: Insert after non-existing node of multi-node d-list should fail */
        ExpectResponse(LL_InsertAfterData(DList, &TestData[1], &TestData[2]), LL_NOT_OK);

        ExpectResponse(LL_DeleteList(SList), LL_OK);
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(EmptyList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 13: LL_RemoveHead Tests");
    {
        /* Test 1: Remove from NULL list */
        List_t* NullList = NULL;
        ExpectResponse(LL_RemoveHead(NullList), LL_NOT_OK);

        /* Test 2: Remove from empty s-list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);
        ExpectResponse(LL_RemoveHead(SList), LL_NOT_OK);

        /* Test 3: Remove from single-node s-list */
        ExpectResponse(LL_AddToFront(SList, &TestData[0]), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);
        ExpectEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectResponse(LL_RemoveHead(SList), LL_OK);
        ExpectEmptyList(SList);

        /* Test 4: Remove from multi-node s-list */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToBack(SList, &TestData[1]), LL_OK);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);
        ExpectResponse(LL_RemoveHead(SList), LL_OK);
        ExpectListWith1Node(SList, TestData[1].Id);
        ExpectEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectPtrNull(LL_GetNext(LL_GetHead(SList)));

        /* Test 5: Remove from multi-node s-list with 3 nodes */
        ExpectResponse(LL_AddToFront(SList, &TestData[3]), LL_OK);
        ExpectResponse(LL_AddToFront(SList, &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[2].Id, TestData[3].Id, TestData[1].Id);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectResponse(LL_RemoveHead(SList), LL_OK);
        ExpectListWith2Nodes(SList, TestData[3].Id, TestData[1].Id);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectResponse(LL_RemoveHead(SList), LL_OK);
        ExpectListWith1Node(SList, TestData[1].Id);
        ExpectPtrNull(LL_GetNext(LL_GetHead(SList)));
        ExpectEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectResponse(LL_RemoveHead(SList), LL_OK);
        ExpectEmptyList(SList);

        /* Test 6: Remove from empty d-list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);
        ExpectResponse(LL_RemoveHead(DList), LL_NOT_OK);

        /* Test 7: Remove from single-node d-list */
        ExpectResponse(LL_AddToFront(DList, &TestData[0]), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);
        ExpectResponse(LL_RemoveHead(DList), LL_OK);
        ExpectEmptyList(DList);

        /* Test 8: Remove from multi-node d-list */
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToBack(DList, &TestData[1]), LL_OK);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);
        ExpectResponse(LL_RemoveHead(DList), LL_OK);
        ExpectListWith1Node(DList, TestData[1].Id);
        ExpectEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));

        /* Test 9: Remove from multi-node d-list with 3 nodes */
        ExpectResponse(LL_AddToBack(DList, &TestData[2]), LL_OK);
        ExpectResponse(LL_AddToBack(DList, &TestData[3]), LL_OK);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectListWith3Nodes(DList, TestData[1].Id, TestData[2].Id, TestData[3].Id);
        ExpectResponse(LL_RemoveHead(DList), LL_OK);
        ExpectListWith2Nodes(DList, TestData[2].Id, TestData[3].Id);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectResponse(LL_RemoveHead(DList), LL_OK);
        ExpectListWith1Node(DList, TestData[3].Id);
        ExpectEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));
        ExpectResponse(LL_RemoveHead(DList), LL_OK);
        ExpectEmptyList(DList);

        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 14: LL_RemoveTail Tests");
    {
        /* Test 1: Remove from NULL list */
        List_t* NullList = NULL;
        ExpectResponse(LL_RemoveTail(NullList), LL_NOT_OK);

        /* Test 2: Remove from empty s-list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectEmptyList(SList);
        ExpectResponse(LL_RemoveTail(SList), LL_NOT_OK);

        /* Test 3: Remove from single-node s-list */
        ExpectResponse(LL_AddToFront(SList, &TestData[0]), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);
        ExpectEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectResponse(LL_RemoveTail(SList), LL_OK);
        ExpectEmptyList(SList);

        /* Test 4: Remove from multi-node s-list */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToBack(SList, &TestData[1]), LL_OK);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);
        ExpectResponse(LL_RemoveTail(SList), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);
        ExpectEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList)));

        /* Test 5: Remove from multi-node s-list with 3 nodes */
        ExpectResponse(LL_AddToBack(SList, &TestData[2]), LL_OK);
        ExpectResponse(LL_AddToBack(SList, &TestData[3]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[2].Id, TestData[3].Id);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectResponse(LL_RemoveTail(SList), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[2].Id);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList)));
        ExpectResponse(LL_RemoveTail(SList), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);
        ExpectEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList)));
        ExpectResponse(LL_RemoveTail(SList), LL_OK);
        ExpectEmptyList(SList);

        /* Test 6: Remove from empty d-list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectEmptyList(DList);
        ExpectResponse(LL_RemoveTail(DList), LL_NOT_OK);

        /* Test 7: Remove from single-node d-list */
        ExpectResponse(LL_AddToFront(DList, &TestData[0]), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);
        ExpectResponse(LL_RemoveTail(DList), LL_OK);
        ExpectEmptyList(DList);

        /* Test 8: Remove from multi-node d-list */
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToBack(DList, &TestData[1]), LL_OK);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);
        ExpectResponse(LL_RemoveTail(DList), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);
        ExpectEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));

        /* Test 9: Remove from multi-node d-list with 3 nodes */
        ExpectResponse(LL_AddToBack(DList, &TestData[2]), LL_OK);
        ExpectResponse(LL_AddToBack(DList, &TestData[3]), LL_OK);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectListWith3Nodes(DList, TestData[0].Id, TestData[2].Id, TestData[3].Id);
        ExpectResponse(LL_RemoveTail(DList), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[2].Id);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectResponse(LL_RemoveTail(DList), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);
        ExpectEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));
        ExpectResponse(LL_RemoveTail(DList), LL_OK);
        ExpectEmptyList(DList);

        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 15: LL_RemoveNode Tests");
    {
        /* Test 1: Remove NULL node */
        ListNode_t* NullNode = NULL;
        ExpectResponse(LL_RemoveNode(NullNode), LL_NOT_OK);

        /* Test 2: Remove head node from single-node singly linked list */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_AddToFront(SList, &TestData[0]), LL_OK);
        ListNode_t* Head = LL_GetHead(SList);
        ExpectResponse(LL_RemoveNode(Head), LL_OK);
        ExpectEmptyList(SList);

        /* Test 3: Remove head node from multi-node s-list */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToBack(SList, &TestData[1]), LL_OK);
        ExpectResponse(LL_AddToBack(SList, &TestData[2]), LL_OK);
        ExpectResponse(LL_RemoveNode(LL_GetHead(SList)), LL_OK);
        ExpectListWith2Nodes(SList, TestData[1].Id, TestData[2].Id);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectEqualPtr(LL_GetNext(LL_GetHead(SList)), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList)));
        ExpectResponse(LL_RemoveNode(LL_GetHead(SList)), LL_OK);
        ExpectListWith1Node(SList, TestData[2].Id);
        ExpectEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList)));
        ExpectResponse(LL_RemoveNode(LL_GetHead(SList)), LL_OK);
        ExpectEmptyList(SList);

        /* Test 4: Remove tail node from multi-node s-list */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToBack(SList, &TestData[1]), LL_OK);
        ExpectResponse(LL_AddToBack(SList, &TestData[2]), LL_OK);
        ExpectResponse(LL_RemoveNode(LL_GetTail(SList)), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[1].Id);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectEqualPtr(LL_GetNext(LL_GetHead(SList)), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList)));
        ExpectResponse(LL_RemoveNode(LL_GetTail(SList)), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);
        ExpectEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList)));
        ExpectResponse(LL_RemoveNode(LL_GetTail(SList)), LL_OK);
        ExpectEmptyList(SList);

        /* Test 5: Remove middle node from s-list */
        ExpectResponse(LL_AddToBack(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToBack(SList, &TestData[1]), LL_OK);
        ExpectResponse(LL_AddToBack(SList, &TestData[2]), LL_OK);
        ListNode_t* Middle = LL_GetNext(LL_GetHead(SList));
        ExpectResponse(LL_RemoveNode(Middle), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[2].Id);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectEqualPtr(LL_GetNext(LL_GetHead(SList)), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList)));

        /* Test 6: Remove head node from single-node d-list */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_AddToFront(DList, &TestData[0]), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);
        ExpectEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        Head = LL_GetHead(DList);
        ExpectResponse(LL_RemoveNode(LL_GetHead(DList)), LL_OK);
        ExpectEmptyList(DList);

        /* Test 7: Remove head node from multi-node d-list */
        ExpectResponse(LL_AddToFront(DList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToBack(DList, &TestData[1]), LL_OK);
        ExpectResponse(LL_AddToBack(DList, &TestData[2]), LL_OK);
        ExpectResponse(LL_RemoveNode(LL_GetHead(DList)), LL_OK);
        ExpectListWith2Nodes(DList, TestData[1].Id, TestData[2].Id);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetNext(LL_GetHead(DList)), LL_GetTail(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));
        ExpectResponse(LL_RemoveNode(LL_GetHead(DList)), LL_OK);
        ExpectListWith1Node(DList, TestData[2].Id);
        ExpectEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));
        ExpectResponse(LL_RemoveNode(LL_GetHead(DList)), LL_OK);
        ExpectEmptyList(DList);

        /* Test 8: Remove tail node from multi-node d-list */
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToBack(DList, &TestData[1]), LL_OK);
        ExpectResponse(LL_AddToBack(DList, &TestData[2]), LL_OK);
        ExpectResponse(LL_RemoveNode(LL_GetTail(DList)), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetNext(LL_GetHead(DList)), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetPrev(LL_GetTail(DList)), LL_GetHead(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));
        ExpectResponse(LL_RemoveNode(LL_GetTail(DList)), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);
        ExpectEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));
        ExpectResponse(LL_RemoveNode(LL_GetTail(DList)), LL_OK);
        ExpectEmptyList(DList);

        /* Test 9: Remove middle node from d-list */
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToBack(DList, &TestData[1]), LL_OK);
        ExpectResponse(LL_AddToBack(DList, &TestData[2]), LL_OK);
        Middle = LL_GetNext(LL_GetHead(DList));
        ExpectResponse(LL_RemoveNode(Middle), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[2].Id);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetNext(LL_GetHead(DList)), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetPrev(LL_GetTail(DList)), LL_GetHead(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));
        
        ExpectResponse(LL_DeleteList(SList), LL_OK);
        ExpectResponse(LL_DeleteList(DList), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 16: LL_RemoveNodeByData Tests");
    {
        /* Test 1: Remove from NULL list should fail */
        List_t* NullList = NULL;
        ExpectResponse(LL_RemoveNodeByData(NullList, &TestData[0]), LL_NOT_OK);

        /* Test 2: Remove NULL data should fail*/
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_AddToFront(SList, &TestData[0]), LL_OK);
        ExpectEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectResponse(LL_RemoveNodeByData(SList, NULL), LL_NOT_OK);

        /* Test 3: Remove from empty singly linked list should fail */
        List_t* EmptyList = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_RemoveNodeByData(EmptyList, &TestData[0]), LL_NOT_OK);

        /* Test 4: Remove non-existing data from singly linked list should fail */
        ExpectResponse(LL_RemoveNodeByData(SList, &TestData[3]), LL_NOT_OK);

        /* Test 5: Remove existing data from single-node s-list */
        ExpectResponse(LL_RemoveNodeByData(SList, &TestData[0]), LL_OK);
        ExpectEmptyList(SList);

        /* Test 6: Remove head data from two-node s-list */
        ExpectResponse(LL_AddToFront(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToFront(SList, &TestData[1]), LL_OK);
        ExpectResponse(LL_RemoveNodeByData(SList, &TestData[0]), LL_OK);
        ExpectListWith1Node(SList, TestData[1].Id);
        ExpectEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectPtrNull(LL_GetNext(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));

        /* Test 7: Remove head data from multi-node s-list */
        ExpectResponse(LL_AddToFront(SList, &TestData[2]), LL_OK);
        ExpectResponse(LL_AddToFront(SList, &TestData[3]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[3].Id, TestData[2].Id, TestData[1].Id);
        ExpectResponse(LL_RemoveNodeByData(SList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[2].Id, TestData[1].Id);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectEqualPtr(LL_GetNext(LL_GetHead(SList)), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList)));

        /* Test 8: Remove tail data from two-node s-list */
        ExpectResponse(LL_RemoveNodeByData(SList, &TestData[1]), LL_OK);
        ExpectListWith1Node(SList, TestData[2].Id);
        ExpectEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectPtrNull(LL_GetNext(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));

        /* Test 9: Remove tail data from multi-node s-list */
        ExpectResponse(LL_AddToFront(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToBack(SList, &TestData[1]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[2].Id, TestData[1].Id);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectResponse(LL_RemoveNodeByData(SList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[2].Id);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectEqualPtr(LL_GetNext(LL_GetHead(SList)), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList)));
        ExpectResponse(LL_RemoveNodeByData(SList, &TestData[2]), LL_OK);
        ExpectListWith1Node(SList, TestData[0].Id);
        ExpectEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectPtrNull(LL_GetNext(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));

        /* Test 10: Remove middle data from multi-node s-list */
        ExpectResponse(LL_AddToBack(SList, &TestData[1]), LL_OK);
        ExpectResponse(LL_AddToBack(SList, &TestData[2]), LL_OK);
        ExpectListWith3Nodes(SList, TestData[0].Id, TestData[1].Id, TestData[2].Id);
        ExpectResponse(LL_RemoveNodeByData(SList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(SList, TestData[0].Id, TestData[2].Id);
        ExpectNotEqualPtr(LL_GetHead(SList), LL_GetTail(SList));
        ExpectEqualPtr(LL_GetNext(LL_GetHead(SList)), LL_GetTail(SList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(SList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(SList)));

        /* Test 11: Remove from empty doubly linked list should fail */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_RemoveNodeByData(DList, &TestData[0]), LL_NOT_OK);

        /* Test 12: Remove non-existing data from doubly linked list should fail */
        ExpectResponse(LL_AddToFront(DList, &TestData[0]), LL_OK);
        ExpectListWith1Node(DList, TestData[0].Id);
        ExpectEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectResponse(LL_RemoveNodeByData(DList, &TestData[3]), LL_NOT_OK);

        /* Test 13: Remove existing data from one-node d-list should succeed */
        ExpectResponse(LL_RemoveNodeByData(DList, &TestData[0]), LL_OK);
        ExpectEmptyList(DList);

        /* Test 14: Remove head data from two-node d-list should succeed */
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToBack(DList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[0].Id, TestData[1].Id);
        ExpectResponse(LL_RemoveNodeByData(DList, &TestData[0]), LL_OK);
        ExpectListWith1Node(DList, TestData[1].Id);
        ExpectEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));

        /* Test 15: Remove head data from multi-node d-list should succeed */
        ExpectResponse(LL_AddToBack(DList, &TestData[2]), LL_OK);
        ExpectResponse(LL_AddToBack(DList, &TestData[3]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[1].Id, TestData[2].Id, TestData[3].Id);
        ExpectResponse(LL_RemoveNodeByData(DList, &TestData[1]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[2].Id, TestData[3].Id);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetNext(LL_GetHead(DList)), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetPrev(LL_GetTail(DList)), LL_GetHead(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));
        ExpectResponse(LL_RemoveNodeByData(DList, &TestData[2]), LL_OK);
        ExpectListWith1Node(DList, TestData[3].Id);
        ExpectEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));

        /* Test 16: Remove tail data from two-node d-list should succeed */
        ExpectResponse(LL_AddToFront(DList, &TestData[2]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[2].Id, TestData[3].Id);
        ExpectResponse(LL_RemoveNodeByData(DList, &TestData[3]), LL_OK);
        ExpectListWith1Node(DList, TestData[2].Id);
        ExpectEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));

        /* Test 17: Remove tail data from multi-node d-list should succeed */
        ExpectResponse(LL_AddToBack(DList, &TestData[3]), LL_OK);
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[2].Id, TestData[3].Id, TestData[0].Id);
        ExpectResponse(LL_RemoveNodeByData(DList, &TestData[0]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[2].Id, TestData[3].Id);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetNext(LL_GetHead(DList)), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetPrev(LL_GetTail(DList)), LL_GetHead(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));
        ExpectResponse(LL_RemoveNodeByData(DList, &TestData[3]), LL_OK);
        ExpectListWith1Node(DList, TestData[2].Id);
        ExpectEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));

        /* Test 18: Remove middle data from multi-node d-list should succeed */
        ExpectResponse(LL_AddToBack(DList, &TestData[3]), LL_OK);
        ExpectResponse(LL_AddToBack(DList, &TestData[0]), LL_OK);
        ExpectListWith3Nodes(DList, TestData[2].Id, TestData[3].Id, TestData[0].Id);
        ExpectResponse(LL_RemoveNodeByData(DList, &TestData[3]), LL_OK);
        ExpectListWith2Nodes(DList, TestData[2].Id, TestData[0].Id);
        ExpectNotEqualPtr(LL_GetHead(DList), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetNext(LL_GetHead(DList)), LL_GetTail(DList));
        ExpectEqualPtr(LL_GetPrev(LL_GetTail(DList)), LL_GetHead(DList));
        ExpectPtrNull(LL_GetPrev(LL_GetHead(DList)));
        ExpectPtrNull(LL_GetNext(LL_GetTail(DList)));

        ExpectResponse(LL_DeleteList(SList), LL_OK);
        ExpectResponse(LL_DeleteList(DList), LL_OK);
        ExpectResponse(LL_DeleteList(EmptyList), LL_OK);
    }
    TestEnd();


    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 17: LL_GetCount Tests");
    {
        /* Test 1: Get count from NULL list should fail */
        List_t* NullList = NULL;
        unsigned int Count = 5;
        ExpectResponse(LL_GetCount(NullList, &Count), LL_NOT_OK);
        ExpectEqual(Count, 5); /* Count should remain unchanged */

        /* Test 2: Get count with NULL counter should fail */
        List_t* List = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_GetCount(List, NULL), LL_NOT_OK);
        ExpectEqual(Count, 5); /* Count should remain unchanged */

        /* Test 3: Get count from empty list should return 0 */
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
        ExpectResponse(LL_RemoveTail(List), LL_OK);
        ExpectResponse(LL_GetCount(List, &Count), LL_OK);
        ExpectEqual(Count, 1);
        ExpectResponse(LL_RemoveTail(List), LL_OK);
        ExpectResponse(LL_GetCount(List, &Count), LL_OK);
        ExpectEqual(Count, 0);

        ExpectResponse(LL_DeleteList(List), LL_OK);
    }
    TestEnd();

    /* ---------------------------------------------------------------------------------------------------------------- */
    TestStart("Test 18: LL_DeleteList Tests");
    {
        /* Test 1: Delete NULL list should fail */
        List_t* NullList = NULL;
        ExpectResponse(LL_DeleteList(NullList), LL_NOT_OK);

        /* Test 2: Delete empty s-list should succeed */
        List_t* EmptyList = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_DeleteList(EmptyList), LL_OK);

        /* Test 3: Delete single-node s-list should succeed */
        List_t* SList = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_AddToFront(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);

        /* Test 4: Delete multi-node s-list should succeed */
        SList = LL_NewList(LL_SINGLE);
        ExpectResponse(LL_AddToFront(SList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToFront(SList, &TestData[1]), LL_OK);
        ExpectResponse(LL_AddToFront(SList, &TestData[2]), LL_OK);
        ExpectResponse(LL_DeleteList(SList), LL_OK);

        /* Test 5: Delete empty d-list should succeed */
        List_t* DList = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_DeleteList(DList), LL_OK);    

        /* Test 6: Delete single-node d-list should succeed */
        DList = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_AddToFront(DList, &TestData[0]), LL_OK);
        ExpectResponse(LL_DeleteList(DList), LL_OK);

        /* Test 7: Delete multi-node d-list should succeed */
        DList = LL_NewList(LL_DOUBLE);
        ExpectResponse(LL_AddToFront(DList, &TestData[0]), LL_OK);
        ExpectResponse(LL_AddToFront(DList, &TestData[1]), LL_OK);
        ExpectResponse(LL_AddToFront(DList, &TestData[2]), LL_OK);
        ExpectResponse(LL_DeleteList(DList), LL_OK);
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
