#include "newSet.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int main()
{
    Set n(1);
    assert(n.empty()); //test if empty function works when set is empty
    assert(n.insert(232)); //test if inserting number will work
    assert(!n.insert(67)); //test if inserting something at max capacity will return false

    Set s(4);
    ItemType x = 123;
    assert( !s.get(42, x)  &&  x == 123); // test if get function works when Set is empty
    assert(s.size() == 0); //test if size function works when Set is empty
    s.insert(234);
    assert(s.size() == 1); //test if size function works when Set is unempty
    assert(s.get(0, x)  &&  x == 234); //test if get function works when Set is unempty
    assert(!s.get(2,x)); //test if get function returns false when int input>size
    assert(!s.insert(234)); //test if insert returns false for duplicates
    assert(s.size() == 1); //test if size remains same after duplicate rejected
    s.insert(345);
    s.insert(456);
    s.erase(234);
    assert(!s.contains(234) && s.contains(345) && s.contains(456)); //test if erase function erases    the value entered and nothing more
    assert(s.size() == 2); // tests if size decreases by one
    assert(!s.erase(789));//test if erase returns false when an item is not in the Set
 
    Set r;
    for (int num = 0; num<DEFAULT_MAX_ITEMS;num++)
    {
        assert(r.insert(num+200)); //test if when r isnt initalized with a length, it automatically can take DEFAULT_MAX_ITEMS
    }
    assert(!r.insert(80)); //test if overcapacity when trying to add one more
 
    n.swap(s);
    assert(s.size() == 1); // tests if s's size changes to the previous size of n
    assert(n.size() == 2); // tests if n's size changes to the previous size of s
    assert(n.get(0, x) && x==345); //test if the first number of n is correct
    assert(n.get(1, x) && x==456); //test if the second number of n is correct
    assert(s.get(0, x) && x==232); //test if the third number of s is correct
 
    n = s;
    assert(n.size() == 1); // tests if n's size changes to the previous size of n
    assert(n.get(0, x) && x==232); //test if the first number of n is correct
 
    s.erase(232);
    assert(n.size() == 1); // tests n remains the same after s changes
    assert(n.get(0, x) && x==232);
    s.insert(232);
    
    Set p(s);
    assert(p.size() == 1); // tests if n's size changes to the previous size of n
    assert(p.get(0, x) && x==232); //test if the first number of n is correct
    
 
    cout<<"All tests passed!"<<endl;
}
