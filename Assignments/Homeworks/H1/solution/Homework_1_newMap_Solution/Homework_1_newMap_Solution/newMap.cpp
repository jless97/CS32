// newMap.cpp

#include "newMap.h"
#include <iostream>
#include <cstdlib>
using namespace std;

Map::Map(int capacity)
: m_size(0), m_capacity(capacity)
{
    if (capacity < 0)
    {
        cout << "A Map capacity must not be negative." << endl;
        exit(1);
    }
    m_data = new Pair[m_capacity];
}

Map::Map(const Map& other)
: m_size(other.m_size), m_capacity(other.m_capacity)
{
    m_data = new Pair[m_capacity];
    for (int k = 0; k < m_size; k++)
        m_data[k] = other.m_data[k];
}

Map::~Map()
{
    delete [] m_data;
}

Map& Map::operator=(const Map& rhs)
{
    if (this != &rhs)
    {
        Map temp(rhs);
        swap(temp);
    }
    return *this;
}

bool Map::erase(const KeyType& key)
{
    int pos = find(key);
    if (pos == -1)  // not found
        return false;
    
    // Move last array item to replace the one to be erased
    
    m_size--;
    m_data[pos] = m_data[m_size];
    return true;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    int pos = find(key);
    if (pos == -1)  // not found
        return false;
    value = m_data[pos].m_value;
    return true;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i < 0  ||  i >= m_size)
        return false;
    key = m_data[i].m_key;
    value = m_data[i].m_value;
    return true;
}

void Map::swap(Map& other)
{
    // Swap the m_data pointers to dynamic arrays.
    
    Pair* tempData = m_data;
    m_data = other.m_data;
    other.m_data = tempData;
    
    // Swap sizes
    
    int t = m_size;
    m_size = other.m_size;
    other.m_size = t;
    
    // Swap capacities.
    
    t = m_capacity;
    m_capacity = other.m_capacity;
    other.m_capacity = t;
}

int Map::find(const KeyType& key) const
{
    // Do a linear search through the array.
    
    for (int pos = 0; pos < m_size; pos++)
        if (m_data[pos].m_key == key)
            return pos;
    return -1;
}

bool Map::doInsertOrUpdate(const KeyType& key, const ValueType& value,
                           bool mayInsert, bool mayUpdate)
{
    int pos = find(key);
    if (pos != -1)  // found
    {
        if (mayUpdate)
            m_data[pos].m_value = value;
        return mayUpdate;
    }
    if (!mayInsert)  // not found, and not allowed to insert
        return false;
    if (m_size == m_capacity)  // no room to insert
        return false;
    m_data[m_size].m_key = key;
    m_data[m_size].m_value = value;
    m_size++;
    return true;
}