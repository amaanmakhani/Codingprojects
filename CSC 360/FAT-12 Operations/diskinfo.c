#include <stdio.h>
#include <stdlib.h>
// Add utilities
#include "utility.h"
// Disk stored in utilities
// This was done to avoid continually passing disk to utility functions
extern char* disk;

// Information storage variables
char* volumeLabel;
char* OS;

// Releases heap space
void freeMemory()
{
   free(OS);
   free(volumeLabel);
}

// Disk information stored in byte 3 to 10 of the bootsector
char* getOS()
{
    OS = auto_malloc(9);
    for(int index = 3; index <= 10 ; index++)
    {
        OS[index - 3] = disk[index];
    }
    return OS;
}

// Stored in root directory where attribute is 8
char* getVolumeLabel()
{
    int index = getStartRootDirectory();
    volumeLabel = auto_malloc(9);
    while(index < getEndRootDirectory())
    {
        if(disk[index + 11] == 8)
        {
            for(int i = 0; i <= 7; i++)
            {
                volumeLabel[i] = disk[index + i];
            }
            return volumeLabel;
        }
        index += 32;
    }
    return volumeLabel;
}

void displayInformation()
{
    printf("OS Name: %s\n", getOS());
    printf("Label of the disk: %s\n", getVolumeLabel());
    // Total size of the disk = total sector count * bytes per sector
    printf("Total size of the disk: %d bytes\n", getSectorCount() * getBytesPerSector());
    // Free size of the disk = total number of sectors that are unused (i.e., 0x000 in an FAT entry means unused) * bytes per sector. First two entries in FAT are reserved.
    printf("Free size of the disk:  %d bytes\n", getFreeSpace());
    printf("==============\n");
    printf("The number of files in the disk (including all files in the root directory and files in all subdirectories): %d\n", getFileCount(getStartRootDirectory(), getEndRootDirectory()));
    printf("==============\n");
    printf("Number of FAT copies: %d\n", getFATCount());
    printf("Sectors per FAT: %d\n", getSectorsPerFAT());
}

int main(int argc, char *argv[])
{
    // Open given disk if possible
    createDisk(argc, argv);
    // Print info
    displayInformation();
    // Cleanup resources
    freeMemory();
    destroyDisk();
    return 0;
}