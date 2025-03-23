#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 *   ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ 
 *  |   |   |   |   |                       |   |
 *  | 0 | 1 | 2 | 3 |     .....             |127|
 *  |___|___|___|___|_______________________|___|
 *  |   \    <-----  data blocks ------>
 *  |     \
 *  |       \
 *  |         \
 *  |           \
 *  |             \
 *  |               \
 *  |                 \
 *  |                   \
 *  |                     \
 *  |                       \
 *  |                         \
 *  |                           \
 *  |                             \
 *  |                               \
 *  |                                 \
 *  |                                   \
 *  |                                     \
 *  |                                       \
 *  |                                         \
 *  |                                           \
 *  |     <--- super block --->                   \
 *  |______________________________________________|
 *  |               |      |      |        |       |
 *  |        free   |      |      |        |       |
 *  |       block   |inode0|inode1|   .... |inode15|
 *  |        list   |      |      |        |       |
 *  |_______________|______|______|________|_______|
 *
 *
 */


#define FILENAME_MAXLEN 8  // including the NULL char
#define MAX_PATH_LENGTH 256
#define SUPERBLOCK_SIZE 128 // blocks

/* 
 * inode 
 */

typedef struct inode {
  int  dir;  // boolean value. 1 if it's a directory.
  char name[FILENAME_MAXLEN];
  int  size;  // actual file/directory size in bytes.
  int  blockptrs [8];  // direct pointers to blocks containing file's content.
  int  used;  // boolean value. 1 if the entry is in use.
  int  rsvd;  // reserved for future use
} inode;

// struct inode inodes[16];


/* 
 * directory entry
 */

typedef struct dirent {
  char name[FILENAME_MAXLEN];
  int  namelen;  // length of entry name
  int  inode;  // this entry inode index
  // struct inode* subdirectories[8];
} dirent;

// Superblock
typedef struct superblock
{
  int free_block_list[128];
  struct inode inode_table[16];
} superblock;


/*
*  Helper functions
*/ 
// Parsing the filepath
char** parse(const char FilePath[]) {
    char *dir_or_file;
    char* directories[16];  // Maximum 16 directories in the path.
    int DirNo = 0;

    dir_or_file = strtok(FilePath, "/");
    while (dir_or_file != NULL) {
        directories[DirNo] = strdup(dir_or_file); // Allocate memory and copy the token
        dir_or_file = strtok(NULL, "/"); // Get the next directory or file
        DirNo++;
    }
    return directories;
}


// Count Directories in FilePath
int CountFilePath(char* directories[]) {
    int i = 0;
    int DirNo = 0; // Initialize DirNo to 0
    while (directories[i] != NULL) {
        DirNo++;
        i++;
    }
    return DirNo;
}


// check if there is space in data block
int check_datablock_space(struct dirent DataBlock[]){
    int DataBlockIndex = -1;
    for(int i = 1; i < 127; i++){            // i = 1 bec 0 per home stored hoga
      if(DataBlock[i].inode == -1){
        DataBlockIndex = i;
        break;
      }
    }
    return DataBlockIndex;
}

// check if there is a free available inode
int check_free_inode(struct superblock* sb){
  int FreeInodeIndex = -1;
  for (int i = 0; i < 16; i ++){
    if(!sb->inode_table[i].used){
      FreeInodeIndex = i;
      break;
    }
  }
  return FreeInodeIndex;
}

// check if there is space in the blockptrs array
int check_free_blockpointers(int blockptrs[8]){
  int BlockptrsIndex = -1;
  for(int i = 0; i < 8; i++){
    if(blockptrs[i] == -1){
      BlockptrsIndex = i;
      break;
    }
    return BlockptrsIndex;
  }
}

// returns the parent inode
int fetch_inode(char directory[], struct superblock* sb){
  int InodeParentDir = -1;
  for(int i = 0; i < 16; i++){
    if(strcmp(sb->inode_table[i].name, directory) == 0 && sb->inode_table[i].dir && sb->inode_table[i].used){
      InodeParentDir = i;
    }
  }
  return InodeParentDir;
}

/*
 * functions
 */
// create file
void CreateFile(char FilePath1[], int fileSize){
  printf("CreateFile\n");
  // check if file already exists
  // check if there is enough space
  // find a free inode
  // find free blocks
  // update inode
  // update directory
  // update superblock
  // update free block list
  // update free inode list
  // update file size
  // update file name
  // update file block pointers
  // update directory entry
  // update superblock
  // update free block list
  // update free inode list
}
// copy file
void Copy(char FilePath1[], char FilePath2[]){
  printf("Copy\n");
}
// remove/delete file
void DeleteFile(char FilePath1[]){
  printf("DeleteFile\n");
}
// move a file
void MoveFile(char FilePath1[], char FilePath2[]){
  printf("MoveFile\n");
}
// list file info
void ListFiles(){
  printf("ListFiles\n");
}

