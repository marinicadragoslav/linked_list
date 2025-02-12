#include <stdlib.h>
#include "linked_list.h"

static ListBool_t _IsValid(List_t* List);
static ListBool_t _IsValidAndNotEmpty(List_t* List);
static ListNode_t* _FindNodeByData(List_t* List, void* Data);
static ListNode_t* _NewNode(void);
static void _DeleteHead(List_t* List);
static void _DeleteTail(List_t* List);
static ListNode_t* _FindPrevNode(ListNode_t* Node);
static ListNode_t* _FindPrevNodeByData(List_t* List, void* Data);
static void _InsertNewNodeAfterNode(ListNode_t* New, ListNode_t* Existing);
static void _DeleteInnerNode(ListNode_t* Node, ListNode_t* Prev);


List_t* NewList(ListLinkage_t Linkage)
{
    if ((Linkage != LL_SINGLE) && (Linkage != LL_DOUBLE))
    {
        return NULL;
    }

    List_t* List = malloc(sizeof(List_t));

    if (List)
    {
        List->Head = NULL;
        List->Tail = NULL;
        List->Count = 0;
        List->Linkage = Linkage;
    }

    return List;
}


ListNode_t* GetHead(List_t* List)
{
    return (List ? List->Head : NULL);
}


ListNode_t* GetTail(List_t* List)
{
    return (List ? List->Tail : NULL);
}


ListNode_t* GetNext(ListNode_t* Node)
{
    return (Node ? Node->Next : NULL);
}


ListNode_t* GetPrev(ListNode_t* Node)
{
    return ((Node && (Node->Owner->Linkage == LL_DOUBLE)) ? Node->Prev : NULL);
}


void* GetData(ListNode_t* Node)
{
    return (Node ? Node->Data : NULL);
}


ListNode_t* GetNodeByData(List_t* List, void* Data)
{
    if ( _IsValidAndNotEmpty(List) && Data)
    {
        return _FindNodeByData(List, Data);
    }

    return NULL;
}


ListStatus_t AddToFront(List_t* List, void* Data)
{
    if (_IsValid(List) && Data)
    {
        ListNode_t* Node = _NewNode();

        if (Node)
        {
            /* Fwd link to old head */
            Node->Next = List->Head;

            /* Bwd link from old head */
            if (List->Head && (List->Linkage == LL_DOUBLE))
            {
                List->Head->Prev = Node;
            }

            /* Update head */
            List->Head = Node;

            /* If the list was empty, update tail */
            if(List->Count == 0)
            {
                List->Tail = Node;
            }

            /* Update node data and owner list, update list count */
            Node->Data = Data;
            Node->Owner = List;
            List->Count++;

            return LL_OK;
        }
    }

    return LL_NOT_OK;
}


ListStatus_t AddToBack(List_t* List, void* Data) 
{
    if (_IsValid(List) && Data)
    {
        ListNode_t* Node = _NewNode();
        
        if (Node)
        {
            /* Bwd link new node to old tail */
            if (List->Linkage == LL_DOUBLE)
            {
                Node->Prev = List->Tail;
            }

            /* Fwd link from old tail */
            if (List->Tail)
            {
                List->Tail->Next = Node;
            }

            /* Update Tail */
            List->Tail = Node;

            /* If the list was empty, update the head */
            if(List->Count == 0)
            {
                List->Head = Node;
            }

            /* Update node data and owner list, update list count */
            Node->Data = Data;
            Node->Owner = List;
            List->Count++;

            return LL_OK;
        }
    }

    return LL_NOT_OK;
}


ListStatus_t InsertAfterNode(ListNode_t* Node, void* Data)
{
    if (Node && Data && _IsValidAndNotEmpty(Node->Owner))
    {
        ListNode_t* NewNode = _NewNode();
        if (NewNode)
        {
            _InsertNewNodeAfterNode(NewNode, Node);

            NewNode->Data = Data;
            NewNode->Owner = Node->Owner;
            NewNode->Owner->Count++;

            return LL_OK;
        }
    }

    return LL_NOT_OK;
}

ListStatus_t SetData(ListNode_t* Node, void* Data)
{
    if (Node && Data && _IsValidAndNotEmpty(Node->Owner))
    {
        Node->Data = Data;
        return LL_OK;
    }

    return LL_NOT_OK;
}

ListStatus_t InsertAfterData(List_t* List, void* ExistingData, void* NewData)
{    
    if (ExistingData && NewData && _IsValidAndNotEmpty(List))
    {
        ListNode_t* Node = _FindNodeByData(List, ExistingData);
        if (Node)
        {
            ListNode_t* NewNode = _NewNode();
            if (NewNode)
            {
                _InsertNewNodeAfterNode(NewNode, Node);

                NewNode->Data = NewData;
                NewNode->Owner = Node->Owner;
                List->Count++;

                return LL_OK;
            }
        }
    }

    return LL_NOT_OK;
}

ListStatus_t RemoveHead(List_t* List)
{
    if (_IsValidAndNotEmpty(List))
    {
        _DeleteHead(List);
        return LL_OK;
    }

    return LL_NOT_OK;
}

ListStatus_t RemoveTail(List_t* List)
{
    if (_IsValidAndNotEmpty(List))
    {
        _DeleteTail(List);
        return LL_OK;
    }

    return LL_NOT_OK;
}

