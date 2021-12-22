#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
// Shared memory mapping variables
char* disk;
int diskReturn;
struct stat sb;
// Disk tools
void createDisk();
void destroyDisk();
// Dynamic memory function
char* auto_malloc(size_t size);
// Computational function
unsigned int convertToDecimalRep(unsigned char LSB, unsigned char MSB);
// Information functions
int getSectorCount();
int getBytesPerSector();
int getFATCount();
int getSectorsPerFAT();
int getRootEntryCount();
int getFatEntry(int index);
int getFATEntryCount();
int getStartRootDirectory();
int getEndRootDirectory();
int getPhysicalClusterStart(int logicalValue);
int getPhysicalClusterEnd(int logicalValue);
int getFileCount(int start, int end);
int getFileSize(int index);
int getLogicalCluster(int index);
int getNextDirectory(int start, int end);
int getSectorsUnused();
int getFreeSpace();