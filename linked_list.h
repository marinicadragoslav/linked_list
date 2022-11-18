/* 
    Create and manipulate singly and doubly linked lists. 
    Feel free to use the code in any way.

    Notes:
    - Internal structure of a list: List object -> Node objects -> Data
    - A node's data is actually a void pointer to an object managed by the user.
    - Nodes contain a reference to the list they belong to, so functions like
      RemoveNode(node) do not need a list argument.
    - Memory for the inner structure of lists and nodes is dynamically allocated.
      Calling DeleteList(list) will free all the memory allocated internally 
      for the list and its nodes.
*/

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#define MEM_MANAGEMENT_TEST 0 /* Default value: 0. Set to 1 only when testing the API. */


/* Type of linkage for a list: single or double. */
typedef enum
{
    LST_L_SINGLE,
    LST_L_DOUBLE
}Linkage_t;


/* Lists contain references to nodes, and nodes contain references to lists,
   so one of them has to be declared before defining the other. */
typedef struct ListNode ListNode_t;


/* Type used for list objects. A list object contains references to its first and last node,
   the type of linkage (single or double) and the number of nodes. */
typedef struct
{
    ListNode_t* head;
    ListNode_t* tail;
    Linkage_t linkage;
    unsigned int numNodes;
}List_t;


/* Type used for node objects. Nodes contain data (void pointers to objects managed by the user).
   A node also contains information about the list it belongs to, and the next & prev nodes. */
struct ListNode
{
    List_t* ownerList;
    ListNode_t* prev;
    ListNode_t* next;
    void* data;
};


/* Type used to return the status of a function call. */
typedef enum
{
    LST_E_INVALID_LIST = -99,
    LST_E_INVALID_NODE,
    LST_E_INVALID_DATA,
    LST_E_INVALID_COUNT,
    LST_E_EMPTY_LIST,
    LST_E_NOT_FOUND,
    LST_E_MEM_ALLOC,
    LST_E_SUCCESS = 0
}ListError_t;


/* Creates an empty list object with the given linkage and returns a pointer to it.
   Returns NULL if memory allocation fails. */
List_t* NewList(Linkage_t linkage);


/* Returns the first node of a list, or NULL if the list is NULL or empty. */
ListNode_t* GetHeadNode(List_t* list);


/* Returns the last node of a list, or NULL if the list is NULL or empty. */
ListNode_t* GetTailNode(List_t* list);


/* Returns the node that comes after a given node. Returns NULL if the argument is a tail node
   or NULL. */
ListNode_t* GetNextNode(ListNode_t* node);


/* Returns the node that comes before a given node. Returns NULL if the argument is a head node
   or NULL. Returns NULL for any node that belongs to a singly linked list. */
ListNode_t* GetPrevNode(ListNode_t* node);


/* Returns a void pointer to the data object that the node refers to. Returns NULL for a NULL node.
   The returned pointer must be cast back to the data object's type by the caller. */
void* GetNodeData(ListNode_t* node);


/* Returns the first node in the list that contains the given data. Returns NULL if any of the
   arguments is NULL, if list is empty or invalid, or the given data is not found in the list. */
ListNode_t* GetNodeByData(List_t* list, void* data);


/* Creates a new node containing the given data and inserts it to the front of the list. 
   Returns LST_E_SUCCESS on success. Returns an error if the list argument is NULL or invalid,
   or the data argument is NULL, or the memory allocation for the new node fails. */
ListError_t InsertToFront(List_t* list, void* data);


/* Creates a new node containing the given data and inserts it to the back of the list. 
   Returns LST_E_SUCCESS on success. Returns an error if the list argument is NULL or invalid,
   or the data argument is NULL, or the memory allocation for the new node fails. */
ListError_t InsertToBack(List_t* list, void* data);


/* Creates a new node containing the given data and inserts it after the given node.
   Returns LST_E_SUCCESS on success. Returns an error if any of the arguments is NULL,
   if the list referenced by the node argument is NULL, invalid or empty, or the memory
   allocation for the new node fails. */
ListError_t InsertAfterNode(ListNode_t* node, void* data);


/* Creates a new node containing the newData argument and inserts it after the first node that
   contains the existingData argument. Returns LST_E_SUCCESS on success. Returns an error if any
   of the arguments is NULL, if the list is invalid or empty, if existingData argument is not found
   in the list, or the memory allocation for the new node fails. */
ListError_t InsertAfterData(List_t* list, void* existingData, void* newData);


/* Removes the head node from the given list. Returns LST_E_SUCCESS on success. Returns an error 
   if the list argument is NULL, invalid or empty. */
ListError_t RemoveFirstNode(List_t* list);


/* Removes the tail node from the given list. Returns LST_E_SUCCESS on success. Returns an error
   if the list argument is NULL, invalid or empty. */
ListError_t RemoveLastNode(List_t* list);


/* Removes the given node from the list that it belongs to. Returns LST_E_SUCCESS on success.
   Returns an error if the node argument is NULL, if the list that the node belongs to is NULL,
   invalid or empty, or if the given node is not found. */
ListError_t RemoveNode(ListNode_t* node);


/* Removes the first node from the given list that contains the given data. Returns LST_E_SUCCESS
   on success. Returns an error if the list argument is NULL, invalid or empty, if the data
   argument is NULL, or if the data is not found in the list. */
ListError_t RemoveNodeByData(List_t* list, void* data);


/* Provides the number of nodes in the given list through the output parameter countOut. 
   Returns LST_E_SUCCESS on success. Returns an error if any of the arguments is NULL. */
ListError_t GetNodeCount(List_t* list, unsigned int* countOut);


/* Deallocates the memory used internally for the list and all of its nodes. Returns LST_E_SUCCESS
   on success. Returns an error if the list argument is NULL. After calling this function, the list
   pointer should be reinitialized to NULL to avoid accessing memory that is not allocated. */
ListError_t DeleteList(List_t* list);

#endif /* LINKED_LIST_H */