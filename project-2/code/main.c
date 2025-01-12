/**
 * @file main.c
 * @brief Main function.
 *
 * This file contains the main function of the program.
 *
 * This program is a driver program for the expression differentiation library.
 *
 * The program will read an expression from stdin, and then print the
 * differentiation result of the expression for each variable in the expression.
 */
#include "expression.h"
#include <stdio.h>

int main(void)
{
    /* read expression */
    String *s = getString();
    VariableList *v = createVariableList();
    /* analyze expression */
    Node *tree = parser(s, v);
    freeString(s);
#ifdef DEBUG
    puts("analyzed expresion: ");
    printTree(tree, v);
    putchar('\n');
#endif
    /* optimize expression */
    Node *optTree = constantOptimizer(tree);
    freeTree(tree);
#ifdef DEBUG
    puts("optimized expresion: ");
    printTree(optTree, v);
    putchar('\n');
#endif
    /* diff for each variable and print result */
    for (int i = 0; i <= v->top; i++)
    {
        Node *diffTree = autoGrad(optTree, v->dictOrder[i]);
#ifdef DEBUG
        printf("origin %s: ", v->s[v->dictOrder[i]]);
        printTree(diffTree, v);
        putchar('\n');
#endif
        /* optimize diffTree */
        Node *optDiffTree = constantOptimizer(diffTree);
        freeTree(diffTree);
#ifdef DEBUG
        printf("optimized %s: ", v->s[v->dictOrder[i]]);
#else
        printf("%s: ", v->s[v->dictOrder[i]]);
#endif
        printTree(optDiffTree, v);
        freeTree(optDiffTree);
        putchar('\n');
    }
    /* warning if no variable in expression */
    if (v->top == -1)
    {
        printf("[warning] No variable in original expression: ");
        printTree(optTree, v);
        putchar('\n');
    }
    freeVariableList(v);
    freeTree(optTree);
    return 0;
}
