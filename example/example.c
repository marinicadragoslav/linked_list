#include <stdio.h>
#include "linked_list.h"

int main(void)
{
    // Create list
    List_t* list = NewList(LST_L_DOUBLE);

    // Add some data to the list. Void pointers are added as data.
    float pi = 3.14, sqrt2 = 1.4142, ln2 = 0.693;  
    if ((InsertToBack(list, &pi) == LST_E_SUCCESS) &&
        (InsertToFront(list, &sqrt2) == LST_E_SUCCESS) &&
        (InsertToBack(list, &ln2) == LST_E_SUCCESS))
    {
        printf("\nSuccesfully added 3 nodes (1.4142, 3.14, 0.693) to the list!\n\n");
    }

    // Iterate through the list and print data
    printf("List content:\n");
    ListNode_t* iter = GetHeadNode(list);
    while(iter)
    {
        float* dataPtr = (float*)GetNodeData(iter); // Cast void pointer to the original type!
        printf("---> Node data: %f\n", (*dataPtr));

        iter = GetNextNode(iter);
    }
    printf("\n");

    // Remove some nodes
    if ((RemoveFirstNode(list) == LST_E_SUCCESS) &&
        (RemoveLastNode(list) == LST_E_SUCCESS))
    {
        printf("Succesfully removed first and last node!\n");
    }

    // List has one node => head and tail data should be the same
    float* head = (float*)GetNodeData(GetHeadNode(list));
    float* tail = (float*)GetNodeData(GetTailNode(list));
    printf("Head data: %f, tail data: %f. Head node should be the same as tail node.\n\n", *head, *tail);

    // Add another node
    float half = 0.5;
    if (InsertToFront(list, &half) == LST_E_SUCCESS)
    {
        printf("Added 0.5 to list!\n\n");
    }

    // Search for a node by data, then remove it
    ListNode_t* piNode = GetNodeByData(list, &pi);
    if (RemoveNode(piNode) == LST_E_SUCCESS)
    {
        printf("Succesfully removed 3.14 from the list!\n\n");
    }

    // Iterate through the list again
    printf("List content:\n");
    iter = GetHeadNode(list);
    while(iter)
    {
        float* dataPtr = (float*)GetNodeData(iter);
        printf("---> Node data: %f\n", *dataPtr);

        iter = GetNextNode(iter);
    }
    printf("\n");

    // Cleanup 
    if(DeleteList(list) == LST_E_SUCCESS)
    {
        printf("Cleanup successful!\n\n");
    }

    list = NULL; // Reinit list pointer for future use.
}