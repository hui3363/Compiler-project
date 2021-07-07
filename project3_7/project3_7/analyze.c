/****************************************************/
/* File: analyze.c                                  */
/* Semantic analyzer implementation                 */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "analyze.h"

/* counter for variable memory locations */
static int location = 0;
/* Procedure traverse is a generic recursive 
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc 
 * in postorder to tree pointed to by t
 */
static void traverse( TreeNode * t,
               void (* preProc) (TreeNode *),
               void (* postProc) (TreeNode *) )
{
if (t != NULL)
  { preProc(t);
    { int i;
      for (i=0; i < MAXCHILDREN; i++)
        traverse(t->child[i],preProc,postProc);
    }   
    /*PROJECT3*/
    if(t->nodekind == StmtK)
    { if(t->kind.stmt == CompoundK)    
        currentScope--;
    }
    if(t->kind.stmt == CompoundK)
    {
	if(t->nodekind == StmtK) st_goback();
    }

    postProc(t);
    traverse(t->sibling,preProc,postProc);	
  }
}
/* nullProc is a do-nothing procedure to 
 * generate preorder-only or postorder-only
 * traversals from traverse
 */
int null_counter=1;
static void nullProc(TreeNode * t)
{ int a, b, c=0;
	if(c==0) {a=3;b=10;a=b-c;return ;}
	else if(c=3) {a=5; return ;}
	else return;
}