// create directory
void CreateDirectory(char FilePath1[], struct superblock* sb, struct dirent DataBlock[]){
  printf("\n");
  printf("CreateDirectory\n");

  // check if directory already exists
  for (int i = 0; i < 16; i++){
    if (sb->inode_table[i].dir && sb->inode_table[i].used && strcmp(sb->inode_table[i].name, FilePath1) == 0){
    printf("Directory already exists.\n");
    break;
    }
  }

  // parsing the file path
  // char* directories[16] = parse(FilePath1);
  char* dir_or_file;
  char* directories[16];  // Maximum 16 directories in the path.
  int DirNo = 0;

  dir_or_file = strtok(FilePath1, "/");
  while (dir_or_file != NULL) {
      directories[DirNo] = strdup(dir_or_file); // Allocate memory and copy the token
      dir_or_file = strtok(NULL, "/"); // Get the next directory or file
      DirNo++;
  }
  int DirNo = CountFilePath(directories);

  // Since we start traversing from root, the current inode is set as 0
  // int InodeCurrent = 0;
  // dirent d;
  for(int i = 0; i < DirNo ; i++){
    // root directory
    if(DirNo == 1){
      sb->inode_table[0].used = 1;
      sb->inode_table[0].dir = 1;                                    // since it is a directory
      strcpy(sb->inode_table[0].name, directories[0]);       // copy the name of the directory
      sb->inode_table[0].rsvd = 1;                                    // since it is a directory
      DataBlock[0].inode = 0;
      strcpy(DataBlock[0].name, directories[0]);
      DataBlock[0].namelen = strlen(directories[0]);
      sb->free_block_list[0] = 1;
    }
    else if (i == DirNo -2){ // jis directory main insert karna hai
      int exists = fetch_inode(directories[i], sb);   // 'exists' holds the inode of the parent directory
      if(exists != -1){      // parent directory does exist
        int BlockptrsIndex = check_free_blockpointers(sb->inode_table[exists].blockptrs);  // free blockptr index
        int DatablockIndex = check_datablock_space(DataBlock);                             // free Datablock index
        int ChildInode = check_free_inode(sb);
        if(ChildInode != -1){
          if(BlockptrsIndex != -1){
            if(DatablockIndex != -1){
              DataBlock[DatablockIndex].inode = ChildInode;                      // Storing inode in DataBlock ka free index
              sb->inode_table[exists].blockptrs[BlockptrsIndex] = DatablockIndex; // Storing Data Block index (in which inode is stored) to blockptr array of parent directory 
              
              // updating datablock
              strcpy(DataBlock[DatablockIndex].name, directories[i+1]);
              DataBlock[DatablockIndex].namelen = strlen(directories[i+1]);
              
              // updating superblock
              // updating inode table
              strcpy(sb->inode_table[ChildInode].name, directories[i+1]);
              sb->inode_table[ChildInode].used = 1;
              sb->inode_table[ChildInode].dir = 1;
              sb->inode_table[ChildInode].rsvd = 1; 
              //updating free block list
              sb->free_block_list[DatablockIndex] = 1;
            }
            else printf("No space in DataBlock");
          }
          else printf("No space on Block Pointers of Parent directory %s.", directories[i]);
        }
        else printf("No free Inode available");
      }
      else{
        printf("The parent directory %s does not exist", directories[i]);
      }
    }
    }
  //     int check = 0;
  //     int Current_se_pehle = InodeCurrent;
  //       if(sb->inode_table[InodeCurrent].used && sb->inode_table[InodeCurrent].dir && (strcmp(sb->inode_table[InodeCurrent].name, directories[i]) == 0)){
  //         check = 1;        // A dir or file in path has been found during the traversal 
  //         // come to this again
  //         printf("Check: %d\n", check);
  //                // InodeCurrent is now equal to the inode of first sub directory
  //         // InodeCurrent = 1;       // InodeCurrent is now equal to the inode of first sub directory
  //       }
  
      // else{
      //   int j = 0;
      //   while(!check && j < 8){
      //     j++;
      //     InodeCurrent = sb->inode_table[Current_se_pehle].blockptrs[j];
      //     if(sb->inode_table[InodeCurrent].used && sb->inode_table[InodeCurrent].dir && (strcmp(sb->inode_table[InodeCurrent].name, directories[i]) == 0)){
      //       check = 1;        // A dir or file in path has been found during the traversal
      //       InodeCurrent = sb->inode_table[InodeCurrent].blockptrs[0];       // InodeCurrent is now equal to the inode of first sub directory
      //     }
      //   }
      // }
    // printf("Current Inode: %d\n", InodeCurrent);
    
}
// remove a directory
void RemoveDirectory(char FilePath1[]){
  printf("RemoveDirectory\n");
}

