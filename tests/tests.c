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
static void ExpectResponse(ListError_t actual, ListError_t expected);
static void ExpectPointerNull(void* pointer);
static void ExpectPointerNotNull(void* pointer);
static void ExpectEqual(unsigned int this, unsigned int toThis);
static void ExpectEqualPtr(void* this, void* toThis);
static void ExpectNotEqualPtr(void* this, void* toThis);
static void ExpectEmptyList(List_t* list);
static void ExpectListWith1Node(List_t* list, int id1);
static void ExpectListWith2Nodes(List_t* list, int id1, int id2);
static void ExpectListWith3Nodes(List_t* list, int id1, int id2, int id3);
static void ExpectListWith4Nodes(List_t* list, int id1, int id2, int id3, int id4);
static void ExpectListWith5Nodes(List_t* list, int id1, int id2, int id3, int id4, int id5);

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
    TestStart("Test 1: Create valid empty lists with different types of linkage");
    {
        // Create valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);
        ExpectEqual(dList->linkage, LST_L_DOUBLE);

        // Create valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);
        ExpectEqual(sList->linkage, LST_L_SINGLE);

        // Create valid empty singly-linked list
        List_t* rList = NewList(5); // argument out of range => default to singly linked
        ExpectEmptyList(rList);
        ExpectEqual(rList->linkage, LST_L_SINGLE);

        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
        ExpectResponse(DeleteList(rList), LST_E_SUCCESS);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 2: NULL list argument => return NULL node");
    {
        List_t* nullList = NULL;
        ExpectPointerNull(GetHeadNode(nullList));
        ExpectPointerNull(GetTailNode(nullList));
        ExpectPointerNull(GetNodeByData(nullList, &dummyData));
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 3: NULL list argument => return LST_E_INVALID_LIST");
    {
        List_t* nullList = NULL;
        unsigned int count;
        ExpectResponse(InsertToFront(nullList, &dummyData), LST_E_INVALID_LIST);
        ExpectResponse(InsertToBack(nullList, &dummyData), LST_E_INVALID_LIST);
        ExpectResponse(InsertAfterData(nullList, &dummyData, &dummyData), LST_E_INVALID_LIST);
        ExpectResponse(RemoveFirstNode(nullList), LST_E_INVALID_LIST);
        ExpectResponse(RemoveLastNode(nullList), LST_E_INVALID_LIST);
        ExpectResponse(RemoveNodeByData(nullList, &dummyData), LST_E_INVALID_LIST);
        ExpectResponse(GetNodeCount(nullList, &count), LST_E_INVALID_LIST);
        ExpectResponse(DeleteList(nullList), LST_E_INVALID_LIST);
    }
    TestEnd();
    
    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 4: NULL node argument => return NULL pointer");
    {
        ListNode_t* nullNode = NULL;
        ExpectPointerNull(GetNextNode(nullNode));
        ExpectPointerNull(GetPrevNode(nullNode));
        ExpectPointerNull(GetNodeData(nullNode));
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 5: NULL node argument => return LST_E_INVALID_NODE");
    {
        ListNode_t* nullNode = NULL;
        ExpectResponse(InsertAfterNode(nullNode, &dummyData), LST_E_INVALID_NODE);
        ExpectResponse(RemoveNode(nullNode), LST_E_INVALID_NODE);
        ExpectResponse(SetNodeData(nullNode, &dummyData), LST_E_INVALID_NODE);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 6: NULL data argument => return NULL node");
    {
        void* nullData = NULL;

        // Create a valid empty list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: add at least one node, otherwise GetNodeByData will 
        // return NULL because of the empty list rather than the NULL data.
        ExpectResponse(InsertToFront(sList, &dummyData), LST_E_SUCCESS);
        ExpectListWith1Node(sList, dummyData.id);

        // Test
        ExpectPointerNull(GetNodeByData(sList, nullData));
        
        // Cleanup
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }
    TestEnd();
    
    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 7: NULL data argument => return LST_E_INVALID_DATA");
    {
        void* nullData = NULL;

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Try inserting NULL data into an empty list
        ExpectResponse(InsertToFront(sList, nullData), LST_E_INVALID_DATA);
        ExpectResponse(InsertToBack(sList, nullData), LST_E_INVALID_DATA);

        // Insert one node
        ExpectResponse(InsertToFront(sList, &dummyData), LST_E_SUCCESS);
        ExpectListWith1Node(sList, dummyData.id);

        // Try inserting NULL data after a valid node (list must have at least one node)
        ExpectResponse(InsertAfterNode(GetHeadNode(sList), nullData), LST_E_INVALID_DATA);

        // Try inserting NULL data after valid data (list must have at least one node)
        ExpectResponse(InsertAfterData(sList, GetNodeData(GetHeadNode(sList)), nullData), LST_E_INVALID_DATA);

        // Try inserting valid data after NULL data (list must have at least one node)
        ExpectResponse(InsertAfterData(sList, nullData, GetNodeData(GetHeadNode(sList))), LST_E_INVALID_DATA);

        // Try removing NULL data from a list that has at least one node.
        ExpectResponse(RemoveNodeByData(sList, nullData), LST_E_INVALID_DATA);

        // Try setting the data of an existing node to NULL
        ExpectResponse(SetNodeData(GetHeadNode(sList), nullData), LST_E_INVALID_DATA);

        // Cleanup
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }    
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 8: NULL counter argument => return LST_E_INVALID_COUNT");
    {
        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Test
        unsigned int* count_ptr = NULL;
        ExpectResponse(GetNodeCount(sList, count_ptr), LST_E_INVALID_COUNT);

        // Cleanup
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 9: Empty list argument => return NULL node");
    {
        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Test
        ExpectPointerNull(GetHeadNode(sList));
        ExpectPointerNull(GetTailNode(sList));
        ExpectPointerNull(GetNodeByData(sList, &dummyData));

        // Cleanup
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 10: Remove from empty list => return LST_E_EMPTY_LIST");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);

        // Test
        ExpectResponse(RemoveFirstNode(dList), LST_E_EMPTY_LIST);
        ExpectResponse(RemoveLastNode(dList), LST_E_EMPTY_LIST);
        ExpectResponse(RemoveNodeByData(dList, &dummyData), LST_E_EMPTY_LIST);
        
        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 11: Insert data after existing data in an empty list => return LST_E_EMPTY_LIST");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);

        // Test
        ExpectResponse(InsertAfterData(dList, &dummyData, &dummyData), LST_E_EMPTY_LIST);

        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
    }    
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 12: Insert data to the front/back of an empty list => return LST_E_SUCCESS");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList); 

        // Insert to front
        ExpectResponse(InsertToFront(dList, &dummyData), LST_E_SUCCESS);
        ExpectListWith1Node(dList, 0);

        // Remove node
        ExpectResponse(RemoveFirstNode(dList), LST_E_SUCCESS);
        ExpectEmptyList(dList);

        // Insert to back
        ExpectResponse(InsertToBack(dList, &dummyData), LST_E_SUCCESS);
        ExpectListWith1Node(dList, 0);

        // Remove node
        ExpectResponse(RemoveLastNode(dList), LST_E_SUCCESS);
        ExpectEmptyList(dList);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList); 

         // Insert to front
        ExpectResponse(InsertToFront(sList, &dummyData), LST_E_SUCCESS);
        ExpectListWith1Node(sList, 0);

        // Remove node
        ExpectResponse(RemoveFirstNode(sList), LST_E_SUCCESS);
        ExpectEmptyList(sList);

        // Insert to back
        ExpectResponse(InsertToBack(sList, &dummyData), LST_E_SUCCESS);
        ExpectListWith1Node(sList, 0);

        // Remove node
        ExpectResponse(RemoveLastNode(sList), LST_E_SUCCESS);
        ExpectEmptyList(sList);
        
        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 13: Insert a node to the front of a non-empty list => return LST_E_SUCCESS");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(InsertToFront(dList, &testData[0]), LST_E_SUCCESS);
        ExpectListWith1Node(dList, testData[0].id);

        // Test: Insert a node
        ExpectResponse(InsertToFront(dList, &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[1].id, testData[0].id); 

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(InsertToFront(sList, &testData[0]), LST_E_SUCCESS);
        ExpectListWith1Node(sList, testData[0].id);

        // Test: Insert a node
        ExpectResponse(InsertToFront(sList, &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[1].id, testData[0].id); 

        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 14: Remove nodes from the back of a non-empty list => return LST_E_SUCCESS");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert two nodes
        ExpectResponse(InsertToFront(dList, &testData[0]), LST_E_SUCCESS);
        ExpectResponse(InsertToFront(dList, &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[1].id, testData[0].id); 

        // Test
        ExpectResponse(RemoveLastNode(dList), LST_E_SUCCESS);
        ExpectListWith1Node(dList, testData[1].id);
        ExpectResponse(RemoveLastNode(dList), LST_E_SUCCESS);
        ExpectEmptyList(dList);

        // Attempt to remove from empty list (already tested - see test 10)
        ExpectResponse(RemoveLastNode(dList), LST_E_EMPTY_LIST); 

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert two nodes
        ExpectResponse(InsertToFront(sList, &testData[0]), LST_E_SUCCESS);
        ExpectResponse(InsertToFront(sList, &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[1].id, testData[0].id);

        // Test
        ExpectResponse(RemoveLastNode(sList), LST_E_SUCCESS);
        ExpectListWith1Node(sList, testData[1].id);
        ExpectResponse(RemoveLastNode(sList), LST_E_SUCCESS);
        ExpectEmptyList(sList);

        // Attempt to remove from empty list (already tested - see test 10)
        ExpectResponse(RemoveLastNode(sList), LST_E_EMPTY_LIST); 

        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 15: Insert a node to the back of a non-empty list => return LST_E_SUCCESS");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(InsertToBack(dList, &testData[0]), LST_E_SUCCESS);
        ExpectListWith1Node(dList, testData[0].id);

        // Test: Insert another node
        ExpectResponse(InsertToBack(dList, &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[0].id, testData[1].id);

        // Test: Insert a third node
        ExpectResponse(InsertToBack(dList, &testData[0]), LST_E_SUCCESS);
        ExpectListWith3Nodes(dList, testData[0].id, testData[1].id, testData[0].id);

        // Test: Insert a fourth node
        ExpectResponse(InsertToBack(dList, &testData[4]), LST_E_SUCCESS);
        ExpectListWith4Nodes(dList, testData[0].id, testData[1].id, testData[0].id, testData[4].id);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(InsertToBack(sList, &testData[0]), LST_E_SUCCESS);
        ExpectListWith1Node(sList, testData[0].id);

        // Test: Insert another node
        ExpectResponse(InsertToBack(sList, &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[0].id, testData[1].id);

        // Test: Insert a third node
        ExpectResponse(InsertToBack(sList, &testData[0]), LST_E_SUCCESS);
        ExpectListWith3Nodes(sList, testData[0].id, testData[1].id, testData[0].id);

        // Test: Insert a fourth node
        ExpectResponse(InsertToBack(sList, &testData[4]), LST_E_SUCCESS);
        ExpectListWith4Nodes(sList, testData[0].id, testData[1].id, testData[0].id, testData[4].id);

        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 16: Remove nodes from the front of a non-empty list => return LST_E_SUCCESS");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert two nodes
        ExpectResponse(InsertToBack(dList, &testData[0]), LST_E_SUCCESS);
        ExpectResponse(InsertToBack(dList, &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[0].id, testData[1].id);

        // Test
        ExpectResponse(RemoveFirstNode(dList), LST_E_SUCCESS);
        ExpectListWith1Node(dList, testData[1].id);
        ExpectResponse(RemoveFirstNode(dList), LST_E_SUCCESS);
        ExpectEmptyList(dList);

        // Attempt to remove from empty list (already tested - see test 10)
        ExpectResponse(RemoveFirstNode(dList), LST_E_EMPTY_LIST);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert two nodes
        ExpectResponse(InsertToBack(sList, &testData[0]), LST_E_SUCCESS);
        ExpectResponse(InsertToBack(sList, &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[0].id, testData[1].id);

        // Test
        ExpectResponse(RemoveFirstNode(sList), LST_E_SUCCESS);
        ExpectListWith1Node(sList, testData[1].id);
        ExpectResponse(RemoveFirstNode(sList), LST_E_SUCCESS);
        ExpectEmptyList(sList);

        // Attempt to remove from empty list (already tested - see test 10)
        ExpectResponse(RemoveFirstNode(sList), LST_E_EMPTY_LIST);

        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 17: Insert a node after the head node => return LST_E_SUCCESS");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(InsertToBack(dList, &testData[0]), LST_E_SUCCESS);
        ExpectListWith1Node(dList, testData[0].id);

        // Test: Insert a node after head
        ExpectResponse(InsertAfterNode(GetHeadNode(dList), &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[0].id, testData[1].id);

        // Test: Insert another node after head
        ExpectResponse(InsertAfterNode(GetHeadNode(dList), &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(dList, testData[0].id, testData[2].id, testData[1].id);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(InsertToBack(sList, &testData[0]), LST_E_SUCCESS);
        ExpectListWith1Node(sList, testData[0].id);

        // Test: Insert a node after head
        ExpectResponse(InsertAfterNode(GetHeadNode(sList), &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[0].id, testData[1].id);

        // Test: Insert another node after head
        ExpectResponse(InsertAfterNode(GetHeadNode(sList), &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(sList, testData[0].id, testData[2].id, testData[1].id);

        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 18: Insert a node after the tail node => return LST_E_SUCCESS");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(InsertToBack(dList, &testData[0]), LST_E_SUCCESS);
        ExpectListWith1Node(dList, testData[0].id);

        // Test: Insert a node after tail
        ExpectResponse(InsertAfterNode(GetTailNode(dList), &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[0].id, testData[1].id);

        // Test: Insert another node after tail
        ExpectResponse(InsertAfterNode(GetTailNode(dList), &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(dList, testData[0].id, testData[1].id, testData[2].id);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(InsertToBack(sList, &testData[0]), LST_E_SUCCESS);
        ExpectListWith1Node(sList, testData[0].id);

        // Test: Insert a node after tail
        ExpectResponse(InsertAfterNode(GetTailNode(sList), &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[0].id, testData[1].id);

        // Test: Insert another node after tail
        ExpectResponse(InsertAfterNode(GetTailNode(sList), &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(sList, testData[0].id, testData[1].id, testData[2].id);

        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 19: Insert a node after any middle node => return LST_E_SUCCESS");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert 3 nodes
        ExpectResponse(InsertToBack(dList, &testData[0]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(dList), &testData[1]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(dList), &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(dList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Test: Add 104 after 102
        ExpectResponse(InsertAfterNode(GetNextNode(GetHeadNode(dList)), &testData[3]), LST_E_SUCCESS);
        ExpectListWith4Nodes(dList, testData[0].id, testData[1].id, testData[3].id, testData[2].id);  
        // List is now: 101, 102, 104, 103

        // Test: Add 105 after 104
        ExpectResponse(InsertAfterNode(GetPrevNode(GetTailNode(dList)), &testData[4]), LST_E_SUCCESS);
        ExpectListWith5Nodes(dList, testData[0].id, testData[1].id, testData[3].id, testData[4].id, testData[2].id);
        // List is now: 101, 102, 104, 105, 103

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert 3 nodes
        ExpectResponse(InsertToBack(sList, &testData[0]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(sList), &testData[1]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(sList), &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(sList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Test: Add 104 after 102
        ExpectResponse(InsertAfterNode(GetNextNode(GetHeadNode(sList)), &testData[3]), LST_E_SUCCESS);
        ExpectListWith4Nodes(sList, testData[0].id, testData[1].id, testData[3].id, testData[2].id);
        // List is now: 101, 102, 104, 103

        // Test: Add 105 after 104
        ExpectResponse(InsertAfterNode(GetNextNode(GetNextNode(GetHeadNode(sList))), &testData[4]), LST_E_SUCCESS);
        ExpectListWith5Nodes(sList, testData[0].id, testData[1].id, testData[3].id, testData[4].id, testData[2].id);
        // List is now: 101, 102, 104, 105, 103

        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 20: Insert data after inexistent data, when list is non-empty => return LST_E_NOT_FOUND");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(InsertToBack(dList, &testData[0]), LST_E_SUCCESS);
        ExpectListWith1Node(dList, testData[0].id);

        // Save head & tail
        ListNode_t* head_dList_1 = GetHeadNode(dList);
        ListNode_t* tail_dList_1 = GetTailNode(dList);

        // Test: Try inserting a node after inexistent data:
        ExpectResponse(InsertAfterData(dList, &dummyData, &testData[0]), LST_E_NOT_FOUND);
        ExpectListWith1Node(dList, testData[0].id);

        // Head and tail didn't change
        ExpectEqualPtr(GetHeadNode(dList), head_dList_1);
        ExpectEqualPtr(GetTailNode(dList), tail_dList_1);

        // Insert another node:
        ExpectResponse(InsertToBack(dList, &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[0].id, testData[1].id);

        // Save head & tail
        ListNode_t* head_dList_2 = GetHeadNode(dList);
        ListNode_t* tail_dList_2 = GetTailNode(dList);

        // Test: Try inserting another node after inexistent data:
        ExpectResponse(InsertAfterData(dList, &testData[2], &testData[3]), LST_E_NOT_FOUND);
        ExpectListWith2Nodes(dList, testData[0].id, testData[1].id);

        // Head and tail didn't change
        ExpectEqualPtr(GetHeadNode(dList), head_dList_2);
        ExpectEqualPtr(GetTailNode(dList), tail_dList_2);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(InsertToBack(sList, &testData[0]), LST_E_SUCCESS);
        ExpectListWith1Node(sList, testData[0].id);

        // Save head & tail
        ListNode_t* head_sList_1 = GetHeadNode(sList);
        ListNode_t* tail_sList_1 = GetTailNode(sList);

        // Test: Try inserting a node after inexistent data:
        ExpectResponse(InsertAfterData(sList, &dummyData, &testData[0]), LST_E_NOT_FOUND);
        ExpectListWith1Node(sList, testData[0].id);

        // Head and tail didn't change
        ExpectEqualPtr(GetHeadNode(sList), head_sList_1);
        ExpectEqualPtr(GetTailNode(sList), tail_sList_1);

        // Insert another node:
        ExpectResponse(InsertToBack(sList, &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[0].id, testData[1].id);

        // Save head & tail
        ListNode_t* head_sList_2 = GetHeadNode(sList);
        ListNode_t* tail_sList_2 = GetTailNode(sList);

        // Test: Try inserting another node after inexistent data:
        ExpectResponse(InsertAfterData(sList, &testData[2], &testData[3]), LST_E_NOT_FOUND);
        ExpectListWith2Nodes(sList, testData[0].id, testData[1].id);

        // Head and tail didn't change
        ExpectEqualPtr(GetHeadNode(sList), head_sList_2);
        ExpectEqualPtr(GetTailNode(sList), tail_sList_2);

        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }    
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 21: Insert data after existing head data => return LST_E_SUCCESS");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(InsertToFront(dList, &testData[0]), LST_E_SUCCESS);
        ExpectListWith1Node(dList, testData[0].id);

        // Save head & tail
        ListNode_t* head_dList_1 = GetHeadNode(dList);
        ListNode_t* tail_dList_1 = GetTailNode(dList);

        // Test: Try inserting some data after the head data:
        ExpectResponse(InsertAfterData(dList, &testData[0], &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[0].id, testData[1].id);

        // Check that the tail node has changed (head node is the same)
        ListNode_t* head_dList_2 = GetHeadNode(dList);
        ListNode_t* tail_dList_2 = GetTailNode(dList);
        ExpectEqualPtr(head_dList_1, head_dList_2);
        ExpectNotEqualPtr(tail_dList_1, tail_dList_2);

        // Test: Try inserting some data after the head data:
        ExpectResponse(InsertAfterData(dList, &testData[0], &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(dList, testData[0].id, testData[2].id, testData[1].id);

        // Check that both head and tail nodes are unchanged
        ListNode_t* head_dList_3 = GetHeadNode(dList);
        ListNode_t* tail_dList_3 = GetTailNode(dList);
        ExpectEqualPtr(head_dList_2, head_dList_3);
        ExpectEqualPtr(tail_dList_2, tail_dList_3);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(InsertToBack(sList, &testData[0]), LST_E_SUCCESS);
        ExpectListWith1Node(sList, testData[0].id);

        // Save head & tail
        ListNode_t* head_sList_1 = GetHeadNode(sList);
        ListNode_t* tail_sList_1 = GetTailNode(sList);

        // Test: Try inserting some data after the head data:
        ExpectResponse(InsertAfterData(sList, &testData[0], &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[0].id, testData[1].id);

        // Check that the tail node has changed (head node is the same)
        ListNode_t* head_sList_2 = GetHeadNode(sList);
        ListNode_t* tail_sList_2 = GetTailNode(sList);
        ExpectEqualPtr(head_sList_1, head_sList_2);
        ExpectNotEqualPtr(tail_sList_1, tail_sList_2);

        // Test: Try inserting some data after the head data:
        ExpectResponse(InsertAfterData(sList, &testData[0], &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(sList, testData[0].id,  testData[2].id, testData[1].id);

        // Check that both head and tail nodes are unchanged
        ListNode_t* head_sList_3 = GetHeadNode(sList);
        ListNode_t* tail_sList_3 = GetTailNode(sList);
        ExpectEqualPtr(head_sList_2, head_sList_3);
        ExpectEqualPtr(tail_sList_2, tail_sList_3);

        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }
    TestEnd();    

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 22: Insert data after existing data when existing data is in the tail => return LST_E_SUCCESS");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(InsertToFront(dList, &testData[0]), LST_E_SUCCESS);
        ExpectListWith1Node(dList, testData[0].id);

        // Save head & tail
        ListNode_t* head_dList_1 = GetHeadNode(dList);
        ListNode_t* tail_dList_1 = GetTailNode(dList);

        // Test: Try inserting some data after the tail data:
        ExpectResponse(InsertAfterData(dList, &testData[0], &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[0].id, testData[1].id);

        // Check that the tail node has changed (head is the same)
        ListNode_t* head_dList_2 = GetHeadNode(dList);
        ListNode_t* tail_dList_2 = GetTailNode(dList);
        ExpectEqualPtr(head_dList_1, head_dList_2);
        ExpectNotEqualPtr(tail_dList_1, tail_dList_2);

        // Test: Try inserting some more data after the tail data:
        ExpectResponse(InsertAfterData(dList, &testData[1], &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(dList, testData[0].id, testData[1].id, testData[2].id);

        // Check that the tail node has changed (head is the same)
        ListNode_t* head_dList_3 = GetHeadNode(dList);
        ListNode_t* tail_dList_3 = GetTailNode(dList);
        ExpectEqualPtr(head_dList_2, head_dList_3);
        ExpectNotEqualPtr(tail_dList_2, tail_dList_3);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(InsertToBack(sList, &testData[0]), LST_E_SUCCESS);
        ExpectListWith1Node(sList, testData[0].id);

        // Save head & tail
        ListNode_t* head_sList_1 = GetHeadNode(sList);
        ListNode_t* tail_sList_1 = GetTailNode(sList);

        // Test: Try inserting some data after the tail data:
        ExpectResponse(InsertAfterData(sList, &testData[0], &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[0].id, testData[1].id);

        // Check that the tail node has changed (head is the same)
        ListNode_t* head_sList_2 = GetHeadNode(sList);
        ListNode_t* tail_sList_2 = GetTailNode(sList);
        ExpectEqualPtr(head_sList_1, head_sList_2);
        ExpectNotEqualPtr(tail_sList_1, tail_sList_2);

        // Test: Try inserting some data after the tail data:
        ExpectResponse(InsertAfterData(sList, &testData[1], &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(sList, testData[0].id, testData[1].id, testData[2].id);

        // Check that the tail node has changed (head is the same)
        ListNode_t* head_sList_3 = GetHeadNode(sList);
        ListNode_t* tail_sList_3 = GetTailNode(sList);
        ExpectEqualPtr(head_sList_2, head_sList_3);
        ExpectNotEqualPtr(tail_sList_2, tail_sList_3);

        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }
    TestEnd();    

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 23: Insert data after existing data when existing data is in the middle => return LST_E_SUCCESS");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert 3 nodes
        ExpectResponse(InsertToBack(dList, &testData[0]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(dList), &testData[1]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(dList), &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(dList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Save head and tail
        ListNode_t* head_dList_1 = GetHeadNode(dList);
        ListNode_t* tail_dList_1 = GetTailNode(dList);

        // Test: Try inserting some data after the data in the middle node:
        ExpectResponse(InsertAfterData(dList, &testData[1], &testData[3]), LST_E_SUCCESS);
        ExpectListWith4Nodes(dList, testData[0].id, testData[1].id, testData[3].id, testData[2].id);
        // List is now: 101, 102, 104, 103

        // Check that the head & tail nodes didn't change
        ListNode_t* head_dList_2 = GetHeadNode(dList);
        ListNode_t* tail_dList_2 = GetTailNode(dList);
        ExpectEqualPtr(head_dList_1, head_dList_2);
        ExpectEqualPtr(tail_dList_1, tail_dList_2);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert 3 nodes
        ExpectResponse(InsertToBack(sList, &testData[0]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(sList), &testData[1]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(sList), &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(sList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Save head and tail
        ListNode_t* head_sList_1 = GetHeadNode(sList);
        ListNode_t* tail_sList_1 = GetTailNode(sList);

        // Test: Try inserting some data after the data in the middle node:
        ExpectResponse(InsertAfterData(sList, &testData[1], &testData[3]), LST_E_SUCCESS);
        ExpectListWith4Nodes(sList, testData[0].id, testData[1].id, testData[3].id, testData[2].id);
        // List is now: 101, 102, 104, 103

        // Check that the head & tail nodes didn't change
        ListNode_t* head_sList_2 = GetHeadNode(sList);
        ListNode_t* tail_sList_2 = GetTailNode(sList);
        ExpectEqualPtr(head_sList_1, head_sList_2);
        ExpectEqualPtr(tail_sList_1, tail_sList_2);
        
        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }    
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 24: Remove node from the front/back of a non-empty list => return LST_E_SUCCESS");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(InsertToFront(dList, &dummyData), LST_E_SUCCESS);
        ExpectListWith1Node(dList, dummyData.id);

        // Test: List with one node -> remove from front
        ExpectResponse(RemoveFirstNode(dList), LST_E_SUCCESS);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(InsertToBack(dList, &dummyData), LST_E_SUCCESS);
        ExpectListWith1Node(dList, dummyData.id);

        // Test: List with one node -> remove from back
        ExpectResponse(RemoveLastNode(dList), LST_E_SUCCESS);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(InsertToFront(dList, &dummyData), LST_E_SUCCESS);
        ExpectListWith1Node(dList, dummyData.id);

        // Test: List with one node -> remove from back
        ExpectResponse(RemoveLastNode(dList), LST_E_SUCCESS);
        ExpectEmptyList(dList);
        
        // Precondition: insert one node
        ExpectResponse(InsertToBack(dList, &dummyData), LST_E_SUCCESS);
        ExpectListWith1Node(dList, dummyData.id);

        // Test: List with one node -> remove from front
        ExpectResponse(RemoveFirstNode(dList), LST_E_SUCCESS);
        ExpectEmptyList(dList);

        // Precondition: Add 2 nodes
        ExpectResponse(InsertToFront(dList, &testData[4]), LST_E_SUCCESS);
        ExpectResponse(InsertToFront(dList, &testData[3]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[3].id, testData[4].id);

        // Test: List with 2 nodes -> remove from front
        ExpectResponse(RemoveFirstNode(dList), LST_E_SUCCESS);
        ExpectListWith1Node(dList, testData[4].id);

        // Precondition: Add one more node
        ExpectResponse(InsertToBack(dList, &testData[3]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[4].id, testData[3].id);

        // Test: List with 2 nodes -> remove from back
        ExpectResponse(RemoveLastNode(dList), LST_E_SUCCESS);
        ExpectListWith1Node(dList, testData[4].id);

        // Precondition: Add one more node
        ExpectResponse(InsertToFront(dList, &testData[3]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[3].id, testData[4].id);

        // Test: List with 2 nodes -> remove from back
        ExpectResponse(RemoveLastNode(dList), LST_E_SUCCESS);
        ExpectListWith1Node(dList, testData[3].id);

        // Precondition: Add one more node
        ExpectResponse(InsertToBack(dList, &testData[4]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[3].id, testData[4].id);

        // Test: List with 2 nodes -> remove from front
        ExpectResponse(RemoveFirstNode(dList), LST_E_SUCCESS);
        ExpectListWith1Node(dList, testData[4].id);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(InsertToFront(sList, &dummyData), LST_E_SUCCESS);
        ExpectListWith1Node(sList, dummyData.id);

        // Test: List with one node -> remove from front
        ExpectResponse(RemoveFirstNode(sList), LST_E_SUCCESS);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(InsertToBack(sList, &dummyData), LST_E_SUCCESS);
        ExpectListWith1Node(sList, dummyData.id);

        // Test: List with one node -> remove from back
        ExpectResponse(RemoveLastNode(sList), LST_E_SUCCESS);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(InsertToFront(sList, &dummyData), LST_E_SUCCESS);
        ExpectListWith1Node(sList, dummyData.id);

        // Test: List with one node -> remove from back
        ExpectResponse(RemoveLastNode(sList), LST_E_SUCCESS);
        ExpectEmptyList(sList);
        
        // Precondition: insert one node
        ExpectResponse(InsertToBack(sList, &dummyData), LST_E_SUCCESS);
        ExpectListWith1Node(sList, dummyData.id);

        // Test: List with one node -> remove from front
        ExpectResponse(RemoveFirstNode(sList), LST_E_SUCCESS);
        ExpectEmptyList(sList);

        // Precondition: Add 2 nodes
        ExpectResponse(InsertToFront(sList, &testData[4]), LST_E_SUCCESS);
        ExpectResponse(InsertToFront(sList, &testData[3]), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[3].id, testData[4].id);

        // Test: List with 2 nodes -> remove from front
        ExpectResponse(RemoveFirstNode(sList), LST_E_SUCCESS);
        ExpectListWith1Node(sList, testData[4].id);

        // Precondition: Add one more node
        ExpectResponse(InsertToBack(sList, &testData[3]), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[4].id, testData[3].id);

        // Test: List with 2 nodes -> remove from back
        ExpectResponse(RemoveLastNode(sList), LST_E_SUCCESS);
        ExpectListWith1Node(sList, testData[4].id);

        // Precondition: Add one more node
        ExpectResponse(InsertToFront(sList, &testData[3]), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[3].id, testData[4].id);

        // Test: List with 2 nodes -> remove from back
        ExpectResponse(RemoveLastNode(sList), LST_E_SUCCESS);
        ExpectListWith1Node(sList, testData[3].id);

        // Precondition: Add one more node
        ExpectResponse(InsertToBack(sList, &testData[4]), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[3].id, testData[4].id);

        // Test: List with 2 nodes -> remove from front
        ExpectResponse(RemoveFirstNode(sList), LST_E_SUCCESS);
        ExpectListWith1Node(sList, testData[4].id);

        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 25: Remove node from the middle of a non-empty list => return LST_E_SUCCESS");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert 3 nodes
        ExpectResponse(InsertToBack(dList, &testData[0]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(dList), &testData[1]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(dList), &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(dList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Save head and tail
        ListNode_t* head_dList_1 = GetHeadNode(dList);
        ListNode_t* tail_dList_1 = GetTailNode(dList);

        // Test: Try to remove the node from the middle
        ExpectResponse(RemoveNode(GetNextNode(GetHeadNode(dList))), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[0].id, testData[2].id);
        // List is now: 101, 103

        // Head and tail are unchanged
        ListNode_t* head_dList_2 = GetHeadNode(dList);
        ListNode_t* tail_dList_2 = GetTailNode(dList);
        ExpectEqualPtr(head_dList_1, head_dList_2);
        ExpectEqualPtr(tail_dList_1, tail_dList_2);

        // Precondition: Add 2 more nodes
        ExpectResponse(InsertToFront(dList, &testData[4]), LST_E_SUCCESS);
        ExpectResponse(InsertToFront(dList, &testData[3]), LST_E_SUCCESS);
        ExpectListWith4Nodes(dList, testData[3].id, testData[4].id, testData[0].id, testData[2].id);
        // List is now: 104, 105, 101, 103

        // Save head & tail
        ListNode_t* head_dList_3 = GetHeadNode(dList);
        ListNode_t* tail_dList_3 = GetTailNode(dList);

        // Try to remove the nodes from the middle
        ExpectResponse(RemoveNode(GetNextNode(GetNextNode(GetHeadNode(dList)))), LST_E_SUCCESS);
        ExpectListWith3Nodes(dList, testData[3].id, testData[4].id, testData[2].id);
        ExpectResponse(RemoveNode(GetNextNode(GetHeadNode(dList))), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[3].id, testData[2].id);
        // List is now: 104, 103

        // Head and tail are not changed
        ListNode_t* head_dList_4 = GetHeadNode(dList);
        ListNode_t* tail_dList_4 = GetTailNode(dList);
        ExpectEqualPtr(head_dList_3, head_dList_4);
        ExpectEqualPtr(tail_dList_3, tail_dList_4);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert 3 nodes
        ExpectResponse(InsertToBack(sList, &testData[0]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(sList), &testData[1]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(sList), &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(sList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Save head and tail
        ListNode_t* head_sList_1 = GetHeadNode(sList);
        ListNode_t* tail_sList_1 = GetTailNode(sList);

        // Test: Try to remove the node from the middle
        ExpectResponse(RemoveNode(GetNextNode(GetHeadNode(sList))), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[0].id, testData[2].id);
        // List is now: 101, 103

        // Head and tail are unchanged
        ListNode_t* head_sList_2 = GetHeadNode(sList);
        ListNode_t* tail_sList_2 = GetTailNode(sList);
        ExpectEqualPtr(head_sList_1, head_sList_2);
        ExpectEqualPtr(tail_sList_1, tail_sList_2);

        // Precondition: Add 2 more nodes
        ExpectResponse(InsertToFront(sList, &testData[4]), LST_E_SUCCESS);
        ExpectResponse(InsertToFront(sList, &testData[3]), LST_E_SUCCESS);
        ExpectListWith4Nodes(sList, testData[3].id, testData[4].id, testData[0].id, testData[2].id);
        // List is now: 104, 105, 101, 103

        // Save head & tail
        ListNode_t* head_sList_3 = GetHeadNode(sList);
        ListNode_t* tail_sList_3 = GetTailNode(sList);

        // Try to remove the nodes from the middle
        ExpectResponse(RemoveNode(GetNextNode(GetNextNode(GetHeadNode(sList)))), LST_E_SUCCESS);
        ExpectListWith3Nodes(sList, testData[3].id, testData[4].id, testData[2].id);
        ExpectResponse(RemoveNode(GetNextNode(GetHeadNode(sList))), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[3].id, testData[2].id);
        // List is now: 104, 103

        // Head and tail are not changed
        ListNode_t* head_sList_4 = GetHeadNode(sList);
        ListNode_t* tail_sList_4 = GetTailNode(sList);
        ExpectEqualPtr(head_sList_3, head_sList_4);
        ExpectEqualPtr(tail_sList_3, tail_sList_4);

        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 26: Remove inexistent data from a non-empty list => return LST_E_NOT_FOUND");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(InsertToFront(dList, &dummyData), LST_E_SUCCESS);
        ExpectListWith1Node(dList, dummyData.id);

        // Test: Remove inexistent data from a list with one node
        ExpectResponse(RemoveNodeByData(dList, &testData[3]), LST_E_NOT_FOUND);
        ExpectListWith1Node(dList, dummyData.id);

        // Precondition: insert one more node
        ExpectResponse(InsertToFront(dList, &testData[4]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[4].id, dummyData.id);

        // Test: Remove inexistent data from a list with two nodes
        ExpectResponse(RemoveNodeByData(dList, &testData[3]), LST_E_NOT_FOUND);
        ExpectListWith2Nodes(dList, testData[4].id, dummyData.id);

        // Precondition: insert one more node
        ExpectResponse(InsertToFront(dList, &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(dList, testData[2].id, testData[4].id, dummyData.id);

        // Test: Remove inexistent data from a list with three nodes
        ExpectResponse(RemoveNodeByData(dList, &testData[0]), LST_E_NOT_FOUND);
        ExpectResponse(RemoveNodeByData(dList, &testData[3]), LST_E_NOT_FOUND);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(InsertToFront(sList, &dummyData), LST_E_SUCCESS);
        ExpectListWith1Node(sList, dummyData.id);

        // Test: Remove inexistent data from a list with one node
        ExpectResponse(RemoveNodeByData(sList, &testData[3]), LST_E_NOT_FOUND);
        ExpectListWith1Node(sList, dummyData.id);

        // Precondition: insert one more node
        ExpectResponse(InsertToFront(sList, &testData[4]), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[4].id, dummyData.id);

        // Test: Remove inexistent data from a list with two nodes
        ExpectResponse(RemoveNodeByData(sList, &testData[3]), LST_E_NOT_FOUND);
        ExpectListWith2Nodes(sList, testData[4].id, dummyData.id);

        // Precondition: insert one more node
        ExpectResponse(InsertToFront(sList, &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(sList, testData[2].id, testData[4].id, dummyData.id);

        // Test: Remove inexistent data from a list with three nodes
        ExpectResponse(RemoveNodeByData(sList, &testData[0]), LST_E_NOT_FOUND);
        ExpectResponse(RemoveNodeByData(sList, &testData[3]), LST_E_NOT_FOUND);

        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 27: Remove node by data from the front/back of a non-empty list => return LST_E_SUCCESS");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(InsertToFront(dList, &dummyData), LST_E_SUCCESS);
        ExpectListWith1Node(dList, dummyData.id);

        // Test: Remove node by data from a list with 1 node
        ExpectResponse(RemoveNodeByData(dList, &dummyData), LST_E_SUCCESS);
        ExpectEmptyList(dList);

        // Precondition: insert one node
        ExpectResponse(InsertToFront(dList, &testData[3]), LST_E_SUCCESS);
        ExpectListWith1Node(dList, testData[3].id);

        // Test: Remove node by data from a list with 1 node
        ExpectResponse(RemoveNodeByData(dList, &testData[3]), LST_E_SUCCESS);
        ExpectEmptyList(dList);

        // Precondition: insert two nodes
        ExpectResponse(InsertToFront(dList, &testData[4]), LST_E_SUCCESS);
        ExpectResponse(InsertToFront(dList, &testData[3]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[3].id, testData[4].id);

        // Test: Remove node by data from a list with 2 nodes
        ExpectResponse(RemoveNodeByData(dList, &testData[4]), LST_E_SUCCESS);
        ExpectListWith1Node(dList, testData[3].id);

        // Precondition: insert two nodes
        ExpectResponse(InsertToFront(dList, &testData[2]), LST_E_SUCCESS);
        ExpectResponse(InsertToFront(dList, &testData[1]), LST_E_SUCCESS);
        ExpectListWith3Nodes(dList, testData[1].id, testData[2].id, testData[3].id);

        // Test: Remove node by data from a list with 3 nodes
        ExpectResponse(RemoveNodeByData(dList, &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[2].id, testData[3].id);

        // Precondition: insert one node
        ExpectResponse(InsertToFront(dList, &testData[1]), LST_E_SUCCESS);
        
        // Test: Remove node by data from a list with 3 nodes
        ExpectResponse(RemoveNodeByData(dList, &testData[3]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[1].id, testData[2].id);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(InsertToFront(sList, &dummyData), LST_E_SUCCESS);
        ExpectListWith1Node(sList, dummyData.id);

        // Test: Remove node by data from a list with 1 node
        ExpectResponse(RemoveNodeByData(sList, &dummyData), LST_E_SUCCESS);
        ExpectEmptyList(sList);

        // Precondition: insert one node
        ExpectResponse(InsertToFront(sList, &testData[3]), LST_E_SUCCESS);
        ExpectListWith1Node(sList, testData[3].id);

        // Test: Remove node by data from a list with 1 node
        ExpectResponse(RemoveNodeByData(sList, &testData[3]), LST_E_SUCCESS);
        ExpectEmptyList(sList);

        // Precondition: insert two nodes
        ExpectResponse(InsertToFront(sList, &testData[4]), LST_E_SUCCESS);
        ExpectResponse(InsertToFront(sList, &testData[3]), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[3].id, testData[4].id);

        // Test: Remove node by data from a list with 2 nodes
        ExpectResponse(RemoveNodeByData(sList, &testData[4]), LST_E_SUCCESS);
        ExpectListWith1Node(sList, testData[3].id);

        // Precondition: insert two nodes
        ExpectResponse(InsertToFront(sList, &testData[2]), LST_E_SUCCESS);
        ExpectResponse(InsertToFront(sList, &testData[1]), LST_E_SUCCESS);
        ExpectListWith3Nodes(sList, testData[1].id, testData[2].id, testData[3].id);

        // Test: Remove node by data from a list with 3 nodes
        ExpectResponse(RemoveNodeByData(sList, &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[2].id, testData[3].id);

        // Precondition: insert one node
        ExpectResponse(InsertToFront(sList, &testData[1]), LST_E_SUCCESS);
        
        // Test: Remove node by data from a list with 3 nodes
        ExpectResponse(RemoveNodeByData(sList, &testData[3]), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[1].id, testData[2].id);

        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 28: Remove node by data from the middle of a list => return LST_E_SUCCESS");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert 3 nodes
        ExpectResponse(InsertToBack(dList, &testData[0]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(dList), &testData[1]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(dList), &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(dList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Save head and tail
        ListNode_t* head_dList_1 = GetHeadNode(dList);
        ListNode_t* tail_dList_1 = GetTailNode(dList);

        // Test: Remove the node from the middle (by data)
        ExpectResponse(RemoveNodeByData(dList, &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[0].id, testData[2].id);
        // List is now: 101, 103

        // Head and tail are unchanged
        ListNode_t* head_dList_2 = GetHeadNode(dList);
        ListNode_t* tail_dList_2 = GetTailNode(dList);
        ExpectEqualPtr(head_dList_1, head_dList_2);
        ExpectEqualPtr(tail_dList_1, tail_dList_2);

        // Precondition: Add 2 more nodes
        ExpectResponse(InsertToFront(dList, &testData[4]), LST_E_SUCCESS);
        ExpectResponse(InsertToFront(dList, &testData[3]), LST_E_SUCCESS);
        ExpectListWith4Nodes(dList, testData[3].id, testData[4].id, testData[0].id, testData[2].id);
        // List is now: 104, 105, 101, 103

        // Save head and tail
        ListNode_t* head_dList_3 = GetHeadNode(dList);
        ListNode_t* tail_dList_3 = GetTailNode(dList);

        // Remove the nodes from the middle
        ExpectResponse(RemoveNodeByData(dList, &testData[0]), LST_E_SUCCESS);
        ExpectListWith3Nodes(dList, testData[3].id, testData[4].id, testData[2].id);
        ExpectResponse(RemoveNodeByData(dList, &testData[4]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[3].id, testData[2].id);
        // List is now: 104, 103

        // Head and tail are not changed
        ListNode_t* head_dList_4 = GetHeadNode(dList);
        ListNode_t* tail_dList_4 = GetTailNode(dList);
        ExpectEqualPtr(head_dList_3, head_dList_4);
        ExpectEqualPtr(tail_dList_3, tail_dList_4);

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert 3 nodes
        ExpectResponse(InsertToBack(sList, &testData[0]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(sList), &testData[1]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(sList), &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(sList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Save head and tail
        ListNode_t* head_sList_1 = GetHeadNode(sList);
        ListNode_t* tail_sList_1 = GetTailNode(sList);

        // Test: Remove the node from the middle (by data)
        ExpectResponse(RemoveNodeByData(sList, &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[0].id, testData[2].id);
        // List is now: 101, 103

        // Head and tail are unchanged
        ListNode_t* head_sList_2 = GetHeadNode(sList);
        ListNode_t* tail_sList_2 = GetTailNode(sList);
        ExpectEqualPtr(head_sList_1, head_sList_2);
        ExpectEqualPtr(tail_sList_1, tail_sList_2);

        // Precondition: Add 2 more nodes
        ExpectResponse(InsertToFront(sList, &testData[4]), LST_E_SUCCESS);
        ExpectResponse(InsertToFront(sList, &testData[3]), LST_E_SUCCESS);
        ExpectListWith4Nodes(sList, testData[3].id, testData[4].id, testData[0].id, testData[2].id);
        // List is now: 104, 105, 101, 103

        // Save head and tail
        ListNode_t* head_sList_3 = GetHeadNode(sList);
        ListNode_t* tail_sList_3 = GetTailNode(sList);

        // Remove the nodes from the middle
        ExpectResponse(RemoveNodeByData(sList, &testData[0]), LST_E_SUCCESS);
        ExpectListWith3Nodes(sList, testData[3].id, testData[4].id, testData[2].id);
        ExpectResponse(RemoveNodeByData(sList, &testData[4]), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[3].id, testData[2].id);
        // List is now: 104, 103

        // Head and tail are not changed
        ListNode_t* head_sList_4 = GetHeadNode(sList);
        ListNode_t* tail_sList_4 = GetTailNode(sList);
        ExpectEqualPtr(head_sList_3, head_sList_4);
        ExpectEqualPtr(tail_sList_3, tail_sList_4);

        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 29: GetPrevNode in a singly-linked list always return NULL");
    {
        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert 3 nodes
        ExpectResponse(InsertToBack(sList, &testData[0]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(sList), &testData[1]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(sList), &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(sList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Test:
        ExpectPointerNull(GetPrevNode(GetHeadNode(sList)));
        ExpectPointerNull(GetPrevNode(GetNextNode(GetHeadNode(sList))));
        ExpectPointerNull(GetPrevNode(GetNextNode(GetNextNode(GetHeadNode(sList)))));

        // Cleanup
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 30: Get node by data from the head/tail/middle of a list");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert 3 nodes
        ExpectResponse(InsertToBack(dList, &testData[0]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(dList), &testData[1]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(dList), &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(dList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Test:
        ExpectEqualPtr(GetNodeByData(dList, &testData[0]), GetHeadNode(dList));
        ExpectEqualPtr(GetNodeByData(dList, &testData[1]), GetNextNode(GetHeadNode(dList)));
        ExpectEqualPtr(GetNodeByData(dList, &testData[2]), GetTailNode(dList));

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert 3 nodes
        ExpectResponse(InsertToBack(sList, &testData[0]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(sList), &testData[1]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(sList), &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(sList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Test:
        ExpectEqualPtr(GetNodeByData(sList, &testData[0]), GetHeadNode(sList));
        ExpectEqualPtr(GetNodeByData(sList, &testData[1]), GetNextNode(GetHeadNode(sList)));
        ExpectEqualPtr(GetNodeByData(sList, &testData[2]), GetTailNode(sList));

        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 31: Remove Node from head/tail of a list");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert 1 node
        ExpectResponse(InsertToFront(dList, &testData[3]), LST_E_SUCCESS);
        ExpectListWith1Node(dList, testData[3].id);

        // Test: Remove head node from a list with 1 node
        ExpectResponse(RemoveNode(GetHeadNode(dList)), LST_E_SUCCESS);
        ExpectEmptyList(dList);

        // Precondition: insert 1 node
        ExpectResponse(InsertToFront(dList, &testData[4]), LST_E_SUCCESS);
        ExpectListWith1Node(dList, testData[4].id);

        // Test: Remove tail node from a list with 1 node
        ExpectResponse(RemoveNode(GetTailNode(dList)), LST_E_SUCCESS);
        ExpectEmptyList(dList);

        // Precondition: insert 2 nodes
        ExpectResponse(InsertToBack(dList, &testData[0]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(dList), &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[0].id, testData[1].id);
        // List is now: 101, 102

        // Test: Remove head node from a list with 2 nodes
        ExpectResponse(RemoveNode(GetHeadNode(dList)), LST_E_SUCCESS);
        ExpectListWith1Node(dList, testData[1].id);
        // List is now: 102

        // Precondition: insert 1 more node
        ExpectResponse(InsertToFront(dList, &testData[4]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[4].id, testData[1].id);
        // List is now: 105, 102

        // Test: Remove tail node from a list with 2 nodes
        ExpectResponse(RemoveNode(GetTailNode(dList)), LST_E_SUCCESS);
        ExpectListWith1Node(dList, testData[4].id);
        // List is now: 105

        // Test: Remove tail node from a list with 1 node
        ExpectResponse(RemoveNode(GetTailNode(dList)), LST_E_SUCCESS);
        ExpectEmptyList(dList);

        // Precondition: insert 3 nodes
        ExpectResponse(InsertToBack(dList, &testData[0]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(dList), &testData[1]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(dList), &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(dList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Test: Remove head node
        ExpectResponse(RemoveNode(GetHeadNode(dList)), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[1].id, testData[2].id);
        // List is now: 102, 103

        // Precondition: insert 1 more node
        ExpectResponse(InsertToFront(dList, &testData[4]), LST_E_SUCCESS);
        ExpectListWith3Nodes(dList, testData[4].id, testData[1].id, testData[2].id);
        // List is now: 105, 102, 103

        // Test: Remove tail node from a list with 3 nodes
        ExpectResponse(RemoveNode(GetTailNode(dList)), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[4].id, testData[1].id);
        // List is now: 105, 102

        // Create a valid empty singly-linked list
        List_t* sList = NewList(LST_L_SINGLE);
        ExpectEmptyList(sList);

        // Precondition: insert 1 node
        ExpectResponse(InsertToFront(sList, &testData[3]), LST_E_SUCCESS);
        ExpectListWith1Node(sList, testData[3].id);

        // Test: Remove head node from a list with 1 node
        ExpectResponse(RemoveNode(GetHeadNode(sList)), LST_E_SUCCESS);
        ExpectEmptyList(sList);

        // Precondition: insert 1 node
        ExpectResponse(InsertToFront(sList, &testData[4]), LST_E_SUCCESS);
        ExpectListWith1Node(sList, testData[4].id);

        // Test: Remove tail node from a list with 1 node
        ExpectResponse(RemoveNode(GetTailNode(sList)), LST_E_SUCCESS);
        ExpectEmptyList(sList);

        // Precondition: insert 2 nodes
        ExpectResponse(InsertToBack(sList, &testData[0]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(sList), &testData[1]), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[0].id, testData[1].id);
        // List is now: 101, 102

        // Test: Remove head node from a list with 2 nodes
        ExpectResponse(RemoveNode(GetHeadNode(sList)), LST_E_SUCCESS);
        ExpectListWith1Node(sList, testData[1].id);
        // List is now: 102

        // Precondition: insert 1 more node
        ExpectResponse(InsertToFront(sList, &testData[4]), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[4].id, testData[1].id);
        // List is now: 105, 102

        // Test: Remove tail node from a list with 2 nodes
        ExpectResponse(RemoveNode(GetTailNode(sList)), LST_E_SUCCESS);
        ExpectListWith1Node(sList, testData[4].id);
        // List is now: 105

        // Test: Remove tail node from a list with 1 node
        ExpectResponse(RemoveNode(GetTailNode(sList)), LST_E_SUCCESS);
        ExpectEmptyList(sList);

        // Precondition: insert 3 nodes
        ExpectResponse(InsertToBack(sList, &testData[0]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(sList), &testData[1]), LST_E_SUCCESS);
        ExpectResponse(InsertAfterNode(GetTailNode(sList), &testData[2]), LST_E_SUCCESS);
        ExpectListWith3Nodes(sList, testData[0].id, testData[1].id, testData[2].id);
        // List is now: 101, 102, 103

        // Test: Remove head node from a list with 3 nodes
        ExpectResponse(RemoveNode(GetHeadNode(sList)), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[1].id, testData[2].id);
        // List is now: 102, 103

        // Precondition: insert 1 more node
        ExpectResponse(InsertToFront(sList, &testData[4]), LST_E_SUCCESS);
        ExpectListWith3Nodes(sList, testData[4].id, testData[1].id, testData[2].id);
        // List is now: 105, 102, 103

        // Test: Remove tail node from a list with 3 nodes
        ExpectResponse(RemoveNode(GetTailNode(sList)), LST_E_SUCCESS);
        ExpectListWith2Nodes(sList, testData[4].id, testData[1].id);
        // List is now: 105, 102

        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
        ExpectResponse(DeleteList(sList), LST_E_SUCCESS);
    }
    TestEnd();

    // ----------------------------------------------------------------------------------------------------------------
    TestStart("Test 32: Set node data for a valid node returns LST_E_SUCCESS");
    {
        // Create a valid empty doubly-linked list
        List_t* dList = NewList(LST_L_DOUBLE);
        ExpectEmptyList(dList);

        // Precondition: insert 1 node
        ExpectResponse(InsertToFront(dList, &testData[3]), LST_E_SUCCESS);
        ExpectListWith1Node(dList, testData[3].id);
        ExpectEqualPtr(GetNodeData(GetHeadNode(dList)), &testData[3]);

        // Change the head node data in a list with one node
        ExpectResponse(SetNodeData(GetHeadNode(dList), &testData[4]), LST_E_SUCCESS);
        ExpectListWith1Node(dList, testData[4].id);
        ExpectEqualPtr(GetNodeData(GetHeadNode(dList)), &testData[4]);

        // Precondition: insert 1 more node
        ExpectResponse(InsertToFront(dList, &testData[0]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[0].id, testData[4].id);
        ExpectEqualPtr(GetNodeData(GetHeadNode(dList)), &testData[0]);
        ExpectEqualPtr(GetNodeData(GetTailNode(dList)), &testData[4]);

        // Change the head and tail data in a list with two nodes
        ExpectResponse(SetNodeData(GetHeadNode(dList), &testData[1]), LST_E_SUCCESS);
        ExpectResponse(SetNodeData(GetTailNode(dList), &testData[2]), LST_E_SUCCESS);
        ExpectListWith2Nodes(dList, testData[1].id, testData[2].id);
        ExpectEqualPtr(GetNodeData(GetHeadNode(dList)), &testData[1]);
        ExpectEqualPtr(GetNodeData(GetTailNode(dList)), &testData[2]);

        // Cleanup
        ExpectResponse(DeleteList(dList), LST_E_SUCCESS);
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

static void ExpectResponse(ListError_t actual, ListError_t expected)
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

static void ExpectEmptyList(List_t* list)
{
    ExpectPointerNotNull(list);
    ExpectPointerNull(GetHeadNode(list));
    ExpectPointerNull(GetTailNode(list));

    unsigned int nodeCount;
    ExpectResponse(GetNodeCount(list, &nodeCount), LST_E_SUCCESS);
    ExpectEqual(nodeCount, 0);
}

static void ExpectListWith1Node(List_t* list, int id1)
{
    unsigned int nodeCount;
    ExpectPointerNotNull(list);
    ListNode_t* head = GetHeadNode(list);
    ListNode_t* tail = GetTailNode(list);
    ExpectPointerNotNull(head);
    ExpectPointerNotNull(tail);
    ExpectEqualPtr(head, tail);
    ExpectPointerNull(GetNextNode(head));
    ExpectPointerNull(GetNextNode(tail));
    if(list->linkage == LST_L_DOUBLE)
    {
        ExpectPointerNull(GetPrevNode(head));
        ExpectPointerNull(GetPrevNode(tail));
    }
    ExpectResponse(GetNodeCount(list, &nodeCount), LST_E_SUCCESS);
    ExpectEqual(nodeCount, 1);
    ExpectEqual(((TestData_t*)GetNodeData(head))->id, id1);
    ExpectEqual(((TestData_t*)GetNodeData(tail))->id, id1);    
}

static void ExpectListWith2Nodes(List_t* list, int id1, int id2)
{
    unsigned int nodeCount;
    ExpectPointerNotNull(list);
    ListNode_t* head = GetHeadNode(list);
    ListNode_t* tail = GetTailNode(list);
    ExpectPointerNotNull(head);
    ExpectPointerNotNull(tail);
    ExpectNotEqualPtr(head, tail);
    ExpectPointerNull(GetNextNode(tail));
    if (list->linkage == LST_L_DOUBLE)
    {
        ExpectPointerNull(GetPrevNode(head));
    }
    ExpectResponse(GetNodeCount(list, &nodeCount), LST_E_SUCCESS);
    ExpectEqual(nodeCount, 2);
    ExpectEqual(((TestData_t*)GetNodeData(head))->id, id1);
    ExpectEqual(((TestData_t*)GetNodeData(tail))->id, id2);
    ExpectEqual(((TestData_t*)GetNodeData(GetNextNode(head)))->id, id2);
    if (list->linkage == LST_L_DOUBLE)
    {
        ExpectEqual(((TestData_t*)GetNodeData(GetPrevNode(tail)))->id, id1);
    }
}

static void ExpectListWith3Nodes(List_t* list, int id1, int id2, int id3)
{
    unsigned int nodeCount;
    ExpectPointerNotNull(list);
    ListNode_t* head = GetHeadNode(list);
    ListNode_t* tail = GetTailNode(list);
    ExpectPointerNotNull(head);
    ExpectPointerNotNull(tail);
    ExpectNotEqualPtr(head, tail);
    ExpectPointerNull(GetNextNode(tail));
    if (list->linkage == LST_L_DOUBLE)
    {
        ExpectPointerNull(GetPrevNode(head));
    }
    ExpectResponse(GetNodeCount(list, &nodeCount), LST_E_SUCCESS);
    ExpectEqual(nodeCount, 3);
    ExpectEqual(((TestData_t*)GetNodeData(head))->id, id1);
    ExpectEqual(((TestData_t*)GetNodeData(tail))->id, id3);
    ExpectEqual(((TestData_t*)GetNodeData(GetNextNode(head)))->id, id2);
    ExpectEqual(((TestData_t*)GetNodeData(GetNextNode(GetNextNode(head))))->id, id3);
    if (list->linkage == LST_L_DOUBLE)
    {
        ExpectEqual(((TestData_t*)GetNodeData(GetPrevNode(tail)))->id, id2);
        ExpectEqual(((TestData_t*)GetNodeData(GetPrevNode(GetPrevNode(tail))))->id, id1);
    }
}

static void ExpectListWith4Nodes(List_t* list, int id1, int id2, int id3, int id4)
{
    unsigned int nodeCount;
    ExpectPointerNotNull(list);
    ListNode_t* head = GetHeadNode(list);
    ListNode_t* tail = GetTailNode(list);
    ExpectPointerNotNull(head);
    ExpectPointerNotNull(tail);
    ExpectNotEqualPtr(head, tail);
    ExpectPointerNull(GetNextNode(tail));
    if (list->linkage == LST_L_DOUBLE)
    {
        ExpectPointerNull(GetPrevNode(head));
    }
    ExpectResponse(GetNodeCount(list, &nodeCount), LST_E_SUCCESS);
    ExpectEqual(nodeCount, 4);
    ExpectEqual(((TestData_t*)GetNodeData(head))->id, id1);
    ExpectEqual(((TestData_t*)GetNodeData(tail))->id, id4);
    ExpectEqual(((TestData_t*)GetNodeData(GetNextNode(head)))->id, id2);
    ExpectEqual(((TestData_t*)GetNodeData(GetNextNode(GetNextNode(head))))->id, id3);
    ExpectEqual(((TestData_t*)GetNodeData(GetNextNode(GetNextNode(GetNextNode(head)))))->id, id4);
    if (list->linkage == LST_L_DOUBLE)
    {
        ExpectEqual(((TestData_t*)GetNodeData(GetPrevNode(tail)))->id, id3);
        ExpectEqual(((TestData_t*)GetNodeData(GetPrevNode(GetPrevNode(tail))))->id, id2);
        ExpectEqual(((TestData_t*)GetNodeData(GetPrevNode(GetPrevNode(GetPrevNode(tail)))))->id, id1);
    }
}

static void ExpectListWith5Nodes(List_t* list, int id1, int id2, int id3, int id4, int id5)
{
    unsigned int nodeCount;
    ExpectPointerNotNull(list);
    ListNode_t* head = GetHeadNode(list);
    ListNode_t* tail = GetTailNode(list);
    ExpectPointerNotNull(head);
    ExpectPointerNotNull(tail);
    ExpectNotEqualPtr(head, tail);
    ExpectPointerNull(GetNextNode(tail));
    if (list->linkage == LST_L_DOUBLE)
    {
        ExpectPointerNull(GetPrevNode(head));
    }
    ExpectResponse(GetNodeCount(list, &nodeCount), LST_E_SUCCESS);
    ExpectEqual(nodeCount, 5);
    ExpectEqual(((TestData_t*)GetNodeData(head))->id, id1);
    ExpectEqual(((TestData_t*)GetNodeData(tail))->id, id5);
    ExpectEqual(((TestData_t*)GetNodeData(GetNextNode(head)))->id, id2);
    ExpectEqual(((TestData_t*)GetNodeData(GetNextNode(GetNextNode(head))))->id, id3);
    ExpectEqual(((TestData_t*)GetNodeData(GetNextNode(GetNextNode(GetNextNode(head)))))->id, id4);
    ExpectEqual(((TestData_t*)GetNodeData(GetNextNode(GetNextNode(GetNextNode(GetNextNode(head))))))->id, id5);
    if (list->linkage == LST_L_DOUBLE)
    {
        ExpectEqual(((TestData_t*)GetNodeData(GetPrevNode(tail)))->id, id4);
        ExpectEqual(((TestData_t*)GetNodeData(GetPrevNode(GetPrevNode(tail))))->id, id3);
        ExpectEqual(((TestData_t*)GetNodeData(GetPrevNode(GetPrevNode(GetPrevNode(tail)))))->id, id2);
        ExpectEqual(((TestData_t*)GetNodeData(GetPrevNode(GetPrevNode(GetPrevNode(GetPrevNode(tail))))))->id, id1);
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