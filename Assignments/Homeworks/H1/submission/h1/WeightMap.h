// WeightMap.h

#ifndef WEIGHTMAP_INCLUDED
#define WEIGHTMAP_INCLUDED

#include "Map.h"

class WeightMap
{
public:
    WeightMap();       // Create an empty weight map.
    
    bool enroll(KeyType name, ValueType startWeight);
    // If a person with the given name is not currently in the map,
    // there is room in the map, and the startWeight is not negative,
    // add an entry for that person and weight and return true.
    // Otherwise make no change to the map and return false.
    
    double weight(KeyType name) const;
    // If a person with the given name is in the map, return that
    // person's weight; otherwise, return -1.
    
    bool adjustWeight(KeyType name, ValueType amt);
    // If no person with the given name is in the map or if
    // weight() + amt is negative, make no change to the map and return
    // false.  Otherwise, change the weight of the indicated person by
    // the given amount and return true.  For example, if amt is -8.2,
    // the person loses 8.2 pounds; if it's 3.7, the person gains 3.7
    // pounds.
    
    int size() const;  // Return the number of people in the WeightMap.
    
    void print() const;
    // Write to cout one line for every person in the map.  Each line
    // has the person's name, followed by one space, followed by that
    // person's weight.
    
private:
    Map m_client;
};

#endif // WEIGHTMAP_INCLUDED