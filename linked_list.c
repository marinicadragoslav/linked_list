#include <stdlib.h>
#include "linked_list.h"

#define IS_NULL(Ptr)                (Ptr == NULL ? LL_TRUE : LL_FALSE)
#define IS_EMPTY(List)              (List->Count == 0 ? LL_TRUE : LL_FALSE)
#define IS_INVALID_OR_EMPTY(List)   (IS_NULL(List) || IS_EMPTY(List) || IS_NULL(List->Head) || IS_NULL(List->Tail))
#define RETURN_LL_NOT_OK_IF(Cond)   do { if(Cond) return LL_NOT_OK; } while(0)
#define RETURN_NULL_IF(Cond)        do { if(Cond) return NULL; } while(0)


static ListNode_t* Static_GetNodeByData(List_t* List, void* Data)
{
    ListNode_t* Iter = List->Head;

    while(Iter && (Iter->Data != Data))
    {
        Iter = Iter->Next;
    }

    return Iter;
}

static ListNode_t* Static_NewNode(void)
{
    ListNode_t* Node = malloc(sizeof(ListNode_t));

    if(Node)
    {
        Node->Data = Node->Next = Node->Prev = NULL;
    }

    return Node;
}

static void Static_RemoveHead(List_t* List)
{
    /* Remove head */
    ListNode_t* OldHead = List->Head;
    List->Head = List->Head->Next;
    List->Count--;
    free(OldHead);

    if(IS_NULL(List->Head))
    {
        /* Remove the tail link too if the list is now empty */
        List->Tail = NULL;
    }
    else if(List->Linkage == LL_DOUBLE)
    {
        /* Remove backward link of new head */
        List->Head->Prev = NULL;
    }
}

static ListNode_t* Static_GetPrevNode(ListNode_t* Node)
{
    ListNode_t* Iter = Node->Owner->Head;

    while(Iter && (Iter->Next != Node))
    {
        Iter = Iter->Next;
    }

    return Iter;
}

static void Static_RemoveTail(List_t* List)
{
    /* Remove tail */
    ListNode_t* OldTail = List->Tail;
    List->Tail = (List->Linkage == LL_DOUBLE ? List->Tail->Prev : Static_GetPrevNode(List->Tail));
    List->Count--;
    free(OldTail);
   
    if(List->Tail)
    {
        /* Remove forward link */
        List->Tail->Next = NULL;
    }
    else
    {
        /* Change the head too if the there was only one node in the list */
        List->Head = NULL;
    }
}

static ListNode_t* Static_GetPrevNodeByData(List_t* List, void* Data)
{
    ListNode_t* Iter = List->Head;

    while(Iter && Iter->Next)
    {
        if(Iter->Next->Data == Data)
        {
            return Iter;
        }
        Iter = Iter->Next;
    }

    return NULL;
}


static void Static_InsertNewNodeAfterNode(ListNode_t* New, ListNode_t* Existing)
{
    /* Fwd links */
    New->Next = Existing->Next;
    Existing->Next = New;

    /* Bwd links */
    if(Existing->Owner->Linkage == LL_DOUBLE)
    {
        New->Prev = Existing;
        if(New->Next)
        {
            New->Next->Prev = New;
        }
    }

    /* Update list tail if applicable */
    if(Existing->Owner->Tail == Existing)
    {
        Existing->Owner->Tail = New;
    }
}

static void Static_RemoveInnerNode(ListNode_t* Node, ListNode_t* Prev)
{
    /* Bypass node (link fwd) */
    Prev->Next = Node->Next;

    if(Node->Owner->Linkage == LL_DOUBLE)
    {
        /* Bypass node (link backwards) */
        Node->Next->Prev = Prev;
    }

    /* Remove node */
    Node->Owner->Count--;
    free(Node);
}

List_t* LL_NewList(ListLinkage_t Linkage)
{
    List_t* List = NULL;

    if((Linkage == LL_SINGLE || Linkage == LL_DOUBLE) && (List = malloc(sizeof(List_t))))
    {
        List->Head = NULL;
        List->Tail = NULL;
        List->Count = 0;
        List->Linkage = Linkage;
    }

    return List;
}


ListNode_t* LL_GetHead(List_t* List)
{
    return (List ? List->Head : NULL);
}


ListNode_t* LL_GetTail(List_t* List)
{
    return (List ? List->Tail : NULL);
}


ListNode_t* LL_GetNext(ListNode_t* Node)
{
    return (Node ? Node->Next : NULL);
}


ListNode_t* LL_GetPrev(ListNode_t* Node)
{
    return ((Node && (Node->Owner->Linkage == LL_DOUBLE)) ? Node->Prev : NULL);
}


void* LL_GetData(ListNode_t* Node)
{
    return (Node ? Node->Data : NULL);
}


ListNode_t* LL_GetNodeByData(List_t* List, void* Data)
{
    RETURN_NULL_IF(IS_NULL(Data) || IS_INVALID_OR_EMPTY(List)); 
    return Static_GetNodeByData(List, Data);
}

ListStatus_t LL_AddToFront(List_t* List, void* Data)
{
    RETURN_LL_NOT_OK_IF(IS_NULL(List) || IS_NULL(Data));

    ListNode_t* Node = Static_NewNode();
    RETURN_LL_NOT_OK_IF(IS_NULL(Node));

    /* Fwd link to old head */
    Node->Next = List->Head;

    if(List->Head && (List->Linkage == LL_DOUBLE))
    {
        /* Bwd link from old head */
        List->Head->Prev = Node;
    }

    /* Update head */
    List->Head = Node;

    /* If the list was empty, update tail */
    if(IS_EMPTY(List))
    {
        List->Tail = Node;
    }

    /* Update node data, owner list, count */
    Node->Data = Data;
    Node->Owner = List;
    List->Count++;

    return LL_OK;
}


