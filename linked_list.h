/* 
    Create and manipulate singly and doubly linked lists. 
    Feel free to use the code in any way.

    Notes:
    - Internal structure of a list: List object -> Node objects -> Data
    - A node's data is actually a void pointer to an object managed by the user.
    - Nodes contain a reference to the list they belong to, so functions like RemoveNode(Node)
      do not need a List argument.
    - Memory for the inner structure of lists and nodes is dynamically allocated.
    - DeleteList(List) will free all the memory allocated internally for the list and its nodes.
*/

#ifndef LINKED_LIST_H
#define LINKED_LIST_H


/* Type of linkage for a list: single or double. */
typedef enum
{
    LL_SINGLE,
    LL_DOUBLE
}ListLinkage_t;

/* Type used to return the status of a function call. */
typedef enum
{
    LL_NOT_OK = -1,
    LL_OK = 0
}ListStatus_t;

/* Boolean type */
typedef enum
{
    LL_FALSE = 0,
    LL_TRUE = 1
}ListBool_t;


/* Lists contain references to nodes, and nodes contain references to lists,
   so one of them has to be declared before defining the other. */
typedef struct ListNode ListNode_t;


/* Type used for list objects. A list object contains references to its first and last node,
   the type of linkage (single or double) and the number of nodes. */
typedef struct
{
    ListNode_t* Head;
    ListNode_t* Tail;
    unsigned int Count;
    ListLinkage_t Linkage;
}List_t;


/* Type used for node objects. Nodes contain data (void pointers to objects managed by the user).
   A node also contains information about the list it belongs to, and the next & prev nodes. */
struct ListNode
{
    List_t* Owner;
    ListNode_t* Prev;
    ListNode_t* Next;
    void* Data;
};


/* Creates an empty list object with the given linkage and returns a pointer to it.
   Returns NULL if memory allocation fails or the parameter is invalid. */
List_t* NewList(ListLinkage_t Linkage);


/* Returns the head (first node) of a list, or NULL if the list is NULL or empty. */
ListNode_t* GetHead(List_t* List);


/* Returns the last node of a list, or NULL if the list is NULL or empty. */
ListNode_t* GetTail(List_t* List);


/* Returns the node that comes after a given node. Returns NULL if the argument is a tail node
   or NULL. */
ListNode_t* GetNext(ListNode_t* Node);


/* Returns the node that comes before a given node. Returns NULL if the argument is a head node
   or NULL. Returns NULL for any node that belongs to a singly linked list. */
ListNode_t* GetPrev(ListNode_t* Node);


/* Returns a void pointer to the data object that the node refers to. Returns NULL for a NULL node.
   The returned pointer must be cast back to the data object's type by the caller. */
void* GetData(ListNode_t* Node);


/* Returns the first node in the list that contains the given data. Returns NULL if any of the
   arguments is NULL, if list is empty or invalid, or the given data is not found in the list. */
ListNode_t* GetNodeByData(List_t* List, void* Data);


/* Sets the data contained by the given node to the given data. Returns LL_OK on success.
   Returns an error if any of the arguments is NULL, or if the owner list of the node argument
   is NULL, invalid, or empty. */
ListStatus_t SetData(ListNode_t* Node, void* Data);


/* Creates a new node containing the given data and inserts it to the front of the list. 
   Returns LL_OK on success. Returns an error if the list argument is NULL or invalid,
   or the data argument is NULL, or the memory allocation for the new node fails. */
ListStatus_t AddToFront(List_t* List, void* Data);


/* Creates a new node containing the given data and inserts it to the back of the list. 
   Returns LL_OK on success. Returns an error if the list argument is NULL or invalid,
   or the data argument is NULL, or the memory allocation for the new node fails. */
ListStatus_t AddToBack(List_t* List, void* Data);


/* Creates a new node containing the given data and inserts it after the given node.
   Returns LL_OK on success. Returns an error if any of the arguments is NULL,
   if the list referenced by the node argument is NULL, invalid or empty, or the memory
   allocation for the new node fails. */
ListStatus_t InsertAfterNode(ListNode_t* Node, void* Data);


/* Creates a new node containing the newData argument and inserts it after the first node that
   contains the existingData argument. Returns LL_OK on success. Returns an error if any
   of the arguments is NULL, if the list is invalid or empty, if existingData argument is not found
   in the list, or the memory allocation for the new node fails. */
ListStatus_t InsertAfterData(List_t* List, void* ExistingData, void* NewData);


/* Removes the head node from the given list. Returns LL_OK on success. Returns an error 
   if the list argument is NULL, invalid or empty. */
ListStatus_t RemoveHead(List_t* List);


/* Removes the tail node from the given list. Returns LL_OK on success. Returns an error
   if the list argument is NULL, invalid or empty. */
ListStatus_t RemoveTail(List_t* List);


/* Removes the given node from the list that it belongs to. Returns LL_OK on success.
   Returns an error if the node argument is NULL, if the list that the node belongs to is NULL,
   invalid or empty, or if the given node is not found. */
ListStatus_t RemoveNode(ListNode_t* Node);


/* Removes the first node from the given list that contains the given data. Returns LL_OK
   on success. Returns an error if the list argument is NULL, invalid or empty, if the data
   argument is NULL, or if the data is not found in the list. */
ListStatus_t RemoveNodeByData(List_t* List, void* Data);


/* Provides the number of nodes in the given list through the output parameter Count. 
   Returns LL_OK on success. Returns an error if any of the arguments is NULL. */
ListStatus_t GetCount(List_t* List, unsigned int* Count);


/* Deallocates the memory used internally for the list and all of its nodes. Returns LL_OK
   on success. Returns an error if the list argument is NULL. After calling this function, the list
   pointer should be reinitialized to NULL to avoid accessing memory that is not allocated. */
ListStatus_t DeleteList(List_t* List);

#endif /* LINKED_LIST_H */
