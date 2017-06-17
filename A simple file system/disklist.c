// disklist
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
//information stored matches the staring byte
#define OS_OFFSET 3
#define NUM_OF_FATS 16
#define SECTOR_PER_FAT 22
#define TOTAL_SECTOR_COUNT 19 

#define BYTES_PER_SECTOR 512
#define LABEL_SIZE 11

typedef struct{
	char *file_type;
	int file_size;
	char *file_name;
	char *create_date;
	char *create_time;
} output_format;

typedef output_format *output_pointer;
output_pointer *root_file;

int file_count(char* disk){
	int file_num = 0;
	int i;
	for(i = 19; i <= 32; i++){
		int j;
		for(j = 0; j < 16; j++){
			int attr_value = disk[(i*BYTES_PER_SECTOR)+(j*32)+LABEL_SIZE];
			if(disk[(i*BYTES_PER_SECTOR)+(j*32)] == 0x00){
				return file_num;
			}
			if((attr_value & 0X0F) != 0x0F && (attr_value & 0x08) != 0x08 && (attr_value & 0x10) != 0x10){
				file_num++;
			}
		}
	}
	
	return file_num;
}

int main(int argc, char *argv[]){
	int fd;
	struct stat fs;
	int file_size;
	
	//invalid input
	if(argc != 2){
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
	
	int file_num = file_count(disk);
	root_file = malloc(file_num * sizeof(output_pointer));
	int index = 0;
	int i;
	for(i = 19; i <= 32; i++){
		int j;
		for(j = 0; j < 16; j++){// 16 entries, each one is 32 bytes
			int offset = (i*BYTES_PER_SECTOR)+(j*32);
			int attr_value = disk[offset+LABEL_SIZE];
			if(disk[offset] == 0x00){
				break;
			}
			if((attr_value & 0X0F) != 0x0F && (attr_value & 0x08) != 0x08 && (attr_value & 0x10) != 0x10){
				root_file[index] = malloc(sizeof(output_format));
				//check attributes for 'D' or 'F'
				root_file[index]->file_type = malloc(sizeof(char)*2);
				if(attr_value == 0x10){
					//subdirectory
					root_file[index]->file_type[0] = 'D';
					root_file[index]->file_type[1] = '\0';
				}else{
					root_file[index]->file_type[0] = 'F';
					root_file[index]->file_type[1] = '\0';
				}
				
				//get file size
				root_file[index]->file_size = (disk[offset]&0x00FF) + ((disk[offset+1]&0X00FF)<<8) + ((disk[offset+2]&0X00FF)<<12) + ((disk[offset+3]&0X00FF)<<16);
				
				//get file name
				root_file[index]->file_name = malloc(sizeof(char) * 20);
				int k;
				for(k = 0; k < 8; k++){
					if(!isspace(disk[offset+k])){
						root_file[index]->file_name[k] = disk[offset+k];
					}else{
						break;	
					}
				}
				root_file[index]->file_name[k++] = '.';
				int p;
				for(p = 0; p <3; p++){
					root_file[index]->file_name[k+p] = disk[offset+8+p];
				}
				
				//get date
				root_file[index]->create_date = malloc(sizeof(char) * 10);
				int date = (disk[offset+16]&0x00FF) + ((disk[offset+17]&0x00FF)<<8); //creation date stored in offset 16 and 2 bytes long
				int day = date & 0X001F;
				int month = (date & 0X01E0)>> 0x05;
				int year = ((date & 0xFE00)>> 0x09) +1980;
				sprintf(root_file[index]->create_date, "%d-%02d-%02d", year, month, day);
				
				//get time
				root_file[index]->create_time = malloc(sizeof(char) * 5);
				int time = (disk[offset+14]&0X00FF) + ((disk[offset+15]&0X00FF)<<8); //creation date stored in offset 14 and 2 bytes long
				int min = (time & 0X07E0)>> 0x05;
				int hour = (date & 0XF800)>> 0x0B;
				sprintf(root_file[index]->create_time, "%02d:%02u", hour, min);
				
				index++;		
			}				
		}
	}
	
	//print accordingly
	for (i = 0; i < file_num; i ++) {
		printf("%1s %10d %20s %10s %5s\n", root_file[i]->file_type, root_file[i]->file_size, root_file[i]->file_name, root_file[i]->create_date, root_file[i]->create_time);
	}
	
	//munmap(disk file)
	munmap(disk,file_size);
	//close disk file
	close(fd);

	return 0;
}

