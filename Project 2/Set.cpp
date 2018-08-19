#include "Set.h"

Set::Set()
{
    m_size = 0;
    m_head = nullptr;
}

Set::~Set()
{
    ItemType temp;
    int p = m_size;
    for (int n=0; n<p; n++)
    {
        get(0,temp);
        erase(temp);
    }
}

Set::Set(const Set& other)

{
    if (m_head!=other.m_head)
    {
        int size = m_size;
        ItemType temp;
        for (int n=0; n<size; n++)
        {
            get(0,temp);
            erase(temp);
        }
        Node* cur = other.m_head;
        for (int n = 0; n<other.m_size; n++)
        {
            insert(cur->data);
            cur = cur->next;
        }
    }
}

Set& Set::operator=(const Set& rhs)

{
    if (this != &rhs)
    {
        ItemType temp;
        int size = m_size;
        for (int n=0; n<size; n++)
        {
            get(0,temp);
            erase(temp);
        }
        Node* cur = rhs.m_head;
        for (int n = 0; n<rhs.m_size; n++)
        {
            insert(cur->data);
            cur = cur->next;
        }
    }
    return *this;
}

bool Set::empty() const
{
    if (m_size == 0)
        return true;
    return false;
}

int Set::size() const
{
    return m_size;
}

bool Set::insert(const ItemType& value)
{
    Node* item = new Node;
    item->data = value;
    Node* n = m_head;
    
    if (empty() || n->data>value)
    {
        m_head = item;
        if (size() == 1)
        {
            item->next = n;
            n->prev = item;
        }
        m_size++;
        return true;
    }
   
    else
    {
        for (int r = 0 ; r < m_size; r++)
        {
            if (n->data == value)
                return false;
            else if (n->data > value)
            {
                item->next = n;
                item->prev = n->prev;
                n->prev = item;
                item->prev->next = item;
                
                m_size++;
                return true;
            }
            else if (r==m_size-1)
            {
                item->prev = n;
                n->next = item;
                
                m_size++;
                return true;
            }
            n = n->next;
        }
        return false;
    }
}

bool Set::erase(const ItemType& value)
{
    Node* n = m_head;
    if(!empty()) // linked-list is not empty
    {
        for (int r = 0 ; r < m_size-1 ; r++)
        {
            if (n->data == value)
            {
                if (r!=0)
                {
                    n->prev->next = n->next;
                    n->next->prev = n->prev;
                }
                else
                {
                    m_head = n->next;
                    n->next->prev = nullptr;
                }
                delete n;
                
                m_size--;
                return true;
            }
            n = n->next;
        }
        if (n-> data == value)
        {
            if(m_size!=1)
            {
                n->prev->next = nullptr;
            }
            delete n;

            m_size--;
            return true;
        }
    }
    return false;
}

bool Set::contains(const ItemType& value) const
{
    Node* n = m_head;
    if(!empty())
    {
        for (int r=0 ; r < m_size ; r++)
        {
            if (n->data == value)
                return true;
            n=n->next;
        }
    }
    return false;
}

bool Set::get(int pos, ItemType& value) const
{
    Node* n = m_head;
    if (pos < m_size)
    {
        for (int r=0 ; r < pos ; r++)
        {
            n=n->next;
        }
        value = n->data;
        return true;
    }
    return false;
}

void Set::swap(Set& other)
{
    Node* n = m_head;
    m_head = other.m_head;
    other.m_head = n;
    
    int size = m_size;
    m_size = other.m_size;
    other.m_size = size;
}

void unite(const Set& s1, const Set& s2, Set& result)
{
    ItemType temp;
    int size = s2.size();
    if (&s1 != &result && &s2 != &result)
    {
        while(!result.empty())
        {
            result.get(0,temp);
            result.erase(temp);
        }
    }
    for(int n = 0; n<s1.size(); n++)
    {
        s1.get(n, temp);
        result.insert(temp);
    }
    for(int n = 0; n<size; n++)
    {
        s2.get(n, temp);
        result.insert(temp);
    }
}

void subtract(const Set& s1, const Set& s2, Set& result)
{
    ItemType temp;
    int size = s2.size();
    if (&s1 != &result && &s2 != &result)
    {
        while(!result.empty())
        {
            result.get(0,temp);
            result.erase(temp);
        }
    }
    for(int n = 0; n<s1.size(); n++)
    {
        s1.get(n, temp);
        result.insert(temp);
    }
    for(int n = 0; n<size; n++)
    {
        s2.get(n, temp);
        result.erase(temp);
    }
}
