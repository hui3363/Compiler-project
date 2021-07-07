#include "globals.h"
#include "symtab.h"
#include "util.h"
#include "code.h"

#define BOOL int
#define TRUE 1
#define FALSE 0

static int numWhile;
static int numIf;

void generateCode(TreeNode *node, BOOL isAddr, BOOL doSibling);

void generateInOut(void)
{
    emitSection("data");
    emitString("prompt0", "Please input an Integer: ");
    emitString("result0", "Output: ");
    emitString("linefeed0", "\\n");
    emitSection("text");

    //input
    emitFuncName("input");

    emitPush("$fp");
    emitMove("$fp", "$sp", "set frame pointer");
    emitPush("$ra");

    emitLi("$v0", 4, "set syscall number");
    emitLa("$a0", "prompt0", "set output message");
    emitSyscall("print prompt");

    emitLi("$v0", 5, "set syscall number");
    emitSyscall("get input");

    emitPop("$ra");
    emitPop("$fp");
    emitJr("$ra", "return");

    //output
    emitFuncName("output");

    emitPush("$fp");
    emitMove("$fp", "$sp", "set frame pointer");
    emitPush("$ra");

    emitLi("$v0", 4, "set syscall number");
    emitLa("$a0", "result0", "set output message");
    emitSyscall("print output message");

    emitLi("$v0", 1, "set syscall number");
    emitLw("$a0", "$fp", RSIZE, "set output");
    emitSyscall("print output");

    emitLi("$v0", 4, "set syscall number");
    emitLa("$a0", "linefeed0", "set output message");
    emitSyscall("linefeed");

    emitPop("$ra");
    emitPop("$fp");
    emitJr("$ra", "return");
}

int generateParamPassing(TreeNode *arg)
{
    int argSize;

    if(!arg) return 0;
    argSize = generateParamPassing(arg->sibling);

    generateCode(arg, FALSE, FALSE);
    emitPush("$v0");

    return argSize + RSIZE;
}

void generateFuncCall(TreeNode *node)
{
    int argSize;

    emitComment("parameter passing begin");
    argSize = generateParamPassing(node->child[0]);
    emitComment("parameter passing end");
    emitJal(node->attr.name, "call");
    if(argSize) emitAddi("$sp", "$sp", argSize, "clear stack");
}

void generateReturn(TreeNode *node)
{
    if(node->child[0]) generateCode(node->child[0], FALSE, TRUE);

    emitAddi("$sp", "$fp", -RSIZE, "clear stack");
    emitPop("$ra");
    emitPop("$fp");
    emitJr("$ra", "return");
}

void generateWhile(TreeNode *node)
{
    int num = numWhile++;

    emitLabel("WCOND", num);
    generateCode(node->child[0], FALSE, TRUE);
    emitBeqz("$v0", "WEND", num, "while begin");
    emitComment("while loop body");
    generateCode(node->child[1], FALSE, TRUE);
    emitJ("WCOND", num, "back to condition");
    emitLabel("WEND", num);
}

void generateFunction(TreeNode *node)
{
    if(strcmp(node->attr.name, "main") == 0) emitSection("globl main");

    emitFuncName(node->attr.name);

    emitComment("function header begin");
    emitPush("$fp");
    emitMove("$fp", "$sp", "set frame pointer");
    emitPush("$ra");
    emitComment("function header end");

    if(node->child[2])
    	generateCode(node->child[2], FALSE, TRUE);

    emitComment("function footer begin");
    emitPop("$ra");
    emitPop("$fp");
    emitJr("$ra", "return");

    emitComment("function footer end");
}

void generateCompound(TreeNode *node)
{
    int sizeLocal = 0;

    if(node->child[0]!=NULL && node->child[0]->nodekind == DeclK)
    {
        struct treeNode *local = node->child[0];

        while(local)
        {
            if(local->arrayIndex != -1) {
		sizeLocal += ISIZE * local->arrayIndex;
	    }
	    else sizeLocal += ISIZE;

            local = local->sibling;
        }

        emitAddi("$sp", "$sp", -sizeLocal, "local variable");
    }
    else generateCode(node->child[0], FALSE, TRUE);
    generateCode(node->child[1], FALSE, TRUE);

    if(sizeLocal) emitAddi("$sp", "$sp", sizeLocal, "clear local");
}

