#include <iostream>
#include <vector>

bool exists(std::vector<int> const& number, int num) {
	for (auto elem : number) {
		if (num == elem) {
			return true;
		}
	}
	return false;
}

std::vector<int> removeDuplicates( std::vector<int> V ){
    std::vector<int> result;
    int num{}; 
    for(unsigned int i=0;i<=V.size()-1;i++){
        num=V.at(i);
        for(unsigned int j=0;j<=V.size()-1;j++){
         if(j==i){
             continue;
            } 
            if(num!=V.at(j)){
                if(!exists(result,num)){
                    result.push_back(num);
                }
            }
        }
    }
    return result;
 }


std::vector<int> filterElement( std::vector<int> V, int x ){
    std::vector<int> filtered;
    for(auto elem: V){
        if(elem!=x){
			filtered.push_back(elem);
        }
    }
    return filtered;
 }

 void printVector(const std::vector<int>& V){
	for(auto x: V) {
	std::cout << x << " " << std::endl;
	}
 }
 
 int main(){
	std::vector<int> V1 {6, 5, 6, 7, 7, 10, 7, 1, 2, 5};
	std::vector<int> V2 {1, 3, 6, 5, 3, 1, 9};
	std::vector<int> V3 {3, 9, 12, 12, 15};
	auto R1 = removeDuplicates(V1);
	auto R2 = removeDuplicates(V2);
	auto R3 = removeDuplicates(V3);
	auto F1 = filterElement(V1, 5);
	auto F2 = filterElement(V2, 1);
	std::cout << "V1 with duplicates removed: ";
	printVector(R1);
	std::cout << "V2 with duplicates removed: ";
	printVector(R2);
	std::cout << "V3 with duplicates removed: ";
	printVector(R3);
	std::cout << "V1 with all 5's removed: ";
	printVector(F1);
	std::cout << "V2 with all 1's removed: ";
	printVector(F2);
	return 0;
 }
