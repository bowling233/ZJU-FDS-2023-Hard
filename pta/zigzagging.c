#include <stdio.h>
#include <stdlib.h>

typedef struct node Node;
struct node
{
    int key, layer;
    const int *istart, *pstart;
    int length;
    Node *left, *right;
};

void Construct(Node *n)
{
    int iIndex = 0;
    for (; n->istart[iIndex] != n->key && iIndex < n->length; iIndex++)
        ;
    if (iIndex != n->length - 1) // has right tree
    {
        n->right = (Node *)malloc(sizeof(Node));
        n->right->istart = n->istart + iIndex + 1;
        n->right->pstart = n->pstart + iIndex;
        n->right->length = n->length - 1 - iIndex;
        n->right->key = n->right->pstart[n->right->length - 1];
        n->right->layer = n->layer + 1;
        n->right->left = n->right->right = NULL;
        Construct(n->right);
    }
    if (iIndex != 0) // has left tree
    {
        n->left = (Node *)malloc(sizeof(Node));
        n->left->istart = n->istart;
        n->left->pstart = n->pstart;
        n->left->length = iIndex;
        n->left->key = n->left->pstart[n->left->length - 1];
        n->left->layer = n->layer + 1;
        n->left->left = n->left->right = NULL;
        Construct(n->left);
    }
}

int main(void)
{
    int num;
    scanf("%d", &num);
    int *inorder = (int *)malloc(sizeof(int) * num);
    int *postorder = (int *)malloc(sizeof(int) * num);
    for (int i = 0; i < num; i++)
        scanf("%d", &inorder[i]);
    for (int i = 0; i < num; i++)
        scanf("%d", &postorder[i]);

    Node *root = (Node *)malloc(sizeof(Node));
    root->key = postorder[num - 1];
    root->layer = 1;
    root->istart = inorder;
    root->pstart = postorder;
    root->length = num;
    root->left = root->right = NULL;

    Construct(root);

    Node **array = (Node **)malloc(sizeof(Node *) * num);
    array[0] = root;
    int top = 0;
    while (1)
    {
        int curLayer = array[top]->layer;
        int oldTop = top;
        for (int i = top; i >= 0 && array[i]->layer == curLayer; i--)
        {
            if (curLayer % 2 == 1) // left to right
            {
                if (array[i]->left)
                    array[++top] = array[i]->left;
                if (array[i]->right)
                    array[++top] = array[i]->right;
            }
            else // right to left
            {
                if (array[i]->right)
                    array[++top] = array[i]->right;
                if (array[i]->left)
                    array[++top] = array[i]->left;
            }
        }
        if (top == oldTop)
            break;
    }
    printf("%d", array[0]->key);
    for (int i = 1; i < num; i++)
        printf(" %d", array[i]->key);

    return 0;
}