/*
lower_bound
-> key값 이상

upper_bound
-> key과 초과
*/

int my_lower_bound(int arr[], int key, int size)
{
    int start = 0;
    int end = size - 1;

    while (start < end)
    {
        int mid = (start + end) / 2;
        if (arr[mid] >= key)
        {
            end = mid;
        }
        else
        {
            start = mid + 1;
        }
    }

    return end;
}

int my_upper_bound(int arr[], int key, int size)
{
    int start = 0;
    int end = size - 1;

    while (start < end)
    {
        int mid = (start + end) / 2;
        if (arr[mid] > key)
        {
            end = mid;
        }
        else
        {
            start = mid + 1;
        }
    }

    return end;
}
