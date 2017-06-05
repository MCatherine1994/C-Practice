#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void StringList_Init(StringList* L){
	L->head = NULL;
	L->tail = NULL;
}

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
void StringList_Destroy(StringList* L){
	if(L->head == NULL){
		return;
	}
	while(L->head != NULL){
		if(L->head->next == NULL){
			StringListNode* temp = L->head;
			L->head = NULL;
			L->tail = NULL;
			temp->next = NULL;
			temp->previous = NULL;
			temp->element = NULL;
		}else{
			StringListNode* temp = L->head;
			L->head = L->head->next;
			L->head->previous = NULL;
			temp->next = NULL;
			temp->previous = NULL;
			temp->element = NULL;
		}		
	}
}

/* StringList_size(L)
   Return the number of nodes in the list L
   
   Pre-conditions:
     L is a pointer to an initialized StringList struct.
   Return value:
     The number of nodes in L.
*/ 
int StringList_Size(StringList* L){
	if(L->head == NULL && L->tail == NULL){
		return 0;
	}
	int size = 0;
	StringListNode* current = L->head;
	while(current != NULL){
		size++;
		current = current->next;
	}
	return size;
}

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
StringListNode* StringList_AddFront(StringList* L, char* str){
	StringListNode* new_node = (StringListNode* )malloc( sizeof(StringListNode) );
	char* new_str = (char*)malloc( sizeof(char)*(strlen(str)+1) );
	strcpy(new_str, str);
	if (L->head == NULL){
		new_node->element = new_str;
		new_node->previous = NULL;
		new_node->next = NULL;
		L->head = new_node;		
		L->tail = new_node;
	}else{
		new_node->element = new_str;		
		new_node->previous = NULL;
		new_node->next = L->head;
		L->head->previous = new_node;
		L->head = new_node;		
	}
	return new_node;
}


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
StringListNode* StringList_AddBack(StringList* L, char* str){
	StringListNode* new_node = (StringListNode* )malloc( sizeof(StringListNode) );
	char* new_str = (char*)malloc( sizeof(char)*(strlen(str)+1) );
	strcpy(new_str, str);
	if (L->head == NULL){
		new_node->element = new_str;
		new_node->previous = NULL;
		new_node->next = NULL;
		L->head = new_node;		
		L->tail = new_node;
	}else{
		new_node->element = new_str;
		new_node->previous = L->tail;
		new_node->next = NULL;
		L->tail->next = new_node;
		L->tail = new_node;
	}	
	return new_node;		
}

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
int StringList_Remove(StringList* L, char* str){
	if(L->head == NULL)
			return 0;
//	StringListNode* curr;
//	StringListNode* temp;
	StringListNode* curr = L->head;
	while(curr != NULL){
		if(strcmp(curr->element, str) == 0)
			break;
		curr = curr->next;
	}
	if(curr == NULL)
		return 0;
	if(curr == L->head){
		if(L->head->next == NULL){
			StringListNode* temp = L->head;
			L->head = NULL;
			L->tail = NULL;
			temp->next = NULL;
			temp->previous = NULL;
			temp->element = NULL;
		//	printf("a");
			return 1;
		}else{
			StringListNode* temp = L->head;
			L->head = L->head->next;
			L->head->previous = NULL;
			temp->next = NULL;
			temp->previous = NULL;
			temp->element = NULL;
		//	printf("b");
			return 1;
		}		
	}else{
		if(curr == L->tail){
			StringListNode* temp = L->tail;
			L->tail = L->tail->previous;
			L->tail->next = NULL;
			temp->next = NULL;
			temp->previous = NULL;
			temp->element = NULL;
		//	printf("c");
			return 1;
		}else{
			StringListNode* temp = curr;
			StringListNode* temp1 = curr->previous;
			StringListNode* temp2 = curr->next;
			temp1->next = temp2;
			temp2->previous = temp1;	
			temp->next = NULL;
			temp->previous = NULL;
			temp->element = NULL;
		//	printf("d");
			return 1;
		}
	}
	return 0;
}

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
void StringList_RemoveNode(StringList* L, StringListNode* node){	
	if(L->head == NULL)
			return;
	StringListNode* curr = L->head;
	while(curr != node){
		curr = curr->next;
	}
	if(curr == L->head){
		if(L->head->next == NULL){
			StringListNode* temp = L->head;
			L->head = NULL;
			L->tail = NULL;
			temp->next = NULL;
			temp->previous = NULL;
			temp->element = NULL;
		//	printf("1");
		}else{
			StringListNode* temp = L->head;
			L->head = L->head->next;
			L->head->previous = NULL;
			temp->next = NULL;
			temp->previous = NULL;
			temp->element = NULL;
		//	printf("2");
		}		
	}else{
		if(curr == L->tail){
			StringListNode* temp = L->tail;
			L->tail = L->tail->previous;
			L->tail->next = NULL;
			temp->next = NULL;
			temp->previous = NULL;
			temp->element = NULL;
		//	printf("3");
		}else{
			StringListNode* temp = curr;
			StringListNode* temp1 = curr->previous;
			StringListNode* temp2 = curr->next;
			temp1->next = temp2;
			temp2->previous = temp1;	
			temp->next = NULL;
			temp->previous = NULL;
			temp->element = NULL;
		//	printf("4");
		}
	}
	
}

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
StringListNode* StringList_InList(StringList* L, char* str){
	StringListNode* curr = (StringListNode* )malloc( sizeof(StringListNode) );
	if(L->head == NULL)
		return NULL;
	curr = L->head;
	while(curr != NULL){
		if(strcmp(curr->element,str) == 0){
			break;
		}
		curr = curr->next;
	}
	if(curr == NULL) return NULL;
	return curr;	
}


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
StringListNode* StringList_GetIndex(StringList* L, int i){
	StringListNode* curr = (StringListNode* )malloc( sizeof(StringListNode) );
	curr = L->head;
	if(i < 0 || i > (StringList_Size(L)-1))
		return NULL;
	if(L->head == NULL)
		return NULL;
	int count = 0;
	while(count != i && curr != NULL){
		curr = curr->next;
		count++;
	}
	if(curr == NULL)
		return NULL;
	return curr;
}


