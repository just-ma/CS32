#include "SSNSet.h"
#include <iostream>
#include <cassert>

using namespace std;

int main()
{
    SSNSet s;
    
    assert(s.size()==0); //test if size function works when empty
     
    ItemType x = 123;
    s.add(x);
    s.add(678);
    assert (s.size()==2); //test if size function works when unempty
    assert (!s.add(678)); //test if add function return false for duplicates
    assert (s.size() ==2); //test if size remains the same after duplicate
    
    SSNSet n;
    
    assert (n.size()==0);//test if new SSNSet begins with 0 size
    n.add(5);
    n.add(7);
    n.add(89);
    
    n=s;
    assert(n.size()==2);//test if = operator works for size
    cout<<"Output must look like:\n123\n678\n\nOutput looks like:\n";
    s.print(); //check if it matches to see if print function works
    cout<<"\nOutput must also look like:\n123\n678\n\nOutput looks like:\n";
    n.print(); //check if = operator works
    
    cout<<"If so, all tests passed!"<<endl;
}

