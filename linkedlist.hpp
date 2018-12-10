/* linkedlist.hpp
   CSC 116 - Fall 2018
	Amaan Makhani
	V00883520
	Created Dec 2,2018
	This programs creates and does functions
	associated with linked lists.
*/

#include <functional>
#include <stdexcept>
#include <memory>
#include <initializer_list>
#include <iostream>

//Important note: You may add other #include directives, but you are NOT
//permitted to use or include ANY of the standard collections, including
//std::list, std::vector, std::set, std::map, std::unordered_set, 
//std::forward_list, std::unordered_map, std::array 
//(or any derivatives thereof, such as std::queue)
//If you do so, your assignment will receive a mark of zero.
//You ARE permitted to use the specialized std::initializer_list collection,
//as indicated below in the constructor.


template<typename T>
class LinkedList{
public:
    /* Default constructor:
       Construct the list to be empty (size = 0)
    */
    LinkedList(){
        /* Your code here */
		head = nullptr;
    }
    /* Initializer list constructor:
       Construct the list to contain all of the elements in the
       provided initializer list. Recall that you can use a for-each
       loop to iterate over initializer_list objects. */
    LinkedList( std::initializer_list<T> initial_contents ){
        /* Your code here */
		for (auto elem : initial_contents) {
			push_back(elem);
		}
    }
    /* Copy constructor:
       Construct the list to contain all of the elements in the provided
       other list. Note that a "deep copy" is needed, such that when the 
       operation completes there are two separate, independent lists.
    */
    LinkedList( LinkedList<T> const& other_list ){
		auto size=other_list.size();
		LinkedList<T> p{};
		if ((int)size == 0) {
			head = nullptr;
		}
		else {
			auto it = other_list.head;
				while (it != nullptr) {
					p.push_back(it->value);
					it = it->next;
				}
				*this = p;
		}
    }
    /* Destructor */
    ~LinkedList(){
        /* Your code here */
		clear();
    }

    /* is_empty()
       Returns true if the list contains zero elements and false otherwise.
    */
    bool is_empty() const{
        /* Your code here */
		if ((int)size() == 0) {
			return true;
		}
		else {
			return false;
		}
    }

    /* size()
       Returns the number of elements in the list.
    */
    unsigned int size() const{
        /* Your code here */
		unsigned int count=0;
		auto it = head;
		while (it != nullptr) {
			it=it->next;
			count++;
		}
		return count;
    }

    /* first() - Version 1
       If the list has at least one element, returns a reference to the first
       element (such that the result can be modified by the caller).
       If the list has zero elements, throw an exception of type std::runtime_error.
    */
    T& first(){
        /* Your code here */
		if ((int)size() != 0) {
			auto it = head;
			return it->value;
		}
		else {
			throw std::runtime_error("No elements");
		}
    }

    /* first() - Version 2
       If the list has at least one element, returns a const reference to the
       first element.
       If the list has zero elements, throw an exception of type std::runtime_error.
    */
    T const& first() const {
        /* Your code here */
		if ((int)size() != 0) {
			auto it = head;
			return it->value;
		}
		else {
			throw std::runtime_error("No elements");
		}
    }
    
    /* last() - Version 1
       If the list has at least one element, returns a reference to the last
       element (such that the result can be modified by the caller).
       If the list has zero elements, throw an exception of type std::runtime_error.
    */
    T& last(){
        /* Your code here */
		if ((int)size() != 0) {
			auto it = head;
			while (it->next != nullptr) {
				it = it->next;
			}
			return it->value;
		}
		else {
			throw std::runtime_error("No elements");
		}
    }

    /* last() - Version 2
       If the list has at least one element, returns a const reference to the
       last element.
       If the list has zero elements, throw an exception of type std::runtime_error.
    */
    T const& last() const {
        /* Your code here */
		if ((int)size() != 0) {
			auto it = head;
			while (it->next != nullptr) {
				it = it->next;
			}
			return it->value;
		}else {
			throw std::runtime_error("No elements");
		}
    }
    


    /* push_front(value)
       Add a copy of the provided value to the list as the first element.
    */
    void push_front(T const& value){
        /* Your code here */
		auto it = head;
		ListNodePtr ptr = std::make_shared<ListNode>(value, it);
		head = ptr;
    }
    
    /* push_front(value)
       Add a copy of the provided value to the list as the last element.
    */
    void push_back(T const& value){
        /* Your code here */
		
		if (size() != 0) {
			auto it = head;
			while (it->next != nullptr) {
				it=it->next;
			}
			ListNodePtr ptr = std::make_shared<ListNode>(value);
			it->next = ptr;
		}
		else {
			ListNodePtr ptr = std::make_shared<ListNode>(value);
			head = ptr;
		}
    }
    

    /* pop_front()
       If the list has at least one element, remove the first element and
       return a copy of its value.
       If the list has zero elements, throw an exception of type std::runtime_error.
    */
	T pop_front() {
		/* Your code here */
		if ((int)size() != 0) {
			auto it = head;
			auto copyhead = head;
			it=it->next;
			head=it;
			return copyhead->value;
		}
		else {
			throw std::runtime_error("No elements");
		}
	}
    
