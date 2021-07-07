/****************************************************/
/* File: globals.h                                  */
/* Global types and vars for C- compiler            */
/* must come before other include files             */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef YYPARSER

/* the name of the following file may change */
#include "y.tab.h"

/* ENDFILE is implicitly defined by Yacc/Bison,
 * and not included in the tab.h file 
 */
#define ENDFILE 0
#define COMMENT 1

#endif

/*
typedef enum
{
	ENDFILE, ERROR, COMERROR,
	// reserved words 
	ELSE, IF, INT, RETURN, VOID, WHILE,
	// multicharacter tokens 
	ID, NUM,
	// special sysbols 
	PLUS, MINUS, MUL, DIVIDE, LT, LE, GT, GE, EQ, NEQ, ASSIGN, SEMI, COMMA, LPAREN, RPAREN, LBRACKET, RBRACKET, LBRACE, RBRACE, COMMENT
} TokenType;
*/
typedef int TokenType;

extern FILE* source; /* source code text file */
extern FILE* listing; /* listing output text file */
extern FILE* code; /* code text file for TM simulator */

extern int lineno; /* source line number for listing */

/**************************************************/
/***********   Syntax tree for parsing ************/
/**************************************************/

typedef enum {StmtK,ExpK,DecK} NodeKind;
typedef enum {CompoundK, ExpressionK, SelectionK, IterationK, ReturnK } StmtKind;
typedef enum {OpK, ConstK, IdK, IdArrayK, AssignK, CallK, TypeK, ParamK} ExpKind;
typedef enum {VarK, ArrayK, FunK} DecKind;

/* ExpType is used for type checking */
typedef enum {Void,Integer,Boolean} ExpType;

#define MAXCHILDREN 3

typedef struct treeNode
   { struct treeNode * child[MAXCHILDREN];
     struct treeNode * sibling;
     int lineno;
     NodeKind nodekind;
     union { StmtKind stmt; ExpKind exp; DecKind dec;} kind;
     union { TokenType op;
             int val;
             char * name; } attr;
     int sizeArray;
     ExpType type; /* for type checking of exps */
   } TreeNode;

/**************************************************/
/***********   Flags for tracing       ************/
/**************************************************/

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern int EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern int TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
 * printed to the listing file in linearized form
 * (using indents for children)
 */
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
 * and lookups to be reported to the listing file
 */
extern int TraceAnalyze;

/* TraceCode = TRUE causes comments to be written
 * to the TM code file as code is generated
 */
extern int TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
extern int Error; 
#endif
