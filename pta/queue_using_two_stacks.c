#include <stdio.h>
#include <stdlib.h>

int ops, old_ops;

struct Stack
{
    int *data;
    int top;
    int size;
};

typedef struct Stack Stack;

Stack *createStack(int N)
{
    Stack *s = (Stack *)malloc(sizeof(Stack));
    s->data = (int *)malloc(N * sizeof(int));
    s->top = -1;
    s->size = N;
    return s;
}

int isEmpty(Stack *s)
{
    return s->top == -1;
}

int isFull(Stack *s)
{
    return s->top == s->size - 1;
}

void push(Stack *s, int data)
{
    if (isFull(s))
        return;
    s->data[++s->top] = data;
    ops++;
}

int pop(Stack *s)
{
    if (isEmpty(s))
        return -1;
    ops++;
    return s->data[s->top--];
}

struct Queue
{
    Stack *sIn, *sOut;
    int size;
};
typedef struct Queue Queue;

Queue *createQueue(int N)
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->sIn = createStack(N);
    q->sOut = createStack(N);
    q->size = N;
    return q;
}

void enQueue(Queue *q, int data)
{
    push(q->sIn, data);
    old_ops = ops;
}

void deQueue(Queue *q)
{
    if (isEmpty(q->sOut))
    {
        while (!isEmpty(q->sIn))
        {
            push(q->sOut, pop(q->sIn));
        }
        if(isEmpty(q->sOut))
        {
            printf("ERROR\n");
            return;
        }
    }
    int num = pop(q->sOut);
    printf("%d %d\n", num, ops - old_ops);
    old_ops = ops;
}

char getOp(void)
{
    char op;
    while ((op = getchar()) != 'I' && op != 'O')
        ;
    return op;
}

int main(void)
{
    int N;
    int data;
    char op;
    scanf("%d", &N);
    Queue *queue = createQueue(N);
    while (N--)
    {
        op = getOp();
        switch (op)
        {
        case 'I':
            scanf("%d", &data);
            enQueue(queue, data);
            break;
        case 'O':
            deQueue(queue);
            break;
        default:
            continue;
        }
    }
    return 0;
}