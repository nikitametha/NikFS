#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>
#include "uthash/src/uthash.h"
#define NUMBER_OF_BLOCKS 500
#define BLOCK_SIZE 1024
#define ISEMPTY printf("\nEMPTY LIST:");


struct meta_file_struct {
    char *filename;            /* we'll use this field as the key */
    int uid;
    int gid;
    int mode;
    int size;
    void *listofblocks;
    int flag_d; //if directory or file
    int blocklocation;
    //char* atime;
    //char* mtime;           
    UT_hash_handle hh; /* makes this structure hashable */
};

struct superblock
{
    int block_size;
    int partition_size;
    char *root_dir;
    void *free_block_list;
};

struct node
{
    int value;
    struct node *next;
};

typedef struct node snode;

snode* create_node(int);
void insert_free_block_number(int val);
int return_free_block_number();
void display_free_block_list();


void mknikfs();
void initialize_mem_disk();
int write_block(char *write_data, int block_num);
char* read_block(int block_num);
void clear_all();

void create_file(char*);
char*  read_file(char *filename);

void initialize_superblock();
void write_file_meta(struct meta_file_struct *, char*, int,int,int);
void initialize_freeblocklist();
struct meta_file_struct* read_meta(char*);
void create_dir(char* dname);
void app_dir(char* dname, char* filename);


char* read_dir(char* name);


char **mem_disk; //in memory buffer (simulates disk)
char **inode_info;