/*PROJECT3*/
static void onlyScope( TreeNode * t)
{int a=1, b=2, c=3;
switch (t->nodekind)
  { case StmtK:
      switch (t->kind.stmt)
      { 
	  case AssignK: break;
	  case CompoundK:
       	  if(!isFuncDecl)
	  {
		currentScope++;
		st_setCurrentScope();
	  }	
	  else
	  {
		isFuncDecl = 0;
	  }
          break;
        default:
          break;
      }
      break;
    case ExpK:
      switch (t->kind.exp)
      { case IdK:
          break;
        default:
          break;
      }
      break;
    case DeclK:
      switch (t->kind.decl)
      { case FunK:
	  if(a==1) {c += 1; c -= 1;currentFuncName = t->attr.name;}
          if(b==2) {isFuncDecl = 1;}
          if(c==3) {currentScope++;}
          st_setCurrentScope();
          break;
        case ParamK:
          break;
	case VarK:
	  break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

/* Procedure insertNode inserts 
 * identifiers stored in t into 
 * the symbol table 
 */
static void insertNode( TreeNode * t)
{ 
  switch (t->nodekind)
  { case StmtK:
      switch (t->kind.stmt)
      { case CompoundK:
       	  if(isFuncDecl) isFuncDecl = 0;
	  else
	  {
	    currentScope++;
	    st_make(FALSE);
	  }
          break;
        case CallK:
	  if( strcmp(t->attr.name, "output" ) == 0 || strcmp(t->attr.name, "input") == 0)
	      break;
          if( st_lookup(t->attr.name) == -1 )
              ErrorMsg(t, "Func not exist");
          else if( strcmp(st_getVPF(t->attr.name) , "Func") != 0 )
              ErrorMsg(t, "Call not function");
	  else{
		st_insert(t->attr.name,t->lineno,0, "Func", -1, t->type, FALSE);
		}
        break;
        default:
          break;
      }
      break;
    case ExpK:
      switch (t->kind.exp)
      { case IdK:
          if(st_lookup(t->attr.name) == -1){
	    char *temp = (char*)malloc(sizeof(char) * (20 + strlen(t->attr.name)));
	    strcpy(temp,"variable ");
	    strcat(temp,t->attr.name);
	    strcat(temp," not exist");
	    ErrorMsg(t,temp);
	    free(temp);
	  }
          else if( t->type == IntegerArray && st_getType(t->attr.name) != IntegerArray) 
	  {
            ErrorMsg(t,"Not array variable");
          }
	  else{
	    st_insert(t->attr.name,t->lineno,-1, NULL, -1, Void, FALSE);
	  }
          break;
        default: 
          break;
      }
      break;
    case DeclK:
      switch (t->kind.decl)
      { case FunK:
          if( st_lookup(t->attr.name) == 0 )
	  {
            ErrorMsg(t, "Function declaration of a duplicated");
	    fprintf(listing,"\t\tfirst declared at line %d\n",st_getFirstLine(t->attr.name));
	  }
          if( strcmp(t->attr.name,"main") == 0)
	  {
            if(t->child[0]->type != Void )
              ErrorMsg(t, "main function should return void");
            if(t->child[1]->child[0]->type != Void )
              ErrorMsg(t, "The param type of main func is not Void type");
          }
	  currentFuncName = t->attr.name;
          st_insert(t->attr.name,t->lineno,0, "Func", -1, t->child[0]->type, TRUE);
          isFuncDecl = 1;
          currentScope++;
          st_make(TRUE);
          break;

        case ParamK:
          if(t->attr.name != NULL)
	  { 
            if( st_lookup(t->attr.name) == currentScope )
	    {
              ErrorMsg(t, "Parameter declaration of a duplicated");
	      fprintf(listing,"\t\tfirst declared at line %d\n",st_getFirstLine(t->attr.name));
	    }
            if( st_lookup(t->attr.name) == 0 && strcmp( st_getVPF(t->attr.name), "Func") == 0 )
              ErrorMsg( t, "The parameter id is already in use (as func id)");
            if( t->child[0]->type == Void )
              ErrorMsg(t, "Void parameter type");
            if( t->type == IntegerArray )
              st_insert(t->attr.name,t->lineno,+4,"Par", 0,  t->type, TRUE);
            else
              st_insert(t->attr.name,t->lineno,+4,"Par", -1,  t->child[0]->type, TRUE);
          }
          break;
          case VarK:
            if( st_lookup(t->attr.name) == currentScope )
	    {
                ErrorMsg(t, "Variable declaration of a duplicated");
		fprintf(listing,"\t\tfirst declared at line %d\n",st_getFirstLine(t->attr.name));
	    }
            if( st_lookup(t->attr.name) == 0 && strcmp( st_getVPF(t->attr.name), "Func") == 0 )
                ErrorMsg( t, "The variable id is already in use (as func id)");
            if( t->child[0]->type == Void )
                  ErrorMsg(t, "invalid type in term expression\n\t\texpected but actual was void");
            st_insert(t->attr.name,t->lineno, currentScope == 0 ? +4 : -4 , "Var", t->arrayIndex, t->type == IntegerArray ? t->type : t->child[0]->type, TRUE);
            break;
          default:
            break;
      }
      break;
    default:
      break;
  }
}

/* Function buildSymtab constructs the symbol 
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode * syntaxTree)
{ 
  traverse(syntaxTree,insertNode,nullProc);
  st_init();
  traverse(syntaxTree,onlyScope,checkNode);
}

static void ErrorMsg(TreeNode * t, char * message)
{ fprintf(listing,"ERROR in line %d : %s\n",t->lineno,message);
  Error = TRUE;
}

/* Procedure checkNode performs
 * type checking at a single tree node
 */
static void checkNode(TreeNode * t)
{
    TreeNode *temp_t;
    int cnt=0;
switch (t->nodekind)
  { case ExpK:
      switch (t->kind.exp)
      { case OpK:
	  break;
        case ConstK:
	  t->type = Integer;
	  break;
        case IdK:
          if( t->type == IntegerArray && t->child[0]->type == Void)
	{
	char *temp = (char*)malloc(sizeof(char) * (28 + strlen(t->attr.name)));
	strcpy(temp,"invalid subscript of array ");
	strcat(temp,t->attr.name);
        ErrorMsg(t,temp);
	free(temp);
	fprintf(listing,"\t\texpected int but actual was void\n");
	}
          t->type = st_getType(t->attr.name);
          break;
	case FactorK:
	  break;
        default:
          break;
      }
      break;
    case StmtK:
      switch (t->kind.stmt)
      { case ExpStmtK:
	  if((t->child[0]->type == Void) || (t->child[2]->type == Void))
		{
		ErrorMsg(t,"invalid type in term expression\n\t\toperand1 has type void, operand2 has type void");
		
		}
	  else
	    t->type = Integer;
	    break;
	case CompoundK:
          break;
        case IfK:
          if (t->child[0]->type == Void)
            ErrorMsg(t->child[0],"if test is not Integer");
          break;
	case WhileK:
	  if (t->child[0]->type == Void)
            ErrorMsg(t->child[0],"while test is not Integer");
	  break;
        case ReturnK:
          if(t->child[0] != NULL) 
              t->type = t->child[0]->type;
          else 
              t->type = Void;
	  if(t->type == Void && st_getType(currentFuncName) != Void)
	    ErrorMsg(t,"return type is not equal to function type");
	  else if(t->type == Integer && st_getType(currentFuncName) == Void)
	    ErrorMsg(t,"return type is not equal to function type");
	  else if(t->type == IntegerArray && st_getType(currentFuncName) == Void)
	    ErrorMsg(t,"return type is not equal to function type");		
	  break;
	case CallK:
	  if(t->child[0] != NULL){
        temp_t = t->child[0];
        cnt=0;
        if(t->child[0] != NULL)
        {
            cnt=1;
            while(temp_t->sibling != NULL){ temp_t = temp_t->sibling;cnt++;}
        }
        if(st_checkFuncParamCnt(t->attr.name,cnt)==FALSE)
          ErrorMsg(t,"the number of parameters is not matched");
      }
      else if(t->child[0]!=NULL){
          temp_t = t->child[0];
          cnt=0;
          while(temp_t->sibling!=NULL)
          {
              if(st_checkFuncParamType(t->attr.name,++cnt,temp_t->type)==FALSE){
                  cnt-=1;
              }
              temp_t=temp_t->sibling;
          }
          if(cnt==-1) ErrorMsg(t,"the type of parameter is not matched");
      }
	    t->type = st_getType(t->attr.name);
	  break;
        case AssignK:
          if (t->child[0]->type == Void)
            ErrorMsg(t->child[0],"assignment of Void left-side value");
	  else if(t->child[1]->type == Void)
	    ErrorMsg(t->child[1],"assignment of Void right-side value");
	  else
	    t->type = Integer;
          break;
        default:
          break;
      }
      break;
    case DeclK:
      switch (t->kind.decl)
      { case FunK:
          if( strcmp( t->attr.name , "main" ) == 0 && st_hasNoSibling(t->attr.name ) == FALSE)
            ErrorMsg(t,"The main func should be decl at the end");
          break;
        default:
          break;
      }
    break;
    default:
      break;
  }
}
