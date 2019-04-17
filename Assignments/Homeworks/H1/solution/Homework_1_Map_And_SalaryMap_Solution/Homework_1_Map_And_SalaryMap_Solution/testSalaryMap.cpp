#include "SalaryMap.h"

#include <iostream>
#include <cassert>

using namespace std;

int main()
{
    SalaryMap badJob;
    
    assert(badJob.size()==0);
    
    assert(badJob.add("Blah", -0.01)==false);
    assert(badJob.add("Jeff", 10.00)==true);
    assert(badJob.add("Jeff", 2.00)==false);
    assert(badJob.size()==1);
    assert(badJob.salary("Jeff")==10);
    
    assert(badJob.add("CEO", 9999)==true);
    assert(badJob.size()==2);
    
    assert(badJob.raise("Blah", 50)==false);
    assert(badJob.raise("Jeff", 50)==true);
    assert(badJob.salary("Jeff")==15);
    
    assert(badJob.raise("Jeff", 0)==true);
    assert(badJob.salary("Jeff")==15);
    
    assert(badJob.raise("Jeff", -50)==true);
    assert(badJob.salary("Jeff")==7.50);
    
    badJob.add("Bob", 8.00);
    badJob.add("Minimum Wage Worker", 7.50);
    
    badJob.print();
    
    SalaryMap goodJob(badJob);
    
    assert(goodJob.size()==4);
    assert(goodJob.salary("Jeff")==7.5);
    assert(goodJob.salary("Bob")==8);
    assert(goodJob.salary("Minimum Wage Worker")==7.5);
    assert(goodJob.salary("Jefff")==-1);
    
    SalaryMap lastJob;
    lastJob=badJob;
    
    assert(lastJob.size()==4);
    assert(lastJob.salary("Jeff")==7.5);
    assert(lastJob.salary("Bob")==8);
    assert(lastJob.salary("Minimum Wage Worker")==7.5);
    assert(lastJob.salary("Jefff")==-1);
    
    cout << "All tests passed!" << endl;
}