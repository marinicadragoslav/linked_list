#include <stdio.h>
#include "linked_list.h"

int main(void)
{
    // Create list
    List_t* List = NewList(LL_DOUBLE);

    // Add some data to the list. Void pointers are added as data.
    float pi = 3.14, sqrt2 = 1.4142, ln2 = 0.693;
    if ((AddToBack(List, &pi) == LL_OK) &&
        (AddToFront(List, &sqrt2) == LL_OK) &&
        (AddToBack(List, &ln2) == LL_OK))
    {
        printf("\nSuccesfully added 3 nodes (1.4142, 3.14, 0.693) to the list!\n\n");
    }

    // Iterate through the list and print data
    printf("List content:\n");
    ListNode_t* iter = GetHead(List);
    while(iter)
    {
        float* dataPtr = (float*)GetData(iter); // Cast void pointer to the original type!
        printf("---> Node data: %f\n", (*dataPtr));

        iter = GetNext(iter);
    }
    printf("\n");

    // Remove some nodes
    if ((RemoveHead(List) == LL_OK) &&
        (RemoveTail(List) == LL_OK))
    {
        printf("Succesfully removed first and last node!\n");
    }

    // List has one node => head and tail data should be the same
    float* head = (float*)GetData(GetHead(List));
    float* tail = (float*)GetData(GetTail(List));
    printf("Head data: %f, tail data: %f. Head node should be the same as tail node.\n\n", *head, *tail);

    // Add another node
    float half = 0.5;
    if (AddToFront(List, &half) == LL_OK)
    {
        printf("Added 0.5 to list!\n\n");
    }

    // Search for a node by data, then remove it
    ListNode_t* piNode = GetNodeByData(List, &pi);
    if (RemoveNode(piNode) == LL_OK)
    {
        printf("Succesfully removed 3.14 from the list!\n\n");
    }

    // Iterate through the list again
    printf("List content:\n");
    iter = GetHead(List);
    while(iter)
    {
        float* dataPtr = (float*)GetData(iter);
        printf("---> Node data: %f\n", *dataPtr);

        iter = GetNext(iter);
    }
    printf("\n");

    // Cleanup 
    if(DeleteList(List) == LL_OK)
    {
        printf("Cleanup successful!\n\n");
    }

    List = NULL; // Reinit list pointer for future use.
}