#include "expression.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Predefined function names.
 *
 * This array contains the names of predefined functions.
 *
 * The order of the functions in this array must be the same as the order of
 * the functions in the enum FunctionType.
 *
 * NUM_FUN1 is the number of functions with one parameter.
 * NUM_FUN2 is the number of functions with two parameters.
 */
const char *fun1s[NUM_FUN1] = {"ln", "cos", "sin", "tan", "exp"};
const char *fun2s[NUM_FUN2] = {"log", "pow"};

/**
 * @brief Get one line of string from stdin.
 *
 * This function will read one line of string from stdin, and return the string
 * as a String object. EOF will be substituted by '\0'.
 *
 * The String object contains the length of the string, the index of the current
 * character, and the string itself.
 *
 * The String object will be dynamically allocated, so it must be freed after
 * use.
 *
 * 
 * 
 * 
 * @return String* string from stdin.
 */
String *getString(void)
{
#ifdef DEBUG
    printf("[getString] Read string.\n");
#endif
    /* init */
    String *ret = (String *)malloc(sizeof(String));
    if (!ret)
    {
        fprintf(stderr, "[getString] malloc failed.\n");
        exit(EXIT_FAILURE);
    }
    ret->length = 10;
    ret->index = 0;
    ret->s = (char *)malloc(sizeof(char) * 10);
    if (!ret->s)
    {
        fprintf(stderr, "[getString] malloc failed.\n");
        exit(EXIT_FAILURE);
    }
    /* read */
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        /* filter space */
        if (isspace(c))
            continue;
        ret->s[ret->index++] = c;
        /* expand string */
        if (ret->index == ret->length - 1)
        {
            ret->s = realloc(ret->s, ret->length * 2);
            if (!ret->s)
            {
                fprintf(stderr, "[getString] malloc failed.\n");
                exit(EXIT_FAILURE);
            }
            ret->length *= 2;
        }
    }
    /* substitute eof */
    ret->s[ret->index] = '\0';
    ret->length = ret->index;
    ret->index = 0;
#ifdef DEBUG
    printf("[getString] Read success.\n");
#endif
    return ret;
}

/**
 * @brief Free the String object
 *
 * This function will free the String object.
 *
 * 
 * 
 * @param s string to be freed.
 */
void freeString(String *s)
{
    free(s->s);
    free(s);
}

/* VariableList */

/**
 * @brief Create a Variable List object
 *
 * This function will create a variable list, which is a stack of strings.
 *
 * The variable list will be dynamically allocated, so it must be freed after
 * use.
 *
 * The variable list contains the size of the list, the index of the top
 * element, the array of strings, and the array of the index of the strings in
 * the dictionary order.
 *
 * The variable list will have an initial size of 10, and will be doubled when
 * the stack is full.
 *
 * 
 * 
 * 
 * 
 * 
 * @param size size of the list
 * @return VariableList* created list
 */
VariableList *createVariableList(void)
{
    int size = 10;
    VariableList *ret = (VariableList *)malloc(sizeof(VariableList));
    ret->s = (char **)malloc(sizeof(char *) * size);
    ret->dictOrder = (int *)malloc(sizeof(int) * size);
    ret->size = size;
    ret->top = -1;
    return ret;
}

/**
 * @brief Query the symbol in the variable list
 *
 * This function will query the symbol in the variable list, and return the
 * index of the symbol in the list.
 *
 * 
 * 
 * 
 * @param s variable list to be queried
 * @param symbol symbol to be queried
 * @return int index of the symbol in the list, -1 if not found
 */
int querySymbol(const VariableList *s, char *symbol)
{
    for (int i = 0; i <= s->top; i++)
    {
        if (!strcmp(s->s[i], symbol))
            return i;
    }
    return -1;
}

/**
 * @brief Add the symbol to the variable list
 *
 * This function will add the symbol to the variable list, and return the index
 * of the symbol in the list.
 *
 * When stack is full, the size of the stack will be doubled.
 *
 * This function is responsible for the sorting of the variable list.
 *
 * 
 * 
 * 
 * 
 * @param s variable list to be added
 * @param symbol symbol to be added
 * @return int index of the symbol in the list
 */
int addSymbol(VariableList *s, char *symbol)
{
    /* query if existed */
    if (querySymbol(s, symbol) != -1)
        return querySymbol(s, symbol);
    /* dynamically allocate memory */
    if (s->top == s->size - 1)
    {
#ifdef DEBUG
        printf("[addSymbol] realloc from %d to %d.\n", s->size, s->size * 2);
#endif
        s->s = realloc(s->s, s->size * 2 * sizeof(char *));
        s->dictOrder = realloc(s->dictOrder, s->size * 2 * sizeof(int));
        if (!s->s || !s->dictOrder)
        {
            fprintf(stderr, "[addSymbol] malloc failed.\n");
            exit(EXIT_FAILURE);
        }
        s->size *= 2;
    }
    s->s[++s->top] = symbol;
    /* insert index to dictOrder array*/
    s->dictOrder[s->top] = s->top;
    for (int i = s->top; i > 0; i--)
    {
        if (strcmp(s->s[s->dictOrder[i]], s->s[s->dictOrder[i - 1]]) < 0)
        {
            int temp = s->dictOrder[i];
            s->dictOrder[i] = s->dictOrder[i - 1];
            s->dictOrder[i - 1] = temp;
        }
    }
    return s->top;
}

