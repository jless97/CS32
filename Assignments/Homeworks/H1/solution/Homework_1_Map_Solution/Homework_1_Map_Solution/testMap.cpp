#include "Map.h"

#include <iostream>
#include <cassert>

using namespace std;

int main()
{
    Map m;  // maps strings to doubles
    
    cout << m.size() << endl;
    
    assert(m.empty());
    ValueType v = -1234.5;
    assert( !m.get("abc", v)  &&  v == -1234.5); // v unchanged by get failure
    m.insert("xyz", 9876.5);
    assert(m.size() == 1);
    KeyType k = "hello";
    assert(m.get(0, k, v)  &&  k == "xyz"  &&  v == 9876.5);
    
    Map gpas;
    gpas.insert("Fred", 2.956);
    gpas.insert("Ethel", 3.538);
    double v0;
    string k1;
    assert(gpas.get(1,k1,v0)  &&  (k1 == "Fred"  ||  k1 == "Ethel"));
    string k2;
    assert(gpas.get(1,k2,v0)  &&  k2 == k1);
    
    cout << "Passed first test." << endl;
    
    Map gpas2;
    gpas2.insert("Fred", 2.956);
    assert(!gpas2.contains(""));
    gpas2.insert("Ethel", 3.538);
    gpas2.insert("", 4.000);
    gpas2.insert("Lucy", 2.956);
    assert(gpas2.contains(""));
    gpas2.erase("Fred");
    assert(gpas2.size() == 3);
    assert(gpas2.contains("Lucy")  &&  gpas2.contains("Ethel")  && gpas2.contains(""));
    
    cout << "Passed second test." << endl;
    
    Map helloMap;
    helloMap.insert("Delete Me", 123);
    helloMap.erase("Delete Me");
    
    Map m1;
    m1.insert("Fred", 2.956);
    Map m2;
    m2.insert("Ethel", 3.538);
    m2.insert("Lucy", 2.956);
    m1.swap(m2);
    
    assert(m1.size() == 2  &&  m1.contains("Ethel")  &&  m1.contains("Lucy")  && m2.size() == 1  &&  m2.contains("Fred"));
    
    cout << "Passed third test. Default test cases have finished." << endl;
    
    //Customized test cases below
    
    Map hereWeGo;
    
    ValueType val;
    KeyType x = "Nathan";
    
    assert(hereWeGo.empty()==true);
    assert(hereWeGo.size()==0);
    
    hereWeGo.insert("Nathan", 3.00);
    assert(hereWeGo.empty()!=true);
    assert(hereWeGo.size()==1);
    
    hereWeGo.insert("Nathan", 3.00);
    assert(hereWeGo.empty()!=true);
    assert(hereWeGo.size()==1);
    
    hereWeGo.insert("John Doe", 2.00);
    assert(hereWeGo.size()==2);
    
    assert(hereWeGo.get(x, val)==true);
    assert(val==3.00);
    assert(hereWeGo.get("Nathan", val)==true);
    assert(val==3.00);
    assert(hereWeGo.get("N", val)==false);
    
    hereWeGo.insertOrUpdate("Nathan", 4.00);
    assert(hereWeGo.size()==2);
    
    assert(hereWeGo.get("Nathan", val)==true);
    assert(val==4.00);
    
    KeyType zName;
    ValueType zVal;
    
    for(int i=0; i<hereWeGo.size(); i++)
    {
        hereWeGo.get(i, zName, zVal);
        cerr << zName << ": " << zVal << endl;
    }
    
    hereWeGo.insertOrUpdate("John Doe", 2.50);
    hereWeGo.insertOrUpdate("Jane Doe", 2.49);
    assert(hereWeGo.size()==3);
    assert(hereWeGo.get("John Doe", val)==true);
    assert(val==2.50);
    assert(hereWeGo.get("Jane Doe", val)==true);
    assert(val==2.49);
    
    assert(hereWeGo.erase("hey")==false);
    
    cout << "Tests involving a single Map passed." << endl;
    
    Map thereTheyGo;
    
    thereTheyGo.insertOrUpdate("Bruin Bear", 5.00);
    thereTheyGo.insertOrUpdate("Tommy Trojan", 1.00);
    assert(thereTheyGo.size()==2);
    
    hereWeGo.swap(thereTheyGo);
    
    assert(hereWeGo.size()==2 && thereTheyGo.size()==3);
    
    assert(hereWeGo.contains("Bruin Bear") && hereWeGo.contains("Tommy Trojan")==true);
    assert(hereWeGo.contains("Nathan")==false);
    assert(thereTheyGo.contains("Nathan") && thereTheyGo.contains("John Doe") && thereTheyGo.contains("Jane Doe")==true);
    
    cerr << "hereWeGo Map:" << endl;
    for(int i=0; i<hereWeGo.size(); i++)
    {
        hereWeGo.get(i, zName, zVal);
        cerr << zName << ": " << zVal << endl;
    }
    
    cerr << "thereTheyGo Map:" << endl;
    for(int i=0; i<thereTheyGo.size(); i++)
    {
        thereTheyGo.get(i, zName, zVal);
        cerr << zName << ": " << zVal << endl;
    }
    
    assert(thereTheyGo.erase("Nathan")==true);
    assert(thereTheyGo.size()==2);
    assert(thereTheyGo.contains("Nathan")==false);
    
    cout << "All tests passed!" << endl;
    
}