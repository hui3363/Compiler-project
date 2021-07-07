/****************************************************/
/* File: util.h                                     */
/* Utility functions for the TINY compiler          */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _UTIL_H_
#define _UTIL_H_
/*PROJECT3*/
static int nodeIndex = 0;

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
void printToken( TokenType, const char* );

/* Function findSibling finds last sibling of input TreeNode
 * and return the sibling TreeNode
 */
TreeNode * findSibling(TreeNode *);

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newStmtNode(StmtKind);

/* Function newExpNode creates a new expression 
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind);

/* Function newDeclNode creates a new expression 
 * node for syntax tree construction
 */
TreeNode * newDeclNode(DeclKind);

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString( char * );

/* procedure printTree prints a syntax tree to the 
 * listing file using indentation to indicate subtrees
 */
void printTree( TreeNode * );

/*PROJECT3*/
void printStmtK( TreeNode * );
void printExpK( TreeNode * );
void printDeclK( TreeNode * );
char * currentId;


void pushSavedId(char[]);
char* popSavedId();
#endif