/**
 * @brief Print the variable list
 *
 * This function will print the variable list.
 *
 * 
 *  
 * 
 * @param s variable list to be printed
 */
void printVariableList(const VariableList *s)
{
    printf("Variable list:\n");
    for (int i = 0; i <= s->top; i++)
    {
        printf("%s\n", s->s[i]);
    }
}

/**
 * @brief Free the variable list
 *
 * This function will free the variable list.
 *
 * 
 * 
 * 
 * @param s variable list to be freed
 */
void freeVariableList(VariableList *s)
{
    for (int i = 0; i <= s->top; i++)
    {
        free(s->s[i]);
    }
    free(s->s);
    free(s->dictOrder);
    free(s);
}

/* Token */
/**
 * @brief Get the Token object
 *
 * This function will get one token from the string.
 *
 * The token contains the type of the token, and the value of the token.
 *
 * This function uses greedy algorithm to get the token. For example, when
 * parsing "ln", the function will first try to parse "l", and then "ln".
 *
 * 
 * 
 * 
 * @param s string to be parsed
 * @param list variable list
 * @return Token one token from the string
 */
Token getToken(String *s, VariableList *list)
{
    /* prevent broken token */
    if (s->s[s->index] == '\0')
        return (Token){eof, 0};

    /* filter space */
    while (isspace(s->s[s->index]))
    {
        s->index++;
    }

    switch (s->s[s->index])
    {
    /* one character token */
    case '(':
        s->index++;
        return (Token){left_bracket, 0};
    case ')':
        s->index++;
        return (Token){right_bracket, 0};
    case '^':
        s->index++;
        return (Token){operator, '^' };
    case '*':
        s->index++;
        return (Token){operator, '*' };
    case '/':
        s->index++;
        return (Token){operator, '/' };
    case '+':
        s->index++;
        return (Token){operator, '+' };
    case '-':
        s->index++;
        return (Token){operator, '-' };
    case ',':
        s->index++;
        return (Token){comma, 0};

    /* multi-character token: constant, functions or variable name */
    default:
    {
        /* number */
        if (isdigit(s->s[s->index]))
        {
            int ret = s->s[s->index++] - '0';
            while (isdigit(s->s[s->index]))
            {
                ret *= 10;
                ret += s->s[s->index] - '0';
                s->index++;
            }
            return (Token){digit, ret};
        }

        /* illegal character */
        if (!islower(s->s[s->index]))
        {
            fprintf(stderr, "[getToken] Syntax error: Undefined character %c\n",
                    s->s[s->index]);
            exit(EXIT_FAILURE);
        }

        /* function1 name */
        for (int i = 0; i < NUM_FUN1; i++)
        {
            if (strstr(s->s + s->index, fun1s[i]) == s->s + s->index &&
                !islower(s->s[s->index + strlen(fun1s[i])]))
            {
                s->index += strlen(fun1s[i]);
                return (Token){fun1, i};
            }
        }

        /* function2 name*/
        for (int i = 0; i < NUM_FUN2; i++)
        {
            if (strstr(s->s + s->index, fun2s[i]) == s->s + s->index &&
                !islower(s->s[s->index + strlen(fun2s[i])]))
            {
                s->index += strlen(fun2s[i]);
                return (Token){fun2, i};
            }
        }

        /* detect variable name */
        int nameLen = 1;
        while (islower(s->s[s->index + nameLen]))
            nameLen++;
        char *varName = (char *)malloc(sizeof(char) * (nameLen + 1));
        strncpy(varName, s->s + s->index, nameLen);
        varName[nameLen] = '\0';
        s->index += nameLen;

        /* query if existed */
        int query_result = querySymbol(list, varName);
        if (query_result == -1)
            return (Token){variable, addSymbol(list, varName)};
        else
            return (Token){variable, query_result};
    }
    }
}

/**
 * @brief Print the token stream
 *
 * This function will print the token stream.
 *
 * 
 * 
 * 
 * 
 * 
 * @param t token stream
 * @param num number of tokens
 * @param v variable list
 */
void printTokens(Token *t, int num, const VariableList *v)
{
    for (int i = 0; i < num; i++)
    {
        printf("[Token %d] ", i);
        switch (t[i].type)
        {
        case variable:
            printf("variable %s\n", v->s[t[i].value]);
            break;
        case operator:
            printf("operator %c\n", t[i].value);
            break;
        case fun1:
            printf("function1 %s\n", fun1s[t[i].value]);
            break;
        case fun2:
            printf("function2 %s\n", fun2s[t[i].value]);
            break;
        case digit:
            printf("constant %d\n", t[i].value);
            break;
        case left_bracket:
            printf("left_bracket\n");
            break;
        case right_bracket:
            printf("right_bracket\n");
            break;
        case comma:
            printf("comma\n");
            break;
        case eof:
            printf("eof\n");
            break;
        }
    }
}

/* Node */
/**
 * @brief Create a Node object
 *
 * This function will create a node, which is a tree node.
 *
 * The node contains the token of the node, and the left and right child of the
 * node.
 *
 * 
 * 
 * 
 * @param t token of the node
 * @param a left child
 * @param b right child
 * @return Node* created node
 */
