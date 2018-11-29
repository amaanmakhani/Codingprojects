echo off
echo "Running Test 1.."
g++ -pedantic -Wall -Wextra -std=c++17 -o 01 01.cpp 
01 > output.txt
fc output.txt 01.txt >out &&Echo Same %tests%|| Echo Different or error

echo "Running Test 2.."
g++ -pedantic -Wall -Wextra -std=c++17 -o 02 02.cpp
02 > output.txt
fc output.txt 02.txt > out &&Echo Same|| Echo Different or error

echo "Running Test 3.."
g++ -pedantic -Wall -Wextra -std=c++17 -o 03 03.cpp
03 > output.txt
fc output.txt 03.txt> out && Echo Same || Echo Different or error

echo "Running Test 4.."
g++ -pedantic -Wall -Wextra -std=c++17 -o 04 04.cpp
04 > output.txt
fc output.txt 04.txt> out && Echo Same|| Echo Different or error

echo "Running Test 5.."
g++ -pedantic -Wall -Wextra -std=c++17 -o 05 05.cpp
05 > output.txt
fc output.txt 05.txt> out && Echo Same|| Echo Different or error

echo "Running Test 6.."
g++ -pedantic -Wall -Wextra -std=c++17 -o 06 06.cpp
06 > output.txt
fc output.txt 06.txt> out && Echo Same  || Echo Different or error

echo "Running Test 7.."
g++ -pedantic -Wall -Wextra -std=c++17 -o 07 07.cpp
07 > output.txt
fc output.txt 07.txt> out && Echo Same || Echo Different or error

echo "Running Test 8.."
g++ -pedantic -Wall -Wextra -std=c++17 -o 08 08.cpp
08 > output.txt
fc output.txt 08.txt> out && Echo Same || Echo Different or error

echo "Running Test 9.."
g++ -pedantic -Wall -Wextra -std=c++17 -o 09 09.cpp
09 > output.txt
fc output.txt 09.txt> out && Echo Same || Echo Different or error

echo "Running Test 10.."
g++ -pedantic -Wall -Wextra -std=c++17 -o 10 10.cpp
10 > output.txt
fc/W output.txt 10.txt> out && Echo Same|| Echo Different or error
