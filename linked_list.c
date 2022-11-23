#include "linked_list.h"
#include <stdio.h>

/* MEM_TEST should be 0 by default. Only set it to 1 when testing the API. */
#if (MEM_TEST)
    #include "mem_test.h"
    #define malloc  mtMalloc
    #define free    mtFree
#else
    #include <stdlib.h>
#endif

static ListError_t ValidateList(List_t* list);
static ListNode_t* NewNode(void);
static void DeleteHead(List_t* list);
static void DeleteTail(List_t* list);
static ListNode_t* FindNodeByData(List_t* list, void* data);
static ListNode_t* FindPrevNodeByData(List_t* list, void* data);
static ListNode_t* FindPrevNode(ListNode_t* node);
static void InsertNewNodeAfterNode(ListNode_t* new, ListNode_t* existing);
static void DeleteInnerNode(ListNode_t* node, ListNode_t* prev);


List_t* NewList(Linkage_t linkage)
{
    if (linkage != LST_L_DOUBLE)
    {
        linkage = LST_L_SINGLE;
    }

    List_t* list = malloc(sizeof(List_t));
    if(!list)
    {
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    list->numNodes = 0;
    list->linkage = linkage;

    return list;
}

ListNode_t* GetHeadNode(List_t* list)
{
    return (list ? list->head : NULL);
}

ListNode_t* GetTailNode(List_t* list)
{
    return (list ? list->tail : NULL);
}

ListNode_t* GetNextNode(ListNode_t* node)
{
    return (node ? node->next : NULL);
}

ListNode_t* GetPrevNode(ListNode_t* node)
{
    return ((node && (node->ownerList->linkage == LST_L_DOUBLE)) ? node->prev : NULL);
}

void* GetNodeData(ListNode_t* node)
{
    return (node ? node->data : NULL);
}

ListNode_t* GetNodeByData(List_t* list, void* data)
{
    if ((ValidateList(list) != LST_E_SUCCESS) || (data == NULL)) 
    {
        return NULL;
    }

    return FindNodeByData(list, data);
}

ListError_t InsertToFront(List_t* list, void* data)
{
    if (ValidateList(list) == LST_E_INVALID_LIST)
    {
        return LST_E_INVALID_LIST;
    }

    if (data == NULL)
    {
        return LST_E_INVALID_DATA;
    }

    ListNode_t* node = NewNode();
    if(!node)
    {
        return LST_E_MEM_ALLOC;
    }

    node->next = list->head;
    if (list->head)
    {
        list->head->prev = (list->linkage == LST_L_DOUBLE ? node : NULL);
    }

    list->head = node;
    if(list->numNodes == 0)
    {        
        list->tail = node;
    }

    node->data = data;
    node->ownerList = list;

    list->numNodes++;
    return LST_E_SUCCESS;
}

ListError_t InsertToBack(List_t* list, void* data)
{
    if (ValidateList(list) == LST_E_INVALID_LIST)
    {
        return LST_E_INVALID_LIST;
    }

    if (data == NULL)
    {
        return LST_E_INVALID_DATA;
    }

    ListNode_t* node = NewNode();
    if(!node)
    {
        return LST_E_MEM_ALLOC;
    } 

    node->prev = (list->linkage == LST_L_DOUBLE ? list->tail : NULL);
    if (list->tail)
    {
        list->tail->next = node;
    }

    list->tail = node;
    if(list->numNodes == 0)
    {
        list->head = node;
    }

    node->data = data;
    node->ownerList = list;

    list->numNodes++;
    return LST_E_SUCCESS;
}

ListError_t InsertAfterNode(ListNode_t* node, void* data)
{
    if(node == NULL)
    {
        return LST_E_INVALID_NODE;
    }

    if(data == NULL)
    {
        return LST_E_INVALID_DATA;
    }
    
    ListError_t err = ValidateList(node->ownerList);
    if (err != LST_E_SUCCESS)
    {
        return err;
    }

    ListNode_t* newNode = NewNode();
    if(newNode == NULL)
    {
        return LST_E_MEM_ALLOC;
    }
    
    InsertNewNodeAfterNode(newNode, node);

    newNode->data = data;
    newNode->ownerList = node->ownerList;

    newNode->ownerList->numNodes++;
    return LST_E_SUCCESS;
}

ListError_t SetNodeData(ListNode_t* node, void* data)
{
    if(node == NULL)
    {
        return LST_E_INVALID_NODE;
    }

    if(data == NULL)
    {
        return LST_E_INVALID_DATA;
    }

    ListError_t err = ValidateList(node->ownerList);
    if (err != LST_E_SUCCESS)
    {
        return err;
    }

    node->data = data;
    return LST_E_SUCCESS;
}

ListError_t InsertAfterData(List_t* list, void* existingData, void* newData)
{    
    ListError_t err = ValidateList(list);
    if (err != LST_E_SUCCESS)
    {
        return err;
    }

    if(existingData == NULL || newData == NULL)
    {
        return LST_E_INVALID_DATA;
    }

    ListNode_t* node = FindNodeByData(list, existingData);
    if(node == NULL)
    {
        return LST_E_NOT_FOUND;
    }
    
    ListNode_t* newNode = NewNode();
    if(newNode == NULL)
    {
        return LST_E_MEM_ALLOC;
    }

    InsertNewNodeAfterNode(newNode, node);

    newNode->data = newData;
    newNode->ownerList = node->ownerList;

    list->numNodes++;
    return LST_E_SUCCESS;
}

ListError_t RemoveFirstNode(List_t* list)
{
    ListError_t err = ValidateList(list);
    if (err != LST_E_SUCCESS)
    {
        return err;
    }

    // List is valid, not-empty, has a valid head
    DeleteHead(list);
    return LST_E_SUCCESS;
}

ListError_t RemoveLastNode(List_t* list)
{
    ListError_t err = ValidateList(list);
    if (err != LST_E_SUCCESS)
    {
        return err;
    }

    // List is valid, not-empty, has a valid tail
    DeleteTail(list);
    return LST_E_SUCCESS;
}

ListError_t RemoveNode(ListNode_t* node)
{
    if(node == NULL)
    {
        return LST_E_INVALID_NODE;
    }

    ListError_t err = ValidateList(node->ownerList);
    if (err != LST_E_SUCCESS)
    {
        return err;
    }

    if(node == node->ownerList->head)
    {
        DeleteHead(node->ownerList);
        return LST_E_SUCCESS;
    }
    else if(node == node->ownerList->tail)
    {
        DeleteTail(node->ownerList);
        return LST_E_SUCCESS;
    }
    else
    {
        // Get previous node
        ListNode_t* prev;
        if (node->ownerList->linkage == LST_L_DOUBLE)
        {
            prev = node->prev;
        }
        else
        {
            prev = FindPrevNode(node);
        }

        if (!prev)
        {
            return LST_E_NOT_FOUND;
        }

        // Bypass node (link forward)
        prev->next = node->next;

        if (node->ownerList->linkage == LST_L_DOUBLE)
        {
            // Bypass node (link backwards)
            node->next->prev = prev;
        }

        // Remove node
        node->ownerList->numNodes--;
        free(node);

        return LST_E_SUCCESS;
    }
}

ListError_t RemoveNodeByData(List_t* list, void* data)
{
    ListError_t err = ValidateList(list);
    if (err != LST_E_SUCCESS)
    {
        return err;
    }

    if(data == NULL)
    {
        return LST_E_INVALID_DATA;
    }

    // List is valid, not-empty, has valid head and tail. Look in the head first.
    if(list->head->data == data)
    {
        DeleteHead(list);
        return LST_E_SUCCESS;
    }
    else
    {
        ListNode_t* prev = FindPrevNodeByData(list, data);

        if (!prev)
        {
            // No prev node found => data is not in the list.
            return LST_E_NOT_FOUND;
        }

        ListNode_t* node = prev->next;
        if (node == list->tail)
        {
            DeleteTail(list);
        }
        else
        {
            DeleteInnerNode(node, prev);
        }

        return LST_E_SUCCESS;
    }
}

ListError_t GetNodeCount(List_t* list, unsigned int* countOut)
{
    if (list == NULL)
    {
        return LST_E_INVALID_LIST;
    }

    if (countOut == NULL)
    {
        return LST_E_INVALID_COUNT;
    }

    *countOut = list->numNodes;
    return LST_E_SUCCESS;
}

ListError_t DeleteList(List_t* list)
{
    if (list == NULL)
    {
        return LST_E_INVALID_LIST;
    }

    // Remove any existing nodes first
    while(RemoveFirstNode(list) == LST_E_SUCCESS)
    {}

    free(list);
    return LST_E_SUCCESS;
}

static ListError_t ValidateList(List_t* list)
{
    if (list == NULL)
    {
        return LST_E_INVALID_LIST;
    }
    else if (list->numNodes == 0)
    {
        return LST_E_EMPTY_LIST;
    }
    else if (!list->head || !list->tail)
    {
        return LST_E_INVALID_LIST;
    }
   
    return LST_E_SUCCESS;
}

static ListNode_t* NewNode(void)
{
    ListNode_t* node = malloc(sizeof(ListNode_t));
    if(node == NULL)
    {
        return NULL;
    }

    node->data = NULL;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

static void DeleteHead(List_t* list)
{    
    // Save head
    ListNode_t* temp = list->head;

    // Change head
    list->head = list->head->next;

    // Update head (remove backward link)
    if(list->head)
    {
        list->head->prev = NULL;
    }
    else
    {
        // Change the tail too if the there was only one node in the list
        list->tail = NULL;
    }

    // Remove old head
    list->numNodes--;
    free(temp);
}

static void DeleteTail(List_t* list)
{    
    // Save tail
    ListNode_t* temp = list->tail;

    // Change tail
    if (list->linkage == LST_L_DOUBLE)
    {
        list->tail = list->tail->prev;
    }
    else
    {
        list->tail = FindPrevNode(list->tail);
    }

    // Update tail (remove forward link)
    if(list->tail)
    {
        list->tail->next = NULL;
    }
    else
    {
        // Change the head too if the there was only one node in the list
        list->head = NULL;
    }

    // Remove old tail
    list->numNodes--;
    free(temp);
}

static ListNode_t* FindNodeByData(List_t* list, void* data)
{
    ListNode_t* iter = list->head;
    while(iter)
    {
        if (iter->data == data)
        {
            return iter;
        }
        iter = iter->next;
    }

    return NULL;
}

static ListNode_t* FindPrevNodeByData(List_t* list, void* data)
{
    ListNode_t* iter = list->head;
    while(iter && iter->next)
    {
        if (iter->next->data == data)
        {
            return iter;
        }
        iter = iter->next;
    }

    return NULL;
}

static ListNode_t* FindPrevNode(ListNode_t* node)
{
    ListNode_t* iter = node->ownerList->head;
    while (iter)
    {
        if (iter->next == node)
        {
            return iter;
        }
        iter = iter->next;
    }

    return NULL;
}

static void InsertNewNodeAfterNode(ListNode_t* new, ListNode_t* existing)
{
    // Fwd links
    new->next = existing->next;
    existing->next = new;

    // Bckwd links
    if (existing->ownerList->linkage == LST_L_DOUBLE)
    {
        new->prev = existing;
        if(new->next)
        {
            new->next->prev = new;
        }
    }

    // Update list tail if applicable
    if(existing->ownerList->tail == existing)
    {
        existing->ownerList->tail = new;
    }
}

static void DeleteInnerNode(ListNode_t* node, ListNode_t* prev)
{
    // Bypass node (link forward)
    prev->next = node->next;

    if (node->ownerList->linkage == LST_L_DOUBLE)
    {
        // Bypass node (link backwards)
        node->next->prev = prev;
    }

    // Remove node
    node->ownerList->numNodes--;
    free(node);
}
