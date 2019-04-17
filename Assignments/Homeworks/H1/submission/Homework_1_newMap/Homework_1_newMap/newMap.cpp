// newMap.cpp

#include "newMap.h"

#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

Map::Map()
{
    m_counter = 0;
    m_maximumSize = DEFAULT_MAX_ITEMS;
    m_studentMap = new MapPair[DEFAULT_MAX_ITEMS];
}

Map::Map(int max)
{
    if (max < 0)
    {
        cout << "***** Cannot create Map with negative size!" << endl;
        exit(1);
    }
    m_counter = 0;
    m_maximumSize = max;
    m_studentMap = new MapPair[m_maximumSize];
}

Map::Map(const Map& other)
{
    m_counter = 0;
    m_maximumSize = other.m_maximumSize;
    m_studentMap = new MapPair[m_maximumSize];
    for (int i = 0; i < other.size(); i++)
    {
        KeyType copyKey;
        ValueType copyValue;
        other.get(i, copyKey, copyValue);
        insert(copyKey, copyValue);
    }
    
}

Map& Map::operator=(const Map& rhs)
{
    //delete [] m_studentMap;
    if (this != &rhs)
    {
        Map temp(rhs);
        delete [] m_studentMap;
        m_counter = 0;
        m_maximumSize = temp.m_maximumSize;
        m_studentMap = new MapPair[m_maximumSize];
        for (int i = 0; i < temp.size(); i++)
        {
            KeyType copyKey;
            ValueType copyValue;
            temp.get(i, copyKey, copyValue);
            insert(copyKey, copyValue);
        }
    }
    return *this;
}

Map::~Map()
{
    delete [] m_studentMap;
}

bool Map::empty() const
{
    if (m_counter == 0)
        return true;
    return false;
}

int Map::size() const
{
    return m_counter;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
    if (m_counter >= m_maximumSize)
        return false;
    for (int i = 0; i < size(); i++)
    {
        if (m_studentMap[i].key == key)
            return false;
    }
    MapPair newStudent;
    newStudent.key = key;
    newStudent.value = value;
    m_studentMap[m_counter] = newStudent;
    m_counter++;
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
    for (int i = 0; i < size(); i++)
    {
        if (m_studentMap[i].key == key)
        {
            m_studentMap[i].value = value;
            return true;
        }
    }
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    if (update(key, value))
        return true;
    else
        insert(key, value);
    return false;
}

bool Map::erase(const KeyType& key)
{
    for (int i = 0; i < size(); i++)
    {
        if (m_studentMap[i].key == key)
        {
            m_counter--;
            for (int j = i; j < size(); j++)
            {
                m_studentMap[j] = m_studentMap[j + 1];
            }
            return true;
        }
    }
    return false;
}

bool Map::contains(const KeyType& key) const
{
    for (int i = 0; i < size(); i++)
    {
        if (m_studentMap[i].key == key)
            return true;
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    for (int i = 0; i < size(); i++)
    {
        if (m_studentMap[i].key == key)
        {
            value = m_studentMap[i].value;
            return true;
        }
    }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i >= 0 && i < size())
    {
        key = m_studentMap[m_counter - i - 1].key;
        value = m_studentMap[m_counter - i - 1].value;
        return true;
    }
    return false;
}

void Map::swap(Map& other)
{
    Map tempMap;
    tempMap = *this;
    *this = other;
    other = tempMap;
}

void Map::dump() const
{
    
}