// delete directory
void DeleteDirectory(char FilePath1[]){
  printf("DeleteDirectory\n");

}


/*
 * main
 */
int main (int argc, char* argv[]) {
  // memory allocation for filesystem
  void* filesystem = malloc(128 * 1024);

  // memory allocation for superblock
  struct superblock* sb = (struct superblock*) filesystem;  // sb (object pointer) points to the beginning of the memory block allocated for the superblock structure within the file system
  
  // memory allocation for data block
  void* db = (void*) (filesystem + 1024);  

  struct dirent DataBlock[127];
  for (int i = 1; i < 127; i++){
    DataBlock[i].inode = -1;
  }
  // struct inode* inodes = (struct inode*) (filesystem + 128);  // stores pointer to a structure of type inode
  // struct dirent* dirent = (struct dirent*) (filesystem + 128);  // stores pointer to a structure of type inode

  // initializing the free blocks in superblock with 0 since no blocks are used
  for (int i = 1; i < 128; i++){
    sb->free_block_list[i] = 0;
  }

  // initializing the inode table in superblock with 0 since no inodes are used
  for (int i =0; i < 16; i++){
    sb->inode_table[i].used = 0;
    for (int j = 0; j < 8; j++){
      sb->inode_table[i].blockptrs[j] = -1;
    }
  }

  


  // while not EOF
    // read command
    FILE *SampleInput = fopen("sampleinput.txt", "r");
    if(SampleInput == NULL){
      printf("The file does not exist.\n");
      return 0;
    }
    // parse command and call appropriate function
    char command[3], FilePath1[MAX_PATH_LENGTH], FilePath2[MAX_PATH_LENGTH];
    int FileSize;
    while(fscanf(SampleInput, "%s", command) != EOF){
      if(strcmp(command, "CD") == 0){
        fscanf(SampleInput, "%s", FilePath1);
        CreateDirectory(FilePath1, sb, DataBlock);
      }
      else if(strcmp(command, "CR") == 0){
        fscanf(SampleInput, "%s %d", FilePath1, &FileSize);
        CreateFile(FilePath1, FileSize);
      }
      else if(strcmp(command, "CP") == 0){
        fscanf(SampleInput, "%s %s", FilePath1, FilePath2);
        Copy(FilePath1, FilePath2);
      }
      else if(strcmp(command, "DL") == 0){
        fscanf(SampleInput, "%s", FilePath1);
        DeleteFile(FilePath1);
      }
      else if(strcmp(command, "MV") == 0){
        fscanf(SampleInput, "%s %s", FilePath1, FilePath2);
        MoveFile(FilePath1, FilePath2);
      }
      else if(strcmp(command, "DD") == 0){
        fscanf(SampleInput, "%s", FilePath1);
        DeleteDirectory(FilePath1);
      }
      else if(strcmp(command, "LL") == 0){
        ListFiles();
      }
    }

  // Checking the functions
  for(int i = 0; i < 16; i++){
    if(sb->inode_table[i].used == 0)break;
    printf("Inode no: %d\n", i);\
    printf("Dir: %d\n", sb->inode_table[i].dir);
    printf("Used: %d\n", sb->inode_table[i].used);
    printf("Name: %s\n", sb->inode_table[i].name);
    printf("Blockptrs: %d %d %d %d %d %d %d %d\n", sb->inode_table[i].blockptrs[0],sb->inode_table[i].blockptrs[1],sb->inode_table[i].blockptrs[2],sb->inode_table[i].blockptrs[3],sb->inode_table[i].blockptrs[4],sb->inode_table[i].blockptrs[5],sb->inode_table[i].blockptrs[6],sb->inode_table[i].blockptrs[7]);
    printf("\n");
  }
  for(int i = 0; i < 127; i++){
    if(DataBlock[i].inode == -1)break;
    printf("Inode No from db: %d\n", DataBlock[i].inode);
    printf("Name from db: %s\n", DataBlock[i].name);
    printf("Namelen from db: %d\n", DataBlock[i].namelen);
    printf("\n");
  }


    // free memory
    free(filesystem);

	return 0;
}


  