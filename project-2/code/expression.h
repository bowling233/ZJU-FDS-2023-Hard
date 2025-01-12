/**
 * @file expression.h
 * @brief Expression storage and parser.
 *
 *
 */
#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

/**
 * @brief Predefined function names.
 *
 */
#define NUM_FUN1 5
#define NUM_FUN2 2
extern const char *fun1s[NUM_FUN1];
extern const char *fun2s[NUM_FUN2];

/**
 * @brief Token type.
 *
 * Used to store the tokens in the expression.
 *
 */
enum token_name
{
    variable,
    operator,
    fun1,
    fun2,
    digit,
    left_bracket,
    right_bracket,
    comma,
    eof
};
typedef enum token_name Type;

/**
 * @brief String type.
 *
 * Used to store the input expression and mark the current position.
 */
typedef struct string String;
struct string
{
    char *s;
    int length, index;
};
String *getString(void);
void freeString(String *s);

/**
 * @brief Variable list type.
 *
 * Used to store the variables in the expression.
 */
typedef struct variable_list VariableList;
struct variable_list
{
    char **s;
    int *dictOrder;
    int size, top;
};
/**
 * @defgroup variable_list Variable list functions
 *
 * The variable list functions are used to store the variables in the expression.
 *
 * @{
 *
 */
VariableList *createVariableList(void);
int querySymbol(const VariableList *s, char *symbol);
int addSymbol(VariableList *s, char *symbol);
void printVariableList(const VariableList *s);
void freeVariableList(VariableList *s);
/** @} */

/**
 * @brief Token type.
 *
 * Used to store the tokens in the expression with their values.
 */
typedef struct token Token;
struct token
{
    Type type;
    int value;
};
/**
 * @defgroup token Token functions
 *
 * The token functions are used to store the tokens in the expression with their values.
 *
 * @{
 *
 */
Token getToken(String *s, VariableList *list);
void printTokens(Token *t, int num, const VariableList *v);
/** @} */

/**
 * @brief Node type.
 *
 * Used to store the expression in the form of a tree.
 */
typedef struct node Node;
struct node
{
    Token token;
    Node *a, *b;
};
/**
 * @defgroup node Node functions
 *
 * The node functions are used to store the expression in the form of a tree.
 *
 * @{
 */
Node *createNode(Token t, Node *a, Node *b);
void printTree(Node *n, const VariableList *v);
Node *copyTree(const Node *n);
int compareTree(const Node *a, const Node *b);
void freeTree(Node *n);
/** @} */

/**
 * @defgroup parser Parser functions
 * @brief Parser functions.
 *
 * The parser functions are used to parse the expression into a tree.
 *
 * @{
 */
int isBracketPaired(Token *t, int start, int end);
Node *parser(String *s, VariableList *list);
Node *exprParser(Token *t, int start, int end);
Node *termParser(Token *t, int start, int end);
Node *powParser(Token *t, int start, int end);
Node *factorParser(Token *t, int start, int end);
/** @} */

/**
 * @defgroup optimizer Optimizer functions
 * @brief Optimizer functions.
 *
 * The optimizer functions are used to optimize the expression.
 *
 * @{
 */
Node *autoGrad(Node *n, const int thisVar);
Node *constantOptimizer(const Node *n);
/** @} */
// Node *collectOptimizer(Node *n); /* not implemented */

#endif
