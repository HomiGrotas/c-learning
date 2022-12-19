#include <stdio.h>
#include <dirent.h> 
#include <string.h>

#define MAX_PATH_LENGTH 4096
#define MAX_FILE_NAME_LENGTH 256

/**
 * @brief encrypts a file using xor encryption
 * encrypts char by char and writing to a new file. At the end,
 * removes origial file and renames the encrypted file to the original's one name
 * 
 * @param file file tp encrypt
 * @param key key to encrypt with
 */
void xorEncrypt(char file[], char key[])
{
    FILE *fileP, *xorFileP;
    char encFileName[MAX_FILE_NAME_LENGTH];
    int keySize = strlen(key), i=0, ch;
    
    strcpy(encFileName, file);
    strcat(encFileName, ".enc");
    fileP = fopen(file, "rb");
    xorFileP = fopen(encFileName, "wb+");   // create if doesn't exists
    
    // Read the input file one byte at a time, encrypting each byte
    // and writing it to the output file
    if (fileP != NULL && xorFileP != NULL)
    {
        while((ch = fgetc(fileP)) != EOF)
        {
            fputc(ch^key[i%keySize], xorFileP);
            i++;
        }
    }else{
        printf("Could't open file %s", file);
    }
    fclose(fileP);
    fclose(xorFileP);

    if (remove(file) != 0){printf("Couldnt remove file\n");}
    if (rename(encFileName, file) != 0){printf("Couldnt rename file\n");}
}


/**
 * @brief recusrisvly goes through a given directory
 * 
 * @param dirName directory to walk in
 */
void recursiveDirectory(char dirName[], char key[])
{
    char path[MAX_PATH_LENGTH];            // path in the directory (baseDir/dir1/dir2/.../file.?)
    DIR *directoryP;            // directory stream
    struct dirent *objInfoP;    // object (file/directory and etc.) in the current directory

    directoryP = opendir(dirName);  // open the directory stream
    if (directoryP != NULL)         // successful openneing
    {
        objInfoP = readdir(directoryP);     // iterate through files in directory 
        while (objInfoP != NULL)
        {
            if (strcmp(objInfoP->d_name, ".") != 0 && strcmp(objInfoP->d_name, "..") != 0)   // ensure not cuurent dir/prev one
            {
                printf("%s\n", objInfoP->d_name);

                strcpy(path, dirName);          // copy current path 
                strcat(path, "/");              // add / sign
                strcat(path, objInfoP->d_name); // add directory name to path

                if(objInfoP->d_type == DT_DIR)      // if file is a directory -> iterate through the directory
                {
                    recursiveDirectory(path, key);   // start iterating through the new directory
                }else{
                    xorEncrypt(path, key);
                }
            }
            objInfoP = readdir(directoryP);
        }
        
    }
    else{ 
        printf("Error: couldn't open directory %s\n", dirName);
    }
    closedir(directoryP);
}

int main(int argc, char *argv[])
{   
    if (argc < 3){
        printf("Usage: %s {base directory} {key}\n", argv[0]);
        return 1;
    }

    printf("Encrypting %s recursivly using key %s\n", argv[1], argv[2]);
    recursiveDirectory(argv[1], argv[2]);
    return 0;
}