    /* pop_back()
       If the list has at least one element, remove the last element and
       return a copy of its value.
       If the list has zero elements, throw an exception of type std::runtime_error.
    */
    T pop_back(){
        /* Your code here */
		if ((int)size() != 0&& (int)size()!=1) {
			auto it = head;
			while (it->next != nullptr) {
				it = it->next;
			}
			auto itcopy = head;
			while (itcopy->next != it) {
				itcopy=itcopy->next;
			}
			itcopy->next = nullptr;
			return it->value;
		}if ((int)size() == 1) {
			auto it = head;
			head = nullptr;
			return it -> value;
		}
		else {
			throw std::runtime_error("No elements");
		}
    }
    
    /* contains(element)
       Return true if any element of the list is equal (using the == operator) to 
       the provided element. Return false otherwise.
    */
    bool contains(T const& element) const{
        /* Your code here */
		if (size() != 0) {
			auto it = head;
			while (it!= nullptr) {
				if (it->value == element) {
					return true;
				}
				it=it->next;
			}
		}
		return false;
    }
    
    /* clear()
       Remove all elements from the list.
    */
    void clear(){
        /* Your code here */
		head = nullptr;
    }
    
    /* operator==
       Return true if this list is equal to the provided other list.
       Two lists are considered equal if they have the same length and
       contain the same sequence of elements (in the same order). Use
       the == operator to compare individual elements to each other.
    */
    bool operator==(LinkedList<T> const& other_list) const{
        /* Your code here */
		if (size() != other_list.size()) {
			return false;
		}
		auto it = head;
		auto it_other_list = other_list.head;
		while (it!= nullptr&& it_other_list != nullptr) {
			if (it->value != it_other_list->value) {
				return false;
			}
			it=it->next;
			it_other_list=it_other_list->next;
		}
		return true;
    }
    
    /* operator!=
       Return true if this list is not equal to the provided other list.
       In this context, two lists are not equal if the operator== above
       returns false.
    */
    bool operator!=(LinkedList<T> const& other_list) const{
        /* Your code here */
		if ( *this == other_list) {
			return false;
		}
		else {
			return true;
		}
    }
    

    /* operator=
       Set this list to be a copy of the provided other list.
       (If this list already contains any values, remove all of them first).
    */
    LinkedList<T>& operator=(LinkedList<T> const& other_list){
        /* Your code here */
		if (size() != 0) {
			clear();
		}
	
		auto it = other_list.head;
		if (it != nullptr) {
		push_back(it->value);
		}
		while (it->next != nullptr) {
		it = it->next;
		push_back(it->value);
		}
		return *this;
    }
	
    
    /* apply(func) - Version 1
       Given a function that takes a single argument (which is a reference to an 
       element), call that function on each element of this list (in forward 
       order). Note that since the element is passed by non-const reference, the 
       provided function has the ability to modify the elements of the list.
    */
    void apply( std::function< void(T&) > func ){
        /* Your code here */
		if (size() != 0) {
			auto it = head;
			while (it != nullptr) {
				func(it->value);
				it=it->next;
			}
		}
    }
    
    /* apply(func) - Version 2
       Given a function that takes a single argument (which is a const reference 
       to an element), call that function on each element of this list (in 
       forward order).

       Note that the only difference between this and version 1 is that this
       is guaranteed not to modify the list elements (since the reference passed
       to the function is const). It will likely be possible to use almost
       identical code for this version as for version 1.
    */
    void apply( std::function< void(T const&) > func ) const{
        /* Your code here */
		
		if (size() != 0) {
			auto it = head;
			while (it != nullptr) {
				func(it->value);
				it=it->next;
			}
		}
    }

    /* remove_where(func)
       Given a function which takes a list element and returns a boolean value, remove
       all elements of the list where the provided function returns true 
    */
    void remove_where( std::function< bool(T const&) > condition_function ){
        /* Your code here */
		LinkedList <T> p{};
		if (size()!= 0) {
			auto it = head;
			while (it != nullptr) {
				if (!condition_function(it->value)) {
					p.push_back(it->value);
				}
				it=it->next;
			}
		}
		*this = p;
    }
    
    
    /* transform(func)
       Given a function that takes a single argument (which is a const reference 
       to an element of this list) and returns a value (of some type OutputType),
       call that function on each element of this list (in forward order), then
       add the result to a new list (of element type OutputType). Return the
       resulting list.
    */

    template<typename OutputType>
    LinkedList<OutputType> transform( std::function< OutputType(T const&) > transform_function ){
        /* Your code here */
		LinkedList<OutputType> p{};
		if (size() != 0) {
			auto it = head;
			p.push_back(transform_function(it->value));
			while (it->next != nullptr) {
				it = it->next;
			p.push_back(transform_function(it->value));
			}
		}
		return p;
    }

    

private:

    /* Note that since ListNode is a struct (not a class), all of its members will be
       public by default. */
    struct ListNode;
    using ListNodePtr = std::shared_ptr<ListNode>;
    struct ListNode{
        ListNode(T const& val, ListNodePtr next_node) {
			value = val;
			next = next_node;
        }
		ListNode(T const& val) {
			value = val;
			next = nullptr;
		}
		ListNode() {
			next = nullptr;
		}
		T value{};
        ListNodePtr next;
    };

    ListNodePtr head;

    /* You may add other private members as needed. */

};