Node *createNode(Token t, Node *a, Node *b)
{
    Node *ret = (Node *)malloc(sizeof(Node));
    if (!ret)
    {
        fprintf(stderr, "[createNode] malloc failed.\n");
        exit(EXIT_FAILURE);
    }
    ret->token = t;
    ret->a = a;
    ret->b = b;
    return ret;
}

/* parser */

/**
 * @brief Check if the brackets are paired
 *
 * This function will check if the brackets are paired.
 *
 * This function is maily used to search for opeartor which is not in any
 * bracket.
 *
 * 
 * 
 * @param t token stream
 * @param start start index
 * @param end end index
 * @return int 1 if paired, 0 if not
 */
int isBracketPaired(Token *t, int start, int end)
{
    if (start > end) // test
        return 0;
    int num_of_bracket = 0;
    for (int i = start; i <= end; i++)
    {
        switch (t[i].type)
        {
        case left_bracket:
            num_of_bracket++;
            break;
        case right_bracket:
            num_of_bracket--;
            break;
        default:
            break;
        }
    }
    return num_of_bracket == 0;
}

/**
 * @brief Parse the expression into a tree
 *
 * This function will parse the expression into a tree.
 *
 * 
 * 
 * @param s string to be parsed
 * @param list variable list
 * @return Node* root of the tree
 */
