// diskput
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define OS_OFFSET 3
#define NUM_OF_FATS 16
#define SECTOR_PER_FAT 22
#define TOTAL_SECTOR_COUNT 19 

#define BYTES_PER_SECTOR 512
#define LABEL_SIZE 11

typedef struct{
    char name[8];
    char ext[3];
	int attr;
    char first_cluster[2];
    char size[4];
} entry_struct;

int next_entry(char* disk, int entry)
{
    /* Return the next logical cluster by current FAT entry, odd or even different rules. */
    char *b = disk + 512 + entry * 3 / 2;
    if(entry & 1)
        return (b[0] >> 4) | (b[1] << 4);
    else
        return b[0] | ((b[1] & 0x0F) << 8);
}

int next_free_entry(char *disk, int start)
{
    int i;
    for(i = start;i < disk[TOTAL_SECTOR_COUNT] + (disk[TOTAL_SECTOR_COUNT+1]<<8); i ++)
    {   
        if(next_entry(disk, i) == 0)
            return i;
    }

    return -1;
}

void write_name(entry_struct *entry, char* filename)
{
    int j;
    for(j = strlen(filename) - 1;j; j --)
        if(filename[j] == '.')
            break;
    filename[j] = '\0';

    strcpy(entry->name, filename);
    strcpy(entry->ext, filename + j + 1);

    for(j = strlen(entry->name); j < 8;j ++)
        entry->name[j] = ' ';

}

int main(int argc, char *argv[]){
	entry_struct *entry;
	int fd;
	int fd2;
	struct stat fs;
	struct stat fs2;
	int file_size;
	int file_size2;
	
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

	//open(check) file to be copied in current dir, grab its file size & related info
	int i;
	for (i = 0, entry = (entry_struct *) (disk + 0x2600); i < 224; i ++, entry++);
    if (i == 224)
    {
        fprintf(stderr, "The root directory is full.");
    }	
    fd = open(argv[2], O_RDONLY);
    if(fd == -1)
    {
        fprintf(stderr, "Cannot open the file.");
		return 1;
    }
	file_size2 = fs2.st_size;
	int need_sectors = fs2.st_size / 512 + (fs2.st_size % 512 != 0);
	
	//check for free space in disk
	int free_entries = 0;
	int total_sectors = disk[TOTAL_SECTOR_COUNT] + (disk[TOTAL_SECTOR_COUNT+1]<<8);
	for(i = 0; i < total_sectors; i++){
		if(next_entry(disk, i) == 0){
			free_entries++;
		}
	}
	if(free_entries < need_sectors)
    {
        fprintf(stderr, "Not enough spaces in the disk image.");
        close(fd);
		return 1;
    }

	//Add file entry in disk root dir 
    int first_cluster = next_free_entry(disk, 2);
	
	entry_struct new_entry;
    write_name(&new_entry, argv[2]); // name and ext
    new_entry.attr = 0x00; 

	munmap(disk,file_size);
	//munmap(file)
	
	//close disk file
	close(fd);
	//close file

	return 0;
}
