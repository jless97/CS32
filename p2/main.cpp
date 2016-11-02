// main.cpp

#include "Map.h"

#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    Map a;
    //For an empty map:
    assert(a.empty() == true); //test empty
    assert(a.size() == 0); //test size
    assert(a.erase("Franky") == false); //test erase for empty map
    assert(a.update("Patrick", 20) == false); //test update for empty map
    assert(a.contains("Patrick") == false); //test contains for empty map
    KeyType k;
    ValueType v;
    assert(a.get(k, v) == false); //test 2-parameter get for empty map
    for (int i = 0; i < a.size(); i++)
        assert(a.get(i, k, v) == false); //test 3-parameter get for empty map
    
    Map b = a; //Copy constructor for empty map
    assert(a.size() == 0); //check the size
    assert(b.size() == 0); //check the size
    
    Map c; //Assignment operator for empty map
    c = a;
    assert(a.size() == 0); //test size
    assert(c.size() == 0); //test size
    
    b.swap(c); //test swap for empty maps
    assert(b.empty() == true); //test empty
    assert(c.empty() ==  true); //test empty
    
    Map d;
    //For an empty map:
    assert(d.insert("a", 1.5) == true); //test insert for empty map
    assert(d.insert("a", 2.5) == false); //test insert for already existing key, but different value
    assert(d.insert("a", 2.5) == false); //test insert for already existing key/value pair
    assert(d.size() == 1); //test size after insert
    assert(d.contains("a") == true); //test contains for existing key
    assert(d.contains("b") == false); //test contains for key not present in map
    assert(d.erase("a") == true); //test erase for map with single node
    assert(d.size() == 0); //test size after erase
    assert(d.insertOrUpdate("b", 3.5) == true); //test insertOrUpdate for the insert
    assert(d.insertOrUpdate("b", 4.5) == true); //test insertOrUpdate for the update;
    assert(d.size() == 1); //test size after insertOrUpdate
    assert(d.update("a", 5.5) == false); //test update after erase
    assert(d.update("b", 5.5) == true); //test update for existing key
    assert(d.empty() == false); //test empty for map with nodes
    KeyType k1 = "b";
    ValueType v1;
    assert(d.get(k1, v1) && v1 == 5.5); //test 2 parameter get for a key present in the map
    //cerr <<  k1 << " " << v1 << endl;
    KeyType k2 = "c";
    ValueType v2 = 0;
    assert(d.get(k2, v2) == false); //test 2 parameter get for a key not present in the map
    //cerr << k2 << " " << v2 << endl;
    
    Map e;
    //For an empty map:
    //test erase for a two node map
    assert(e.insert("a", 1.5) == true);
    assert(e.insert("b", 2.5) == true);
    //e.dump();
    assert(e.erase("a") == true); //test erase for the first node, then the second
    //e.dump();
    assert(e.erase("b") == true);
    assert(e.insert("a", 1.5) == true);
    assert(e.insert("b", 2.5) == true);
    //e.dump();
    assert(e.erase("b") == true); //test erase for the second node, then the first
    //e.dump();
    assert(e.erase("a") == true);
    assert(e.size() == 0);
    
    Map f;
    //For an empty map:
    //test erase for a three node map
    assert(f.insert("a", 1.5) == true);
    assert(f.insert("b", 2.5) == true);
    assert(f.insert("c", 3.5) == true);
    //f.dump();
    assert(f.erase("a") == true); //test erase for the last node first
    //f.dump();
    assert(f.erase("b") == true);
    //f.dump();
    assert(f.erase("c") == true);
    assert(f.size() == 0);
    //f.dump();
    
    assert(f.insert("a", 1.5) == true);
    assert(f.insert("b", 2.5) == true);
    assert(f.insert("c", 3.5) == true);
    //f.dump();
    assert(f.erase("c") == true); //test erase for the first node first
    //f.dump();
    assert(f.erase("a") == true);
    //f.dump();
    assert(f.erase("b") == true);
    assert(f.size() == 0);
    //f.dump();
    
    assert(f.insert("a", 1.5) == true);
    assert(f.insert("b", 2.5) == true);
    assert(f.insert("c", 3.5) == true);
    //f.dump();
    assert(f.erase("b") == true); //test erase for the middle node first
    //f.dump();
    assert(f.erase("a") == true);
    //f.dump();
    assert(f.erase("c") == true);
    assert(f.size() == 0);
    //f.dump();
    
    Map g;
    //For an empty map:
    //test erase for a map with four nodes
    assert(g.insert("a", 1.5) == true);
    assert(g.insert("b", 2.5) == true);
    assert(g.insert("c", 3.5) == true);
    assert(g.insert("d", 4.5) == true);
    //g.dump();
    assert(g.erase("b") == true); //test erase for the node right before the tail
    //g.dump();
    assert(g.erase("c") == true);
    //g.dump();
    assert(g.erase("a") == true);
    //g.dump();
    assert(g.erase("d") == true);
    assert(g.size() == 0);
    //g.dump();
    
    assert(g.insert("a", 1.5) == true);
    assert(g.insert("b", 2.5) == true);
    assert(g.insert("c", 3.5) == true);
    assert(g.insert("d", 4.5) == true);
    //g.dump();
    assert(g.erase("c") == true); //test erase for the node right after the head
    //g.dump();
    assert(g.erase("b") == true);
    //g.dump();
    assert(g.erase("a") == true);
    //g.dump();
    assert(g.erase("d") == true);
    assert(g.size() == 0);
    //g.dump();
    
    Map h;
    //For an empty map:
    //test erase for a map with an odd number of nodes
    assert(h.insert("a", 1.5) == true);
    assert(h.insert("b", 2.5) == true);
    assert(h.insert("c", 3.5) == true);
    assert(h.insert("d", 4.5) == true);
    assert(h.insert("e", 5.5) == true);
    //h.dump();
    assert(h.erase("c") == true); //test erase for map with odd number of nodes
    //h.dump();
    assert(h.erase("a") == true);
    assert(h.erase("b") == true);
    assert(h.erase("d") == true);
    assert(h.erase("e") == true);
    assert(h.size() == 0);
    
    Map i;
    //For an empty map:
    //test erase for a map with an even number of nodes
    assert(i.insert("a", 1.5) == true);
    assert(i.insert("b", 2.5) == true);
    assert(i.insert("c", 3.5) == true);
    assert(i.insert("d", 4.5) == true);
    assert(i.insert("e", 5.5) == true);
    assert(i.insert("f", 6.5) == true);
    //i.dump();
    assert(i.erase("d") == true); //test erase for map with even number of nodes
    //i.dump();
    assert(i.erase("c") == true);
    //i.dump();
    assert(i.erase("a") == true);
    assert(i.erase("b") == true);
    assert(i.erase("e") == true);
    assert(i.erase("f") == true);
    assert(i.size() == 0);
    
    Map j;
    //For an empty map:
    //test get for a map with nodes
    assert(j.insert("a", 1.5) == true);
    assert(j.insert("b", 2.5) == true);
    KeyType k3;
    ValueType v3;
    for (int i = 0; i < j.size(); i++)
    {
        j.get(i, k3, v3);
        //cerr << k3 << " " << v3 << endl;
    }
    
    //test copy constructor
    
    Map l;
    assert(l.insert("a", 1.5) == true);
    assert(l.insert("b", 2.5) == true);
    
    Map m(l);
    //test copy constructor for a map with nodes
    assert(m.size() == 2);
    assert(m.contains("a") == true);
    assert(m.contains("b") == true);
    
    //test swap
    
    Map n;
    assert(n.insert("a", 1.5) == true);
    assert(n.insert("b", 2.5) == true);
    assert(n.insert("c", 3.5) == true);
    
    Map o;
    assert(o.insert("d", 4.5) == true);
    assert(o.insert("e", 5.5) == true);
    
    o.swap(n); //test swap for maps with different sizes
    assert(n.size() == 2);
    assert(o.size() == 3);
    
    Map p;
    assert(p.insert("a", 1.5) == true);
    assert(p.insert("b", 2.5) == true);
    Map q;
    
    p.swap(q); //test swap when one map is empty and the other is not
    assert(p.size() == 0);
    assert(q.size() == 2);
    
    Map r;
    assert(r.insert("a", 1.5) == true);
    assert(r.insert("b", 2.5) == true);
    
    Map s;
    assert(s.insert("c", 1.5) == true);
    assert(s.insert("d", 2.5) == true);
    
    r.swap(s); //test swap when both maps are the same size
    assert(r.contains("c") == true);
    assert(r.contains("d") == true);
    assert(s.contains("a") == true);
    assert(s.contains("b") == true);
    
    //test assignment operator
    
    Map t;
    assert(t.insert("a", 1.5) == true);
    t = t; //test assignment operator for assignment to itself
    assert(t.size() == 1);
    
    Map u;
    
    u = t; //test assignment for an empty map to a map with nodes
    assert(u.size() == 1);
    assert(u.contains("a") == true);
    
    Map w;
    assert(w.insert("b", 2.5) == true);
    assert(w.insert("c", 3.5) == true);
    
    w = u; //test assignment for a map with more nodes to be assignmed a map with less nodes
    assert(w.size() == 1);
    assert(u.size() == 1);
    
    Map x;
    assert(x.insert("a", 1.5) == true);
    assert(x.insert("b", 2.5) == true);
    
    Map y;
    assert(y.insert("c", 3.5) == true);
    assert(y.insert("d", 4.5) == true);
    
    x = y; //test assignment for maps with the same size
    assert(x.contains("c") == true);
    assert(x.contains("d") == true);
    
    //test combine
    Map m1;
    assert(m1.insert("a", 1.5) == true);
    assert(m1.insert("b", 2.5) == true);
    
    Map m2;
    assert(m2.insert("a", 1.5) == true);
    assert(m2.insert("b", 2.5) == true);
    
    Map result1;
    assert(result1.insert("c", 3.5) == true);
    assert(result1.insert("d", 4.5) == true);
    
    assert(combine(m1, m2, result1) == true); //test if m1 and m2 have the same key/value pairs
    assert(result1.size() == 2); //Also to test if result had nodes to begin with
    
    Map m3;
    assert(m3.insert("a", 1.5) == true);
    assert(m3.insert("b", 2.5) == true);
    
    Map m4;
    assert(m4.insert("a", 3.5) == true);
    assert(m4.insert("b", 4.5) == true);
    
    Map result2;
    
    assert(combine(m3, m4, result2) == false); //test if m1 and m2 have the same keys, but different values
    assert(result2.size() == 0);
    
    Map m5;
    assert(m5.insert("a", 1.5) == true);
    assert(m5.insert("b", 2.5) == true);
    
    Map result3;
    assert(m5.insert("c", 1.5) == true);
    assert(m5.insert("d", 2.5) == true);
    
    assert(combine(result3, m5, result3) == true); //test if one of maps is an alias of result
    assert(result3.size() == 4);
    
    //test subtract
    Map m6;
    assert(m6.insert("a", 1.5) == true);
    assert(m6.insert("b", 2.5) == true);
    assert(m6.insert("c", 3.5) == true);
    assert(m6.insert("d", 4.5) == true);
    
    Map m7;
    assert(m7.insert("a", 1.5) == true);
    assert(m7.insert("b", 2.5) == true);
    
    Map result4;
    assert(result4.insert("c", 3.5) == true);
    assert(result4.insert("d", 4.5) == true);
    
    subtract(m6, m7, result1); //test if m6 has pairs that m7 doesn't
    assert(result4.size() == 2); //Also to test if result had nodes to begin with
    
    Map m8;
    assert(m8.insert("a", 1.5) == true);
    assert(m8.insert("b", 2.5) == true);
    
    Map m9;
    assert(m9.insert("a", 1.5) == true);
    assert(m9.insert("b", 2.5) == true);
    
    Map result5;
    assert(result5.insert("c", 3.5) == true);
    assert(result5.insert("d", 4.5) == true);
    
    subtract(m8, m9, result5); //test if m8 is identical to m9
    assert(result5.size() == 0); //Also to test if result had nodes to begin with
    
    cout << "All tests passed." << endl;
    
    return 0;
}