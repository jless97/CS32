#include "newMap.h"

#include <iostream>
#include <cassert>

using namespace std;

int main()
{
    Map a(1000);   // a can hold at most 1000 key/value pairs
    Map b(5);      // b can hold at most 5 key/value pairs
    Map c;         // c can hold at most DEFAULT_MAX_ITEMS key/value pairs
    KeyType k[6] = {"a", "ab", "abc", "x", "xy", "xyz"};
    ValueType v  = 5.5;
    
    // No failures inserting pairs with 5 distinct keys into b
    for (int n = 0; n < 5; n++)
        assert(b.insert(k[n], v));
    
    // Failure if we try to insert a pair with a sixth distinct key into b
    assert(!b.insert(k[5], v));
    
    // When two Maps' contents are swapped, their capacities are swapped
    // as well:
    a.swap(b);
    
    assert(!a.insert(k[5], v)  &&  b.insert(k[5],v));
    
    cout << "Passed all default tests." << endl;
    
    //Customized test cases below
    
    Map trashCan(5);
    
    KeyType k2[5] = {"one", "two", "three", "four", "five"};
    
    for (int n = 0; n < 5; n++)
        assert(trashCan.insert(k2[n], 100));
    assert(!trashCan.insert("six", 6));
    assert(trashCan.size()==5);
    
    cout << "Passed tests with first trash can." << endl;
    
    Map largeTrashCan(100);
    
    assert(largeTrashCan.insert("crap", 1));
    assert(largeTrashCan.insert("junk", 1));
    assert(!largeTrashCan.insert("crap", 2));
    assert(largeTrashCan.size()==2);
    
    Map sameLargeTrashCan(largeTrashCan);
    
    cout << "sameLargeTrashCan size is " << sameLargeTrashCan.size() << endl;
    
    KeyType tempK;
    ValueType tempV;
    for(int i=0; i<sameLargeTrashCan.size(); i++)
    {
        sameLargeTrashCan.get(i, tempK, tempV);
        cerr << tempK << " -> " << tempV << endl;
    }
    
    assert(sameLargeTrashCan.size()==2);
    
    assert(sameLargeTrashCan.contains("crap")==true);
    assert(sameLargeTrashCan.contains("junk")==true);
    assert(sameLargeTrashCan.contains("blah")==false);
    
    Map otherTrashCan(5);
    otherTrashCan = sameLargeTrashCan;
    
    for(int i=0; i<otherTrashCan.size(); i++)
    {
        otherTrashCan.get(i, tempK, tempV);
        cerr << tempK << " -> " << tempV << endl;
    }
    
    assert(otherTrashCan.size()==2);
    
    assert(otherTrashCan.contains("crap")==true);
    assert(otherTrashCan.contains("junk")==true);
    assert(otherTrashCan.contains("blah")==false);
    
    trashCan.swap(otherTrashCan);
    
    assert(trashCan.size()==2);
    assert(otherTrashCan.size()==5);
    assert(otherTrashCan.contains("one") && otherTrashCan.contains("two") && otherTrashCan.contains("five"));
    assert(trashCan.contains("crap") && trashCan.contains("junk"));
    assert(trashCan.contains("one")==false);
    
    
    assert(sameLargeTrashCan.insert("asdf", 10));
    assert(sameLargeTrashCan.insert("eww", 11));
    assert(sameLargeTrashCan.insert("gross", 12));
    assert(sameLargeTrashCan.insert("nasty", 13));
    
    otherTrashCan=sameLargeTrashCan;
    trashCan=sameLargeTrashCan;
    
    assert(otherTrashCan.size()==6 && trashCan.size()==6);
    
    cout << "Passed all tests!" << endl;
}