Node *parser(String *s, VariableList *list)
{
    /* convert to token stream */
    int size = 10, index = 0;
    Token *tokens = (Token *)malloc(sizeof(Token) * size);
    Token temp;
    while ((temp = getToken(s, list)).type != eof)
    {
        tokens[index++] = temp;
        if (index == size - 1)
        {
            size *= 2;
            tokens = realloc(tokens, size * sizeof(Token));
            if (!tokens)
            {
                fprintf(stderr, "[parser] malloc failed.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    tokens[index] = temp;
    /* avaliable tokens from 0 to index - 1 */
    /* actually we do not mind eof */

#ifdef DEBUG
    printTokens(tokens, index, list);
#endif

    Node *ret = exprParser(tokens, 0, index - 1);
    free(tokens);
    return ret;
}

/**
 * @brief Pattern: expr -> term | expr + term | expr - term
 *
 * 
 * 
 * 
 * @param t token stream
 * @param start parse from start
 * @param end parse to end
 * @return Node* parsed tree
 */
Node *exprParser(Token *t, int start, int end)
{
    if (start > end)
    {
        fprintf(stderr, "[exprParser] Invalid expression.\n");
        exit(EXIT_FAILURE);
    }
#ifdef DEBUG
    fprintf(stderr, "[exprParser] from %d to %d\n", start, end);
#endif

    /* find prodution */
    for (int index = end; index >= start; index--)
    {
        if (t[index].type == operator&&(t[index].value == '+' ||
                                        t[index].value == '-'))
        {
            /* expr-> +term | -term */
            if (t[index - 1].type == operator&&(t[index - 1].value == '+' ||
                                                t[index - 1].value == '-'))
                continue;
            /* is a production */
            if (isBracketPaired(t, start, index - 1) &&
                isBracketPaired(t, index + 1, end))
                return createNode(t[index], exprParser(t, start, index - 1),
                                  termParser(t, index + 1, end));
        }
    }

    /* find term */
    return termParser(t, start, end);
}

/**
 * @brief Pattern: term -> pow | term * pow | term / pow
 *
 * 
 * @param t token stream
 * @param start parse from start
 * @param end parse to end
 * @return Node* parsed tree
 */
Node *termParser(Token *t, int start, int end)
{
    if (start > end)
    {
        fprintf(stderr, "[termParser] Invalid term.\n");
        exit(EXIT_FAILURE);
    }
#ifdef DEBUG
    fprintf(stderr, "[termParser] from %d to %d\n", start, end);
#endif

    /* term-> -term | +term */
    if (t[start].type == operator&&(t[start].value == '+' ||
                                    t[start].value == '-'))
    {
    return createNode((Token){operator, '*'}, createNode((Token){digit, t[start].value == '+' ? 1 : -1}, NULL, NULL), termParser(t, start + 1, end));
    }

    /* find term */
    for (int index = end; index >= start; index--)
    {
    if (t[index].type == operator&&(t[index].value == '*' ||
                                    t[index].value == '/'))
    {
        if (isBracketPaired(t, start, index - 1) &&
            isBracketPaired(t, index + 1, end))
            return createNode(t[index], termParser(t, start, index - 1),
                              powParser(t, index + 1, end));
    }
    }

    /* find term */
    return powParser(t, start, end);
}

/**
 * @brief Pattern: pow -> factor | pow ^ factor
 *
 * 
 * @param t token stream
 * @param start parse from start
 * @param end parse to end
 * @return Node* parsed tree
 */
Node *powParser(Token *t, int start, int end)
{
    if (start > end)
    {
    fprintf(stderr, "[powParser] Invalid pow.\n");
    exit(EXIT_FAILURE);
    }
#ifdef DEBUG
    fprintf(stderr, "[powParser] from %d to %d\n", start, end);
#endif

    /* pow-> -pow | +pow */
    if (t[start].type == operator&&(t[start].value == '+' ||
                                    t[start].value == '-'))
    {
    return createNode((Token){operator, '*'}, createNode((Token){digit, t[start].value == '+' ? 1 : -1}, NULL, NULL), powParser(t, start + 1, end));
    }

    /* find pow */
    for (int index = start; index <= end; index++)
    {
    if (t[index].type == operator&& t[index].value == '^')
    {
        /* is a pow */
        if (isBracketPaired(t, start, index - 1) &&
            isBracketPaired(t, index + 1, end))
            return createNode(t[index], factorParser(t, start, index - 1),
                              powParser(t, index + 1, end));
    }
    }

    /* find factor */
    return factorParser(t, start, end);
}

/**
 * @brief Pattern: factor -> constant | variable | (expr) | fun1(expr) |
 * fun2(expr, expr) | -factor | +factor
 *
 * 
 * @param t token stream
 * @param start parse from start
 * @param end parse to end
 * @return Node* parsed tree
 */
Node *factorParser(Token *t, int start, int end)
{
    if (start > end)
    {
    fprintf(stderr, "[factorParser] Invalid factor.\n");
    exit(EXIT_FAILURE);
    }
#ifdef DEBUG
    fprintf(stderr, "[factorParser] from %d to %d\n", start, end);
#endif
    /* deprecated: unified to start symbol switch if (start == end)
    {
      if (t[start].type == digit)
      {
        return createNode(t[start], NULL, NULL);
      }
      else if (t[start].type == variable)
      {
        return createNode(t[start], NULL, NULL);
      }
      fprintf(stderr, "[factorParser] Can't parse single token: "); printTokens(t
      + start, 1, NULL); exit(EXIT_FAILURE);
    }*/

    switch (t[start].type)
    {
    case left_bracket:
    /* factor-> (expr)*/
    {
        if (t[end].type == right_bracket)
        {
            return exprParser(t, start + 1, end - 1);
        }
        fprintf(stderr, "[factorParser] Missing bracket.\n");
        exit(EXIT_FAILURE);
    }
    case fun1:
    /* factor-> fun1(expr)*/
    {
        if (t[start + 1].type == left_bracket && t[end].type == right_bracket)
        {
            return createNode(t[start], exprParser(t, start + 2, end - 1), NULL);
        }
        fprintf(stderr, "[factorParser] Invalid function1 usage.\n");
        exit(EXIT_FAILURE);
    }
    case fun2:
    /* factor-> fun2(expr, expr) */
    {
        if (t[start + 1].type == left_bracket && t[end].type == right_bracket)
        {
            /* find comma, the only sigle comma out side of any bracket */
            for (int index = start + 2; index <= end - 1; index++)
            {
                if (t[index].type == comma)
                {
                    /* is a valid comma */
                    if (isBracketPaired(t, start + 2, index - 1) &&
                        isBracketPaired(t, index + 1, end - 1))
                        return createNode(t[start], exprParser(t, start + 2, index - 1),
                                          exprParser(t, index + 1, end - 1));
                }
            }
            fprintf(stderr,
                    "[factorParser] Can't find legal comma within function2.\n");
            exit(EXIT_FAILURE);
        }
        fprintf(stderr, "[factorParser] function2 missing brackets.\n");
        exit(EXIT_FAILURE);
    }
    case operator:
    /* factor-> +factor | -factor */
    {
        switch (t[start].value)
        {
        case '+':
        case '-':
      return createNode((Token){operator, '*'}, createNode((Token){digit, t[start].value == '+' ? 1 : -1}, NULL, NULL), factorParser(t, start + 1, end));
        }
        fprintf(stderr, "[factorParser] Invalid operator at the begining.\n");
        exit(EXIT_FAILURE);
    case digit:
        /* factor-> digit */
        if (start == end)
      return createNode(t[start], NULL, NULL);
        fprintf(stderr, "[factorParser] Invalid digit.\n");
        exit(EXIT_FAILURE);
    case variable:
        /* factor-> variable */
        if (start == end)
      return createNode(t[start], NULL, NULL);
        fprintf(stderr, "[factorParser] Invalid variable.\n");
        exit(EXIT_FAILURE);
    }

    default:
    break;
    }

    fprintf(stderr, "[factorParser] Can't parse factor from token %d to %d.\n",
            start, end);
    exit(EXIT_FAILURE);
}

/* Tree */

/**
 * @brief Print the tree
 *
 * 
 * @param n root of the tree
 * @param v variable list
 */
void printTree(Node *n, const VariableList *v)
{
    if (!n)
    {
    printf("Empty tree.\n");
    return;
    }

    switch (n->token.type)
    {
    /* factor */
    case variable:
    printf("%s", v->s[n->token.value]);
    return;
    case fun1:
    printf("%s(", fun1s[n->token.value]);
    printTree(n->a, v);
    printf(")");
    return;
    case fun2:
    printf("%s(", fun2s[n->token.value]);
    printTree(n->a, v);
    putchar(',');
    printTree(n->b, v);
    putchar(')');
    return;
    case digit:
    n->token.value > 0 ? printf("%d", n->token.value)
                       : printf("(%d)", n->token.value);
    return;
    case
    operator:
    /* compare priority of this node and its child to add brackets */
    switch (n->token.value)
    {
    case '+':
    case '-': /* lowest-priority */
        printTree(n->a, v);
        putchar(n->token.value);
        /* right tree of - need bracket */
        if (n->b->token.type == operator&& n->b->token.value == '-')
      putchar('(');
        printTree(n->b, v);
        if (n->b->token.type == operator&& n->b->token.value == '-')
      putchar(')');
        return;
    case '*':
    case '/':
        /* medium-priority */
        if (n->a->token.type == operator&&(n->a->token.value == '+' ||
                                           n->a->token.value == '-' ||
                                           n->a->token.value == '*' ||
                                           n->a->token.value == '/'))
      putchar('(');
        printTree(n->a, v);
        if (n->a->token.type == operator&&(n->a->token.value == '+' ||
                                           n->a->token.value == '-' ||
                                           n->a->token.value == '*' ||
                                           n->a->token.value == '/'))
      putchar(')');
        putchar(n->token.value);
        if (n->b->token.type == operator&&(n->b->token.value == '+' ||
                                           n->b->token.value == '-' ||
                                           n->b->token.value == '*' ||
                                           n->b->token.value == '/'))
      putchar('(');
        printTree(n->b, v);
        if (n->b->token.type == operator&&(n->b->token.value == '+' ||
                                           n->b->token.value == '-' ||
                                           n->b->token.value == '*' ||
                                           n->b->token.value == '/'))
      putchar(')');
        return;
    case '^':
        /* highest-priority */
        if (n->a->token.type == operator&&(n->a->token.value == '+' ||
                                           n->a->token.value == '-' ||
                                           n->a->token.value == '*' ||
                                           n->a->token.value == '/' || 
                                           n->a->token.value == '^'))
      putchar('(');
        printTree(n->a, v);
        if (n->a->token.type == operator&&(n->a->token.value == '+' ||
                                           n->a->token.value == '-' ||
                                           n->a->token.value == '*' ||
                                           n->a->token.value == '/' ||
                                           n->a->token.value == '^'))
      putchar(')');
        putchar(n->token.value);
        if (n->b->token.type == operator&&(n->b->token.value == '+' ||
                                           n->b->token.value == '-' ||
                                           n->b->token.value == '*' ||
                                           n->b->token.value == '/' ||
                                           n->b->token.value == '^'))
      putchar('(');
        printTree(n->b, v);
        if (n->b->token.type == operator&&(n->b->token.value == '+' ||
                                           n->b->token.value == '-' ||
                                           n->b->token.value == '*' ||
                                           n->b->token.value == '/' ||
                                           n->b->token.value == '^'))
      putchar(')');
        return;
    }
    return;
    default:
    fprintf(stderr, "[printTree] Invalid token type appeared in tree.\n");
    exit(EXIT_FAILURE);
    }
}

/**
 * @brief Copy the tree
 *
 * 
 * @param n root of the tree
 * @return Node* copied tree
 */
Node *copyTree(const Node *n)
{
    if (!n)
    return NULL;
    return createNode(n->token, copyTree(n->a), copyTree(n->b));
}

/**
 * @brief Compare two trees
 *
 * note: operator + * is commutative, so the order of a and b does not matter
 *
 * 
 * 
 * @param a root of the first tree
 * @param b root of the second tree
 * @return int 1 if equal, 0 if not
 */
int compareTree(const Node *a, const Node *b)
{
    if (!a && !b)
    return 1;
    if (!a || !b)
    return 0;

    /* operator + commutative */
    if (a->token.type == operator&& a->token.value == '+' &&
                         b->token.type ==
                         operator&& b->token.value == '+')
    return (compareTree(a->a, b->a) && compareTree(a->b, b->b)) ||
           (compareTree(a->a, b->b) && compareTree(a->b, b->a));

    /* operator * commutative */
    if (a->token.type == operator&& a->token.value == '*' &&
                         b->token.type ==
                         operator&& b->token.value == '*')
    return (compareTree(a->a, b->a) && compareTree(a->b, b->b)) ||
           (compareTree(a->a, b->b) && compareTree(a->b, b->a));

    if (a->token.type != b->token.type || a->token.value != b->token.value)
    return 0;
    return compareTree(a->a, b->a) && compareTree(a->b, b->b);
}

/**
 * @brief Free the tree
 * 
 * This function will free the tree.
 * 
 * 
 * 
 *
 * @param n root of the tree
 */
void freeTree(Node *n)
{
    if (!n)
    return;
    freeTree(n->a);
    freeTree(n->b);
    free(n);
}

/* Diff engine */

/**
 * @brief Automatic differentiation
 * 
 * This function will differentiate the tree with respect to the variable.
 * 
 * This function will return a new tree, so the original tree will not be
 * modified.
 * 
 * This function will not free the original tree, so the original tree must be
 * freed after use.
 * 
 * The function will use the following rules:
 * 
 * 1. x' = 1, y' = 0
 * 2. (a + b)' = a' + b'
 * 3. (a - b)' = a' - b'
 * 4. (a * b)' = a' * b + a * b'
 * 5. (a / b)' = (a' * b - a * b') / b^2
 * 6. (a ^ b)' = a ^ b * (a' * b / a + b' * ln(a))
 * 7. sin(a)' = a' * cos(a)
 * 8. cos(a)' = -a' * sin(a)
 * 9. tan(a)' = a' / cos(a)^2 * (a)
 * 10. cot(a)' = -a' / sin(a)^2
 * 11. ln(a)' = a' / a
 * 12. log(a, b)' = (a' * log(b) - b' * log(a)) / (a * log(b))^2
 *
 * 
 * 
 * 
 * 
 * 
 * @param n root of the tree
 * @param thisVar variable to be differentiated
 * @param v variable list
 * @return Node* differentiated tree
 */
Node *autoGrad(Node *n, const int thisVar)
{
    if (!n)
    {
    fprintf(stderr, "[autoGrad] null tree.\n");
    exit(EXIT_FAILURE);
    }

    switch (n->token.type)
    {
    case variable:
    /* x' = 1, y' = 0 */
    if (n->token.value == thisVar)
    {
        return createNode((Token){digit, 1}, NULL, NULL);
    }
    return createNode((Token){digit, 0}, NULL, NULL);
    case digit:
    /* c' = 0 */
    return createNode((Token){digit, 0}, NULL, NULL);
    case operator:
    switch (n->token.value)
    {
    case '+':
        /* (a + b)' = a' + b' */
            return createNode((Token){operator, '+'}, autoGrad(n->a, thisVar), autoGrad(n->b, thisVar));
    case '-':
            /* (a - b)' = a' - b' */
            return createNode((Token){operator, '-'}, autoGrad(n->a, thisVar), autoGrad(n->b, thisVar));
    case '*':
            /* (a * b)' = a' * b + a * b' */
            return createNode((Token){operator, '+'},
                              createNode((Token){operator, '*'},
                                         autoGrad(n->a, thisVar),
                                         copyTree(n->b)),
                              createNode((Token){operator, '*'},
                                         copyTree(n->a),
                                         autoGrad(n->b, thisVar)));
    case '/':
            /* (a / b)' = (a' * b - a * b') / b^2 */
            return createNode((Token){operator, '/'},
                              createNode((Token){operator, '-'},
                                         createNode((Token){operator, '*'},
                                                    autoGrad(n->a, thisVar),
                                                    copyTree(n->b)),
                                         createNode((Token){operator, '*'},
                                                    copyTree(n->a),
                                                    autoGrad(n->b, thisVar))),
                              createNode((Token){operator, '^'},
                                         copyTree(n->b),
                                         createNode((Token){digit, 2}, NULL, NULL)));
    case '^':
            /* (a ^ b)' = a ^ b * (a' * b / a + b' * ln(a)) */
            return createNode((Token){operator, '*'},
                              copyTree(n),
                              createNode((Token){operator, '+'},
                                          createNode((Token){operator, '*'},
                                                      autoGrad(n->a, thisVar),
                                                      createNode((Token){operator, '/'},
                                                                 copyTree(n->b),
                                                                 copyTree(n->a))),
                                          createNode((Token){operator, '*'},
                                                      autoGrad(n->b, thisVar),
                                                      createNode((Token){fun1, 0},
                                                                 copyTree(n->a),
                                                                 NULL))));
    }
    break;
    case fun1:
    switch (n->token.value)
    {
    case 0:
            /* sin(a)' = a' * cos(a) */
            return createNode((Token){operator, '*'},
                autoGrad(n->a, thisVar),
                createNode((Token){operator, '/'},
                    createNode((Token){digit, 1}, NULL, NULL),
                    copyTree(n->a)));
    case 1:
            /* cos(a)' = -a' * sin(a) */
            return createNode((Token){operator, '*'},
                              createNode((Token){digit, -1}, NULL, NULL),
                              createNode((Token){operator, '*'},
                                         autoGrad(n->a, thisVar),
                                         createNode((Token){fun1, 2}, copyTree(n->a), NULL)));
    case 2:
            /* tan(a)' = a' / cos(a)^2 */
            return createNode((Token){operator, '*'},
                              autoGrad(n->a, thisVar),
                              createNode((Token){fun1, 1}, copyTree(n->a), NULL));
    case 3:
            /* cot(a)' = -a' / sin(a)^2 */
            return createNode((Token){operator, '/'},
                              autoGrad(n->a, thisVar),
                              createNode((Token){operator, '^'},
                                         createNode((Token){fun1, 1}, copyTree(n->a), NULL),
                                         createNode((Token){digit, 2}, NULL, NULL)));
    case 4:
            /* ln(a)' = a' / a */
            return createNode((Token){operator, '*'},
                              autoGrad(n->a, thisVar),
                              createNode((Token){fun1, 4}, copyTree(n->a), NULL));
    }
    break;
    case fun2:
    switch (n->token.value)
    {
    case 0:
            /* log(a, b)' = (a' * log(b) - b' * log(a)) / (a * log(b))^2 */
            return createNode((Token){operator, '/'},
                              createNode((Token){operator, '-'},
                                         createNode((Token){operator, '*'},
                                                    autoGrad(n->b, thisVar),
                                                    createNode((Token){operator, '/'},
                                                               createNode((Token){fun1, 0},
                                                                          copyTree(n->a),
                                                                          NULL),
                                                               copyTree(n->b))),
                                         createNode((Token){operator, '*'},
                                                    autoGrad(n->a, thisVar),
                                                    createNode((Token){operator, '/'},
                                                               createNode((Token){fun1, 0},
                                                                          copyTree(n->b),
                                                                          NULL),
                                                               copyTree(n->a)))),
                              createNode((Token){operator, '^'},
                                          createNode((Token){fun1, 0},
                                                      copyTree(n->a),
                                                      NULL),
                                          createNode((Token){digit, 2}, NULL, NULL)));
    case 1:
            /* max(a, b)' = a' * (a > b) + b' * (a <= b) */
            return createNode((Token){operator, '*'},
                              copyTree(n),
                              createNode((Token){operator, '+'},
                                          createNode((Token){operator, '*'},
                                                      autoGrad(n->a, thisVar),
                                                      createNode((Token){operator, '/'},
                                                                 copyTree(n->b),
                                                                 copyTree(n->a))),
                                          createNode((Token){operator, '*'},
                                                      autoGrad(n->b, thisVar),
                                                      createNode((Token){fun1, 0},
                                                                 copyTree(n->a),
                                                                 NULL))));
    }
    break;
    default:
    fprintf(stderr, "[atuoDiff] Invalid node.\n");
    exit(EXIT_FAILURE);
    }
    fprintf(stderr, "[atuoDiff] Invalid node.\n");
    exit(EXIT_FAILURE);
}

/* Optimizer */

/**
 * @brief Constant optimizer
 * 
 * This function will optimize the tree by computing the constant.
 * 
 * This function will not change the tree structure.
 * 
 * The constant optimizer will do the following things:
 * 
 * 1. Compute the constant of the tree.
 * 2. Remove the useless node.
 * 3. Simplify the tree.
 * 4. Remove the useless node.
 * 5. Simplify the tree.
 *
 * 
 * 
 * 
 * 
 * 
 * @param n root of the tree
 * @return Node* optimized tree
 */
Node *constantOptimizer(const Node *n)
{
    if (!n)
    return NULL;
    if (n->token.type != operator&& n->token.type != fun1 &&
                         n->token.type != fun2)
    {
    return copyTree(n);
    }

    Node *a = NULL, *b = NULL;
    if (n->a)
    a = constantOptimizer(n->a);
    if (n->b)
    b = constantOptimizer(n->b);

    switch (n->token.type)
    {
    /* operator constant optimizer */
    case operator:
    switch (n->token.value)
    {
    case '+':
            /* compute two constant */
            if (a->token.type == digit && b->token.type == digit)
      return createNode((Token){digit, a->token.value + b->token.value},
                        NULL, NULL);
            /* 0+f(x) = f(x) */
            if (a->token.type == digit && a->token.value == 0)
            {
      freeTree(a);
      return b;
            }
            /* f(x)+0 = f(x) */
            if (b->token.type == digit && b->token.value == 0)
            {
      freeTree(b);
      return a;
            }
            return createNode(n->token, a, b);
    case '-':
            /* compute two constant */
            if (a->token.type == digit && b->token.type == digit)
      return createNode((Token){digit, a->token.value - b->token.value},
                        NULL, NULL);
            /* 0-f(x) = -f(x) */
            if (a->token.type == digit && a->token.value == 0)
            {
      freeTree(a);
        return createNode((Token){operator, '*'}, createNode((Token){digit, -1}, NULL, NULL), b);
            }
            /* f(x)-0 = f(x) */
            if (b->token.type == digit && b->token.value == 0)
            {
        freeTree(b);
        return a;
            }
            return createNode(n->token, a, b);
    case '*':
            /* compute two constant */
            if (a->token.type == digit && b->token.type == digit)
        return createNode((Token){digit, a->token.value * b->token.value}, NULL,
                          NULL);
            /* 0*f(x) = 0 */
            if (a->token.type == digit && a->token.value == 0)
            {
#ifdef DEBUG
        fprintf(stderr, "[constantOptimizer] 0*f(x) = 0.\n");
#endif
        freeTree(a);
        freeTree(b);
        return createNode((Token){digit, 0}, NULL, NULL);
            }
            if (b->token.type == digit && b->token.value == 0)
            {
#ifdef DEBUG
        fprintf(stderr, "[constantOptimizer] f(x)*0 = 0.\n");
#endif
        freeTree(a);
        freeTree(b);
        return createNode((Token){digit, 0}, NULL, NULL);
            }
            /* 1*f(x) = f(x) */
            if (a->token.type == digit && a->token.value == 1)
            {
#ifdef DEBUG
        fprintf(stderr, "[constantOptimizer] 1*f(x) = f(x).\n");
#endif
        freeTree(a);
        return b;
            }
            if (b->token.type == digit && b->token.value == 1)
            {
#ifdef DEBUG
        fprintf(stderr, "[constantOptimizer] f(x)*1 = f(x).\n");
#endif
        freeTree(b);
        return a;
            }
            return createNode(n->token, a, b);
    case '/':
            /* compute two constant */
            if (a->token.type == digit && b->token.type == digit)
            {
        if (a->token.value % b->token.value == 0)
            return createNode((Token){digit, a->token.value / b->token.value},
                              NULL, NULL);
        return createNode(n->token, a, b);
            }
            /* 0/f(x) = 0 */
            if (a->token.type == digit && a->token.value == 0)
            {
#ifdef DEBUG
        fprintf(stderr, "[constantOptimizer] 0/f(x) = 0.\n");
#endif
        freeTree(a);
        freeTree(b);
        return createNode((Token){digit, 0}, NULL, NULL);
            }
            /* f(x)/1 = f(x) */
            if (b->token.type == digit && b->token.value == 1)
            {
#ifdef DEBUG
        fprintf(stderr, "[constantOptimizer] f(x)/1 = f(x).\n");
#endif
        freeTree(b);
        return a;
            }
            /* f(x)/f(x) = 1 */
            if (compareTree(a, b))
            {
#ifdef DEBUG
        fprintf(stderr, "[constantOptimizer] f(x)/f(x) = 1.\n");
#endif
        freeTree(a);
        freeTree(b);
        return createNode((Token){digit, 1}, NULL, NULL);
            }
            return createNode(n->token, a, b);
    case '^':
            /* compute two constant */
            /* TOCHECK: will it only produce integer? */
            if (a->token.type == digit && b->token.type == digit)
        return createNode(
            (Token){digit, (int)pow(a->token.value, b->token.value)}, NULL,
            NULL);
            /* 0^f(x) = 0 */
            if (a->token.type == digit && a->token.value == 0)
            {
        /* 0^0 is undefined, not used 
        This code snippet is commented out to avoid warning.
        */
        /*
        if (b->token.type == constant && b->token.value == 0)
        {
          freeTree(a); 
          freeTree(b); 
          fprintf(stderr, "[constantOptimizer] warning: 0^0 is undefined. "
          "In algebra and combinatorics, one typically defines 0^0 = 1. "
          "Here use it.\n"); 
          return createNode((Token){constant, 1}, NULL, NULL);
        }
        */
        freeTree(a);
        freeTree(b);
        return createNode((Token){digit, 0}, NULL, NULL);
            }
            /* f(x)^0 = 1 */
            if (b->token.type == digit && b->token.value == 0)
            {
        freeTree(a);
        freeTree(b);
        return createNode((Token){digit, 1}, NULL, NULL);
            }
            /* 1^f(x) = 1 */
            if (a->token.type == digit && a->token.value == 1)
            {
        freeTree(a);
        freeTree(b);
        return createNode((Token){digit, 1}, NULL, NULL);
            }
            /* f(x)^1 = f(x) */
            if (b->token.type == digit && b->token.value == 1)
            {
        freeTree(b);
        return a;
            }
            return createNode(n->token, a, b);
    default:
            return createNode(n->token, NULL, NULL);
    }
    break;
    /* unary function constant optimizer */
    case fun1:
    switch (n->token.value)
    {
    case 0:
            /* ln */
            /* ln(1) = 0 */
            if (a->token.type == digit && a->token.value == 1)
            {
        freeTree(a);
        return createNode((Token){digit, 0}, NULL, NULL);
            }
            return createNode(n->token, a, NULL);
    case 1:
            /* cos */
            /* cos(0) = 1 */
            if (a->token.type == digit && a->token.value == 0)
            {
        freeTree(a);
        return createNode((Token){digit, 1}, NULL, NULL);
            }
            return createNode(n->token, a, NULL);
    case 2:
            /* sin */
            /* sin(0) = 0 */
            if (a->token.type == digit && a->token.value == 0)
            {
        freeTree(a);
        return createNode((Token){digit, 0}, NULL, NULL);
            }
            return createNode(n->token, a, NULL);
    case 3:
            /* tan */
            /* tan(0) = 0 */
            if (a->token.type == digit && a->token.value == 0)
            {
        freeTree(a);
        return createNode((Token){digit, 0}, NULL, NULL);
            }
            return createNode(n->token, a, NULL);
    case 4:
            /* exp */
            /* exp(0) = 1 */
            if (a->token.type == digit && a->token.value == 0)
            {
        freeTree(a);
        return createNode((Token){digit, 1}, NULL, NULL);
            }
            return createNode(n->token, a, NULL);
    default:
            break;
    }
    break;
    /* binary function constant optimizer */
    case fun2:
    switch (n->token.value)
    {
    case 0:
            /* log */
            /* log(f(x), f(x)) = 1 */
            if (compareTree(a, b))
            {
        freeTree(a);
        freeTree(b);
        return createNode((Token){digit, 1}, NULL, NULL);
            }
            /* log(f(x), 1) = 0 */
            if (b->token.type == digit && b->token.value == 1)
            {
        freeTree(a);
        freeTree(b);
        return createNode((Token){digit, 0}, NULL, NULL);
            }
            return createNode(n->token, a, b);
    case 1:
            /* pow */
            /* pow(c, c) = c^c */
            if (a->token.type == digit && b->token.type == digit)
        return createNode(
            (Token){digit, (int)pow(a->token.value, b->token.value)}, NULL,
            NULL);
            /* pow(0, f(x)) = 0 */
            if (a->token.type == digit && a->token.value == 0)
            {
        freeTree(a);
        freeTree(b);
        return createNode((Token){digit, 0}, NULL, NULL);
            }
            /* pow(f(x), 0) = 1 */
            if (b->token.type == digit && b->token.value == 0)
            {
        freeTree(a);
        freeTree(b);
        return createNode((Token){digit, 1}, NULL, NULL);
            }
            /* pow(1, f(x)) = 1 */
            if (a->token.type == digit && a->token.value == 1)
            {
        freeTree(a);
        freeTree(b);
        return createNode((Token){digit, 1}, NULL, NULL);
            }
            /* pow(f(x), 1) = f(x) */
            if (b->token.type == digit && b->token.value == 1)
            {
        freeTree(b);
        return a;
            }
            /* convert to operator ^ */
      return createNode((Token){operator, '^'}, a, b);
    default:
      break;
    }
    break;
    default:
    break;
    }

    fprintf(stderr, "[constantOptimizer] Program error.\n");
    exit(EXIT_FAILURE);
}
