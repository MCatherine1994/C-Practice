// diskget
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h> 
#include <fcntl.h> 
#include <string.h>
#include <errno.h>
#define OS_OFFSET 3
#define NUM_OF_FATS 16
#define SECTOR_PER_FAT 22
#define TOTAL_SECTOR_COUNT 19 

#define BYTES_PER_SECTOR 512
#define LABEL_SIZE 11

typedef struct{
    char* name;
    char* ext;
    int filesize, logical_sector;
} disk_file;

int get_sector(char* disk, int current){
	int i = current;
    int* temp1 = malloc(sizeof(int));
    int* temp2 = malloc(sizeof(int));
    int entry = 0;
    
    // if the logical number is even
    if(i%2 == 0){
		*temp1 = (disk[BYTES_PER_SECTOR+ 3*i/2] & 0x00FF);
		*temp2 = (disk[BYTES_PER_SECTOR + 1 + 3 * i/2] & 0x00FF); 
		*temp2 = *temp2 & 0x0F; //low 4 bits	
		entry = *temp1 + (*temp2<<8);		
	}else{
		*temp1 = (disk[BYTES_PER_SECTOR+(3*i)/2]& 0x00FF);
		*temp1 = ((*temp1 & 0xF0)>>4);//high 4 bits
		*temp2 = (disk[BYTES_PER_SECTOR+1+(3*i)/2]& 0x00FF); 
		entry = *temp1 + (*temp2<<4);	
	}
	
    free(temp1);
    free(temp2);
    
    return entry;
}


void get_file(char* disk, char* new_file){
	int first_byte = 9728;  
    int current = first_byte;   
    int offset = 32;  // each entry has 32 bytes 
    int* temp1 = malloc(sizeof(int));
    int* temp2 = malloc(sizeof(int));
    char* temp3 = malloc(sizeof(char)*2);
    char* check_name = malloc(sizeof(char)*14);
	disk_file* file = malloc(sizeof(disk_file));
    int counter = 0;
	
	*temp1 = disk[current];
	 while(*temp1 != 0x00 && current < (33 * 512))
    {
        char* file_name = malloc(sizeof(char)*14);
        if (*temp1 != 0xE5)
        {
            *temp2 = disk[current + 11];
            if (*temp2 != 0x08 && *temp2 != 0x0F )
            { 
                file -> name = malloc(sizeof(char)*9); 
                file -> ext = malloc(sizeof(char)*4);    
                int i;
                for (i = 0; i < 8; i++){
                    file -> name[i] = disk[current + i];
                    if (file -> name[i] == ' '){
                        file -> name[i] = '\0';
                        break;
                    }
                }    
                while(i < 11){
                    file -> ext[i - 8] = disk[current + i];
					i++;
                }
				file -> name[8] = '\0';
                file -> ext[3] = '\0';
				strcat(file_name, file -> name);
                strcat(file_name, ".");
                strcat(file_name, file -> ext);
                if(!strcmp(file_name, new_file)){
                    // Find the logical sector
                    file -> logical_sector = (disk[current + 26] & 0x00FF) + ((disk[current + 27] & 0x00FF) << 8);
                    file -> filesize = (disk[current + 28] & 0x00FF) + ((disk[current + 29] & 0x00FF) << 8) + ((disk[current + 30] & 0x00FF) << 16) + ((disk[current + 31] & 0x00FF) << 24);
                    strcpy(check_name, file_name);
                    break; 
                }
                
            }
        }
        
        // Go to next entry in Root Directory
        current = current + offset;
        *temp1 = disk[current];
        free(file_name);
    }
   
    if(strcmp(check_name, new_file))
    {
        fprintf(stderr,"File not found\n");
		return;
    }
    
    FILE* fp = fopen(new_file, "w");
    int next = file -> logical_sector; 
    int bytes_left = file -> filesize; 
    int bytes_to_read;
    //printf("%d",next);
    while(next < 0xFF8){
        int start_byte = 512 * (next + 31);
        if (bytes_left >= 512)
        {
            bytes_to_read = 512;
        } else
        {
            bytes_to_read = bytes_left;
        }
        fwrite (disk + start_byte, sizeof(char), bytes_to_read, fp);
		next = (get_sector(disk,next) & 0xFFF);
		bytes_left -= 512;
    }
    
    free(file);
    free(temp1);
    free(temp2);
    free(temp3);
    free(check_name);
    fclose(fp);
}



int main(int argc, char *argv[]){
	int fd;
	struct stat fs;
	int file_size;
	
	//invalid input
	if(argc != 3){
		fprintf(stderr, "Invalid input.\n");
		return 1;
	}
	
	//open disk file
	fd = open(argv[1], O_RDONLY);
	//cannot open the file
	if(fd == -1){
		fprintf(stderr, "Cannot open the file.\n");
		return 1;
	}

	//store the information of the file into fs
	fstat(fd, &fs);
	//get disk size
	file_size = fs.st_size;

	char *disk = mmap(NULL, fs.st_size, PROT_READ, MAP_SHARED, fd, 0);
	
	get_file(disk, argv[2]);

	//munmap(disk file)
	munmap(disk,file_size);
	
	//close disk file
	close(fd);

	return 0;
}
