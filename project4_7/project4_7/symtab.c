/****************************************************/
/* File: symtab.c                                   */
/* Symbol table implementation for the TINY compiler*/
/* (allows only one symbol table)                   */
/* Symbol table is implemented as a chained         */
/* hash table                                       */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

/* the hash function */
int hash ( char *key ){
  int temp = 0;
  int i = 0;
  while (key[i] != '\0'){
    temp = ((temp << SHIFT) + key[i]) % SIZE;
    ++i;
  }
  return temp;
}

void st_init(){
  currentScope = 0;
  currentScopeList = headScopeList->scopeChild;
}

void st_setCurrentScope(){	
  ScopeList t = currentScopeList->scopeChild;
  while(t->isSearched != FALSE)
    t = t->scopeSibling;	 
  currentScopeList = t;
  currentScopeList->isSearched = TRUE;
}

void st_make(int isFuncDeclaration){
  ScopeList temp = (ScopeList) malloc(sizeof(struct ScopeListRec));
  temp->scope = currentScope;

  if(currentScope != 1) {
    temp->upLocation = currentScopeList->upLocation;
    temp->downLocation = currentScopeList->downLocation;
    if(isFuncDeclaration)
      temp->downLocation -= 4;
    temp->funcLocation = currentScopeList->funcLocation;
  } else {
    temp->upLocation = 0;
    temp->downLocation = -4;
    temp->funcLocation = 0;
  }

  temp->scopeChild = NULL;
  temp->scopeParent = currentScopeList;
  temp->funcName = currentFuncName;
  temp->isSearched = FALSE;

  temp->params = (ParamList) malloc(sizeof(struct ParamListRec));
  temp->params->paramNum = 0;
  temp->params->next = NULL;

  if(currentScopeList->scopeChild == NULL)
    currentScopeList->scopeChild = temp;
  else{	 
    ScopeList t, firstChild = currentScopeList->scopeChild;
    t = firstChild;
    while(t->scopeSibling != NULL)t = t->scopeSibling;
      t->scopeSibling = temp;
  }	 
  currentScopeList = temp;
  //printSymTab(listing);
  //fprintf(listing,"##########################################################\n\n");
}

ScopeList findScope(char *name){
  int h = hash(name);
  ScopeList s = currentScopeList;

  while(s!= NULL){
    BucketList l =  s->hashTable[h];
    while ((l != NULL) && (strcmp(name,l->name) != 0)) l = l->next;
    if (l == NULL) s = s->scopeParent;
    else break;
  }
  return s;
}

ScopeList getInsertScopeList(char *name, int isDeclaration ){
  if(headScopeList == NULL){
    headScopeList = (ScopeList) malloc(sizeof(struct ScopeListRec));
    headScopeList->scopeChild = (ScopeList) malloc(sizeof(struct ScopeListRec));
    headScopeList->scopeChild->scope = 0;
    headScopeList->scopeChild->upLocation = 0;
    headScopeList->scopeChild->downLocation = 0;
    headScopeList->scopeChild->funcLocation = 0;
    headScopeList->scopeChild->scopeChild = NULL;
    headScopeList->scopeChild->scopeParent = NULL;
    headScopeList->scopeChild->scopeSibling = NULL;
    headScopeList->scopeChild->funcName = NULL;
    currentScopeList = headScopeList->scopeChild;
  }

  if(!isDeclaration)  {
      //seohee
      if(strcmp(name,"input")==0 || strcmp(name,"output")==0)
          return headScopeList->scopeChild;
      else
        return findScope(name);
  }
  else                return currentScopeList;
}

int calculateLocation (ScopeList s,int loc, int arraySize){

  if(arraySize <= 0)  arraySize = 1;

  if(loc == 0) return s->funcLocation ++;
  else if(loc > 0)       return s->upLocation += loc * arraySize;
  else if(loc < 0)  return s->downLocation += loc * arraySize;
  else              return 0;
}

void st_insert( char * name, int lineno, int loc , char *vpf, int arraySize, ExpKind type, int isDeclaration)
{
  int h = hash(name);
  ScopeList s = getInsertScopeList(name, isDeclaration);
  BucketList l =  s->hashTable[h];

  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if (l == NULL){
    l = (BucketList) malloc(sizeof(struct BucketListRec));
    l->name = name;
    l->lines = (LineList) malloc(sizeof(struct LineListRec));
    l->lines->lineno = lineno;
    l->scope = s->scope;
    l->memloc = calculateLocation(s, loc, arraySize);
    l->VPF = vpf;
    l->arraySize = arraySize;
    l->type = type;
    l->lines->next = NULL;
    l->next = s->hashTable[h];
    s->hashTable[h] = l; 
    if(strcmp(l->VPF, "Par") == 0){
      ParamList p = s->params;
      while(p->next != NULL) p = p->next;		
      p->next = (ParamList) malloc(sizeof(struct ParamListRec));
      p->next->paramNum = p->paramNum + 1; 
      p->next->type = l->type;
      p->next->next = NULL;
    }
  }
  else{
    LineList t = l->lines;
    while (t->next != NULL){
      if(t->next->lineno == lineno) return;
      t = t->next;
    }
    t->next = (LineList) malloc(sizeof(struct LineListRec));
    t->next->lineno = lineno;
    t->next->next = NULL;
  }
}

