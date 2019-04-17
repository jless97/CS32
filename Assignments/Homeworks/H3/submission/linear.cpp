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