#ifndef Set_hpp
#define Set_hpp

#include<string>

typedef std::string ItemType;

class Set
{
public:
    Set();
    ~Set();
    Set(const Set& other);
    Set& operator=(const Set& rhs);
    bool empty() const;
    int size() const;
    bool insert(const ItemType& value);
    bool erase(const ItemType& value);
    bool contains(const ItemType& value) const;
    bool get(int pos, ItemType& value) const;
    void swap(Set& other);
private:
    struct Node
    {
        ItemType data;
        Node* next = nullptr;
        Node* prev = nullptr;
    };
    int m_size;
    Node* m_head;
};

void unite(const Set& s1, const Set& s2, Set& result);

void subtract(const Set& s1, const Set& s2, Set& result);


#endif /* Set_hpp */
