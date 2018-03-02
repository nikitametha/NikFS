#include "disk_sim.h"

// usage 
// gcc -w disk_sim.c nifs.c -o ssfs `pkg-config fuse --cflags --libs`
//  ./ssfs -f niki

struct meta_file_struct *inode_list = NULL;
struct superblock * m_superblock;
int diskfd;


void main(int argc, char* argv[])
{
    printf("Root dir = %s\n", argv[1]);
    mknikfs(argv[1]);
}

void mknikfs(char *root)
{
    printf("Initializing disk..\n");
    int x = diskfd = open("fusedata", O_RDWR | O_CREAT, 0777);
    if(x<0)
    {
        perror("Error opening disk");
    }
    else
    {
        printf("Done\n");
    }

    printf("Block Size = %d\nNumber of blocks=%d\n", BLOCK_SIZE, NUMBER_OF_BLOCKS); 
   
    printf("Initializing superblock..\n");
    initialize_superblock(root);
    
    writenew_meta();
    initialize_meta();

    write_block("first block\n",1,0);

    write_block("file2 hereee\n",2,0);

    write_block("THIRD block\n",22,0);
    write_block("file 2 heree",3,0);

    read_file("file1",0);
    read_file("file2",0);
    read_file("file1",1024);
    //read_block(1,0);
    //read_block(2,0);
    //read_block(3,0);
}

void initialize_superblock(char *root)
{
    FILE* sbfd = fopen("superblock_info", "w+");
    //root dir, partition size, block size
    //printf("DOne\n");
    fprintf(sbfd,"%s\n", root); 
    printf("Done\n"); 
    
}


void writenew_meta()
{
    FILE* fd = fopen("metafile","w");	
	fprintf(fd, "file1\tfile1\t0\t1000\t1000\t0777\tf\t.1.22.\n");
    fprintf(fd, "file2\tfile1\t0\t1000\t1000\t0777\tf\t.2.3.\n");
	fclose(fd);
}

void initialize_meta()
{
    char *fileContent = (char *) malloc(BLOCK_SIZE);
    FILE* fd = fopen("metafile","r");
    int r;
    char filename[20], path[50];
    int size, uid, gid, mode;
    char ch;
    char listofblocks[1000];
    struct meta_file_struct *new_meta;
    printf("Going to read meta now..\n");

    while (1)
    {
        // filename path size uid gid mode d/f
        printf("Reading..\n");
        r = fscanf(fd, "%s\t%s\t%d\t%d\t%d\t%d\t%c\t%s\n", filename, path, &size, &uid, &gid, &mode, &ch, listofblocks);
        printf("Read one entry of file\n");
        new_meta = malloc(sizeof(struct meta_file_struct));
        write_file_meta(new_meta, filename, path, size, uid, gid, mode, ch, listofblocks);
        printf("Hashing file\n");
        if( r == EOF ) 
           break;
    }
}

void write_file_meta(struct meta_file_struct *new_meta, char *fname, char *pathname, int sizefile, int uid, int gid, int mode, char ch, char* listofblocks) //write meta
{
    printf("Writing\n");
     
    new_meta->filename=malloc(20*sizeof(char));
    strcpy(new_meta->filename, fname);

    new_meta->pathname=malloc(50*sizeof(char));
    strcpy(new_meta->pathname, pathname);

    new_meta->listofblocks=malloc(1000*sizeof(char));
    
  
    strcpy(new_meta->listofblocks, listofblocks);
    printf("Written all strings\n");
    new_meta->uid=1000;
    new_meta->gid=1000;
    new_meta->size=sizefile;
    new_meta->flag_d = ch;
    new_meta->mode= 0777;
    HASH_ADD_STR( inode_list, filename, new_meta );   
    printf ("%s inserted\n", new_meta->filename);

}

char* read_block(int block_num, int offset)
{   //here, offset is within this block
    char* readx = malloc(sizeof(char)*BLOCK_SIZE);
    if(block_num>(NUMBER_OF_BLOCKS-1))
    {
        printf("Access of block beyond File System!\n");
    }
    else{
    int ret = lseek(diskfd, (sizeof(char)*BLOCK_SIZE*block_num)+offset, SEEK_SET);
    if(ret<0)
    {
        perror("Lseek error");
    }
    printf("READING at offset = %d block = %d\n", ret, block_num);
    read(diskfd, readx, sizeof(char)*BLOCK_SIZE);
    printf("Read= %s\n", readx);
    return readx;
    //printf("readblock called");
        
    }
}

int write_block(char *write_data, int block_num, int offset)
{
    if(block_num>(NUMBER_OF_BLOCKS-1))
    {
        printf("Access of block beyond File System!\n");
        return -1;}
    else if((strlen(write_data))>BLOCK_SIZE)
    {
        printf("Data size exceeds block size!\n"); 
        return -1;
    }
    else
    {
    int ret = lseek(diskfd, (sizeof(char)*block_num*BLOCK_SIZE)+offset, SEEK_SET);
    if(ret<0)
    {
        perror("Lseek error");
    }
    printf("Writing at offset = %d block num= %d\n", ret, block_num);
    write(diskfd, write_data, strlen(write_data));
    //printf("Read= %s\n", x);
    }
}

struct meta_file_struct* read_meta(char* filename_tr) //filename to read
{
    //printf("in read_meta found %s\n", filename_tr);
    struct meta_file_struct *foundmeta;
    HASH_FIND_STR( inode_list, filename_tr, foundmeta );  
  
    
    return foundmeta;
}

char* read_file(char * filename,int offset)
{
    struct meta_file_struct *t =  read_meta(filename);
    int block_number = (offset/BLOCK_SIZE)+1; 
    char *blocklist = t->listofblocks;
    // 1st, 2nd, 3rd....block
    //printf("Read from %d block\n",block_number);
    char tmplob[100];
    strcpy(tmplob,blocklist);
    //printf("%s tmplob\n",tmplob);
    char blknumc[NUMBER_OF_BLOCKS];
    int j=0, k=0;
    int c=0, blknum;
    int block_th = block_number;
    
    for(int i=0; i<(strlen(tmplob)); i++)
    {

        if(tmplob[i]=='.')
        {
            block_number=block_number-1;
      //      printf("Reducing ");
        }
        if(block_number == 0)
        {
            while(tmplob[++i]!='.')
            {
        //        printf("cha %c\n",tmplob[i]);
                blknumc[j++]=tmplob[i];
                //i++;
            }

            for (i = 0; i < strlen(blknumc); i++)
            k = 10 * k + (blknumc[i]-48);
            //actual block number
            blknum = k;
          //  printf("Block is = %d\n", blknum);
            break;
        }
    }
    
    //offset within that block
    int final_offset = offset-(BLOCK_SIZE*(block_th-1));
    printf("Offset within block = %d\n", final_offset);
    return (read_block(blknum,final_offset));

}

int write_file()
{
    
}