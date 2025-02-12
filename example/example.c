

#include <stdio.h>
#include "linked_list.h"

int main(void)
{
    /* Create list */
    List_t* List = NewList(LL_DOUBLE);

    /* Add some data to the list. Void pointers are added as data. */
    float Pi = 3.14, Sqrt2 = 1.4142, Ln2 = 0.693;
    if ((AddToBack(List, &Pi) == LL_OK) &&
        (AddToFront(List, &Sqrt2) == LL_OK) &&
        (AddToBack(List, &Ln2) == LL_OK))
    {
        printf("\nSuccesfully added 3 nodes (1.4142, 3.14, 0.693) to the list!\n\n");
    }

    /* Iterate through the list and print data */
    printf("List content:\n");
    ListNode_t* Iter = GetHead(List);
    while(Iter)
    {
        float* DataPtr = (float*)GetData(Iter); /* Cast void pointer to the original type */
        printf("---> Node data: %f\n", (*DataPtr));

        Iter = GetNext(Iter);
    }
    printf("\n");

    /* Remove some nodes */
    if ((RemoveHead(List) == LL_OK) &&
        (RemoveTail(List) == LL_OK))
    {
        printf("Succesfully removed first and last node!\n");
    }

    /* List has one node => head and tail data should be the same */
    float* HeadData = (float*)GetData(GetHead(List));
    float* TailData = (float*)GetData(GetTail(List));
    printf("Head data: %f, tail data: %f. Head node should be the same as tail node.\n\n", *HeadData, *TailData);

    /* Add another node */
    float Half = 0.5;
    if (AddToFront(List, &Half) == LL_OK)
    {
        printf("Added 0.5 to list!\n\n");
    }

    /* Search for a node by data, then remove it */
    ListNode_t* PiNode = GetNodeByData(List, &Pi);
    if (RemoveNode(PiNode) == LL_OK)
    {
        printf("Succesfully removed 3.14 from the list!\n\n");
    }

    /* Iterate through the list again */
    printf("List content:\n");
    Iter = GetHead(List);
    while(Iter)
    {
        float* DataPtr = (float*)GetData(Iter);
        printf("---> Node data: %f\n", *DataPtr);

        Iter = GetNext(Iter);
    }
    printf("\n");

    /* Cleanup */
    if(DeleteList(List) == LL_OK)
    {
        printf("Cleanup successful!\n\n");
    }
}