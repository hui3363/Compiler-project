/****************************************************/
/* File: tiny.y                                     */
/* The C- Yacc/Bison specification file             */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/
%{
#define YYPARSER /* distinguishes Yacc output from other code files */

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

#define YYSTYPE TreeNode *
static char * savedName[10000]; /* for use in assignments */
static int  savedLineNo[10000];  /* ditto */
static int savedSize; /* for storing size of array var_declaration */
static TreeNode * savedTree; /* stores syntax tree for later return */

static int level = 0;
%}

%token  ERROR COMERROR
%token  ELSE IF INT RETURN VOID WHILE
%token  ID NUM 
%token  PLUS MINUS MUL DIVIDE 
%token  LT LE GT GE EQ NEQ ASSIGN 
%token  SEMI COMMA
%token  LPAREN RPAREN 
%token  LBRACKET RBRACKET 
%token  LBRACE RBRACE
%nonassoc _ELSE_PRIORITY
%nonassoc ELSE

%% /* Grammar for TINY */

program          : declaration_list
                 { savedTree = $1;} 
                 ;
declaration_list : declaration_list declaration
                 {
                    YYSTYPE t = $1;
                    if(t!=NULL)
                    {
                        while(t->sibling != NULL)
                            t= t->sibling;
                        t->sibling = $2;
                        $$=$1;
                    }
                    else $$ = $2;
                }
                | declaration
                { $$ = $1; }
                ;
declaration      : var_declaration
                 { $$ = $1;}
                 | fun_declaration
                 { $$ = $1; }
                 ;
var_declaration  : type_specifier ID SEMI
                 {
                 $$ = newDecNode(VarK);
                 $$->child[0] = $1;
                 $$->attr.name = copyString(priorToken);
                 $$->lineno = lineno;
                 }
                 |type_specifier ID      
                 {
                 savedName[++level] = copyString(priorToken);
                 savedLineNo[level] = lineno;
                 }
                 LBRACKET NUM RBRACKET
                 {
                 savedSize = atoi(priorToken);
                 }
                 SEMI
                 {
                 $$ = newDecNode(ArrayK);
                 $$->child[0] = $1;
                 $$->sizeArray = savedSize;
                 $$->attr.name = savedName[level];
                 $$->lineno = savedLineNo[level--];
                 }
                 ;
type_specifier   : INT
                 {
                $$ = newExpNode(TypeK);
                $$->type = Integer;
                }
                | VOID
                {
                $$ = newExpNode(TypeK);
                $$->type = Void;
                }
                ;
fun_declaration : type_specifier ID
                {
                savedName[++level] = copyString(priorToken);
                savedLineNo[level] = lineno;
                }
                LPAREN params RPAREN compound_stmt
                {
                $$ = newDecNode(FunK);
                $$->attr.name = savedName[level];
                $$->lineno = savedLineNo[level--];
                $$->child[0] = $1;
                $$->child[1] = $5;
                $$->child[2] = $7;
                }
                ;
params          : param_list
                {
                    $$=$1;
                }
                |VOID
                {
                $$ = newExpNode(ParamK);
                $$ -> type = Void;
                }
                ;
param_list      : param_list COMMA param
                {
                    YYSTYPE t= $1;
                    if(t!=NULL)
                    {
                        while(t->sibling != NULL)
                            t=t->sibling;
                        t->sibling = $3;
                        $$=$1;
                    }
                    else $$=$3;
                 }
                | param
                {
                    $$ = $1;
                }
                ;
param           :type_specifier ID
                {
                $$ = newExpNode(ParamK);
                $$->attr.name = copyString(priorToken);
                $$->child[0] = $1;
                }
                |type_specifier ID LBRACKET RBRACKET
                {
                $$ = newExpNode(ParamK);
                $$->attr.name = copyString(beforePrior);
                $$->child[0] = $1;
                }
                ;
compound_stmt   : LBRACE local_declarations statement_list RBRACE
                {
                $$ = newStmtNode(CompoundK);
                $$->child[0] = $2;
                $$->child[1] = $3;
                }
                ;
local_declarations : local_declarations var_declaration
                   {
                   YYSTYPE t= $1;
                   if(t!=NULL)
                   {
                    while(t->sibling!=NULL)
                        t=t->sibling;
                    t->sibling = $2;
                    $$ = $1;
                    }
                    else    
                    {
                    $$ = $2;}
                    }
                |
                {$$=NULL;}
                ;
statement_list  : statement_list statement
                {
                YYSTYPE t = $1;
                if(t!=NULL)
                {
                while(t->sibling!=NULL)
                    t=t->sibling;
                t->sibling=$2;
                $$=$1;
                }
                else {
                $$=$2;}
                }
                |
                {$$=NULL;}
                ;
statement       :expression_stmt
                {
                $$=$1;}
                |compound_stmt
                {
                $$=$1;}
                |selection_stmt
                {
                $$=$1;}
                |iteration_stmt
                {
                $$=$1;}
                |return_stmt
                {
                $$=$1;}
                ;
expression_stmt : expression SEMI{
                $$=$1;
                }
                |SEMI
                {
                $$=NULL;
                }
                ;
