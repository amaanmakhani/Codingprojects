#include <iostream>
#include <vector>

 /* Given a vector V of integers and a single integer x, return a new vector
5 containing all of the elements of V (with the original order maintained),
6 which are not equal to x. */

/* Given a vector V of integers, return a new vector containing the elements
10 in V with all duplicates elements ignored (so only the first occurrence of
11 each element is kept). The order of the non-duplicate elements should be
12 the same as the original ordering. */
bool exists(std::vector<int> const& number, int num) {
	for (auto elem : number) {
		if (num == elem) {
			return true;
		}
	}
	return false;
}

std::vector<int> remove_duplicates( std::vector<int> V ){
     std::vector<int> number;
     int num{};
     
     for(unsigned int i=0;i<=V.size()-1;i++){
         num=V.at(i);
        for(unsigned int j=0;j<=V.size()-1;j++){
         if(j==i){
             continue;
            } 
            if(num!=V.at(j)){
                if(!exists(number,num)){
                    number.push_back(num);
                }
            }
        }
    }
     return number;
 }



 /* Given a vector V of integers and a single integer x, return a new vector
5 containing all of the elements of V (with the original order maintained),
6 which are not equal to x. */
std::vector<int> filter_element( std::vector<int> V, int x ){
     std::vector<int> number;
     for(auto elem: V){
         if(elem!=x){
             number.push_back(elem);
         }
     }
     return number;
 }

 void print_vector(const std::vector<int>& V){
 for(auto x: V)
 std::cout << x << " ";
 std::cout << std::endl;
 }
 int main(){
 std::vector<int> V1 {6, 5, 6, 7, 7, 10, 7, 1, 2, 5};
 std::vector<int> V2 {1, 3, 6, 5, 3, 1, 9};
 std::vector<int> V3 {3, 9, 12, 12, 15};

 auto R1 = remove_duplicates(V1);
auto R2 = remove_duplicates(V2);
auto R3 = remove_duplicates(V3);
 auto F1 = filter_element(V1, 5);
 auto F2 = filter_element(V2, 1);

std::cout << "V1 with duplicates removed: ";
print_vector(R1);
std::cout << "V2 with duplicates removed: ";
print_vector(R2);
std::cout << "V3 with duplicates removed: ";
 print_vector(R3);
 std::cout << "V1 with all 5's removed: ";
 print_vector(F1);
 std::cout << "V2 with all 1's removed: ";
 print_vector(F2);

 return 0;
 }
