# CSC360 A2

- To build type 
    make

- To run ACS with the given input file in the assignment description use:

    ./ACS customers.txt

- Other test files are also included such as:
    - allCustomersArriveTogether.txt
    - complexCustomerSchedule.txt
    - fewCustomers.txt

    These robust test files are used to test the correctness of the ACS algorithm and functionality.

- To ensure no hanging or infinite loops occurred the tester.sh bash script was used. This script runs all the mentioned test files above 50 times. After running this I found that the ACS algorithm was working correctly and no hanging or infinite loops occurred.

    To run this script type:
    
    bash tester.sh