int st_lookup ( char *name ){ 
  int h = hash(name);

  if(headScopeList == NULL) 
    return -1;

  if(currentScopeList == NULL) 
    return -1;

  ScopeList s = findScope(name);
  if(s == NULL)
    return -1;
  return s->scope;
}

void st_goback(){ 
  currentScopeList = currentScopeList->scopeParent;
}

int st_hasNoSibling(char *name){
  ScopeList s = findScope(name);
  if(s == NULL) return FALSE;

  int h = hash(name);
  int i;

  s = s->scopeChild;
  while(s != NULL){
    if(strcmp(s->funcName, name) == 0){
      if(s->scopeSibling != NULL) return FALSE;
      else return TRUE;			
    }
    s = s->scopeSibling;
  }	
  return FALSE;
}

char *st_getVPF(char *name){
  ScopeList s = findScope(name);
  if(s == NULL) return NULL;

  int h = hash(name);
  BucketList l = s->hashTable[h];	

  while ((l != NULL) && (strcmp(name,l->name) != 0)) l = l->next;
  
  return l->VPF;
}

int st_getIsParam(char *name){
	ScopeList s = findScope(name);
	if(s == NULL) return NULL;
	
	int h = hash(name);
	BucketList l = s->hashTable[h];

	while((l != NULL) && (strcmp(name, l->name) != 0)) l = l->next;

	if( strcmp(l->VPF, "Par") == 0 ) return 1;
	else return 0;
}

ExpKind st_getType(char * name){
  ScopeList s = findScope(name);
  if(s == NULL) return -1;
  
  int h = hash(name);
  BucketList l = s->hashTable[h];	

  while ((l != NULL) && (strcmp(name,l->name) != 0)) l = l->next;
  
  return l->type;
}

int st_getAddr(char * name){
  	ScopeList s = findScope(name);
	if(s == NULL) return 0;

	int h = hash(name);
	BucketList l = s->hashTable[h];

	while((l != NULL) && (strcmp(name, l->name) != 0)) l = l->next;

	return l->memloc;
}

ScopeList st_getScopeByFuncName(char * funcName){
  ScopeList s = headScopeList->scopeChild->scopeChild;
  while(s->scopeSibling != NULL){
    if(strcmp(s->funcName, funcName)==0)
      return s;
    s=s->scopeSibling;
  }
  return NULL;
}

int st_checkFuncParamCnt(char * funcName, int cnt){
  ScopeList s = st_getScopeByFuncName(funcName);
  if(s == NULL) return FALSE;
  
  ParamList p = s->params;
  while(p->next != NULL) p = p->next;

  return cnt == p->paramNum;
}

int st_getFirstLine(char *name){
  int h = hash(name);
  ScopeList s = findScope(name);
  if(s==NULL) return -1;

  BucketList l = s->hashTable[h];
  while((l!=NULL) && (strcmp(name,l->name) !=0))
    l=l->next;
  return l->lines->lineno;
}

int st_checkFuncParamType(char * funcName, int idx, ExpKind type){
	ScopeList s = st_getScopeByFuncName(funcName);
  if(s == NULL) return FALSE;
	
  ParamList p = s->params;
	while(idx > 0){
    idx--;
		p = p->next;
	}
	
	return (p->type != Void && type != Void);
}

void printFormat(FILE * listing, ScopeList s){ 
  int i;
  int COLUMN_SIZE = 8;

  char *columnName[] = {"Name", "Scope", "Loc", "V/P/F", "Array?", "ArrSize", "Type", "Line Numbers"};
  char *type; 
  for(i=0; i< COLUMN_SIZE; i++){
    fprintf(listing, "%-8s ", columnName[i]);
  }
  fprintf(listing,"\n");

  for(i=0; i< COLUMN_SIZE; i++){
    fprintf(listing, "%-8s-", "--------");
    if(i == COLUMN_SIZE -1 )
      fprintf(listing, "%-8s-", "--------");
  }
  fprintf(listing,"\n");

  for (i=0;i<SIZE;++i){
    if (s->hashTable[i] != NULL){
      BucketList l = s->hashTable[i];
      while (l != NULL){ 
        fprintf(listing, "%-8s ", l->name);
        fprintf(listing, "%-8d ", l->scope);
        fprintf(listing, "%-8d ", l->memloc);
        fprintf(listing, "%-8s ", l->VPF);
        fprintf(listing, "%-8s ", l->arraySize == -1 ? "No" : "Array");
        if(l->arraySize == -1 )
          fprintf(listing, "%-8s ", "-");
        else
          fprintf(listing, "%-8d ", l->arraySize);
        switch(l->type){
          case Void:
            type = "void";
            break;
          case Integer:
            type = "int";
            break;
          case IntegerArray:
            type = "array";
            break;
          default:
            type = NULL;
            break;
        }
        fprintf(listing, "%-8s ", type);

        LineList t = l->lines;
        while (t != NULL){
          fprintf(listing,"%4d ",t->lineno);
          t = t->next;
        }
        fprintf(listing,"\n");
        l = l->next;
      }
    }
  }
  fprintf(listing,"\n");
}

void printScopeList(FILE * listing, ScopeList s){ 
  if (s != NULL){ 
    printScopeList(listing, s->scopeChild);
    printFormat(listing, s);
    printScopeList(listing, s->scopeSibling);
  }
}

void printSymTab(FILE * listing){
  printScopeList(listing, headScopeList->scopeChild);
}
