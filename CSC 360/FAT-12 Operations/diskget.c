#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> 
// Add utilities
#include "utility.h"
// Disk stored in utilities
// This was done to avoid continually passing disk to utility functions
extern char* disk;

// Information storage variables
char* filename;

char* getFileName(int index, int isDirectory)
{
    int length = 0;
    // Get filename
    for(int i = 0; i <= 7; i++)
    {
        if(disk[index + i] == 32)
        {
            break;
        }
        filename[length] = disk[index + i];
        length++;
    }
    if(isDirectory == 1)
    {
        filename[length] = '\0';
        return filename;
    }
    filename[length] = '.';
    length++;
    // Get extension
    for(int i = 0; i < 3; i++)
    {
        if(disk[index + i + 8] == 32)
        {
            break;
        }
        filename[length] = disk[index + i + 8];
        length++;
    }
    filename[length] = '\0';
    return filename;
}

int existsInRoot(char* expectedFile)
{
    int index = getStartRootDirectory();
    while(index < getEndRootDirectory())
    {
        if(disk[index] == 0)
        {
            // No more directory entries
            break;
        }
        if(getLogicalCluster(index) < 2)
        {
            // Points to root directory not a file to be copied
            index += 32;
            continue;
        }
        if((disk[index + 11] & 0x10) != 16)
        {
            getFileName(index, 0);
            // Convert to upper case name as stored file names are upper case
            for (int i = 0; i < 12 && i < strlen(expectedFile); i++)
            {
                expectedFile[i] = toupper(expectedFile[i]);
            }
            // Compare in a case insensitive manner
            if(strncmp(expectedFile, filename, strlen(filename)) == 0 && strlen(filename) == strlen(expectedFile))
            {
                return index;
            }
        }
        index += 32;
    }
    printf("File not found.\n");
    exit(1);
}

// Follows fat entries to write file of specified size
void exportFile(int logicalCluster, FILE* fp, int fileSize)
{
    int currentSize = 0;
    while (logicalCluster < 4088 && currentSize < fileSize)
    {
        int index = getPhysicalClusterStart(logicalCluster);
        for (int i = 0; i < getBytesPerSector() && currentSize < fileSize; i++)
        {
            fputc(disk[index + i], fp);
            currentSize++;
        }
        logicalCluster = getFatEntry(logicalCluster);
    }
}

int main (int argc, char *argv[])
{
    // Ensure filename provided
    if(argc != 3)
    {
        printf("Error: filename must be provided\n");
        exit(1);
    }
    // Open given disk if possible
    createDisk(argc, argv);
    // Create filename for use by getfile name
    // Intended to prevent numerous mallocs
    filename = auto_malloc(12);
    // Check for file existence
    int index = existsInRoot(argv[2]);
    FILE* fp = fopen(argv[2], "w");
    // Write to file on local system
    exportFile(getLogicalCluster(index), fp, getFileSize(index));
    // Cleanup resources
    free(filename);
    destroyDisk();
}