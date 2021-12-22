#include "utility.h"

void createDisk(int argc, char *argv[])
{
    diskReturn = 0;
    // Check the disk is given
    if(argc < 2)
    {
        printf("Error: disk image must be provided\n");
        exit(1);
    }
    // Open disk image in RW mode
    if((diskReturn = open(argv[1], O_RDWR)) == -1)
    {
        printf("Error: can't open %s\n", argv[1]);
        exit(1);
    }
    // Get starting point of the mapped memory
    fstat(diskReturn, &sb);
    // Map shared memory in RW mode
    disk = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, diskReturn, 0);
    if (disk == MAP_FAILED)
    {
        printf("Error: failed to map memory\n");
        exit(1);
    }
}

void destroyDisk()
{
    // Unmap shared memory
    if(munmap(disk, sb.st_size) == -1)
    {
        printf("Error: failed to unmap memory\n");
        exit(1);
    }
    // Close file
    if(close(diskReturn) == -1)
    {
        printf("Error: failed to close file\n");
        exit(1);
    }
}

// Allocate memory using malloc and check for success 
char* auto_malloc(size_t size){
    char* ptr = (char*)malloc(size);
    if (ptr == NULL)
    {
        printf("Malloc failed");
        exit(1);
    }
    ptr[size - 1] = '\0';
    return ptr;
}

// Converts from little endian to big endian
unsigned int convertToDecimalRep(unsigned char LSB, unsigned char MSB)
{
    unsigned int ret = (MSB & 0xFF) << 8;
    return ret + (unsigned int)LSB;
}

// Disk information stored in byte 19 and 20 of the bootsector
int getSectorCount()
{
    return convertToDecimalRep(disk[19], disk[20]);
}

// Disk information stored in byte 11 and 12 of the bootsector
int getBytesPerSector()
{
    return convertToDecimalRep(disk[11], disk[12]);
}

// Disk information stored in byte 16 of the bootsector
int getFATCount()
{
    return disk[16];
}

// Disk information stored in byte 22 and 23 of the bootsector
int getSectorsPerFAT()
{
    return convertToDecimalRep(disk[22], disk[23]);
}

// Disk information stored in byte 17 and 18 of the bootsector
int getRootEntryCount()
{
    return convertToDecimalRep(disk[17], disk[18]);
}

// Get fat entry value, 3 byte entries, different shift if odd or even index
int getFatEntry(int index)
{
    if(index % 2 == 0)
    {
        int entry = (disk[getBytesPerSector() + (int)((3 * index) / 2) + 1] & 0x0F) << 8;
        return entry + (disk[getBytesPerSector() + (int)((3 * index) / 2)] & 0xFF);
    }
    else
    {
        int entry = (disk[getBytesPerSector() + (int)((3 * index) / 2)] & 0xF0) >> 4;
        return entry + ((disk[getBytesPerSector() + (int)((3 * index) / 2) + 1] & 0xFF) << 4);
    }
}

int getFATEntryCount()
{
    return (getSectorCount() + 2) - (1 + (getFATCount() * getSectorsPerFAT()) + (getRootEntryCount()/16));
}

int getStartRootDirectory()
{
    return getBytesPerSector() * ((getFATCount() * getSectorsPerFAT()) + 1);
}

int getEndRootDirectory()
{
    return getStartRootDirectory() + ((getRootEntryCount()/16) * getBytesPerSector());
}

int getPhysicalClusterStart(int logicalValue)
{
    int sectors = (logicalValue - 2) + ((getFATCount() * getSectorsPerFAT()) + 1 + (getRootEntryCount()/16));
    return getBytesPerSector() * sectors;
}

int getPhysicalClusterEnd(int logicalValue)
{
    int sectors = (logicalValue - 2) + ((getFATCount() * getSectorsPerFAT()) + 1 + (getRootEntryCount()/16)) + 1;
    return getBytesPerSector() * sectors;
}

int getFileCount(int start, int end)
{
    // Total number of files in the disk, a subdirectory name is not considered a file
    // Directory entry, if the field of “First Logical Cluster” is 0 or 1, then this directory entry should not be counted
    int count = 0;
    int numSubDirectories = 0;
    int index = start;
    while(index < end)
    {
        if(disk[index] == 0)
        {
            break;
            // No more directory entries
        }
        if(getLogicalCluster(index) < 2)
        {
            // Ignore as it points to the root directory, not a file
            index += 32;
            continue;
        }
        if((disk[index + 11] & 0x10) == 16)
        {
            // Don't count as a file but if it's not . or .. must transverse recursively
            if(disk[index] != '.')
            {
                numSubDirectories++;
            }
        }
        else
        {
            // File found
            count++;
        }
        index += 32;
    }
    // Recursively transverse directories
    index = start;
    for(int dir = 0; dir < numSubDirectories; dir++)
    {
        index = getNextDirectory(index, end);
        int logicalCLuster = getLogicalCluster(index);
        // Take sum of recursive calls
        count += getFileCount(getPhysicalClusterStart(logicalCLuster), getPhysicalClusterEnd(logicalCLuster));
    }
    return count;
}

// File size is found in directory entry index 28 to 31
int getFileSize(int index)
{
    int fileSize = (disk[index + 28] & 0xFF);
    fileSize += ((disk[index + 29] & 0xFF) << 8);
    fileSize += ((disk[index + 30] & 0xFF) << 16);
    return fileSize + ((disk[index + 31] & 0xFF) << 24);
}

// Logical cluster found in directory entry index 26 and 27
int getLogicalCluster(int index)
{
    return convertToDecimalRep(disk[index + 26], disk[index + 27]);
}

int getNextDirectory(int start, int end)
{
    while(start < end)
    {
        if(disk[start] == 0)
        {
            // No more directory entries
            return -1;
        }
        if(getLogicalCluster(start) < 2)
        {
            // Points to root directory, doesn'ty count as a directory
            start += 32;
            continue;
        }
        if((disk[start + 11] & 0x10) == 16 && disk[start] != '.')
        {
            // Directory found that is not . or ..
            return start;
        }
        start += 32;
    }
    return -1;
}

// Go through all fat entries and see which ones have a value of 0
int getSectorsUnused()
{
    int count = 0;
    for(int index = 0; index < getFATEntryCount(); index++)
    {
        if(getFatEntry(index) == 0)
        {
            count++;
        }
    }
    return count;
}

int getFreeSpace(){
    return getSectorsUnused() * getBytesPerSector();
}