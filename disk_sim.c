
#include "disk_sim.h"

struct meta_file_struct *inode_list = NULL;
struct superblock * m_superblock;

snode *newnode, *ptr, *prev, *temp;
snode *first = NULL, *last = NULL;
 

/*
void main()
{
    mknikfs();
}
*/
void mknikfs(char *root)
{
    printf("Initializing mem_disk..\n");
    printf("Block Size = %d\nNumber of blocks=%d\n", BLOCK_SIZE, NUMBER_OF_BLOCKS); 
    initialize_mem_disk();
    
    printf("Initializing root directory meta..\n");
    //struct meta_file_struct *new_meta= malloc(sizeof(struct meta_file_struct));
    //write_file_meta(new_meta, root, return_free_block_number(),0,0 );
   
    printf("Initializing free_block_list..\n");
    initialize_freeblocklist();
    printf("Initializing superblock..\n");
    initialize_superblock();
    printf("creating root dir..\n");
    create_dir(root);
    struct meta_file_struct *t =  read_meta(root);
    printf("Filename = %s\n",  t->filename);
    create_file("/File1");
    create_file("/File2");
    create_file("/File3");

    //dirinfo = read_dir(root);
    //printf("Directory disksimmm2 has %s\n",dirinfo);
    


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

    //printf("readblock called");
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
    m_superblock->free_block_list=first;

}

void write_file_meta(struct meta_file_struct *new_meta, char *fname, int blockloc, int sizefile, int flagg) //write meta
{
    new_meta->filename=malloc(20*sizeof(char));
    new_meta->listofblocks=NULL;
    strcpy(new_meta->filename, fname);
    new_meta->uid=1200;
    new_meta->gid=1000;
    new_meta->listofblocks=NULL;
    new_meta->flag_d= flagg;
    //new_meta->atime=NULL;
    //new_meta->mtime=NULL;
    new_meta->blocklocation=blockloc;
    new_meta->size=sizefile;
    //new_meta->mode= malloc(4*sizeof(char)); changed mode from int* to int
    new_meta->mode= 0777;
    //write_block("",0);


    HASH_ADD_STR( inode_list, filename, new_meta );   
    //printf ("%s inserted\n", new_meta->filename);

}

struct meta_file_struct* read_meta(char* filename_tr) //filename to read
{
    //printf("in read_meta found %s\n", filename_tr);
    struct meta_file_struct *foundmeta;
    HASH_FIND_STR( inode_list, filename_tr, foundmeta );  
  
    
    return foundmeta;
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

int return_free_block_number() //deleting from free block list
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
    //display_free_block_list();
}

void listofblocks_for_meta(struct meta_file_struct *xmeta)
{
    //TODO

}

void create_file(char *filename)
{
    int x = return_free_block_number();
    
    
    char* text = "FILEEEE HEREE";
  
    struct meta_file_struct *new_meta= malloc(sizeof(struct meta_file_struct));
    char *filetxt = malloc(strlen(text) + 1);
    strcpy(filetxt,text);
    int flag= 1; //file
    write_file_meta(new_meta,filename,x,strlen(text),flag);
    write_block(filetxt,x);
    app_dir("niki",filename);
    printf("Created file %s\n", filename);

}

char* read_file(char *filename)
{
    struct meta_file_struct *t =  read_meta(filename);
    int blockloc= t->blocklocation;
    printf("blockloc file = %d\n",blockloc);
    return (read_block(blockloc));   
}

void create_dir(char* dname)
{
    int x = return_free_block_number();
    //printf("dddddone\n");
    
    char* text = "";
  
    struct meta_file_struct *new_meta= malloc(sizeof(struct meta_file_struct));
    char *filetxt = malloc(strlen(text) + 1);
    strcpy(filetxt,text);
    int flag= 0; //directory
    write_file_meta(new_meta,dname,x,strlen(text),flag);
    write_block(filetxt,x);
    printf("Created Directory %s\n",dname);
}

char* read_dir(char* name)
{
    struct meta_file_struct *t =  read_meta(name);
    int blockloc= t->blocklocation;
    printf("blockloc dir  = %d\n",blockloc);
    return (read_block(blockloc));   

}

void app_dir(char* dname, char* filename)
{
    char* dir_info= read_dir(dname);
    printf("Appending to directory\n");
    char *dirtxt = malloc(strlen(dir_info) + strlen(filename)+1);
    strcat(dir_info,",");
    //printf("dirinfo== %s\n", dir_info);
    dirtxt = strcat(dir_info,filename);
    //printf("dirtxt== %s\n", dirtxt);

}