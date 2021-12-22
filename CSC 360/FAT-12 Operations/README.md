# CSC360 A3

- To build all parts of the project, run the following commands: 
    make

- FAT12 file system operations:

    - To display information about the file system:

        ./diskinfo disk.IMA

    - To display information about the contents of the root directory and all sub directories:

        ./disklist disk.IMA

    -  To copy a file from the root directory of the file system to the current directory in Linux:

        ./diskget disk.IMA DesiredFileName

    - To copy a file from the current Linux directory into specified directory (i.e., the root directory or a subdirectory) of the file system

        ./diskput disk.IMA DesiredFileName