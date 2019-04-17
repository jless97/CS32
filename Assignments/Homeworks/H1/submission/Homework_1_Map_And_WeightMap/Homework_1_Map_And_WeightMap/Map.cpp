// Map.cpp

#include "Map.h"

#include <iostream>
#include <string>
using namespace std;

Map::Map()
{
    m_counter = 0;
}

bool Map::empty() const
{
    if (size() == 0)
        return true;
    return false;
}

int Map::size() const
{
    return m_counter;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
    if (size() >= DEFAULT_MAX_ITEMS)
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



