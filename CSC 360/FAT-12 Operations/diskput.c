#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "utility.h"
// Disk stored in utilities
// This was done to avoid continually passing disk to utility functions
extern char* disk;
// Max directory depth is 4096
char path[4096][9];
int pathIndex = 0;

// Information storage variables
char* filename;

void addDirectoryEntry(int index, int firstCluster, struct stat* fileinfo, char* filename)
{
    // Get next empty directory
    while(disk[index] != 0)
    {
        index +=32;
    }
    // Fill all with spaces first
    for(int i = 0; i < 11; i++)
    {
        disk[index + i] = 0x20;
    }
    // Overwrite spaces with text until extension discovered
    int i = 0;
    for(; i < 8; i++)
    {
        if(filename[i] == '.')
        {
            break;
        }
        disk[index + i] = toupper(filename[i]);
    }
    i++;
    // Write extension from where it left off when writing the file name
    for(int j = 0; j < 3; j++)
    {
        disk[index + 8 + j] = toupper(filename[i]);
        i++;
    }
    // Set the creation time of the disk file to the last write time of the linux file
    time_t mTime = fileinfo->st_mtime;
    struct tm *breakdown = localtime(&mTime);
    int year = breakdown->tm_year + 1900;
    int month = breakdown->tm_mon + 1;
    int day = breakdown->tm_mday;
    int hour = breakdown->tm_hour;
    int minute = breakdown->tm_min;
    // Write creation time
    disk[index + 14] |= ((minute << 5) & 0xE0);
    disk[index + 15] |= ((minute >> 3) & 0x07);
    disk[index + 15] |= ((hour << 3) & 0xF8);
    // Write creation date
    disk[index + 16] |= (day & 0x1F);
    disk[index + 16] |= ((month << 5) & 0xE0);
    disk[index + 17] |= ((month >> 3) & 0x01);
    disk[index + 17] |= (((year - 1980) & 0x7F) << 1);
    // Last write time
    disk[index + 22] |= ((minute << 5) & 0xE0);
    disk[index + 23] |= ((minute >> 3) & 0x07);
    disk[index + 23] |= ((hour << 3) & 0xF8);
    // Last write date
    disk[index + 24] |= (day & 0x1F);
    disk[index + 24] |= ((month << 5) & 0xE0);
    disk[index + 25] |= ((month >> 3) & 0x01);
    disk[index + 25] |= (((year - 1980) & 0x7F) << 1);
    // Write cluster
    disk[index + 26] = (firstCluster & 0x00FF);
    disk[index + 27] = (firstCluster & 0xFF00) >> 8;
    // Write file size
    disk[index + 28] = (fileinfo->st_size & 0x000000FF);
    disk[index + 29] = (fileinfo->st_size & 0x0000FF00) >> 8;
    disk[index + 30] = (fileinfo->st_size & 0x00FF0000) >> 16;
    disk[index + 31] = (fileinfo->st_size & 0xFF000000) >> 24;
}

void writeFatEntry(int index, int entry, int offset)
{
    if(index % 2 == 0)
    {
        disk[offset + (int)((3 * index) / 2) + 1] = ((entry >> 8) & 0x0F) +  (disk[offset + (int)((3 * index) / 2) + 1] & 0xF0);
        disk[offset + (int)((3 * index) / 2)] = entry & 0xFF;
    }
    else
    {
        disk[offset + (int)((3 * index) / 2)] = ((entry << 4) & 0xF0) + (disk[offset + (int)((3 * index) / 2)] & 0x0F);
        disk[offset + (int)((3 * index) / 2) + 1] = ((entry >> 4) & 0xFF);
    }
}

int getnextFreeFAT(int start)
{
    for(int i = start + 1; i < getFATEntryCount(); i++)
    {
        if(getFatEntry(i) == 0)
        {
            return i;
        }
    }
    return -1;
}

