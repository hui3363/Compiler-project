#include <string.h>
#include "globals.h"
#include "scan.h"

enum
{
	BUFFER_SIZE = 1024,

	SYM_LEN		= -13,
	CODE_LEN	= -70
};

void printComment(const char *comment)
{
	fprintf(code, " # %s\n", comment);
	if(TraceCode) fprintf(listing, " # %s\n", comment);
}

void instType1(const char *inst, const char *r1, const char *r2, int offset, const char *comment)
{
	char line[BUFFER_SIZE];
	char buffer1[MAXTOKENLEN + 2];
	char buffer2[MAXTOKENLEN + 14];

	sprintf(buffer1, "%s,", r1);
	sprintf(buffer2, "%d(%s)", offset, r2);

	sprintf(line, "%*s %*s %*s %*s", SYM_LEN, "", SYM_LEN, inst, SYM_LEN, buffer1, SYM_LEN, buffer2);

	fprintf(code, "%*s", CODE_LEN, line);
	if(TraceCode) fprintf(listing, "%*s", CODE_LEN, line);

	printComment(comment);
}

void instType2(const char *inst, const char *r1, const char *r2, const char *r3, const char *comment)
{
	char line[BUFFER_SIZE];
	char buffer1[MAXTOKENLEN + 2];
	char buffer2[MAXTOKENLEN + 3];

	sprintf(buffer1, "%s,", r1);
	sprintf(buffer2, "%s,", r2);
	sprintf(line, "%*s %*s %*s %*s %*s",
		SYM_LEN, "", SYM_LEN, inst, SYM_LEN, buffer1, SYM_LEN, buffer2, SYM_LEN, r3);

	fprintf(code, "%*s", CODE_LEN, line);
	if(TraceCode) fprintf(listing, "%*s", CODE_LEN, line);

	printComment(comment);
}

void instType3(const char *inst, const char *r1, const char *r2, const char *comment)
{
	char line[BUFFER_SIZE];
	char buffer[MAXTOKENLEN + 2];

	sprintf(buffer, "%s,", r1);
	sprintf(line, "%*s %*s %*s %*s", SYM_LEN, "", SYM_LEN, inst, SYM_LEN, buffer, SYM_LEN, r2);

	fprintf(code, "%*s", CODE_LEN, line);
	if(TraceCode) fprintf(listing, "%*s", CODE_LEN, line);

	printComment(comment);
}

void instType4(const char *inst, const char *r1, const char *comment)
{
	char line[BUFFER_SIZE];
	sprintf(line, "%*s %*s %*s", SYM_LEN, "", SYM_LEN, inst, SYM_LEN, r1);

	fprintf(code, "%*s", CODE_LEN, line);
	if(TraceCode) fprintf(listing, "%*s", CODE_LEN, line);

	printComment(comment);
}

void emitString(const char *name, const char *str)
{
	char buffer[MAXTOKENLEN + 2];
	sprintf(buffer, "_%s:", name);

	fprintf(code, "%*s %*s \"%s\"\n", SYM_LEN, buffer, SYM_LEN, ".asciiz", str);
	if(TraceCode) fprintf(listing, "%*s %*s \"%s\"\n", SYM_LEN, buffer, SYM_LEN, ".asciiz", str);
}

void emitArrayDecl(const char *name, int size)
{
	char buffer[MAXTOKENLEN + 2];
	char arrayCount[14];

	sprintf(buffer, "_%s:", name);
	sprintf(arrayCount, "0:%d", size);

	fprintf(code, "%*s %*s %*s\n", SYM_LEN, buffer, SYM_LEN, ".word", SYM_LEN, arrayCount);

	if(TraceCode)
	{
		fprintf(listing, "%*s %*s %*s\n", SYM_LEN, buffer, SYM_LEN, ".word", SYM_LEN, arrayCount);
	}
}

void emitVarDecl(const char *name)
{
	char buffer[MAXTOKENLEN + 2];
	sprintf(buffer, "_%s:", name);

	fprintf(code, "%*s %*s %*d\n", SYM_LEN, buffer, SYM_LEN, ".word", SYM_LEN, 0);
	if(TraceCode) fprintf(listing, "%*s %*s %*d\n", SYM_LEN, buffer, SYM_LEN, ".word", SYM_LEN, 0); 
}

void emitSyscall(const char *comment)
{
	char line[BUFFER_SIZE];
	sprintf(line, "%*s %s", SYM_LEN, "", "syscall");

	fprintf(code, "%*s", CODE_LEN, line);
	if(TraceCode) fprintf(listing, "%*s", CODE_LEN, line);

	printComment(comment);
}

//

