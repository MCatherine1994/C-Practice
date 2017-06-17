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

char * os_name(char *osname,char *disk){
	int i = 0;
	for(i = 0; i < 8; i++){
		osname[i] = disk[OS_OFFSET+i];
	}
	return osname;
}

char *disk_label(char* buffer, char* disk){	
	int i;
	//root directory 19-32
	for(i = 19; i <= 32; i++){
		int j;
		for(j = 0; j < 16; j++){
			int attr_value = disk[(i*BYTES_PER_SECTOR)+(j*32)+LABEL_SIZE];
			if( (attr_value & 0x08) == 0x08 && (attr_value & 0x0F) != 0x0F){
				int k;
				for(k = 0; k < LABEL_SIZE; k++) {
					buffer[k] = disk[(i * BYTES_PER_SECTOR) + (j * 32) + k];
				}
			}
		}
	}

    buffer[LABEL_SIZE] = '\0';
    return buffer;
}

int total_size(char* disk){
	int num_sectors = disk[TOTAL_SECTOR_COUNT] + (disk[TOTAL_SECTOR_COUNT+1]<<8);
	//total number of sectors * bytes for one sector
	return num_sectors*BYTES_PER_SECTOR;
	
}

int free_size(char* disk){
	int free_sectors = 0;
	int i;
	for(i = 2; i <= 2879; i++){
		int *tmp1 = malloc(sizeof(int));
		int *tmp2 = malloc(sizeof(int));
		int entry;
		
		//for the n-th FAT entry, if n is even
		if(i%2 == 0){
			*tmp1 = disk[BYTES_PER_SECTOR+(3*i)/2]; //position (3*i)/2
			*tmp2 = disk[BYTES_PER_SECTOR+1+(3*i)/2]; 
			*tmp2 = *tmp2 & 0x0F; //low 4 bits
			
			entry = *tmp1 + (*tmp2<<8);
			
		}else{
			*tmp1 = disk[BYTES_PER_SECTOR+(3*i)/2];
			*tmp1 = *tmp1 & 0xF0;//high 4 bits
			*tmp2 = disk[BYTES_PER_SECTOR+1+(3*i)/2]; 
			
			entry =  *tmp2 + (*tmp1<<8);
			
		}

		if(entry == 0x00){
			free_sectors++;
		}
	}
	return free_sectors * 512;
}

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
	char *osname = malloc(sizeof(char)*8);
	char label_buffer[LABEL_SIZE + 1];
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

	printf("OS Name: %s\n", os_name(osname, disk)); 
	printf("Label of the disk: %s\n", disk_label(label_buffer, disk));
	printf("Total size of the disk: %u bytes\n", total_size(disk));
	printf("Free size of the disk: %u bytes\n", free_size(disk));
	printf("\n==============\n");
	printf("\nThe number of files in the root directory (not including subdirectories): %u\n", file_count(disk));
	printf("\n==============\n");
	printf("\nNumber of FAT copies: %u\n", disk[NUM_OF_FATS]); 
	printf("Sectors per FAT: %u\n\n", disk[SECTOR_PER_FAT] + (disk[SECTOR_PER_FAT+1] << 8)); 

	free(osname);
	//munmap(disk file)
	munmap(disk,file_size);
	//close disk file
	close(fd);

	return 0;
}

