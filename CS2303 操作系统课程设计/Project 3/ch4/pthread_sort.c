#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

int n;
int *arr;
int *sorted;

struct pthread_sort
{
    /* data */
    int start;
    int end;
};

int compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

void *sort(void *args)
{
    struct pthread_sort *arg = (struct pthread_sort *)args;
    int start = arg->start;
    int end = arg->end;
    qsort(arr + start, end - start + 1, sizeof(int), compare);
    // for (int i = start; i <= end; i++)
    // {
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");
    pthread_exit(NULL);
}

void *merge(void *args)
{
    struct pthread_sort *arg = (struct pthread_sort *)args;
    int start = arg->start;
    int end = arg->end;
    int mid = (start + end + 1) / 2;
    int i = start, j = mid + 1, k = start;
    while (i <= mid && j <= end)
    {
        if (arr[i] < arr[j])
        {
            sorted[k++] = arr[i++];
        }
        else
        {
            sorted[k++] = arr[j++];
        }
    }
    while (i <= mid)
    {
        sorted[k++] = arr[i++];
    }
    while (j <= end)
    {
        sorted[k++] = arr[j++];
    }
    pthread_exit(NULL);
}

int main()
{
    printf("Enter the number of elements:\n ");
    scanf("%d", &n);
    printf("Enter the elements:\n");
    arr = (int *)malloc(n * sizeof(int));
    sorted = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }
    struct pthread_sort args[3];
    args[0].start = 0;
    args[0].end = n / 2;
    args[1].start = n / 2 + 1;
    args[1].end = n - 1;
    args[2].start = 0;
    args[2].end = n - 1;
    pthread_t tid[3];
    for (int i = 0; i < 2; i++)
    {
        pthread_create(&tid[i], NULL, sort, &args[i]);
    }
    for (int i = 0; i < 2; i++)
    {
        pthread_join(tid[i], NULL);
    }
    pthread_create(&tid[2], NULL, merge, &args[2]);
    pthread_join(tid[2], NULL);
    printf("After multithreaded sort, the sorted array is:\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", sorted[i]);
    }
    printf("\n");
    return 0;
}