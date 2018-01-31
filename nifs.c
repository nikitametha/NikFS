#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "disk_sim.h"

static const char *filepath = "/file";
static const char *filename = "file";
static const char *filecontent = "I'm the content of the only file available there\n";

static int getattr_callback(const char *path, struct stat *stbuf) {
  memset(stbuf, 0, sizeof(struct stat));
    //printf("Get attr called for path %s\n", path);
 //printf("illegal?%c\n", path[0]);

  if (strcmp(path, "/") == 0) {
    stbuf->st_mode = S_IFDIR | 0755;
    stbuf->st_nlink = 2;
   // stbuf->st_uid = t->uid;
    //stbuf->st_gid = t->gid;
    return 0;
  }

  if (strcmp(path, filepath) == 0) {
    stbuf->st_mode = S_IFREG | 0777;
    stbuf->st_nlink = 1;
    //stbuf->st_uid = t->uid;
    //stbuf->st_gid = t->gid;
    stbuf->st_size = 1024;
    return 0;
  }

    if( (strcmp(path, "/.Trash")==0) || (strcmp(path, "/.Trash-1000")==0) || (strcmp(path, "/.xdg-volume-info")==0) || (strcmp(path, "/autorun.inf")==0) || (strcmp(path, "/.gitignore")==0) || (strcmp(path, "/.git")==0) )
  {
    printf("Illegals\n");

  return -ENOENT;}

  //printf("Others?\n");
  struct meta_file_struct *t =  read_meta(path);
  stbuf->st_mode = S_IFREG | 0777;
    stbuf->st_nlink = 1;
    stbuf->st_uid = t->uid;
    stbuf->st_gid = t->gid;
    stbuf->st_size = t->size;
    return 0;
 
}

static int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler,
    off_t offset, struct fuse_file_info *fi) {
  (void) offset;
  (void) fi;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    //printf("%s----\n",path);
    filler(buf, filename, NULL, 0);
    
    char *dirinfo = malloc(100*sizeof(char)); 
    strcpy(dirinfo,read_dir("niki"));
    char*dirinfotmp= dirinfo;
    //printf("Directory disksimmm has %s\n",dirinfo);
  
    char* pch;
    
    pch = strtok (dirinfotmp,", /");
    while (pch != NULL)
    {

        ///static const char *filepath = pch;
        //printf ("pch disksimmm = %s\n",pch);
        filler(buf, pch, NULL, 0);
        pch = strtok (NULL, ", /");
    }
    free(dirinfo);

  return 0;
}

static int open_callback(const char *path, struct fuse_file_info *fi) {
  return 0;
}

static int read_callback(const char *path, char *buf, size_t size, off_t offset,
    struct fuse_file_info *fi) {

  if (strcmp(path, filepath) == 0) {
    size_t len = strlen(filecontent);
    if (offset >= len) {
      return 0;
    }

    

    
    if (offset + size > len) {
      memcpy(buf, filecontent + offset, len - offset);
      return len - offset;
    }

    memcpy(buf, filecontent + offset, size);
    return size;
  }

  char *filetext = read_file(path);
  //printf("Before memcpy\n");
  
  memcpy(buf, filetext, strlen(filetext));
  return size;

  return -ENOENT;
}

static struct fuse_operations operations = {
  .getattr = getattr_callback,
  .open = open_callback,
  .read = read_callback,
  .readdir = readdir_callback,
};



int main( int argc, char *argv[] )
{
    //printf("STARTED %s %s\n", argv[0], argv[1]);
    mknikfs(argv[2]);
	return fuse_main( argc, argv, &operations, NULL );
}
