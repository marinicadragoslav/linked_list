

#include <stdio.h>
#include "linked_list.h"

int main(void)
{
    /* Create list */
    List_t* List = LL_NewList(LL_DOUBLE);

    /* Add some data to the list. Void pointers are added as data. */
    float Pi = 3.14, Sqrt2 = 1.4142, Ln2 = 0.693;
    if ((LL_AddToBack(List, &Pi) == LL_OK) &&
        (LL_AddToFront(List, &Sqrt2) == LL_OK) &&
        (LL_AddToBack(List, &Ln2) == LL_OK))
    {
        printf("\nSuccesfully added 3 nodes (1.4142, 3.14, 0.693) to the list!\n\n");
    }

    /* Iterate through the list and print data */
    printf("List content:\n");
    ListNode_t* Iter = LL_GetHead(List);
    while(Iter)
    {
        float* DataPtr = (float*)LL_GetData(Iter); /* Cast void pointer to the original type */
        printf("---> Node data: %f\n", (*DataPtr));

        Iter = LL_GetNext(Iter);
    }
    printf("\n");

    /* Remove some nodes */
    if ((LL_RemoveHead(List) == LL_OK) &&
        (LL_RemoveTail(List) == LL_OK))
    {
        printf("Succesfully removed first and last node!\n");
    }

    /* List has one node => head and tail data should be the same */
    float* HeadData = (float*)LL_GetData(LL_GetHead(List));
    float* TailData = (float*)LL_GetData(LL_GetTail(List));
    printf("Head data: %f, tail data: %f. Head node should be the same as tail node.\n\n", *HeadData, *TailData);

    /* Add another node */
    float Half = 0.5;
    if (LL_AddToFront(List, &Half) == LL_OK)
    {
        printf("Added 0.5 to list!\n\n");
    }

    /* Search for a node by data, then remove it */
    ListNode_t* PiNode = LL_GetNodeByData(List, &Pi);
    if (LL_RemoveNode(PiNode) == LL_OK)
    {
        printf("Succesfully removed 3.14 from the list!\n\n");
    }

    /* Iterate through the list again */
    printf("List content:\n");
    Iter = LL_GetHead(List);
    while(Iter)
    {
        float* DataPtr = (float*)LL_GetData(Iter);
        printf("---> Node data: %f\n", *DataPtr);

        Iter = LL_GetNext(Iter);
    }
    printf("\n");

    /* Cleanup */
    if(LL_DeleteList(List) == LL_OK)
    {
        printf("Cleanup successful!\n\n");
    }
}
