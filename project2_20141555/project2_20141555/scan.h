/****************************************************/
/* File: scan.h                                     */
/* The scanner interface for the C- compiler        */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _SCAN_H_
#define _SCAN_H_

/* MAXTOKENLEN is the maximum size of a token */
#define MAXTOKENLEN 40

/* tokenString array stores the lexeme of each token */
extern char tokenString[MAXTOKENLEN+1];

/* tokenString array stores the lexeme of shortly before token */
extern char priorToken[MAXTOKENLEN+1];

/* tokenString array stores the lexeme of shortly before the priorToken */
extern char beforePrior[MAXTOKENLEN+1];

/* function getToken returns the 
 * next token in source file
 */
TokenType getToken(void);

/* yylex calls getToken to make Yacc/Bison output
 * compatible with earlier versions of the TINY scanner
 */
//TreeNode *parse(void);

#endif
