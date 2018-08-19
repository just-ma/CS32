#include "newSet.h"

Set::Set(int length)
{
    if (length<=0 || m_length<0)
        exit(1);
    else
        m_length = length;
    
    m_num = 0;
    m_pointer = new ItemType[m_length];
}

Set::~Set()
{
    delete [] m_pointer;
}

Set::Set(const Set& other)
: m_length(other.m_length), m_num(other.m_num)
{
    delete m_pointer;
    m_pointer = new ItemType[m_length];
    for (int n=0; n<m_length; n++)
    {
        m_pointer[n]=other.m_pointer[n];
    }
}

Set& Set::operator=(const Set& rhs)

{
    if (this != &rhs)
    {
        m_length=rhs.m_length;
        m_num=rhs.m_num;
        delete m_pointer;
        m_pointer = new ItemType[m_length];
        for (int n=0; n<m_length; n++)
        {
            m_pointer[n]=rhs.m_pointer[n];
        }
    }
    return *this;
}

bool Set::empty() const
{
    if (m_num == 0)
        return true;
    else
        return false;
}

int Set::size() const
{
    return m_num;
}

bool Set::insert(const ItemType& value)
{
    if (m_num >= m_length)
        return false;
    for (int n=0; n<m_num; n++)
    {
        if (value == m_pointer[n])
        {
            return false;
        }
    }
    m_pointer[m_num]=value;
    m_num ++;
    return true;
}

bool Set::erase(const ItemType& value)
{
    if (m_pointer[m_num-1] == value)
    {
        m_num--;
        return true;
    }
    for (int n=0; n<m_num; n++)
    {
        if (value == m_pointer[n])
        {
            for (int m=n+1; m<m_num;m++)
            {
                m_pointer[m-1]=m_pointer[m];
            }
            m_num--;
            return true;
        }
    }
    return false;
}

bool Set::contains(const ItemType& value) const
{
    for (int n=0; n<m_num; n++)
    {
        if (value == m_pointer[n])
            return true;
    }
    return false;
}

bool Set::get(int i, ItemType& value) const
{
    if (0<= i && i< m_num)
    {
        for (int n=0; n<m_num; n++)
        {
            int count = 0;
            for (int m=0; m<m_num; m++)
            {
                if (m_pointer[n] > m_pointer[m])
                    count++;
            }
            if (count == i)
            {
                value = m_pointer[n];
                return true;
            }
        }
    }
    return false;
}

void Set::swap(Set& other)
{
    ItemType *temp;
    int length;
    int num;
    
    temp = m_pointer;
    m_pointer = other.m_pointer;
    other.m_pointer = temp;
    
    length = other.m_length;
    other.m_length = m_length;
    m_length = length;
    
    num = other.m_num;
    other.m_num = m_num;
    m_num = num;
}
