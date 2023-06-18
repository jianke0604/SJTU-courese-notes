# Project 7

$$
\textbf{秦啸涵\quad 521021910604}
$$

## 实现连续内存分配器

由于内存被划分为等大小的页，总内存为`1MB`，一页为`4kB`，因此共有256页。我初始采用大小为256的数组来进行内存的模拟(后续更改为双链表)，`main()`函数主体如下

```c
int main(int argc, char *argv[])
{
    int shouldrun = 1;
    char* input;
    input = (char*) malloc(MAX_LINE * sizeof(char));
    while(shouldrun)
    {      
        printf("\033[1;35mallocator>\033[0m");
        fflush(stdout);
        fgets(input, 100, stdin);
        input[strlen(input) - 1] = '\0';
        if (strcmp(input, "exit") == 0) {
            shouldrun = 0;
            continue;
        }
        char* args[10];
        for (int i=0; i<10; i++)
        {
            args[i] = (char*) malloc(10 * sizeof(char));
        }
        int arg_num = parse(input, args);					//传递参数
        if (arg_num == 1 && strcmp(args[0], "STAT") == 0)
        {
            printstatics();									//支持STAT指令
        }
        if (arg_num == 4 && strcmp(args[0], "RQ") == 0)
        {
            if (strcmp(args[3], "F") == 0)
                first_fit(args[1], atoi(args[2]));
            if (strcmp(args[3], "B") == 0)
                best_fit(args[1], atoi(args[2]));
            if (strcmp(args[3], "W") == 0)
                worst_fit(args[1], atoi(args[2]));			//三种内存分配方式
        }
        if (arg_num == 2 && strcmp(args[0], "RL") == 0)
        {
            RL(args[1]);									//支持内存的释放
        }
    }
	return 0;
}
```

实现结果如下：

![image-20230524172610188](C:\Users\HP\AppData\Roaming\Typora\typora-user-images\image-20230524172610188.png)

## 实现三种分配策略

### first-fit()

```c
void first_fit(char* name, int size)
{
    int i = 0;
    char* tmp = (char*) malloc(10 * sizeof(char));
    strcpy(tmp, name+1);
    int pid = atoi(tmp);
    int page = size/4096 + (size%4096 == 0 ? 0 : 1);
    while (i < 256)
    {
        if (memory[i] == 0)
        {
            int j = i;
            while (j < 256 && memory[j] == 0)
            {
                j++;
            }
            if (j - i >= page)
            {
                for (int k=i; k<i+page; k++)
                {
                    memory[k] = pid+1;
                }
                printf("Allocated %s from page %d to page %d\n", name, i, i+page-1);
                return;
            }
            else
            {
                i = j;
            }
        }
        else
        {
            i++;
        }
    }
    printf("Cannot allocate %s, %d\n", name, size);
}
```

### best-fit()

```c
void best_fit(char* name, int size)
{
    int i = 0;
    char* tmp = (char*) malloc(10 * sizeof(char));
    strcpy(tmp, name+1);
    int pid = atoi(tmp);
    int page = size/4096 + (size%4096 == 0 ? 0 : 1);
    int min = 256;
    int min_index = -1;
    while (i < 256)
    {
        if (memory[i] == 0)
        {
            int j = i;
            while (j < 256 && memory[j] == 0)
            {
                j++;
            }
            if (j - i >= page && j - i < min)
            {
                min = j - i;
                min_index = i;
            }
            i = j;
        }
        else
        {
            i++;
        }
    }
    if (min_index == -1)
    {
        printf("Cannot allocate %s, %d\n", name, size);
        return;
    }
    for (int k=min_index; k<min_index+page; k++)
    {
        memory[k] = pid+1;
    }
    printf("Allocated %s from page %d to page %d\n", name, min_index, min_index+page-1);
}
```

### worst-fit()

```c
void worst_fit(char* name, int size)
{
    int i = 0;
    char* tmp = (char*) malloc(10 * sizeof(char));
    strcpy(tmp, name+1);
    int pid = atoi(tmp);
    int page = size/4096 + (size%4096 == 0 ? 0 : 1);
    int max = 0;
    int max_index = -1;
    while (i < 256)
    {
        if (memory[i] == 0)
        {
            int j = i;
            while (j < 256 && memory[j] == 0)
            {
                j++;
            }
            if (j - i >= page && j - i > max)
            {
                max = j - i;
                max_index = i;
            }
            i = j;
        }
        else
        {
            i++;
        }
    }
    if (max_index == -1)
    {
        printf("Cannot allocate %s, %d\n", name, size);
        return;
    }
    for (int k=max_index; k<max_index+page; k++)
    {
        memory[k] = pid+1;
    }
    printf("Allocated %s from page %d to page %d\n", name, max_index, max_index+page-1);
}
```

实现结果上图已展示

## bonus:使用有序双链表加速best-fit()和worst-fit()

更改内存结构为双链表，并更改对应的插入、删除逻辑

```c
typedef struct Block{
    int used;
    int start;
    int end;
    char name[5];
}block;

typedef struct Node{
   block *hole;
   struct Node* next;
   struct Node* pre;
}node;
```

更改后的`bestFit()`，只需要按链表顺序查找至首个满足要求的`hole`即可

```c
int bestFit(int process_size, char* name){
    int min_fit_hole_size = 1e9;
    node*tmp = head -> next;
    while(tmp != tail){
       if(tmp -> hole -> used){
           tmp = tmp -> next;
           continue;
       }
       int hole_size = tmp -> hole -> end - tmp -> hole -> start + 1;
       if(hole_size < process_size){
           tmp = tmp -> next;
           continue;
       }
       else{
           min_fit_hole_size = (hole_size < min_fit_hole_size) ? hole_size : min_fit_hole_size;
           tmp = tmp -> next;
       }
    }

    tmp = head -> next;
    if(min_fit_hole_size == -1) return -1;
    while(1){
         if(tmp -> hole -> used){
           tmp = tmp -> next;
         }else{
           int hole_size = tmp -> hole -> end - tmp -> hole -> start + 1;
           if(hole_size == min_fit_hole_size){
               insert_process(tmp, process_size, name);
               break;
           }
           tmp = tmp -> next;
       }
    }
    return 0;
}
```

更改后的`worstFit()`，直接顺着链表找最大的`hole`，判断是否满足要求即可。(注意这里面涉及到一些特判，避免出现段错误)

```c
int worstFit(int process_size, char* name){
    // find the largest hole;
    int max_hole_size = -1;
    node*tmp = head -> next;
    while(tmp != tail){
       if(tmp -> hole -> used){
           tmp = tmp -> next;
       }else{
           int hole_size = tmp -> hole -> end - tmp -> hole -> start;
           max_hole_size = (hole_size > max_hole_size) ? hole_size : max_hole_size;
           tmp = tmp -> next;
       }
    }
    tmp = head -> next;
    //printf("%d", process_size);
    if(max_hole_size + 1 < process_size) return -1;
    while(1){
         if(tmp -> hole -> used){
           tmp = tmp -> next;
         }else{
           int hole_size = tmp -> hole -> end - tmp -> hole -> start;
           if(hole_size == max_hole_size){
              // printf("insert");
               insert_process(tmp, process_size, name);
               break;
           }
           tmp = tmp -> next;
       }
    }
    return 0;

}
```

实现结果：

![image-20230524195027182](C:\Users\HP\AppData\Roaming\Typora\typora-user-images\image-20230524195027182.png)
