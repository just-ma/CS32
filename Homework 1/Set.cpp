#include "Set.h"

Set::Set():m_length(0)
{
    if (m_length<0 || m_length>DEFAULT_MAX_ITEMS)
    {
        exit(1);
    }
}

bool Set::empty() const
{
    if (m_length == 0)
        return true;
    else
        return false;
}

int Set::size() const
{
    return m_length;
}

bool Set::insert(const ItemType& value)
{
    if (m_length >= DEFAULT_MAX_ITEMS)
        return false;
    for (int n=0; n<m_length; n++)
    {
        if (value == m_set[n])
            return false;
    }
    m_set[m_length]=value;
    m_length ++;
    return true;
}

bool Set::erase(const ItemType& value)
{
    if (m_set[m_length-1] == value)
    {
        m_length--;
        return true;
    }
    for (int n=0; n<m_length; n++)
    {
        if (value == m_set[n])
        {
            for (int m=n+1; m<m_length;m++)
            {
                m_set[m-1]=m_set[m];
            }
            m_length--;
            return true;
        }
    }
    return false;
}

bool Set::contains(const ItemType& value) const
{
    for (int n=0; n<m_length; n++)
    {
        if (value == m_set[n])
        {
            return true;
        }
    }
    return false;
}

bool Set::get(int i, ItemType& value) const
{
    if (0<= i && i< m_length)
    {
        for (int n=0; n<m_length; n++)
        {
            int count = 0;
            for (int m=0; m<m_length; m++)
            {
                if (m_set[n] > m_set[m])
                    count++;
            }
            if (count == i)
            {
                value = m_set[n];
                return true;
            }
        }
    }
    return false;
}

void Set::swap(Set& other)
{
    ItemType temp;
    int num;
    for(int n=0; n<DEFAULT_MAX_ITEMS; n++)
    {
        temp = other.m_set[n];
        other.m_set[n] = m_set[n];
        m_set[n] = temp;
    }
    num = other.size();
    other.m_length = m_length;
    m_length = num;
}