selection_stmt  : IF LPAREN expression RPAREN statement  %prec _ELSE_PRIORITY
                {
                $$ = newStmtNode(SelectionK);
                $$->child[0] = $3;
                $$->child[1] = $5;
                }
                | IF LPAREN expression RPAREN statement ELSE statement
                {
                $$ = newStmtNode(SelectionK);
                $$->child[0] = $3;
                $$->child[1] = $5;
                $$->child[2] = $7;
                }
                ;
iteration_stmt  : WHILE LPAREN expression RPAREN statement
                {
                $$ = newStmtNode(IterationK);
                $$->child[0]=$3;
                $$->child[1]=$5;
                }
                ;
return_stmt     :RETURN SEMI
                {
                $$ = newStmtNode(ReturnK);
                }
                |
                RETURN expression SEMI
                {
                $$ = newStmtNode(ReturnK);
                $$->child[0] = $2;
                }
                ;
expression      : var ASSIGN expression
                {
                $$ = newExpNode(AssignK);
                $$->child[0] = $1;
                $$->child[1] = $3;
                $$->attr.op = ASSIGN;
                }
                | simple_expression
                { $$ = $1; }
                ;
var                 : ID
                    {
                    $$ = newExpNode(IdK);
                    $$->attr.name = copyString(priorToken);
                    }
                    | ID
                    { 
                    savedName[++level] = copyString(priorToken);
                    savedLineNo[level] = lineno;
                   }
                    LBRACKET expression RBRACKET
                    {
                    $$ = newExpNode(IdArrayK);
                    $$ ->attr.name = savedName[level];
                    $$ ->lineno = savedLineNo[level--];
                    $$ ->child[0] = $4;
                    }
                    ;
simple_expression   : additive_expression LT additive_expression
                    {
                    $$ = newExpNode(OpK);
                    $$->attr.op = LT;
                    $$->child[0] = $1;
                    $$->child[1] = $3;
                    }
                    |additive_expression LE additive_expression
                    {   
                    $$ = newExpNode(OpK);
                    $$->attr.op = LE;
                    $$->child[0] = $1;
                    $$->child[1] = $3;
                    }
                    |additive_expression GT additive_expression
                    {
                    $$ = newExpNode(OpK);
                    $$->attr.op = GT;
                    $$->child[0] = $1;
                    $$->child[1] = $3;
                    }
                    |additive_expression GE additive_expression
                    {
                    $$ = newExpNode(OpK);
                    $$->attr.op = GE;
                    $$->child[0] = $1;
                    $$->child[1] = $3;
                    }
                    |additive_expression EQ additive_expression
                    {   $$ = newExpNode(OpK);
                    $$->attr.op = EQ;
                    $$->child[0] = $1;
                    $$->child[1] = $3;
                    }
                    |additive_expression NEQ additive_expression
                    {
                    $$ = newExpNode(OpK);
                    $$->attr.op = NEQ;
                    $$->child[0] = $1;
                    $$->child[1] = $3;
                    }
                    | additive_expression
                    {
                    $$ = $1;
                    }
                    ;
additive_expression : additive_expression PLUS term
                    {
                    $$ = newExpNode(OpK);
                    $$->attr.op = PLUS;
                    $$->child[0] = $1;
                    $$->child[1] = $3;
                    }
                    | additive_expression MINUS term
                    {
                    $$ = newExpNode(OpK);
                    $$->attr.op = MINUS;
                    $$->child[0] = $1;
                    $$->child[1] = $3;
                    }
                    |term
                    {
                    $$ = $1;
                    }
                    ;
term                : term MUL factor
                    {
                        $$ = newExpNode(OpK);
                        $$->attr.op = MUL;
                        $$->child[0] = $1;
                        $$->child[1] = $3;
                     }
                    |term DIVIDE factor
                    {
                    $$ = newExpNode(OpK);
                    $$->attr.op = DIVIDE;
                    $$->child[0] = $1;
                    $$->child[1] = $3;
                    }
                    | factor
                    { $$ = $1; }
                    ;
factor              : LPAREN expression RPAREN
                    { $$ = $2; }
                    | var
                    { $$ = $1; }
                    | call 
                    { $$ = $1; }
                    | NUM
                    {
                    $$ = newExpNode(ConstK);
                    $$->attr.val = atoi(tokenString);
                    }
                    ;
call                : ID
                    { 
                    savedName[++level] = copyString(priorToken);
                    savedLineNo[level] = lineno;}
                    LPAREN args RPAREN
                    {
                    $$ = newExpNode(CallK);
                    $$->child[0] = $4;
                    $$->attr.name = savedName[level];
                    $$->lineno = savedLineNo[level--];
                    }
                    ;
args                :args_list
                    {$$=$1;}
                    |
                    {$$=NULL;}
                    ;
args_list           : args_list COMMA expression
                    {
                    YYSTYPE t = $1;
                    if(t!=NULL)
                    {
                        while(t->sibling != NULL)
                            t=t->sibling;
                        t->sibling = $3;
                        $$ = $1;
                    }
                    else 
                        $$ = $3;
                    }
                    |expression
                    {$$=$1;}
                    ;
%%

int yyerror(char * message)
{ fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
  fprintf(listing,"Current token: ");
  printToken(yychar,tokenString);
  Error = TRUE;
  return 0;
}

/* yylex calls getToken to make Yacc/Bison output
 * compatible with ealier versions of the C- scanner
 */
static int yylex(void)
{ return getToken(); }

TreeNode * parse(void)
{ 
    yyparse();
  return savedTree;
}

