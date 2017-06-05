/* string_list_tester.c

   
   Template for a tester for the StringList datatype.
   
   Do not change any of the function signatures or the contents
   of main() or your submission may be invalid.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_list.h"

#define TEST_FAILED 0
#define TEST_PASSED 1

/* Each of the test functions below should return TEST_FAILED if the test fails
   and TEST_PASSED if the test is passed.
*/

int test1(){
	//First test case: Basic functionality I
	//Test only the StringList_Init, StringList_Size,
	//and StringList_AddFront functions.
	//(Note that since you cannot use StringList_Destroy, this function
	// will produce a memory leak. However, this is acceptable in a testing
	// context).
	StringList InputLines;
	StringList_Init(&InputLines);
	StringListNode* newNode = InputLines.tail;
	if(StringList_Size(&InputLines) != 0){
		printf("Error: size is incorrect.\n");
		return TEST_FAILED;
	}
	if(newNode != NULL){
		printf("Error: StringList_Init method is incorrect.\n");
		return TEST_FAILED;
	}
	newNode = InputLines.head;
	if(newNode != NULL){
		printf("Error: StringList_Init method is incorrect.\n");
		return TEST_FAILED;
	}

	char* s[]= {"f","de","abc"};
	int i;
	for(i = 0; i < 3; i++){
		StringList_AddFront(&InputLines,s[i]);	
		newNode = InputLines.head;
		if(newNode == NULL){
			printf("Error: StringList_AddFront method is incorrect.\n");
			return TEST_FAILED;
		}
		if(strcmp(newNode->element,s[i]) != 0){
			printf("Error: head contains incorrect value.\n");
			return TEST_FAILED;
		}
		if(StringList_Size(&InputLines) != i+1){
			printf("Error: size is incorrect.\n");
			return TEST_FAILED;
		}
	}
	return TEST_PASSED;
}

int test2(){
	//Second test case: Basic functionality II
	//Test only the StringList_Init, StringList_Size,
	//StringList_AddFront and StringList_AddBack functions.
	StringList InputLines;
	StringList_Init(&InputLines);
	if(StringList_Size(&InputLines) != 0){
		printf("Error: size is incorrect.\n");
		return TEST_FAILED;
	}
	if(InputLines.head != NULL){
		printf("Error: StringList_Init method is incorrect.\n");
		return TEST_FAILED;
	}
	if(InputLines.tail != NULL){
		printf("Error: StringList_Init method is incorrect.\n");
		return TEST_FAILED;
	}

	char* s[]= {"f","de","abc"};
	int i;
	for(i = 0; i < 3; i++){
		StringList_AddFront(&InputLines,s[i]);
		if(InputLines.head == NULL){
			printf("Error: StringList_AddFront method is incorrect.\n");
			return TEST_FAILED;
		}
		if(strcmp(InputLines.head->element,s[i]) != 0){
			printf("Error: head contains incorrect value.\n");
			return TEST_FAILED;
		}
	}

	char* c[]= {"ghij","k","lmn"};
	for(i = 0; i < 3; i++){
		StringList_AddBack(&InputLines,c[i]);	
		if(StringList_Size(&InputLines) != i+4){
			printf("Error: size is incorrect.\n");
			return TEST_FAILED;
		}
		if(strcmp(InputLines.tail->element,c[i]) != 0){
			printf("Error:tail contains incorrect value.\n");
			return TEST_FAILED;
		}
	}
	
	return TEST_PASSED;
}

int test3(){
	//Third test case: Basic functionality III
	//Test only the StringList_Init, StringList_Size,
	//StringList_AddFront and StringList_AddBack functions.
	//Use a different methodology for this test than test 2.
	StringList InputLines;
	StringList_Init(&InputLines);
	if(StringList_Size(&InputLines) != 0){
		printf("Error: size is incorrect.\n");
		return TEST_FAILED;
	}

	char* s[]= {"f","de","abc"};
	int i;
	for(i = 0; i < 3; i++){
		StringList_AddFront(&InputLines,s[i]);
	}
	char* s1 = "abc";
	if(strcmp(InputLines.head->element,s1) != 0){
		printf("Error:head contains incorrect value.\n");
		return TEST_FAILED;
	}
	s1 = "f";
	if(strcmp(InputLines.tail->element,s1) != 0){
		printf("Error:tail contains incorrect value.\n");
		return TEST_FAILED;
	}

	char* c[]= {"ghij","k","lmn","opqrst"};
	for(i = 0; i < 4; i++){
		StringList_AddBack(&InputLines,c[i]);	
	}
	s1 = "abc";
	if(strcmp(InputLines.head->element,s1) != 0){
		printf("Error:head contains incorrect value.\n");
		return TEST_FAILED;
	}
	s1 = "opqrst";
	if(strcmp(InputLines.tail->element,s1) != 0){
		printf("Error:tail contains incorrect value.\n");
		return TEST_FAILED;
	}

	if(StringList_Size(&InputLines) != 7){
		printf("Error: size is incorrect.\n");
		return TEST_FAILED;
	}

	return TEST_PASSED;
}

