#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include "../uthash/src/uthash.h"

#define NUMBER_OF_BLOCKS 500
#define BLOCK_SIZE 1024
#define ISEMPTY printf("\nEMPTY LIST:");

struct meta_file_struct {
    char *filename;            /* we'll use this field as the key */
    char * pathname;
    char *listofblocks;
    int uid;
    int gid;
    int mode;
    int size;
    char flag_d; //if directory or file
    //char* atime;
    //char* mtime;           
    UT_hash_handle hh; /* makes this structure hashable */
};


void mknikfs();
void initialize_mem_disk();
int write_block(char *write_data, int block_num, int offset);
char* read_block(int block_num, int offset);
void clear_all();
void writenew_meta();


void write_file_meta(struct meta_file_struct *new_meta, char *fname, char *pathname, int sizefile, int uid, int gid, int mode, char ch, char* listofblocks); //write meta


void create_file(char*, char*, char*);


void initialize_superblock(char* root);

void initialize_freeblocklist();

void create_dir(char* dname);
void app_dir(char* dname, char* filename);

void initialize_meta();
char* read_dir(char* name);


char* read_file(char * filename,int offset);