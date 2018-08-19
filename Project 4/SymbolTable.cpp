#include "SymbolTable.h"
#include <string>
#include <vector>
using namespace std;

class SymbolTableImpl
{
  public:
    void enterScope();
    bool exitScope();
    bool declare(const string& id, int lineNum);
    int find(const string& id) const;
  private:
    struct info //structure that keeps track of line number, depth, and string input
    {
        int line;
        int depth;
        string str;
    };
    vector<info> array[7499]; //7499 buckets of info structure
    vector<int> m_lines; //vector that keeps track of all inputs in current depth
    int depth = 0; //keeps track of depth
};

void SymbolTableImpl::enterScope()
{
    m_lines.push_back(-1); //input into vector to mark entry into new scope
    depth ++; //mark entry into new scope
}

bool SymbolTableImpl::exitScope()
{
    while (!m_lines.empty() && m_lines.back() != -1) //empty out vector until it hits the inital marker
    {
        array[m_lines.back()].pop_back();
        m_lines.pop_back();  //vector helps find where in the array the value is
    }
    if (m_lines.empty()) //this means it is trying to exit a scope that hasn't been entered
        return false;
    m_lines.pop_back(); //pop off the marker of scope entry
    depth --; //mark exit of scope
    return true;
}

bool SymbolTableImpl::declare(const string& id, int lineNum)
{
    unsigned int h = 2166136261u; //This hash function turns every string into an integer
    for (int k = 0; k != id.size(); k++)
    {
        h += id[k];
        h *= 16319; //these integers are deterministic and evenly spread
    }
    int fin = h%7499;
    
        //this makes sure the final number will be less than 10000 so it will fit in the array
    
    info i; //creates info structure that takes in the depth, line number, and string
    i.depth = depth;
    i.line = lineNum;
    i.str = id;
    
    if (array[fin].size() == 0) //if the bucket is blank, insert
    {
        array[fin].push_back(i);
        m_lines.push_back(fin); //record in the vector
        return true;
    }
    while (array[fin].front().str != id) //while the bucket's string does not equal the string
    {
        fin ++; // go to next bucket
        if (fin == 7499) //if past edge of array, go back to 0 position
            fin = 0;
        if (array[fin].size() == 0) //if you find an empty bucket, break
            break;
    }
    if (array[fin].size() != 0 && depth <= array[fin].back().depth)
        //if the depth of the last item is greater than the current depth, return false
        return false;
    array[fin].push_back(i); //push into array
    m_lines.push_back(fin); //record with the vector
    return true;
}

int SymbolTableImpl::find(const string& id) const
{
    if (id.empty()) //if there is no string, return -1
        return -1;
    
    unsigned int h = 2166136261u; //again, the hash function
    for (int k = 0; k != id.size(); k++)
    {
        h += id[k];
        h *= 16319;
    }
    int fin = h%7499;
    
    if (array[fin].size() == 0) //if that bucket has nothing in it, return -1
        return -1;
    else
    {
        while (array[fin].size() != 0 && array[fin].front().str != id)
            //keep going until u reach a bucket with a matching string
        {
            fin ++;
            if (fin == 7499) //loop back if it reaches the end
                fin = 0;
        }
        if (array[fin].size() == 0) //if it reaches an empty bucket, return -1
            return -1;
        return array[fin].back().line;
    }
}

//*********** SymbolTable functions **************

// For the most part, these functions simply delegate to SymbolTableImpl's
// functions.

SymbolTable::SymbolTable()
{
    m_impl = new SymbolTableImpl;
}

SymbolTable::~SymbolTable()
{
    delete m_impl;
}

void SymbolTable::enterScope()
{
    m_impl->enterScope();
}

bool SymbolTable::exitScope()
{
    return m_impl->exitScope();
}

bool SymbolTable::declare(const string& id, int lineNum)
{
    return m_impl->declare(id, lineNum);
}

int SymbolTable::find(const string& id) const
{
    return m_impl->find(id);
}
//*/