int test4(){
	//Fourth test case: List membership I
	//Test the StringList_GetIndex function, using any of the functions
	//tested in previous test cases if necessary.
	//Use a different methodology for this test than test 2.
	StringList InputLines;
	StringList_Init(&InputLines);
	if(StringList_GetIndex(&InputLines, 1) != NULL){
		printf("Error: get an index from an empty list.\n");
		return TEST_FAILED;
	}
	if(StringList_GetIndex(&InputLines, -1) != NULL){
		printf("Error: invalid input value.\n");
		return TEST_FAILED;
	}
	
	char* s[]= {"klmno","ghij","f","de","abc"};
	int i;
	for(i = 0; i < 5; i++){
		StringList_AddFront(&InputLines,s[i]);
	}
	StringListNode* newNode = StringList_GetIndex(&InputLines, 0);
	char* s1 = "abc";
	if(strcmp(newNode->element,s1) != 0){
		printf("Error: incorrect node at index 0.\n");
		return TEST_FAILED;
	}

	newNode = StringList_GetIndex(&InputLines, 4);
	s1 = "klmno"; 
	if(strcmp(newNode->element,s1) != 0){
		printf("Error: incorrect node at index 4.\n");
		return TEST_FAILED;
	}

	newNode = StringList_GetIndex(&InputLines, 5);
	if(newNode != NULL){
		printf("Error: invalid input value.\n");
		return TEST_FAILED;
	}

	return TEST_PASSED;
}

int test5(){
	//Fifth test case: List membership II
	//Test the StringList_InList function, using any of the functions
	//tested in previous test cases if necessary.
	StringList InputLines;
	StringList_Init(&InputLines);
	char* s[]= {"klmno","ghij","f","de","abc"};
	int i;
	for(i = 0; i < 5; i++){
		StringList_AddFront(&InputLines,s[i]);
	}
	char* s1 = "f"; 
	StringListNode* newNode = StringList_InList(&InputLines, s1);
	if(strcmp(newNode->element,s1) != 0){
		printf("Error: %s is in the list.\n",s1);
		return TEST_FAILED;
	}
	s1 = "ghij";
	newNode = newNode->next;
	if(strcmp(newNode->element,s1) != 0){
		printf("Error: %s is in the list.\n",s1);
		return TEST_FAILED;
	}

	return TEST_PASSED;
}