int updateFATs(int filesize)
{
    int numberFATs = (filesize/getBytesPerSector());
    int cluster =  getnextFreeFAT(0);
    int firstCluster = cluster;
    int nextCluster = 0;
    for(int i = 0; i < numberFATs; i++)
    {
        nextCluster = getnextFreeFAT(cluster);
        // Write to FAT1
        writeFatEntry(cluster, nextCluster, getBytesPerSector());
        // Write to FAT2
        writeFatEntry(cluster, nextCluster, getBytesPerSector() * (getSectorsPerFAT() + 1));
        cluster = nextCluster;
    }
    // Write to FAT1
    writeFatEntry(cluster, 0xFFF, getBytesPerSector());
    // Write to FAT2
    writeFatEntry(cluster, 0xFFF, getBytesPerSector() * (getSectorsPerFAT() + 1));
    return firstCluster;
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

void copy(int logicalCluster, FILE* fp, int fileSize)
{
    // Write using FAT chain indexes and while file size not reached
    int currentSize = 0;
    while (logicalCluster < 4088 && currentSize < fileSize)
    {
        int index = getPhysicalClusterStart(logicalCluster);
        for (int i = 0; i < getBytesPerSector() && currentSize < fileSize; i++)
        {
            disk[index + i] = fgetc(fp);
            currentSize++;
        }
        logicalCluster = getFatEntry(logicalCluster);
    }
}

int directoryExists(int start, int end, int depth)
{
    while(start < end)
    {
        if(disk[start] == 0)
        {
            // No more directory entries
            return 0;
        }
        if(getLogicalCluster(start) < 2)
        {
            // Points to root directory not a file to be copied
            start += 32;
            continue;
        }
        if((disk[start + 11] & 0x10) == 16)
        {
            getFileName(start, 1);
            // Convert to upper case name as stored file names are upper case
            for (int i = 0; i < 11 && i < strlen(path[depth]); i++)
            {
                path[depth][i] = toupper(path[depth][i]);
            }
            // Compare in a case insensitive manner
            if(strncmp(path[depth], filename, strlen(filename)) == 0 && strlen(filename) == strlen(path[depth]))
            {
                break;
            }
        }
        start += 32;
    }
    if(depth == pathIndex - 1)
    {
        // Return start of directory, indicates all directories exist
        return getPhysicalClusterStart(getLogicalCluster(start));
    }
    else
    {
        // If more directories continue transvering
        return directoryExists(getPhysicalClusterStart(getLogicalCluster(start)), getPhysicalClusterStart(getLogicalCluster(start)) + getBytesPerSector(), depth + 1);
    }
}

int fileExists(int start, char* fileGiven)
{
    int end = start + getBytesPerSector();
    if(start < getEndRootDirectory())
    {
        end = getEndRootDirectory();
    }
    // This allows filename to be copied without affecting the original copy
    char expectedFile[13];
    for (int i = 0; i < 13; i++)
    {
        expectedFile[i] = fileGiven[i];
    }
    while(start < end)
    {
        if(disk[start] == 0)
        {
            // No more directory entries
            break;
        }
        if(getLogicalCluster(start) < 2)
        {
            // Points to root directory not a file to be copied
            start += 32;
            continue;
        }
        if((disk[start + 11] & 0x10) != 16)
        {
            getFileName(start, 0);
            // Convert to upper case name as stored file names are upper case
            for (int i = 0; i < 11 && i < strlen(expectedFile); i++)
            {
                expectedFile[i] = toupper(expectedFile[i]);
            }
            // Compare in a case insensitive manner
            if(strncmp(expectedFile, filename, strlen(filename)) == 0 && strlen(filename) == strlen(expectedFile))
            {
                return 1;
            }
        }
        start += 32;
    }
    return 0;
}

void parsePath(char *argv)
{
    // Start from 1 to ignore first /
    int length = 0;
    for(int i = 1; i < strlen(argv); i++)
    {
        // Directory entry ended
        if(argv[i] == '/')
        {
            pathIndex++;
            path[pathIndex][length] = '\0';
            length = 0;
        }
        else
        {
            path[pathIndex][length] = argv[i];
            length++;
        }
    }
    // Terminate last entry
    path[pathIndex][length] = '\0';
}

int main(int argc, char *argv[])
{
    FILE *fp;
    struct stat fileinfo;
    // Create filename for use by getfile name
    // Intended to prevent numerous mallocs
    filename = auto_malloc(12);
    // Open given disk if possible
    createDisk(argc, argv);
    // Ensure filename provided
    if(argc != 3)
    {
        printf("Error: file path must be provided\n");
        exit(1);
    }
    // Check if the specified directory exists
    int directoryEntryIndex = getStartRootDirectory();
    if(argv[2][0] == '/')
    {
        parsePath(argv[2]);
        if((directoryEntryIndex = directoryExists(getStartRootDirectory(), getEndRootDirectory(), 0)) == 0)
        {
            printf("The directory not found.\n");
            exit(1);
        }
    }
    else
    {
        // Copy filename
        int i = 0;
        for(; i < strlen(argv[2]); i++)
        {
            path[pathIndex][i] = argv[2][i];
        }
        path[pathIndex][i + 1] = '\0';
    }
    // Check if the file already exists on the disk
    if(fileExists(directoryEntryIndex, path[pathIndex]) == 1)
    {
        printf("File already exists unable to overwrite the file.\n");
        exit(1);
    }
    // Get information for the file to copy
    stat(path[pathIndex], &fileinfo);
    // Check if the file system has enough free space
    if(fileinfo.st_size > getFreeSpace())
    {
        printf("Not enough free space in the disk image.\n");
        exit(1);
    }
    // Open file from the current linux directory
    if((fp = fopen(path[pathIndex], "r")) == NULL)
    {
        printf("File not found.\n");
        exit(1);
    }
    // Note: FAT uses Little Endian format so all integer values were converted to Little Endian format before writing them to the disk.
    // Update FAT1 and FAT2
    int firstCluster = updateFATs(fileinfo.st_size);
    // Add a directory entry
    addDirectoryEntry(directoryEntryIndex, firstCluster, &fileinfo, path[pathIndex]);
    // Put file on disk
    copy(firstCluster, fp, fileinfo.st_size);
    // Cleanup resources
    destroyDisk();
}