ListStatus_t LL_AddToBack(List_t* List, void* Data)
{
    RETURN_LL_NOT_OK_IF(IS_NULL(List) || IS_NULL(Data));

    ListNode_t* Node = Static_NewNode();
    RETURN_LL_NOT_OK_IF(IS_NULL(Node));

    /* Bwd link new node to old tail */
    if(List->Linkage == LL_DOUBLE)
    {
        Node->Prev = List->Tail;
    }

    /* Fwd link from old tail */
    if(List->Tail)
    {
        List->Tail->Next = Node;
    }

    /* Update Tail */
    List->Tail = Node;

    /* If the list was empty, update the head */
    if(IS_EMPTY(List))
    {
        List->Head = Node;
    }

    /* Update node data, owner list, count */
    Node->Data = Data;
    Node->Owner = List;
    List->Count++;

    return LL_OK;
}


ListStatus_t LL_InsertAfterNode(ListNode_t* Node, void* Data)
{
    RETURN_LL_NOT_OK_IF(IS_NULL(Node) || IS_NULL(Data) || IS_INVALID_OR_EMPTY(Node->Owner));
   
    ListNode_t* NewNode = Static_NewNode();
    RETURN_LL_NOT_OK_IF(IS_NULL(NewNode));

    Static_InsertNewNodeAfterNode(NewNode, Node);
    NewNode->Data = Data;
    NewNode->Owner = Node->Owner;
    NewNode->Owner->Count++;

    return LL_OK;
}

ListStatus_t LL_SetData(ListNode_t* Node, void* Data)
{
    RETURN_LL_NOT_OK_IF(IS_NULL(Node) || IS_NULL(Data) || IS_INVALID_OR_EMPTY(Node->Owner));

    Node->Data = Data;
    return LL_OK;
}

ListStatus_t LL_InsertAfterData(List_t* List, void* ExistingData, void* NewData)
{   
    RETURN_LL_NOT_OK_IF(IS_NULL(ExistingData) || IS_NULL(NewData) || IS_INVALID_OR_EMPTY(List));
   
    ListNode_t* Node = Static_GetNodeByData(List, ExistingData);
    RETURN_LL_NOT_OK_IF(IS_NULL(Node));

    ListNode_t* NewNode = Static_NewNode();
    RETURN_LL_NOT_OK_IF(IS_NULL(NewNode));

    Static_InsertNewNodeAfterNode(NewNode, Node);

    NewNode->Data = NewData;
    NewNode->Owner = Node->Owner;
    List->Count++;

    return LL_OK;
}

ListStatus_t LL_RemoveHead(List_t* List)
{
    RETURN_LL_NOT_OK_IF(IS_INVALID_OR_EMPTY(List));
    Static_RemoveHead(List);

    return LL_OK;
}

ListStatus_t LL_RemoveTail(List_t* List)
{
    RETURN_LL_NOT_OK_IF(IS_INVALID_OR_EMPTY(List));
    Static_RemoveTail(List);
        
    return LL_OK;
}

ListStatus_t LL_RemoveNode(ListNode_t* Node)
{
    RETURN_LL_NOT_OK_IF(IS_NULL(Node) || IS_INVALID_OR_EMPTY(Node->Owner));

    if(Node == Node->Owner->Head)
    {
        Static_RemoveHead(Node->Owner);
    }
    else if(Node == Node->Owner->Tail)
    {
        Static_RemoveTail(Node->Owner);
    }
    else
    {
        /* Get prev node */
        ListNode_t* Prev = (Node->Owner->Linkage == LL_DOUBLE ? Node->Prev : Static_GetPrevNode(Node));
        RETURN_LL_NOT_OK_IF(IS_NULL(Prev));

        Static_RemoveInnerNode(Node, Prev);
    }
    
    return LL_OK;
}

ListStatus_t LL_RemoveNodeByData(List_t* List, void* Data)
{
    RETURN_LL_NOT_OK_IF(IS_NULL(Data) || IS_INVALID_OR_EMPTY(List));
    
    if(List->Head->Data == Data)
    {
        Static_RemoveHead(List);
    }
    else
    {
        ListNode_t* Prev = Static_GetPrevNodeByData(List, Data);
        RETURN_LL_NOT_OK_IF(IS_NULL(Prev));

        ListNode_t* Node = Prev->Next;
        if(Node == List->Tail)
        {
            Static_RemoveTail(List);
        }
        else
        {
            Static_RemoveInnerNode(Node, Prev);
        }
    }

    return LL_OK;
}

ListStatus_t LL_GetCount(List_t* List, unsigned int* Count)
{
    RETURN_LL_NOT_OK_IF(IS_NULL(List) || IS_NULL(Count));
    *Count = List->Count;

    return LL_OK;
}

ListStatus_t LL_DeleteList(List_t* List)
{
    RETURN_LL_NOT_OK_IF(IS_NULL(List));

    /* Remove any existing nodes first, then free list */
    do {} while(LL_RemoveHead(List) == LL_OK);
    free(List);

    return LL_OK;
}