int test6(){
	//Sixth test case: Deletion I
	//Test the StringList_Remove and StringList_RemoveNode functions,
	//using any of the functions tested in previous test cases if necessary.
	StringList InputLines;
	StringList_Init(&InputLines);
	char* s = "a";
	StringList_AddBack(&InputLines,s);
	StringListNode* newNode = InputLines.head;
	if(strcmp(newNode->element,s) != 0){
		printf("Error: %s does not add into the list.\n",s);
		return TEST_FAILED;
	}
	StringList_RemoveNode(&InputLines,newNode);
	if(InputLines.head !=  NULL){
		printf("Error: StringList_RemoveNode failed, does not remove %s.\n",s);
		return TEST_FAILED;
	}

	char* c[]= {"ab","c","d","ef","ghij","k"};
	int i;
	for(i = 0; i < 6; i++){
		StringList_AddBack(&InputLines,c[i]);	
	}
	newNode = InputLines.head;
	s = "ab";
	if(strcmp(newNode->element,s) != 0){
		printf("Error: %s does not add into the list.\n",s);
		return TEST_FAILED;
	}
	StringList_RemoveNode(&InputLines,newNode);
	newNode = InputLines.head;
	s = "c";
	if(strcmp(newNode->element,s) != 0){
		printf("Error: StringList_RemoveNode failed, does not remove.\n");
		return TEST_FAILED;
	}

	newNode = (InputLines.head)->next;
	s = "d";
	if(strcmp(newNode->element,s) != 0){
		printf("Error: %s does not add into the list.\n",s);
		return TEST_FAILED;
	}
	StringList_RemoveNode(&InputLines,newNode);
	newNode = (InputLines.head)->next;
	s = "ef";
	if(strcmp(newNode->element,s) != 0){
		printf("Error: StringList_RemoveNode failed, does not remove.\n");
		return TEST_FAILED;
	}

	newNode = InputLines.tail;
	s = "k";
	if(strcmp(newNode->element,s) != 0){
		printf("Error: %s does not add into the list.\n",s);
		return TEST_FAILED;
	}
	StringList_RemoveNode(&InputLines,newNode);
	newNode = InputLines.tail;
	s = "ghij";
	if(strcmp(newNode->element,s) != 0){
		printf("Error: StringList_RemoveNode failed, does not remove.\n");
		return TEST_FAILED;
	}

	s = "klmn";
	StringList_AddBack(&InputLines,s);
	s = "o";
	StringList_AddBack(&InputLines,s);

	i = StringList_Remove(&InputLines,s);
	if( i == 0){
		printf("Error: Cannot find.\n");
		return TEST_FAILED;
	}
	if(StringList_Size(&InputLines) != 4){
			printf("Error: size is incorrect.\n");
			return TEST_FAILED;
	}

	return TEST_PASSED;
}

int test7(){
	//Seventh test case: Deletion II
	//Test the StringList_Remove, StringList_RemoveNode and StringList_Destroy functions,
	//using any of the functions tested in previous test cases if necessary.
	//Use a different methodology for this test than test 6.
	StringList InputLines;
	StringList_Init(&InputLines);
	char* c[]= {"ab","c","d"};
	char* s = "ab";
	int i;
	for(i = 0; i < 3; i++){
		StringList_AddBack(&InputLines,c[i]);	
	}
//str in the head
	StringList_Remove(&InputLines,s);
	StringListNode* newNode = InputLines.head;
	s = "c";
	if(strcmp(newNode->element,s) != 0){
		printf("Error: StringList_RemoveNode failed, does not remove. 1\n");
		return TEST_FAILED;
	}
//str in the tail
	s = "d";
	StringList_Remove(&InputLines,s);
	newNode = InputLines.tail;
	s = "c";
	if(strcmp(newNode->element,s) != 0){
		printf("Error: StringList_RemoveNode failed, does not remove. 2\n");
		return TEST_FAILED;
	}
//str is in the only node
	StringList_Remove(&InputLines,s);
	newNode = InputLines.tail;
	if(newNode != NULL){
		printf("Error: StringList_RemoveNode failed, does not remove. 3\n");
		return TEST_FAILED;
	}

	for(i = 0; i < 3; i++){
		StringList_AddBack(&InputLines,c[i]);	
	}
//str is not in the list
	s = "e";
	int j = StringList_Remove(&InputLines,s);
	if( j == 1){
		printf("Error: find a sting which is not in the list.\n");
		return TEST_FAILED;
	}
//str is in the middle of the list
	s = "c";
	StringList_Remove(&InputLines,s);
	newNode = (InputLines.tail)->previous;
	s = "ab";
	if(strcmp(newNode->element,s) != 0){
		printf("Error: StringList_RemoveNode failed, does not remove. 4\n");
		return TEST_FAILED;
	}
	if(StringList_Size(&InputLines) != 2){
		printf("Error: StringList_Remove failed: size is wrong.\n");
		return TEST_FAILED;
	}

	StringList_RemoveNode(&InputLines,newNode);
	newNode = InputLines.head;
	s = "d";
	if(strcmp(newNode->element,s) != 0){
		printf("Error: StringList_RemoveNode failed, does not remove.\n");
		return TEST_FAILED;
	}
	if(StringList_Size(&InputLines) != 1){
		printf("Error: StringList_Remove failed: size is wrong.\n");
		return TEST_FAILED;
	}

	StringList_RemoveNode(&InputLines,newNode);
	if(InputLines.tail != NULL){
		printf("Error: StringList_RemoveNode failed, does not remove.\n");
		return TEST_FAILED;
	}

	for(i = 0; i < 3; i++){
		StringList_AddBack(&InputLines,c[i]);	
	}
	StringList_Destroy(&InputLines);
	if((InputLines.head != NULL) || (InputLines.tail != NULL)){
		printf("Error: StringList_Destory failed.\n");
		return TEST_FAILED;
	}

	return TEST_PASSED;
}

