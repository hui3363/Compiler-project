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
    //if(token != COMMENT)
    //    fprintf(listing, "%10d", lineno);
    switch (token)
    {
        case ELSE: fprintf(listing,"%s", "ELSE");break;
        case IF: fprintf(listing,"%s","IF");break;
        case INT: fprintf(listing,"%s","INT");break;
        case RETURN: fprintf(listing,"%s","RETURN");break;
        case VOID: fprintf(listing,"%s","VOID");break;
        case WHILE: fprintf(listing, "%s","WHILE");break;
        case PLUS: fprintf(listing,"%s","+"); break;
        case MINUS: fprintf(listing, "%s","-"); break;
        case MUL: fprintf(listing, "%s","*"); break;
        case DIVIDE: fprintf(listing, "%s","/"); break;
        case LT: fprintf(listing, "%s","<"); break;
        case LE: fprintf(listing, "%s","<="); break;
        case GT: fprintf(listing, "%s",">"); break;
        case GE: fprintf(listing, "%s",">="); break;
        case EQ: fprintf(listing, "%s","=="); break;
        case NEQ: fprintf(listing, "%s","!="); break;
        case ASSIGN: fprintf(listing, "%s","="); break;
        case SEMI: fprintf(listing, "%s",";"); break;
        case COMMA: fprintf(listing, "%s",","); break;
        case LPAREN: fprintf(listing, "%s","("); break;
        case RPAREN: fprintf(listing, "%s",")"); break;
        case LBRACKET: fprintf(listing, "%s","["); break;
        case RBRACKET: fprintf(listing, "%s","]"); break;
        case LBRACE: fprintf(listing, "%s","{"); break;
        case RBRACE: fprintf(listing, "%s","}"); break;
        case COMMENT: break;
        case ENDFILE: fprintf(listing, "%s","ENDFILE"); break;
        case ID: fprintf(listing, "%s", "ID"); break;
        case NUM: fprintf(listing, "%s", "NUM"); break;
        case ERROR: fprintf(listing, "%s", "ERROR");break;
        case COMERROR: fprintf(listing, "%s", "ERROR");break;
        //default: fprintf(listing, "%d\n", token);
    }
    if(token==COMERROR){
        fprintf(listing,"%s\n","Comment Error");
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

/* Function newDecNode creates a new declaration 
 * node for syntax tree construction
 */
TreeNode * newDecNode(DecKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t==NULL)
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    else {
        for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = DecK;
        t->kind.dec = kind;
        t->lineno = lineno;
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
    //if(Error!=TRUE)
    while (tree != NULL) {
        printSpaces();
        if (tree->nodekind==StmtK)
        { 
            //printf("lineno: %d\n",lineno);
            switch (tree->kind.stmt) {
                case CompoundK:
                    fprintf(listing, "[Compound Statement]\n");
                    break;
                case SelectionK:
                    fprintf(listing, "[Selection Statement]\n");
                    break;
                case IterationK:
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
                    if(tree->attr.op==MUL){
                        fprintf(listing,"*\n");
                    } 
                    if(tree->attr.op==DIVIDE){
                        fprintf(listing,"/\n");
                    } 
                    if(tree->attr.op==LT){
                        fprintf(listing,"<\n");
                    } 
                    if(tree->attr.op==LE){
                        fprintf(listing,"<=\n");
                    } 
                    if(tree->attr.op==GT){
                        fprintf(listing,">\n");
                    } 
                    if(tree->attr.op==GE){
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
                    fprintf(listing,"Id: %s\n",tree->attr.name);
                    break;
                case IdArrayK:
                    fprintf(listing,"Id of Array: %s\n",tree->attr.name);
                    printSpaces();
                    if(tree->child[0]->kind.exp==ConstK){
                        fprintf(listing,"Index of Array: %d\n",tree->child[0]->attr.val);
                    }
                    else if(tree->child[0]->kind.exp==IdK){
                        fprintf(listing,"Index of Array: %s\n",tree->child[0]->attr.name);
                    }
                    break;
                case AssignK:
                    fprintf(listing,"Assign to: ");
                    if((tree->child[0])->kind.exp==IdK){
                        fprintf(listing,"%s\n",(tree->child[0])->attr.name);
                    }
                    else if((tree->child[0])->kind.exp==IdArrayK){
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
                case ParamK://revise
                    //printSpaces();
                    if(tree->child[0]!=NULL){
                        fprintf(listing,"[Parameter]\n");
                        printTree(tree->child[0]); //type
                        printSpaces();
                        fprintf(listing,"  Id: %s\n",tree->attr.name); //id
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
        else if (tree->nodekind == DecK)
        {
            switch (tree->kind.dec){
                case VarK:
                    fprintf(listing,"[Variable Declaration]\n");
                    break;
                case ArrayK:
                    fprintf(listing, "[Variable Declaration]\n");
                    break;
                case FunK:
                    fprintf(listing, "[Function Declaration] %d\n",lineno);
                    
                    break;
                default:
                    fprintf(listing, "Unknown DecNode kind\n");
                    break;
            }
            printTree(tree->child[0]); //type print
            printSpaces();
            if(tree->kind.dec == ArrayK){ //array declaration의 배열 크기는 정수로만 주어짐.
                fprintf(listing, "  Id of Array : %s\n",tree->attr.name);
                printSpaces();
                fprintf(listing, "  Size of Array: %d\n", tree->sizeArray);
            }
            else {
                fprintf(listing,"  Id: %s\n", tree->attr.name); //id print
                if(tree->kind.dec==FunK){
                    for(i=1;i<MAXCHILDREN;i++){
                        printTree(tree->child[i]);
                    }
                }
            }
        }
        else { 
            fprintf(listing,"Unknown node kind\n");
        }
        if(((tree->nodekind!=DecK) && (tree->nodekind==ExpK && tree->kind.exp!=ParamK && tree->kind.exp!=AssignK && tree->kind.exp!=IdArrayK )) || tree->nodekind==StmtK)
        {
            for (i=0;i<MAXCHILDREN;i++){
                printTree(tree->child[i]);
            }
        }
        tree = tree->sibling;
    }
    UNINDENT;
}
