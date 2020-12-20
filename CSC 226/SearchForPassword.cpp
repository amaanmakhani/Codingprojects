// Description: Search for password of size N and find the most frequent substring.

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int hashValue(std::string strInput) {
    int value = 0;
    for (auto ch : strInput) {
        value += int(ch);
    }
    return value % 7; 
}

void hashInput(std::vector<std::vector<std::string>> &v, std::string strInput, int n) {
    for (int index = 0; index <= strInput.length() - n; index++) {
        std::string substring = "";
        for (int i = 0; i < n; i++ ) {
            substring.push_back(strInput[index + i]);
        }
        v[hashValue(substring)].push_back(substring);
    }
}

std::string findMostFrequent(std::vector<std::vector<std::string>> v) {
    int max = 0;
    std::string maxString = "";
    for (auto vector : v) {
        std::sort(v.begin(), v.end());
        int counter = 0;
        std::string temp = "";
        for (auto elem : vector) {
            if (elem == temp) {
                counter++;
            } else {
                temp = elem;
                counter = 0;
            }
        }
        if (counter > max) {
            maxString = temp;
            max = counter;
        } else {
            temp.clear();
            counter = 0;
        }
    }
    return maxString;
}

int main() {
    int n;
    std::string str;
    
    // Recieve input from the user.
    std::cin>>n;
    std::cin>>str;
    
    std::vector<std::vector<std::string>>v(7);
    
    // Write your hash function and calculate hash for each index
    hashInput(v, str , n);

    // Determine which hash appears the most
    std::string strResult = findMostFrequent(v);

    // Output the result
    std::cout << "The password is \"" << strResult << "\"!" << std::endl;

    return 0;
}