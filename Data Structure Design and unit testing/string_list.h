/* string_list.h
  
   SENG 265 - Summer 2016
   Assignment 2
  
   Declarations for a linked list containing C strings.
  
   B. Bird - 05/26/2016
*/

typedef struct StringListNode{
	char* element;
	struct StringListNode* next;
	struct StringListNode* previous;
} StringListNode;

typedef struct{
	
	StringListNode* head;
	StringListNode* tail;
	
} StringList;


/* StringList_init(L)
   Initialize the provided StringList instance.
   
   Pre-conditions:
     L is a pointer to a StringList struct.
   Post-conditions:
     The head and tail pointers of L have been
     initialized to NULL.
*/ 
void StringList_Init(StringList* L);



/* StringList_destroy(L)
   Free all storage associated with each node of the StringList
   object and reset all pointers to NULL.
   
   Pre-conditions:
     L is a pointer to a StringList struct which has been
	 initialized with StringList_init and has not already
	 been destroyed.
   Post-conditions:
     Each node of L, and its associated string, has been freed.
	 The head and tail pointers of L have been set to NULL.
*/ 
void StringList_Destroy(StringList* L);


/* StringList_size(L)
   Return the number of nodes in the list L
   
   Pre-conditions:
     L is a pointer to an initialized StringList struct.
   Return value:
     The number of nodes in L.
*/ 
int StringList_Size(StringList* L);


/* StringList_AddFront(L, str)
   Create a new list node containing a copy of the string str and
   add the node to the front (head) of the list L.
   
   Note: The created node must not contain a pointer to the string
   str. Instead, it should contain a pointer to a new array containing
   a copy of str.
   
   Pre-conditions:
     L is a pointer to an initialized StringList struct.
	 str points to a valid null terminated C string.
   Post-conditions:
     A new node containing a copy of str has been attached
	 to the beginning of the list L.
   Return value:
     A pointer to the created node.
*/ 
StringListNode* StringList_AddFront(StringList* L, char* str);


/* StringList_AddBack(L, str)
   Create a new list node containing a copy of the string str and
   add the node to the back (tail) of the list L.
   
   Note: The created node must not contain a pointer to the string
   str. Instead, it should contain a pointer to a new array containing
   a copy of str.
   
   Pre-conditions:
     L is a pointer to an initialized StringList struct.
	 str points to a valid null terminated C string.
   Post-conditions:
     A new node containing a copy of str has been attached
	 to the end of the list L.
   Return value:
     A pointer to the created node.
*/ 
StringListNode* StringList_AddBack(StringList* L, char* str);

/* StringList_remove(L, str)
   Search the list L for a node containing the provided string str
   and remove the first such node if it is present.
   
   Note that the removal process should free all storage associated
   with the node and the string it contains.
   
   Pre-conditions:
     L is a pointer to an initialized StringList struct.
	 str points to a valid null terminated C string.
   Post-conditions:
     If a node containing str was found, it has been removed
	 from the list L.
   Return value:
     1 if a node containing str was found.
	 0 otherwise.
*/ 
int StringList_Remove(StringList* L, char* str);

/* StringList_RemoveNode(L, node)
   Remove the provided node from the list L.
   
   Note that the removal process should free all storage associated
   with the node and the string it contains.
   
   Pre-conditions:
     L is a pointer to an initialized StringList struct.
	 node points to a valid node in L.
   Post-conditions:
     node has been removed from L.
*/ 
void StringList_RemoveNode(StringList* L, StringListNode* node);


/* StringList_InList(L, str)
   Search the list L for a node containing the provided string str
   and return a pointer to the first node containing str if it is found.
   
   The "first" node, in this context, refers to the first node encountered
   on a forward traversal of the list starting at the head.
   
   Pre-conditions:
     L is a pointer to an initialized StringList struct.
	 str points to a valid null terminated C string.
   Return value:
     A pointer to the first node containing str if it was found in the list.
	 NULL otherwise.
*/ 
StringListNode* StringList_InList(StringList* L, char* str);


/* StringList_GetIndex(L, i)
   Return the node at the provided index i (where index 0 is 
   the node at the head of the list).
   
   
   Pre-conditions:
     L is a pointer to an initialized StringList struct.
   Return value:
     A pointer to node i if 0 <= i <= size - 1 (where size is the
	 number of elements in the list.
	 NULL if i is not a valid index into the list.
*/ 
StringListNode* StringList_GetIndex(StringList* L, int i);

