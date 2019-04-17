#include <iostream>
using namespace std;

bool somePredicate(double x)
{
    return x < 0;
}

bool anyTrue (const double a[], int n)
{
    if (n <= 0)
        return false;
    if (somePredicate(a[0]))
        return true;
    return anyTrue(a + 1, n - 1);
}

int countTrue(const double a[], int n)
{
    if (n <= 0)
        return 0;
    int count = 0;
    if (somePredicate(a[0]))
    {
        count = 1;
        return count + countTrue(a + 1, n - 1);
    }
    else
        return countTrue(a + 1, n - 1);
}

int firstTrue(const double a[], int n)
{
    if (n <= 0)
        return -1;
    if (somePredicate(a[0]))
        return 0;
    int count = firstTrue(a + 1, n - 1);
    if (count == -1)
        return -1;
    else
        return count + 1;
    
}

int indexOfMin(const double a[], int n)
{
    if (n <= 0)
        return -1;
    if (n == 1)
        return 0;
    int count = 0;
    if (a[0] > a[n - 1])
    {
        count = 1;
        return count + indexOfMin(a + 1, n - 1);
    }
    else
        return indexOfMin(a, n - 1);
}

bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 <= 0)
        return true;
    if (n1 <= 0)
        return false;
    if (a1[0] != a2[0])
        return includes(a1 + 1, n1 - 1, a2, n2);
    else
        return includes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
}

int main()
{
    //anyTrue Test
    double b[3] = {1,2,-3};
    bool m = anyTrue(b, 3);
    cout << m << endl;
    
    //firstTrue Test
    double a[3] = {1,2,3};
    int n = firstTrue(a, 3);
    cout << n << endl;
    
    //countTrue Test
    double array[5] = {-1,-2,-3,-4,-5};
    int p = countTrue(array, 5);
    cout << p << endl;
    
    //indexOfMin Test
    double arr[10] = {0,1,-1,3,-2,4,-5,3,0,0};
    int o = indexOfMin(arr, 10);
    cout << o << endl;
    
    //includes Test
    double c[7] = {10, 50, 40, 20, 50, 40, 30};
    double d[3] = {10, 20, 20};
    bool isTrue = includes(c,7,d,3);
    cout << isTrue << endl;
    
    return 0;
}