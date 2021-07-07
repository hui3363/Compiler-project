/****************************************************/
/* File: symtab.h                                   */
/* Symbol table interface for the TINY compiler     */
/* (allows only one symbol table)                   */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _SYMTAB_H_
#define _SYMTAB_H_
#include "globals.h"

/* SIZE is the size of the hash table */
#define SIZE 211

/* SHIFT is the power of two used as multiplier
   in hash function  */
#define SHIFT 4

typedef struct LineListRec{
  int lineno;
  struct LineListRec * next;
} *LineList;

typedef struct BucketListRec{
  char *name;
  char *VPF;
  int memloc;
  int scope; 
  int arraySize;
  struct BucketListRec *next;
  LineList lines;
  ExpKind type;
  int emitLoc;
} *BucketList;

typedef struct ParamListRec{
  int paramNum;
  struct ParamListRec *next;
  ExpKind type;
} *ParamList;

typedef struct ScopeListRec{
  BucketList hashTable[SIZE];
  char *funcName;
  int scope;
  int isSearched;
  int upLocation;
  int downLocation;
  int funcLocation;
  struct ScopeListRec *scopeChild;
  struct ScopeListRec *scopeSibling;
  struct ScopeListRec *scopeParent;
  ParamList params;
} *ScopeList;

static ScopeList headScopeList;
static ScopeList currentScopeList;

int hash (char *);
void st_init();
void st_setCurrentScope();
ScopeList st_makeCurrentScopeList(int isFuncDeclaration);

void st_insert(char*, int, int, char*, int, ExpKind, int);
void st_make(int);
int st_lookup (char*);
void st_goback();
int st_hasSibling(char*);
int st_hasNoSibling(char*);
char *st_getVPF(char*);
ExpKind st_getType(char*);
int st_getFirstLine(char*);
void printSymTab(FILE*);
int st_checkFuncParamType(char*, int, ExpKind);
int st_checkFuncParamCnt(char*, int);
ScopeList findScope(char *);
int st_getAddr(char *);
int st_getIsParam(char*);

#endif
