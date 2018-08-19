bool allTrue(const string a[], int n)
{
    if (n < 1)
        return true;
    return (somePredicate(a[0]) && allTrue(a+1, n-1));
}

int countFalse(const string a[], int n)
{
    if (n < 1)
        return 0;
    return ((!somePredicate(a[0]))+countFalse(a+1,n-1));
}

int firstFalse(const string a[], int n)
{
    if (n == 0)
        return -1;
    int indexWhereFalse = -1;
    if (somePredicate(a[0]) == 0)
        indexWhereFalse = 0;
    else
    {
        int recursiveIndex = firstFalse(a + 1, n - 1) + 1;
        if (recursiveIndex != 0)
            indexWhereFalse = recursiveIndex;
    }
    return indexWhereFalse;
}

int indexOfLeast(const string a[], int n)
{
    if (n < 1)
        return -1;
    if (n == 1)
        return 0;
    int index;
    int recursiveLeast = indexOfLeast(a+1, n-1) + 1;
    if (a[0] <= a[recursiveLeast])
        index = 0;
    else
        index = recursiveLeast;
    return index;
}

bool includes(const string a1[], int n1, const string a2[], int n2)
{
    if (n2 < 1)
        return true;
    if (n1 < 1)
        return false;
    return (((a1[0] == a2[0]) && includes(a1+1, n1-1, a2+1, n2-1)) || (includes(a1+1, n1-1, a2, n2)));
}
