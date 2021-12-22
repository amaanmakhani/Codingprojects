#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// Add utilities
#include "utility.h"
// Disk stored in utilities
// This was done to avoid continually passing disk to utility functions
extern char* disk;

// Information storage variables
char* filename;
// Max directory depth is 4096
char parentDirectories[4096][9];

void printMonth(int num)
{
    switch(num)
    {
        case 1:
            printf("Jan");
            return;
        case 2:
            printf("Feb");
            return;
        case 3:
            printf("Mar");
            return;
        case 4:
            printf("Apr");
            return;
        case 5:
            printf("May");
            return;
        case 6:
            printf("Jun");
            return;
        case 7:
            printf("Jul");
            return;
        case 8:
            printf("Aug");
            return;
        case 9:
            printf("Sep");
            return;
        case 10:
            printf("Oct");
            return;
        case 11:
            printf("Nov");
            return;
        case 12:
            printf("Dec");
            return;
        default:
            printf("Invalid Month");
            return;
    }
}

void printDateTime(int index){
    int hours, minutes, day, month, year;
    // Year is stored as a value since 1980
    // Year is stored in the high seven bits
    year = ((disk[index + 17] & 0xFE) >> 1) + 1980;
    // Month is stored in the middle four bits
    month = ((disk[index + 16] & 0xE0) >> 5);
    month += ((disk[index + 17] & 0x01) << 3);
    // Day is stored in the low five bits
    day = (disk[index + 16] & 0x1F);
    printMonth(month);
    printf(" %02d %02d ", day, year);
    // Hours are stored in the high five bits
    hours = ((disk[index + 15] & 0xF8) >> 3);
    // Minutes are stored in the middle 6 bits
    minutes = ((disk[index + 14] & 0xE0) >> 5);
    minutes += ((disk[index + 15] & 0x7) << 3);
    printf("%02d:%02d\n", hours, minutes);
}

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

void printFileInformation(int index)
{
    getFileName(index, 0);
    printf("F %10d %20s ", getFileSize(index), filename);
    printDateTime(index);
}

void printDirectoryInformation(int index)
{
    getFileName(index, 1);
    printf("D          0 %20s ", filename);
    printDateTime(index + 8);
}

void printStructure(int start, int end, int depth)
{
    int numSubDirectories = 0;
    if(depth != 0)
    {
        for(int i = 0; i < depth; i++)
        {
            printf("/%s", parentDirectories[i]);
        }
    }
    printf("\n==================\n");
    int index = start;
    while(index < end)
    {
        if(disk[index] == 0)
        {
            // No more directory entries
            break;
        }
        if(getLogicalCluster(index) < 2)
        {
            // Points to the root directory, not included as directories we need to list
            index += 32;
            continue;
        }
        if((disk[index + 11] & 0x10) == 16)
        {
            // . and .. not included as directories we need to list
            if(getFileName(index, 0)[0] != '.')
            {
                printDirectoryInformation(index);
                numSubDirectories++;
            }
        }
        else
        {
            printFileInformation(index);
        }
        index += 32;
    }
    // Recursive directory transversal
    // Depth is used to store level of recursion and saves directory name to certain depth
    index = start;
    for(int dir = 0; dir < numSubDirectories; dir++)
    {
        index = getNextDirectory(index, end);
        int logicalCLuster = getLogicalCluster(index);
        getFileName(index, 1);
        strncpy(parentDirectories[depth], filename, 9);
        printStructure(getPhysicalClusterStart(logicalCLuster), getPhysicalClusterEnd(logicalCLuster), depth + 1);
    }
}

int main (int argc, char *argv[])
{
    // Open given disk if possible
    createDisk(argc, argv);
    // Create filename for use by getfile name
    // Intended to prevent numerous mallocs
    filename = auto_malloc(12);
    // Print layout
    printf("Root");
    printStructure(getStartRootDirectory(), getEndRootDirectory(), 0);
    // Cleanup resources
    free(filename);
    destroyDisk();
    return 0;
}