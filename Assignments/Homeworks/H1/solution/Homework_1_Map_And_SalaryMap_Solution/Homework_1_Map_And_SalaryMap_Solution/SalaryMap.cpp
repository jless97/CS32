// SalaryMap.cpp

#include "Map.h"
#include "SalaryMap.h"
#include <iostream>
using namespace std;

// Actually, we did not have to declare and implement the default
// constructor:  If we declare no constructors whatsoever, the compiler
// writes a default constructor for us that would do nothing more than
// default construct the m_salaryMap data member.

// We do not have to declare a destructor, copy constructor, or assignment
// operator, because the compiler-generated ones do the right thing.

SalaryMap::SalaryMap()
{}

bool SalaryMap::raise(string name, double pct)
{
    double sal;
    if (pct < -100  ||  !m_salaryMap.get(name, sal))
        return false;
    return m_salaryMap.update(name, sal * (1 + pct/100));
}

void SalaryMap::print() const
{
    string name;
    double salary;
    for (int n = 0; n < m_salaryMap.size(); n++)
    {
        m_salaryMap.get(n, name, salary);
        cout << name << ' ' << salary << endl;
    }
}