void generateArrayDecl(TreeNode *node)
{
    emitArrayDecl(node->attr.name, node->arrayIndex);
}

void generateVarDecl(TreeNode *node)
{
    emitVarDecl(node->attr.name);
}

void generateConst(TreeNode *node)
{
    emitLi("$v0", node->attr.val, "const");
}

void generateIf(TreeNode *node){
    int end = numIf++;

    emitComment("if contidion");
    generateCode(node -> child[0],FALSE,TRUE);

    if(node->child[2] != NULL){//ELSE
        emitBeqz("$v0", "Else", end, "then");
        generateCode(node->child[1],FALSE,TRUE);
        emitJ("End", end, "end if");

        emitLabel("Else", end);
        generateCode(node->child[2],FALSE,TRUE);
    }
    else{//NO ELSE
        emitBeqz("$v0", "End", end, "then");
        generateCode(node->child[1],FALSE,TRUE);
    }

    emitLabel("End", end);
}

void generateVal(TreeNode * node, char *name, BOOL isAddr){
    if(node -> global == 1){
        emitLa("$v0", name, "global address");
    }
    
    else if( node->isParam && node->type == IntegerArray)  
    {
        emitAddi("$v0", "$fp", node->addr, "pointer 1/2");			//KWS address should be here
        emitLw("$v0", "$v0", 0, "pointer 2/2");
    }
    else{
        emitAddi("$v0", "$fp", node->addr, "local address");			//KWS address should be here
    }

    if(!(isAddr) && node->type != IntegerArray){
        emitLw("$v0", "$v0", 0, "variable value");
    }
}

void generateArrayVal(TreeNode * node, BOOL isAddr){
	if(node->isParam ) {
    		emitAddi("$v0", "$fp", node->addr, "pointer 1/2");			//seohee
    		emitLw("$v0", "$v0", 0, "pointer 2/2");
	} else if(node->global!=1){
		emitAddi("$v0", "$fp", node->addr, "local address [array]"); 
	}else if(node->global==1){
        emitLa("$v0", node->attr.name, "global address");
    }

    emitPush("$v0");
    generateCode(node -> child[0],FALSE,TRUE);//v0에 index값 저장

    emitPop("$v1");

    emitLi("$t2", ISIZE, "[] 1/4");
    emitMul("$t0", "$t2", "$v0", "[] 2/4");
    emitAdd("$t1", "$v1", "$t0", "[] 3/4");

    if(isAddr){
        emitMove("$v0", "$t1", "[] 4/4");
    }
    else{
        emitLw("$v0", "$t1", 0, "[] 4/4");
    }

}

void generateRelop(TreeNode * node){
    generateCode(node -> child[0], FALSE, TRUE);
    emitPush("$v0");
    generateCode(node -> child[2], FALSE, TRUE);
    //이렇게하면 v1 : child[0], v0 : child[1]
    emitPop("$v1");

    switch(node ->child[1]-> attr.op){
        case LTE:// v1<=v2
            emitSlt("$v0", "$v0", "$v1", "$ <= 1/2");//v2 < v1인 경우 true 반환
            emitXori("$v0", "$v0", 1, "$ <= 2/2");
            break;
        case LT://<
            emitSlt("$v0", "$v1", "$v0", "<");
            break;
        case GT://>
            emitSlt("$v0", "$v0", "$v1", ">");
            break;
        case GTE://>=
            emitSlt("$v0", "$v1", "$v0", ">= 1/2");//v1 < v2인 경우 true 반환
            emitXori("$v0", "$v0", 1, ">= 2/2");
            break;
        case EQ://==
            emitSlt("$t0", "$v1", "$v0", "== 1/4");
            emitSlt("$t1", "$v0", "$v1", "== 2/4");
            emitOr("$v0", "$t1", "$t0", "== 3/4");
            emitXori("$v0", "$v0", 1, "== 4/4");
            break;
        case NEQ://!=
            emitSlt("$t0", "$v1", "$v0", "== 1/3");
            emitSlt("$t1", "$v0", "$v1", "== 2/3");
            emitOr("$v0", "$t1", "$t0", "== 3/3");
            break;
        }
}

