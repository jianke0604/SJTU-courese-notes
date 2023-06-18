#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdbool.h>

int A[9][9];
bool flag = true;

bool checkrow(int row)
{
    int check[9] = {0};
    for (int i = 0; i < 9; i++)
    {
        if (check[A[row][i] - 1] == 1)
        {
            flag = false;
            return false;
        }
        else
        {
            check[A[row][i] - 1] = 1;
        }
    }
    return true;
}
bool checkcol(int col)
{
    int check[9] = {0};
    for (int i = 0; i < 9; i++)
    {
        if (check[A[i][col] - 1] == 1)
        {
            flag = false;
            return false;
        }
        else
        {
            check[A[i][col] - 1] = 1;
        }
    }
    return true;
}

bool checkgrid(int row, int col)
{
    int check[9] = {0};
    for (int i = row; i < row + 3; i++)
    {
        for (int j = col; j < col + 3; j++)
        {
            if (check[A[i][j] - 1] == 1)
            {
                flag = false;
                return false;
            }
            else
            {
                check[A[i][j] - 1] = 1;
            }
        }
    }
    return true;
}
//检查数独的所有行的线程函数
void *checkrow_thread(void *param)
{
    int row = (int)param;
    if (checkrow(row))
    {
        printf("\033[1;32mrow %d is legal\033[0m\n", row);
    }
    else
    {
        printf("\033[1;31mrow %d is illegal\033[0m\n", row);
    }
    pthread_exit(0);
}
//检查数独的所有列的线程函数
void *checkcol_thread(void *param)
{
    int col = (int)param;
    if (checkcol(col))
    {
        printf("\033[1;32mcol %d is legal\033[0m\n", col);
    }
    else
    {
        printf("\033[1;31mcol %d is illegal\033[0m\n", col);
    }
    pthread_exit(0);
}
//检查数独的所有九宫格的线程函数
void *checkgrid_thread(void *param)
{
    int grid = (int)param;
    int row = grid / 3 * 3;
    int col = grid % 3 * 3;
    if (checkgrid(row, col))
    {
        printf("\033[1;32mgrid %d is legal\033[0m\n", grid);
    }
    else
    {
        printf("\033[1;31mgrid %d is illegal\033[0m\n", grid);
    }
    pthread_exit(0);
}


int main()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            scanf("%d", &A[i][j]);
        }
    }
    pthread_t tid[27];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    //创建检查数独的所有行的线程
    for (int i = 0; i < 9; i++)
    {
        pthread_create(&tid[i], &attr, checkrow_thread, (void *)i);
    }
    //创建检查数独的所有列的线程
    for (int i = 0; i < 9; i++)
    {
        pthread_create(&tid[i + 9], &attr, checkcol_thread, (void *)i);
    }
    //创建检查数独的所有九宫格的线程
    for (int i = 0; i < 9; i++)
    {
        pthread_create(&tid[i + 18], &attr, checkgrid_thread, (void *)i);
    }
    //等待所有线程结束
    for (int i = 0; i < 27; i++)
    {
        pthread_join(tid[i], NULL);
    }
    if (flag)
    {
        printf("\033[1;32mThis sudoku is legal\033[0m\n");
    }
    else
    {
        printf("\033[1;31mThis sudoku is illegal\033[0m\n");
    }
    return 0;
}