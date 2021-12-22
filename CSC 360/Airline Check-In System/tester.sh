#!/bin/bash
# Run option 1
# bash tester.sh
# Run option 2
# chmod a+x tester.sh
# ./tester.sh
make
for i in {1..50}
do
./ACS fewCustomers.txt > output.txt
echo Few Customers $i
done

for i in {1..50}
do
./ACS customers.txt > output.txt
echo Customers $i
done

for i in {1..50}
do
./ACS allCustomersArriveTogether.txt > output.txt
echo All Customers Arrive Together $i
done

for i in {1..50}
do
./ACS complexCustomerSchedule.txt > output.txt
echo Complex Customer Schedule $i
done
