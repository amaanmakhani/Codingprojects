#include <iostream>
#include <string>
#include <vector>
 struct StockItem{
 std::string item_name; //Name of the item
 double price_kg; //Price per kilogram
 };

StockItem new_stock_item( std::string name, double price ){
 StockItem result;
 result.item_name = name; result.price_kg = price;
 return result;
 }

 /* Given a list of products (as StockItem structs), reduce
16 the price of each item by 1 dollar per kilogram */
 void lower_prices( std::vector<StockItem>& product_list ){
     for(auto&elem:product_list){
         elem.price_kg=elem.price_kg-1;
     }
 }

 /* Given a list of products (as StockItem structs), print
20 out the entire list, with one product per line, in the format
21 <item_name>: $<price_kg> per kg
22 (e.g. "Apples: $1.11 per kg") */
 void print_products( const std::vector<StockItem>& product_list ){
     for(auto elem:product_list){
         std::cout<<elem.item_name<<": "<<elem.price_kg<<" per kg"<<std::endl;
        }
 }

 int main(){
 std::vector<StockItem> products {};
 products.push_back( new_stock_item("Raspberries", 10.6) );
 products.push_back( new_stock_item("Pears", 3.71) );
 products.push_back( new_stock_item("Bananas", 2.25) );
 lower_prices(products);
 print_products(products);
 return 0;
 }