#include "Set.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;


int main()
{
    
    Set s;
    assert(s.empty()); //test if empty function works
    
    ItemType x = "one";
    assert( !s.get(42, x)  &&  x == "one"); // test if get function works when Set is empty
    assert(s.size() == 0); //test if size function works when Set is empty
    s.insert("two");
    assert(s.size() == 1); //test if size function works when Set is unempty
    assert(s.get(0, x)  &&  x == "two"); //test if get function works when Set is unempty
    assert(!s.get(2,x)); //test if get function returns false when int input>size
    assert(!s.insert("two")); //test if insert returns false for duplicates
    assert(s.size() == 1); //test if size remains same after duplicate rejected
    s.insert("too");
    s.insert("to");
    s.erase("two");
    assert(!s.contains("two") && s.contains("to") && s.contains("too")); //test if erase function erases the value entered and nothing more
    assert(s.size() == 2); // tests if size decreases by one
    assert(!s.erase("three"));//test if erase returns false when an item is not in the Set
    
    Set n;
    n.insert("three");
    n.insert("four");
    n.insert("five");
    
    n.swap(s);
    assert(s.size() == 3); // tests if s's size changes to the previous size of n
    assert(n.size() == 2); // tests if n's size changes to the previous size of s
    assert(n.get(0, x) && x=="to"); //test if the first number of n is correct
    assert(n.get(1, x) && x=="too"); //test if the second number of n is correct
    assert(s.get(0, x) && x=="five"); //test if the first number of s is correct
    assert(s.get(1, x) && x=="four"); //test if the second number of s is correct
    assert(s.get(2, x) && x=="three"); //test if the third number of s is correct
    
    n = s;
    assert(n.size() == 3); // tests if n's size changes to the previous size of n
    assert(n.get(0, x) && x=="five"); //test if the first number of n is correct
    assert(n.get(1, x) && x=="four"); //test if the second number of n is correct
    assert(n.get(2, x) && x=="three"); //test if the third number of n is correct
    
    s.erase("five");
    assert(n.size() == 3); // tests n remains the same if s changes
    assert(n.get(0, x) && x=="five");
    assert(n.get(1, x) && x=="four");
    assert(n.get(2, x) && x=="three");

    
    cout<<"All tests passed!"<<endl;
 
    
}
