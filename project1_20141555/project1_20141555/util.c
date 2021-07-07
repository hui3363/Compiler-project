/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the C- compiler                              */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
void printToken(TokenType token, const char* tokenString)
{
    //lineno - token - lexeme
    if(token != COMMENT)
        fprintf(listing, "%10d", lineno);
    switch (token)
    {
        case ELSE: fprintf(listing,"%20s", "ELSE");break;
        case IF: fprintf(listing,"%20s","IF");break;
        case INT: fprintf(listing,"%20s","INT");break;
        case RETURN: fprintf(listing,"%20s","RETURN");break;
        case VOID: fprintf(listing,"%20s","VOID");break;
        case WHILE: fprintf(listing, "%20s","WHILE");break;
        case PLUS: fprintf(listing,"%20s","+"); break;
        case MINUS: fprintf(listing, "%20s","-"); break;
        case MUL: fprintf(listing, "%20s","*"); break;
        case DIVIDE: fprintf(listing, "%20s","/"); break;
        case LT: fprintf(listing, "%20s","<"); break;
        case LE: fprintf(listing, "%20s","<="); break;
        case GT: fprintf(listing, "%20s",">"); break;
        case GE: fprintf(listing, "%20s",">="); break;
        case EQ: fprintf(listing, "%20s","=="); break;
        case NEQ: fprintf(listing, "%20s","!="); break;
        case ASSIGN: fprintf(listing, "%20s","="); break;
        case SEMI: fprintf(listing, "%20s",";"); break;
        case COMMA: fprintf(listing, "%20s",","); break;
        case LPAREN: fprintf(listing, "%20s","("); break;
        case RPAREN: fprintf(listing, "%20s",")"); break;
        case LBRACKET: fprintf(listing, "%20s","["); break;
        case RBRACKET: fprintf(listing, "%20s","]"); break;
        case LBRACE: fprintf(listing, "%20s","{"); break;
        case RBRACE: fprintf(listing, "%20s","}"); break;
        case COMMENT: break;
        case ENDFILE: fprintf(listing, "%20s","ENDFILE"); break;
        case ID: fprintf(listing, "%20s", "ID"); break;
        case NUM: fprintf(listing, "%20s", "NUM"); break;
        case ERROR: fprintf(listing, "%20s", "ERROR");break;
        case COMERROR: fprintf(listing, "%20s", "ERROR");break;
        default:/*should never happen */
            fprintf(listing, "%d\n", token);
    }
    if(token==COMERROR){
        fprintf(listing,"%20s\n","Comment Error");
    }
    else if(token!=COMMENT)
        fprintf(listing, "%20s\n", tokenString);
}

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newStmtNode(StmtKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t==NULL)
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    else {
        for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = StmtK;
        t->kind.stmt = kind;
        t->lineno = lineno;
    }
    return t;
}

/* Function newExpNode creates a new expression 
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t==NULL)
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    else {
        for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = ExpK;
        t->kind.exp = kind;
        t->lineno = lineno;
        t->type = Void;
    }
    return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString(char * s)
{ int n;
    char * t;
    if (s==NULL) return NULL;
    n = strlen(s)+1;
    t = malloc(n);
    if (t==NULL)
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    else strcpy(t,s);
    return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{ int i;
    for (i=0;i<indentno;i++)
        fprintf(listing," ");
}

/* procedure printTree prints a syntax tree to the 
 * listing file using indentation to indicate subtrees
 */
void printTree( TreeNode * tree )
{ int i;
    INDENT;
    while (tree != NULL) {
        printSpaces();
        if (tree->nodekind==StmtK)
        { switch (tree->kind.stmt) {
                                       case IfK:
                                           fprintf(listing,"If\n");
                                           break;
                                       case RepeatK:
                                           fprintf(listing,"Repeat\n");
                                           break;
                                       case AssignK:
                                           fprintf(listing,"Assign to: %s\n",tree->attr.name);
                                           break;
                                       case ReadK:
                                           fprintf(listing,"Read: %s\n",tree->attr.name);
                                           break;
                                       case WriteK:
                                           fprintf(listing,"Write\n");
                                           break;
                                       default:
                                           fprintf(listing,"Unknown ExpNode kind\n");
                                           break;
                                   }
        }
        else if (tree->nodekind==ExpK)
        { switch (tree->kind.exp) {
                                      case OpK:
                                          fprintf(listing,"Op: ");
                                          printToken(tree->attr.op,"\0");
                                          break;
                                      case ConstK:
                                          fprintf(listing,"Const: %d\n",tree->attr.val);
                                          break;
                                      case IdK:
                                          fprintf(listing,"Id: %s\n",tree->attr.name);
                                          break;
                                      default:
                                          fprintf(listing,"Unknown ExpNode kind\n");
                                          break;
                                  }
        }
        else fprintf(listing,"Unknown node kind\n");
        for (i=0;i<MAXCHILDREN;i++)
            printTree(tree->child[i]);
        tree = tree->sibling;
    }
    UNINDENT;
}
