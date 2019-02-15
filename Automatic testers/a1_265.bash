#!/bin/bash

echo "Running Test 0.."
gcc -Wall -g -std=c99 seng.c -o test 
./test test00.txt
diff test00.mtf mtf_solutions/test00.mtf

echo "Running Test 1.."
gcc -Wall -g -std=c99 seng.c -o test
./test test01.txt
diff test01.mtf mtf_solutions/test01.mtf

echo "Running Test 2.."
gcc -Wall -g -std=c99 seng.c -o test 
./test test02.txt
diff test02.mtf mtf_solutions/test02.mtf

echo "Running Test 3.."
gcc -Wall -g -std=c99 seng.c -o test 
./test test03.txt
diff test03.mtf mtf_solutions/test03.mtf

echo "Running Test 4.."
gcc -Wall -g -std=c99 seng.c -o test
./test test04.txt
diff test04.mtf mtf_solutions/test04.mtf

echo "Running Test 5.."
gcc -Wall -g -std=c99 seng.c -o test
./test test05.txt
diff test05.mtf mtf_solutions/test05.mtf

echo "Running Test 6.."
gcc -Wall -g -std=c99 seng.c -o test
./test test06.txt
diff test06.mtf mtf_solutions/test06.mtf

echo "Running Test 7.."
gcc -Wall -g -std=c99 seng.c -o test
./test test07.txt
diff test07.mtf mtf_solutions/test07.mtf

echo "Running Test 8.."
gcc -Wall -g -std=c99 seng.c -o test
./test test08.txt
diff test08.mtf mtf_solutions/test08.mtf

echo "Running Test 9.."
gcc -Wall -g -std=c99 seng.c -o test
./test test09.txt
diff test09.mtf mtf_solutions/test09.mtf

echo "Running Test 10.."
gcc -Wall -g -std=c99 seng.c -o test
./test test10.txt
diff test10.mtf mtf_solutions/test10.mtf
