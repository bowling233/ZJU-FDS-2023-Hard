#include <stdio.h>
#include <stdlib.h>

struct stack
{
    int size;
    int *bottom, *top;
};
typedef struct stack Stack;

void printStack(Stack *s)
{
    if (s->bottom != s->top)
    {
        // printf("Stack:");
        int *ptr = s->bottom + 1;
        while (ptr != s->top)
        {
            printf("%d ", *(ptr++));
        }
        printf("%d", *(ptr));
    }
    else
    {
        printf("Empty");
    }
    putchar('\n');
}

int peek(Stack *s)
{
    if (s->top == s->bottom)
        return -1;
    return *(s->top);
}

int push(Stack *s, int num)
{
    //printf("[Push] %d to ", num);
    //printStack(s);
    if (s->top - s->bottom == s->size)
        return -1;
    s->top++;
    *(s->top) = num;
    return 1;
}

int pop(Stack *s)
{
    //printf("[Pop] from ");
    //printStack(s);
    if (s->top == s->bottom)
        return -1;
    int temp = *(s->top);
    s->top--;
    return temp;
}

int main(void)
{
    int m, n, k;
    scanf("%d%d%d", &m, &n, &k);

    for (int k_i = 0; k_i < k; k_i++)
    {
        int *ptr = (int *)malloc(sizeof(int) * (m + 1));
        Stack *s = (Stack *)malloc(sizeof(Stack));
        s->size = m;
        s->bottom = s->top = ptr;

        int valid = 1;
        int input, max;
        scanf("%d", &input);
        max = input;
        // push 1 to input to stack, then pop out
        for (int i = 1; i <= input; i++)
        {
            if (push(s, i) == -1)
            {
                valid = 0;
                printf("NO\n");
                while (getchar() != '\n')
                    ;
                break;
            }
        }
        if (!valid)
            continue;
        pop(s);

        // if >, then push
        // if <, then pop
        for (int i = 2; i <= n; i++)
        {
            scanf("%d", &input);
            if (input < max)
            {
                if (pop(s) != input)
                {
                    valid = 0;
                    printf("NO\n");
                    while (getchar() != '\n')
                        ;
                    break;
                }
            }
            else if (input > max)
            {
                for (int i = max + 1; i <= input; i++)
                {
                    if (push(s, i) == -1)
                    {
                        valid = 0;
                        printf("NO\n");
                        while (getchar() != '\n')
                            ;
                        break;
                    }
                }
                if (!valid)
                    break;
                pop(s);
                max = input;
            }
            else
            {
                valid = 0;
                printf("NO\n");
                while (getchar() != '\n')
                    ;
                break;
            }
        }
        if (valid)
            printf("YES\n");
    }
    return 0;
}