int test8(){
	//Eighth test case: Stress Test
	//Test all of the methods of the StringList, in an attempt to simulate the
	//actual use of the data structure in practice. For logistical reasons, 
	//keep in mind that your test must complete within 30 seconds for the 
	//automated system, so some types of exhaustive testing are not practical
	//here.
	StringList InputLines;
//test StringList_Init
	StringList_Init(&InputLines);
	if((InputLines.head != NULL) || (InputLines.tail != NULL)){
		printf("Error: StringList_Init failed.\n");
		return TEST_FAILED;
	}
//test StringList_AddFront and StringList_AddBack
	char* s[]= {"f","de","abc"};
	int i;
	for(i = 0; i < 3; i++){
		StringList_AddFront(&InputLines,s[i]);	
	}
	char* c[]= {"ghij","k","lmn"};
	for(i = 0; i < 3; i++){
		StringList_AddBack(&InputLines,c[i]);	
	}
	char* s1 = "abc";
	StringListNode* newNode = InputLines.head;
	if(strcmp(newNode->element,s1) != 0){
		printf("Error: StringList_AddFront failed.\n");
		return TEST_FAILED;
	}
	s1 = "lmn";
	newNode = InputLines.tail;
	if(strcmp(newNode->element,s1) != 0){
		printf("Error: StringList_AddBack failed.\n");
		return TEST_FAILED;
	}
//test StringList_Size
	if(StringList_Size(&InputLines) != 6){
		printf("Error: StringList_Size failed.\n");
		return TEST_FAILED;
	}
//test StringList_GetIndex
	s1 = "f";
	newNode = StringList_GetIndex(&InputLines, 2);
	if(strcmp(newNode->element,s1) != 0){
		printf("Error: StringList_GetIndex failed.\n");
		return TEST_FAILED;
	}
//test StringList_InList
	newNode = StringList_InList(&InputLines, s1);
	if(strcmp(newNode->element,s1) != 0){
		printf("Error: StringList_InList failed.\n");
		return TEST_FAILED;
	}
//test StringList_Remove
	i = StringList_Remove(&InputLines,s1);
	if( i == 0){
		printf("Error: StringList_Remove failed.\n");
		return TEST_FAILED;
	}
	if(StringList_Size(&InputLines) != 5){
		printf("Error: StringList_Size failed 1.\n");
		return TEST_FAILED;
	}
//test StringList_RemoveNode
	newNode = InputLines.head;
	StringList_RemoveNode(&InputLines,newNode);
	newNode = InputLines.head;
	s1 = "de";
	if(strcmp(newNode->element,s1) != 0){
		printf("Error: StringList_RemoveNode failed 2.\n");
		return TEST_FAILED;
	}
	if(StringList_Size(&InputLines) != 4){
		printf("Error: StringList_Size failed.\n");
		return TEST_FAILED;
	}

//test StringList_Destroy
	StringList_Destroy(&InputLines);
	if((InputLines.head != NULL) || (InputLines.tail != NULL)){
		printf("Error: StringList_Destory failed.\n");
		return TEST_FAILED;
	}

	return TEST_PASSED;
}




/* Do not change any of the code below, to ensure consistency during all vs. all testing */

void show_usage(){
	fprintf(stderr,"Usage: ./string_list_tester <test_number>\n");
	fprintf(stderr,"Where <test_number> is an integer between 1 and 8.\n");
}

int main(int argc, char** argv){
	if (argc < 2){
		show_usage();
		return 0;
	}
	//The atoi function converts a string to an integer (and returns 0 if
	//the conversion fails).
	int test_number = atoi(argv[1]);
	
	if (test_number <= 0 || test_number > 8){
		show_usage();
		return 0;
	}
	
	int result = 1;
	switch(test_number){
		case 1:
			result = test1();
			break;
		case 2:
			result = test2();
			break;
		case 3:
			result = test3();
			break;
		case 4:
			result = test4();
			break;
		case 5:
			result = test5();
			break;
		case 6:
			result = test6();
			break;
		case 7:
			result = test7();
			break;
		case 8:
			result = test8();
			break;
	}
	
	if (result == TEST_PASSED){
		printf("\nTEST PASSED\n");
	}else{
		printf("\nTEST FAILED\n");
	}
	
	return 0;
}