ListStatus_t RemoveNode(ListNode_t* Node)
{
    if (Node && _IsValidAndNotEmpty(Node->Owner))
    {
        if(Node == Node->Owner->Head)
        {
            _DeleteHead(Node->Owner);
            return LL_OK;
        }
        else if(Node == Node->Owner->Tail)
        {
            _DeleteTail(Node->Owner);
            return LL_OK;
        }
        else
        {
            /* Get prev node */
            ListNode_t* Prev = (Node->Owner->Linkage == LL_DOUBLE ? Node->Prev : _FindPrevNode(Node));

            if (Prev)
            {
                /* Bypass node (link fwd) */
                Prev->Next = Node->Next;

                if (Node->Owner->Linkage == LL_DOUBLE)
                {
                    /* Bypass node (link bwd) */
                    Node->Next->Prev = Prev;
                }

                /* Remove node */
                Node->Owner->Count--;
                free(Node);

                return LL_OK;
            }
        }
    }

    return LL_NOT_OK;
}

ListStatus_t RemoveNodeByData(List_t* List, void* Data)
{
    if (Data && _IsValidAndNotEmpty(List))
    {
        if(List->Head->Data == Data)
        {
            _DeleteHead(List);
        }
        else
        {
            ListNode_t* Prev = _FindPrevNodeByData(List, Data);
            if(Prev)
            {
                ListNode_t* Node = Prev->Next;
                if (Node == List->Tail)
                {
                    _DeleteTail(List);
                }
                else
                {
                    _DeleteInnerNode(Node, Prev);
                }
            }
            else
            {
                return LL_NOT_OK;
            }
        }

        return LL_OK;
    }

    return LL_NOT_OK;
}

ListStatus_t GetCount(List_t* List, unsigned int* Count)
{
    if (List && Count)
    {
        *Count = List->Count;
        return LL_OK;
    }

    return LL_NOT_OK;
}

ListStatus_t DeleteList(List_t* List)
{
    if (List)
    {
        /* Remove any existing nodes first */
        while(RemoveHead(List) == LL_OK)
        {
            ;
        }

        free(List);
        return LL_OK;
    }

    return LL_NOT_OK;
}

static ListBool_t _IsValid(List_t* List)
{
    return (!List ? LL_FALSE : LL_TRUE);
}

static ListBool_t _IsValidAndNotEmpty(List_t* List)
{
    return ((!List) || (List->Count == 0) || (!List->Head) || (!List->Tail) ? LL_FALSE : LL_TRUE);
}

static ListNode_t* _FindNodeByData(List_t* List, void* Data)
{
    ListNode_t* Iter = List->Head;
    while(Iter)
    {
        if (Iter->Data == Data)
        {
            return Iter;
        }
        Iter = Iter->Next;
    }

    return NULL;
}

static ListNode_t* _NewNode(void)
{
    ListNode_t* Node = malloc(sizeof(ListNode_t));
    if (Node)
    {
        Node->Data = NULL;
        Node->Next = NULL;
        Node->Prev = NULL;
    }

    return Node;
}

/* Assumes List is valid and not-empty */
static void _DeleteHead(List_t* List)
{
    /* Save head */
    ListNode_t* OldHead = List->Head;

    /* Change head */
    List->Head = List->Head->Next;

    /* Update head (remove backward link) */
    if(List->Head)
    {
        if (List->Linkage == LL_DOUBLE)
        {
            List->Head->Prev = NULL;
        }
    }
    else
    {
        /* Change the tail too if the there was only one node in the list */
        List->Tail = NULL;
    }

    /* Remove old head */
    List->Count--;
    free(OldHead);
}

/* Assumes List is valid and not-empty */
static void _DeleteTail(List_t* List)
{
    /* Save tail */
    ListNode_t* Temp = List->Tail;

    /* Change tail */
    if (List->Linkage == LL_DOUBLE)
    {
        List->Tail = List->Tail->Prev;
    }
    else
    {
        List->Tail = _FindPrevNode(List->Tail);
    }

    /* Update tail (remove fwd link) */
    if(List->Tail)
    {
        List->Tail->Next = NULL;
    }
    else
    {
        /* Change the head too if the there was only one node in the list */
        List->Head = NULL;
    }

    /* Remove old tail */
    List->Count--;
    free(Temp);
}


static ListNode_t* _FindPrevNode(ListNode_t* Node)
{
    ListNode_t* Iter = Node->Owner->Head;
    while (Iter)
    {
        if (Iter->Next == Node)
        {
            return Iter;
        }
        Iter = Iter->Next;
    }

    return NULL;
}


static ListNode_t* _FindPrevNodeByData(List_t* List, void* Data)
{
    ListNode_t* Iter = List->Head;
    while(Iter && Iter->Next)
    {
        if (Iter->Next->Data == Data)
        {
            return Iter;
        }
        Iter = Iter->Next;
    }

    return NULL;
}


static void _InsertNewNodeAfterNode(ListNode_t* New, ListNode_t* Existing)
{
    /* Fwd links */
    New->Next = Existing->Next;
    Existing->Next = New;

    /* Bwd links */
    if (Existing->Owner->Linkage == LL_DOUBLE)
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

static void _DeleteInnerNode(ListNode_t* Node, ListNode_t* Prev)
{
    /* Bypass node (link fwd) */
    Prev->Next = Node->Next;

    if (Node->Owner->Linkage == LL_DOUBLE)
    {
        /* Bypass node (link backwards) */
        Node->Next->Prev = Prev;
    }

    /* Remove node */
    Node->Owner->Count--;
    free(Node);
}
