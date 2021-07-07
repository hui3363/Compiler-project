/****************************************************/
/* File: tiny.l                                     */
/* Lex specification for C-                         */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

%{
#include "globals.h"
#include "util.h"
#include "scan.h"
/* lexeme of Identifier or reserved word */
char tokenString[MAXTOKENLEN+1];
char priorToken[MAXTOKENLEN+1];
char beforePrior[MAXTOKENLEN+1];
%}

digit       [0-9]
NUM      {digit}+
letter      [a-zA-Z]
ID  {letter}+
newline     \n
whitespace  [ \t]+

%%

"else"	{return ELSE;}
"if"	{return IF;}
"int"	{return INT;}
"return"	{return RETURN;}
"void"	{return VOID;}
"while"	{return WHILE;}

"+"	{return PLUS;}
"-"	{return MINUS;}
"*"	{return MUL;}
"/"	{return DIVIDE;}
"<"	{return LT;}
"<="	{return LE;}
">"	{return GT;}
">="	{return GE;}
"=="	{return EQ;}
"!="	{return NEQ;}
"="	{return ASSIGN;}
";"	{return SEMI;}
","	{return COMMA;}
"("	{return LPAREN;}
")"	{return RPAREN;}
"["	{return LBRACKET;}
"]"	{return RBRACKET;}
"{"	{return LBRACE;}
"}"	{return RBRACE;}

{NUM}        {return NUM;}
{ID}    {return ID;}
{newline}       {lineno++;}
{whitespace}    {/* skip whitespace */}

"/*"             {
    register int c;
    int num_of_com=1;
    while(1)
    {
        while((c=input())!='*' && c!=EOF){
            if(c=='\n'){
                /* There is '\n' in comment. 
                   */
                lineno++;
            }
            if(c=='/'){
                if((c=input())=='*'){
                    /* two or more comment. */
                    num_of_com++;
                    continue;
                }
                else if(c=='\n'){
                    /* When there is '\n' after '/'. */
                    lineno++;
                }
            }
            /*eat up test of comment */
        }
        if(c=='*')
        {
            while((c=input())=='*'){
                ;
            }
            if(c=='\n'){
                /* When there is '\n' after '*'. */
                lineno++;
            }
            else if(c=='/'){
                /* A comment is closed properly. */
                num_of_com--;
                if(num_of_com==0)
                    break;/*All comments are closed*/
                
            }
        }
        if(c==EOF)
        {
            return COMERROR;
            break;
        }
    }
}

.   {return ERROR;}

%%

TokenType getToken(void)
{ static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
  { firstTime = FALSE;
    lineno++;
    yyin = source;
    yyout = listing;
  }
  if(priorToken!=NULL){
    strncpy(beforePrior,priorToken,MAXTOKENLEN);
  }
  if(tokenString!=NULL){
    strncpy(priorToken,tokenString,MAXTOKENLEN);
  }
  currentToken = yylex();
  strncpy(tokenString,yytext,MAXTOKENLEN);
  if (TraceScan) {
    //printToken(currentToken,tokenString);
  }
  return currentToken;
}

int yywrap(void){
	/* If reached End Of File, returns 1.
       else returns -1.
       This is called in line 1035 of lex.yy.c.
       Seldomly used. Written to handle linking error (undefined function)
       */
    return 1;
}
