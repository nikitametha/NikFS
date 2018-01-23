#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

#define NUMBER_OF_BLOCKS 500
#define BLOCK_SIZE 1024

void initialize_mem_disk();
int write_block(char *write_data, int block_num);
char* read_block(int block_num);
void clear_all();


char **mem_disk; //in memory buffer (simulates disk)
char **inode_info;

void main()
{
    printf("Initializing mem_disk..\n");
    printf("Block Size = %d\nNumber of blocks=%d\n", BLOCK_SIZE, NUMBER_OF_BLOCKS); 
    initialize_mem_disk();
    //write_block("AAAA",5);
    //write_block("AAAAwwww",908);
    //char *x = read_block(3);
    printf("Block read - %s\n", x);
    //char** mem_disk = calloc(NUMBER_OF_BLOCKS, BLOCK_SIZE*sizeof(char)); 
    //strcpy(mem_disk[1], "frrr");
    //printf("%s\n",mem_disk[1] );

}

void initialize_mem_disk()
{
    mem_disk = malloc( NUMBER_OF_BLOCKS * sizeof(char*) );
    for (int i=0; i<NUMBER_OF_BLOCKS; i++)
    {
        mem_disk[i] = malloc(BLOCK_SIZE * sizeof(char));
    }


}

char* read_block(int block_num)
{
    if(block_num>(NUMBER_OF_BLOCKS-1))
    {
        printf("Access of block beyond File System!\n");
    }
    else{
        return mem_disk[block_num];
    }
}

int write_block(char *write_data, int block_num)
{
    if(block_num>(NUMBER_OF_BLOCKS-1))
    {
        printf("Access of block beyond File System!\n");
        return -1;}
    else if((strlen(write_data))>BLOCK_SIZE)
    {
        printf("Data size exceeds block size!\n"); return -1;
    }
    {
        mem_disk[block_num]= write_data;
        return 1; 
    }
}


void clear_all()
{
    //freeing up mem disk
    for (int i=0; i<NUMBER_OF_BLOCKS; i++)
    {
       free(mem_disk[i] );
    }
    free(mem_disk);
}