void generateAssign(TreeNode *node , BOOL isAddr){
    generateCode(node->child[1],FALSE,TRUE);
    emitPush("$v0");
    generateCode(node->child[0],TRUE,TRUE);
    emitPop("$v1");

    emitSw("$v1", "$v0", 0, "assign");
    if(!isAddr) emitLw("$v0", "$v0", 0, "assign value");
}


void generateArithmetic(TreeNode *node){
    generateCode(node->child[0], FALSE, TRUE);
    emitPush("$v0");

    generateCode(node ->child[2], FALSE, TRUE);
    emitPop("$v1");
    //first : v1 , second : v0

    switch(node->child[1]-> attr.op){
        //PLUS,MINUS,TIMES,OVER
        case PLUS:{
            emitAdd("$v0", "$v1", "$v0", "plus");
            break;
        }
        case MINUS:{
            emitSub("$v0", "$v1", "$v0", "minus");
            break;
        }
        case TIMES:{
            emitMul("$v0", "$v1", "$v0", "mul");
            break;
        }
        case DIV:{
            emitDiv("$v0", "$v1", "$v0", "div");
            break;
        }
    }
}

void generateCode(TreeNode *node, BOOL isAddr, BOOL doSibling)
{
    while(node)
    {
        if(node->nodekind == StmtK)
        {
            switch(node->kind.stmt)
            {
                case CallK:
                    generateFuncCall(node);
                    break;
                case ReturnK:
                    generateReturn(node);
                    break;
				case WhileK:
					generateWhile(node);
					break;
				case CompoundK:
					generateCompound(node);
					break;
				case IfK:
					generateIf(node);
					break;
                case AssignK:
					generateAssign(node, isAddr);
                    break;
                case ExpStmtK:
                    if(node->child[1]->attr.op==LT || node->child[1]->attr.op==LTE ||
                            node->child[1]->attr.op==GT ||node->child[1]->attr.op==GTE ||
                            node->child[1]->attr.op==EQ ||node->child[1]->attr.op==NEQ)
                        generateRelop(node);
					else if(node->child[1]->attr.op==PLUS || 
                            node->child[1]->attr.op==MINUS || 
                            node->child[1]->attr.op == TIMES ||
                            node->child[1]->attr.op==DIV) 
                        generateArithmetic(node);
                    break;
            }
        }
		else if(node->nodekind == DeclK)
		{
			switch(node->kind.decl)
			{
				case FunK:
					generateFunction(node);
					break;
				case VarK:
					if(node->type == IntegerArray) {
						generateArrayDecl(node);
					} else {
						generateVarDecl(node);
					}
					break;
			}
		}
		else if(node->nodekind == ExpK)
		{
			switch(node->kind.exp)
			{
				case IdK:
                    if(node->type==IntegerArray && node->child[0]!=NULL){
						generateArrayVal(node, isAddr);
                    }
                    else {
					    generateVal(node, node->attr.name, isAddr);
                    }
					break;
				case ConstK:
					generateConst(node);
					break;
			}
		}

        node = doSibling ? node->sibling : NULL;
    }
}

void codeGen(TreeNode *syntaxTree, char *codefile)
{
	numWhile	= 0;
	numIf		= 0;

	if(TraceCode)
	{
		fprintf(listing, "\nGenerating SPIM code to [%s]\n", codefile);
		fprintf(listing, "-----------------------------------------------\n\n\n");
	}

    generateInOut();
    generateCode(syntaxTree, FALSE, TRUE);

	if(TraceCode)
	{
		fprintf(listing, "\n\n-----------------------------------------------\n");
		fprintf(listing, "Code generation is done\n\n");
	}
}

