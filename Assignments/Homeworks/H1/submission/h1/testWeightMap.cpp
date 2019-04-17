// testWeightMap.cpp

#include "WeightMap.h"

#include <iostream>
#include <cassert>

using namespace std;

int main()
{
    WeightMap a;
    
    assert(a.size()==0);
    
    assert(a.enroll("Fred", -10) == false);
    assert(a.enroll("John", 20) == true);
    assert(a.enroll("Bill", 50) == true);
    assert(a.enroll("Carl", 70) == true);
    
    assert(a.size() == 3);
    
    assert(a.weight("John") != 10);
    assert(a.weight("John") == 20);
    assert(a.weight("Bill") == 50);
    assert(a.weight("Carl") == 70);
    
    assert(a.adjustWeight("Wendy",10) == false);
    assert(a.adjustWeight("John", 10));
    assert(a.adjustWeight("Bill", 10));
    assert(a.adjustWeight("Carl", 10));
    
    assert(a.weight("John") == 30);
    assert(a.weight("Bill") == 60);
    assert(a.weight("Carl") == 80);
    
    assert(a.size() == 3);
    
    a.print();
    
    cout << "All tests passed." << endl;
    
    return 0;
}