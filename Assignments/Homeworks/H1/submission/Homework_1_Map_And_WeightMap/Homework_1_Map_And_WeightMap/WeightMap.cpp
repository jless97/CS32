// WeightMap.cpp

#include "WeightMap.h"

#include <iostream>
#include <string>
using namespace std;

WeightMap::WeightMap()
{
    Map m_client;
}

bool WeightMap::enroll(KeyType name, ValueType startWeight)
{
    if (startWeight <= 0)
        return false;
    return (m_client.insert(name, startWeight));
}

double WeightMap::weight(KeyType name) const
{
    if (! m_client.contains(name))
        return -1;
    ValueType currentWeight;
    m_client.get(name, currentWeight);
    return currentWeight;
}

bool WeightMap::adjustWeight(KeyType name, ValueType amt)
{
    if (! m_client.contains(name) || (weight(name) + amt) < 0)
        return false;
    ValueType originalWeight;
    m_client.get(name, originalWeight);
    ValueType newWeight = originalWeight + amt;
    m_client.update(name, newWeight);
    return true;
}

int WeightMap::size() const
{
    return m_client.size();
}

void WeightMap::print() const
{
    int mapSize = m_client.size() - 1;
    KeyType currentKey;
    ValueType currentValue;
    while(m_client.get(mapSize, currentKey, currentValue))
    {
        cout << currentKey << " " << currentValue << endl;
        mapSize--;
    }
}




