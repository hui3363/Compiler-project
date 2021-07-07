/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"
#include "scan.h"

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
void printToken( TokenType token, const char* tokenString )
{ 
    /* For printing operand of proj2*/
    fprintf(listing,"%s", TokenTypeLabel[token-ERROR+1]);

    return;
}

TreeNode * findSibling(TreeNode * node)
{
    if(node == NULL)
        fprintf(listing,"findSibling NULL\n");

    TreeNode * t = node;
    while(t->sibling != NULL)
        t = t->sibling;
    return t;
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
        /*PROJECT3*/
        t->nodeIndex = nodeIndex++;
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
        /*PROJECT3*/
        t->nodeIndex = nodeIndex++;
    }
    return t;
}

/* Function newDeclNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newDeclNode(DeclKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t==NULL)
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    else {
        for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = DeclK;
        t->kind.decl = kind;
        t->lineno = lineno;
        t->type = 0;
        t->arrayIndex = -1;
        t->global = 0;
        t->nodeIndex = nodeIndex++;
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
#define INDENT indentno+=4
#define UNINDENT indentno-=4

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
        {
            switch (tree->kind.stmt) {
                case CompoundK:
                    fprintf(listing, "[Compound Statement]\n");
                    break;
                case IfK:
                    fprintf(listing, "[Selection Statement]\n");
                    break;
                case WhileK:
                    fprintf(listing, "[Iteration Statement]\n");
                    break;
                case ReturnK:
                    fprintf(listing, "[Return Statement]\n");
                    break;
                default:
                    fprintf(listing,"Unknown ExpNode kind\n");
                    break;
            }
        }
        else if (tree->nodekind==ExpK)
        { 
            switch (tree->kind.exp){

                case OpK:
                    fprintf(listing,"Op: ");
                    if(tree->attr.op==PLUS){
                        fprintf(listing,"+\n");
                    } 
                    if(tree->attr.op==MINUS){
                        fprintf(listing,"-\n");
                    } 
                    if(tree->attr.op==TIMES){
                        fprintf(listing,"*\n");
                    } 
                    if(tree->attr.op==DIV){
                        fprintf(listing,"/\n");
                    } 
                    if(tree->attr.op==LT){
                        fprintf(listing,"<\n");
                    } 
                    if(tree->attr.op==LTE){
                        fprintf(listing,"<=\n");
                    } 
                    if(tree->attr.op==GT){
                        fprintf(listing,">\n");
                    } 
                    if(tree->attr.op==GTE){
                        fprintf(listing,">=\n");
                    } 
                    if(tree->attr.op==EQ){
                        fprintf(listing,"==\n");
                    }
                    if(tree->attr.op==NEQ){
                        fprintf(listing,"!=\n");
                    }
                    break;
                case ConstK:
                    fprintf(listing,"Const: %d\n",tree->attr.val);
                    break;
                case IdK:
                if(tree->type == IntegerArray){
                    fprintf(listing,"Id of Array: %s\n",tree->attr.name);
                    printSpaces();
                    if(tree->child[0]->kind.exp==ConstK){
                        fprintf(listing,"Index of Array: %d\n",tree->child[0]->attr.val);
                    }
                    else if(tree->child[0]->kind.exp==IdK){
                        fprintf(listing,"Index of Array: %s\n",tree->child[0]->attr.name);
                    }
                    break;
                }
                else{
                    fprintf(listing,"Id: %s\n",tree->attr.name);
                    break;
                }

                case AssignK:
                    fprintf(listing,"Assign to: ");
                    if((tree->child[0])->kind.exp==IdK){
                        fprintf(listing,"%s\n",(tree->child[0])->attr.name);
                    }
                    //else if((tree->child[0])->kind.exp==IdArrayK){
                    else if((tree->child[0])->type==IntegerArray){
                        fprintf(listing,"%s",(tree->child[0])->attr.name);
                        if((tree->child[0])->child[0]->kind.exp==ConstK){
                            fprintf(listing,"[%d]\n",(tree->child[0])->child[0]->attr.val);
                        }
                        else if((tree->child[0])->child[0]->kind.exp==IdK){
                            fprintf(listing,"[%s]\n",(tree->child[0])->child[0]->attr.name);
                        }   
                    }
                    printTree(tree->child[1]);
                    break; 
                case CallK:
                    fprintf(listing,"Call: %s\n",tree->attr.name);
                    break;
                case TypeK:
                    fprintf(listing,"Type : ");
                    if(tree->type==0){
                        fprintf(listing,"void\n");
                    }
                    else if(tree->type==1){
                        fprintf(listing,"int\n");
                    }
                    else if(tree->type==2){
                        fprintf(listing,"boolean\n");
                    }
                    break;    
                case ParamK:
                    if(tree->child[0]!=NULL){
                        fprintf(listing,"[Parameter]\n");
                        printTree(tree->child[0]); 
                        printSpaces();
                        fprintf(listing,"  Id: %s\n",tree->attr.name); 
                    }
                    else{
                        printf("[No parameter]\n");
                    }
                    break;

                default:
                    fprintf(listing,"Unknown ExpNode kind\n");
                    break;
            }
        }
        else if (tree->nodekind == DeclK)
        {
            switch (tree->kind.decl){
                case VarK:
                    fprintf(listing,"[Variable Declaration]\n");
                    break;
                /*case ArrayK:
                    fprintf(listing, "[Variable Declaration]\n");
                    break;*/
                case FunK:
                    fprintf(listing, "[Function Declaration] %d\n",lineno);

                    break;
                default:
                    fprintf(listing, "Unknown DecNode kind\n");
                    break;
            }
            printTree(tree->child[0]); 
            printSpaces();
            //if(tree->kind.decl == ArrayK){ 
            if(tree->type==IntegerArray){
                fprintf(listing, "  Id of Array : %s\n",tree->attr.name);
                //printSpaces();
                //fprintf(listing, "  Size of Array: %d\n", tree->sizeArray);
            }
            else {
                fprintf(listing,"  Id: %s\n", tree->attr.name);
                if(tree->kind.decl==FunK){
                    for(i=1;i<MAXCHILDREN;i++){
                        printTree(tree->child[i]);
                    }
                }
            }
        }
        else { 
            fprintf(listing,"Unknown node kind\n");
        }
        if(((tree->nodekind!=DeclK) && (tree->nodekind==ExpK && tree->kind.exp!=ParamK && tree->kind.exp!=AssignK && tree->type!=IntegerArray )) || tree->nodekind==StmtK)
        {
            for (i=0;i<MAXCHILDREN;i++){
                printTree(tree->child[i]);
            }
        }
        tree = tree->sibling;
    }
    UNINDENT;
}

void pushSavedId(char savedId[])
{
    stackId * item = (stackId *) malloc(sizeof(stackId));

    if(top == NULL){
        top =  (stackId *) malloc(sizeof(stackId));
        top->head = NULL;
    }

    item->savedId = copyString(savedId);
    item->head = top->head;
    top->head = item;
}
char* popSavedId(){
    stackId *item = top->head;
    char *retId;

    if(item == NULL)
        return NULL;

    top->head = item->head;
    retId = item->savedId;
    free(item);

    return retId;
}
