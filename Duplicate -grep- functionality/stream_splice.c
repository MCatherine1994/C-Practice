#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[]){
	FILE* input_file = stdin;	
	char fline_input[1000];
	char *newline = NULL; 
	if(argc != 2){
                fprintf(stderr, "Usage: ./stream_search <word list file>\n");
		return 1;
        }

	char *fp = argv[1];

	if(fp == NULL){
		printf("Error: Unable to open word list\n");
		return 2;
	}


	while(fgets(fline_input, 1000, input_file) != NULL){
		if((newline = strstr(fline_input,"\n")) != NULL) {
			*newline = '\0';
		}
		while(strstr(fline_input, fp) != NULL){	
			char* found = strstr(fline_input, fp);
			int len_input = strlen(fline_input);
			int len_fp = strlen(fp);
			int len_found = strlen(found);
			int pos = len_input - len_found;
			int j;
				
			for(j = pos; j < len_input; j++){
				fline_input[j] = fline_input[j+len_fp];
			}

			fline_input[j] = '\0';

		}
		printf("%s\n",fline_input);
	}

	return 0;

}
