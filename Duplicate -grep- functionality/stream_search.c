#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char* argv[]){

	FILE* input_file = stdin;	
	char fline_input[1000];
	char fline_fp[100];
	char *newline = NULL;

	if(argc != 2){
		fprintf(stderr, "Usage: ./stream_search <word list file>\n");
		return 1;
	}	

	while(fgets(fline_input, 1000, input_file) != NULL){
		FILE* fp =fopen(argv[1],"r");
		if(fp == NULL){
			fprintf(stderr, "Error: Unable to open word list\n");
			return 2;
		}

		if((newline = strstr(fline_input,"\n")) != NULL) {
			*newline = '\0';
		} 

		while(fgets(fline_fp, 100, fp) != NULL){

			if((newline = strstr(fline_fp,"\n")) != NULL) {
				*newline = '\0';
			}
			 				
			if(strstr(fline_input, fline_fp) != NULL){
				printf("%s\n",fline_input);
				break;
			}
		}

		fclose(fp);	
	}

	return 0;

}
