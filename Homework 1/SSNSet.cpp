#include "SSNSet.h"

using namespace std;

SSNSet::SSNSet()
{}

bool SSNSet::add(unsigned long ssn)
{
    if(m_set.insert(ssn))
        return true;
    return false;
}

int SSNSet::size() const
{
    return m_set.size();
}

void SSNSet::print() const
{
    for (int n=0; n<m_set.size(); n++)
    {
        unsigned long num;
        m_set.get(n, num);
        cout<<num<<endl;
    }
}