void emitComment(const char *comment)
{
	char line[BUFFER_SIZE];
	int len		= strlen(comment);
	int padding	= -CODE_LEN + SYM_LEN - len - 5;
	int left	= padding / 2;
	int right	= padding - left;
	int i;

	if(0 < padding)
	{
		for(i = 0; i < left; ++i) line[i] = '-';
		line[i++] = ' ';

		i += sprintf(line + i, "%s ", comment);

		while(0 < right--) line[i++] = '-';
		line[i++]	= ' ';
		line[i++]	= '#';
		line[i]		= '\0';
	}
	else sprintf(line, "%s", comment);

	fprintf(code, "%*s # %s\n", SYM_LEN, "", line);
	if(TraceCode) fprintf(listing, "%*s # %s\n", SYM_LEN, "", line);
}

void emitLabel(const char *labelname, int labelnum)
{
	fprintf(code, "%s%d:\n", labelname, labelnum);
	if(TraceCode) fprintf(listing, "%s%d:\n", labelname, labelnum);
}

void emitFuncName(const char *name)
{
	char label[MAXTOKENLEN + 2];

	if(strcmp(name, "main") == 0) sprintf(label, "main");
	else sprintf(label, "_%s", name);

	fprintf(code, "%s:\n", label);
	if(TraceCode) fprintf(listing, "%s:\n", label);
}

void emitSection(const char *name)
{
	fprintf(code, "%*s .%s\n", SYM_LEN, "", name);
	if(TraceCode) fprintf(listing, "%*s .%s\n", SYM_LEN, "", name);
}

//

/*addi, add, sub, mul, div*/
void emitAddi(const char *reg1, const char *reg2, int value, const char * comment){
	char buf[12];
	sprintf(buf, "%d", value);
	instType2("addi", reg1, reg2, buf, comment);
}
void emitAdd(const char *reg1, const char *reg2, const char *reg3, const char * comment){
	instType2("add", reg1, reg2, reg3, comment);
}
void emitSub(const char *reg1, const char *reg2, const char *reg3, const char * comment){
	instType2("sub", reg1, reg2, reg3, comment);
}
void emitMul(const char *reg1, const char *reg2, const char *reg3, const char * comment){
	instType2("mul", reg1, reg2, reg3, comment);
}
void emitDiv(const char *reg1, const char *reg2, const char *reg3, const char * comment){
	instType2("div", reg1, reg2, reg3, comment);
}
/*xori, or*/
void emitXori(const char *reg1, const char *reg2, int value, const char * comment){
	char val[12];
	sprintf(val, "%d", value);

	instType2("xori", reg1, reg2, val, comment);
}
void emitOr(const char *reg1, const char *reg2, const char *reg3, const char * comment){
	instType2("or", reg1, reg2, reg3, comment);
}
/*move, li*/
void emitMove(const char *reg1, const char *reg2, const char * comment){
	instType3("move", reg1, reg2, comment);
}
void emitLi(const char *reg, int value, const char * comment){
	char val[12];
	sprintf(val, "%d", value);

	instType3("li", reg, val, comment);
}
/*lw, sw, la*/
void emitLw(const char *reg1, const char *reg2, int offset, const char * comment){
	instType1("lw", reg1, reg2, offset, comment);
}

void emitSw(const char *reg1, const char *reg2, int offset, const char * comment){
	instType1("sw", reg1, reg2, offset, comment);
}
void emitLa(const char *reg, const char *name, const char * comment){
	char label[MAXTOKENLEN + 2];
	sprintf(label, "_%s", name);

	instType3("la", reg, label, comment);
}
/*slt*/
void emitSlt(const char *reg1, const char *reg2, const char* reg3, const char * comment){
	instType2("slt", reg1, reg2, reg3, comment);
}
/*beqz*/
void emitBeqz(const char *reg, const char *labelname, int labelnum, const char * comment){
	char label[MAXTOKENLEN + 12];
	sprintf(label, "%s%d", labelname, labelnum);

	instType3("beqz", reg, label, comment);
}
/*j, jr, jal*/
void emitJ(const char *labelname, int labelnum, const char * comment){
	char label[MAXTOKENLEN + 12];
	sprintf(label, "%s%d", labelname, labelnum);

	instType4("j", label, comment);
}
void emitJr(const char *reg, const char * comment){
	instType4("jr", reg, comment);
}
void emitJal(const char *name, const char * comment){
	char label[MAXTOKENLEN + 2];

	if(strcmp(name, "main") == 0) sprintf(label, "main");
	else sprintf(label, "_%s", name);

	instType4("jal", label, comment);
}

void emitPush(const char *reg)
{
    emitAddi("$sp", "$sp", -RSIZE, "push 1/2");
    emitSw(reg, "$sp", 0, "push 2/2");
}

void emitPop(const char *reg)
{
	emitLw(reg, "$sp", 0, "pop 1/2");
	emitAddi("$sp", "$sp", RSIZE, "pop 2/2");
}


