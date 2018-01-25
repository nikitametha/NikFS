#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>
#include "uthash/src/uthash.h"
#define NUMBER_OF_BLOCKS 500
#define BLOCK_SIZE 1024
#define ISEMPTY printf("\nEMPTY LIST:");



struct file_control_block {
    char *filename;            /* we'll use this field as the key */
    int uid;
    int gid;
    int *mode;
    int size;
    //char* atime;
    //char* mtime;           
    UT_hash_handle hh; /* makes this structure hashable */
};

struct file_control_block *meta_files = NULL;



struct superblock
{
    int block_size;
    int partition_size;
    char *root_dir;
    void *free_block_list;
};

struct superblock * m_superblock;

struct node
{
    int value;
    struct node *next;
};

typedef struct node snode;
snode *newnode, *ptr, *prev, *temp;
snode *first = NULL, *last = NULL;
 
snode* create_node(int);
void insert_free_block_number(int val);
int return_free_block_number();
void display_free_block_list();



void initialize_mem_disk();
int write_block(char *write_data, int block_num);
char* read_block(int block_num);
void clear_all();

void initialize_superblock();
void initialize_file_meta(struct file_control_block *);
void initialize_freeblocklist();


char **mem_disk; //in memory buffer (simulates disk)
char **inode_info;

void main()
{
    printf("Initializing mem_disk..\n");
    printf("Block Size = %d\nNumber of blocks=%d\n", BLOCK_SIZE, NUMBER_OF_BLOCKS); 
    initialize_mem_disk();
    printf("Initializing superblock..\n");
    initialize_superblock();
    printf("Initializing file_meta..\n");
    struct file_control_block *new_meta= malloc(sizeof(struct file_control_block));
    initialize_file_meta(new_meta);
    printf("Initializing free_block_list..\n");
    initialize_freeblocklist();

    //write_block("AAAA",5);
    //write_block("AAAAwwww",908);
    //char *x = read_block(3);
    //printf("Block read - %s\n", x);
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

void initialize_superblock()
{
    m_superblock = (struct superblock *)malloc(sizeof(struct superblock));
    m_superblock->root_dir= malloc(20*sizeof(char));
    m_superblock->partition_size= NUMBER_OF_BLOCKS*BLOCK_SIZE;
    m_superblock->block_size= BLOCK_SIZE;
    m_superblock->free_block_list=NULL;

}

void initialize_file_meta(struct file_control_block *new_meta)
{
    new_meta->filename=malloc(20*sizeof(char));
    new_meta->uid=1000;
    new_meta->gid=1000;
    //new_meta->atime=NULL;
    //new_meta->mtime=NULL;
    new_meta->size=0;
    new_meta->mode= malloc(4*sizeof(char));
    new_meta->mode[0]= 0;
    new_meta->mode[0]= 7;
    new_meta->mode[0]= 7;
    new_meta->mode[0]= 7;
    


    HASH_ADD_INT( meta_files, filename, new_meta );    

}

snode* create_node(int val)
{
    newnode = (snode *)malloc(sizeof(snode));
    if (newnode == NULL)
    {
        printf("\nMemory was not allocated");
        return 0;
    }
    else
    {
        newnode->value = val;
        newnode->next = NULL;
        return newnode;
    }
}

void insert_free_block_number(int val)
{
    newnode = create_node(val);
    if (first == last && last == NULL)
    {
        first = last = newnode;
        first->next = NULL;
        last->next = NULL;
    }
    else
    {
        last->next = newnode;
        last = newnode;
        last->next = NULL;
    }
 //printf("\n----INSERTED----");
} 

int return_free_block_number()
{
    int free_block_value;
 
    if (first == NULL)
    {
        ISEMPTY;
        printf(":No free blocks\n");
    }
    else
    {
        ptr = first;
        free_block_value = ptr->value;
        first = ptr->next;  
        free(ptr);
        //printf("\nElement deleted");
        }
    return free_block_value;
}

void display_free_block_list()
{
    if (first == NULL)
    {
        ISEMPTY;
        printf(":No nodes in the list to display\n");
    }
    else
    {
        for (ptr = first;ptr != NULL;ptr = ptr->next)
        {    
            printf("%d\t", ptr->value);
        }
    }
}


void initialize_freeblocklist()
{
    for (int i=1; i<=NUMBER_OF_BLOCKS; i++)
    {
        insert_free_block_number(i);
    }
    display_free_block_list();
}


