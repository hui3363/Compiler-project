#ifndef _CODE_H_
#define _CODE_H_

void emitString(const char *name, const char *str);
void emitArrayDecl(const char *name, int size);
void emitVarDecl(const char *name);
void emitSyscall(const char *comment);

void emitComment(const char *comment);
void emitLabel(const char *labelname, int labelnum);
void emitFuncName(const char *name);
void emitSection(const char *name);

void emitAddi(const char *reg1, const char *reg2, int value, const char * comment);
void emitAdd(const char *reg1, const char *reg2, const char *reg3, const char * comment);
void emitSub(const char *reg1, const char *reg2, const char *reg3, const char * comment);
void emitMul(const char *reg1, const char *reg2, const char *reg3, const char * comment);
void emitDiv(const char *reg1, const char *reg2, const char *reg3, const char * comment);
void emitXori(const char *reg1, const char *reg2, int value, const char * comment);
void emitOr(const char *reg1, const char *reg2, const char *reg3, const char * comment);
void emitMove(const char *reg1, const char *reg2, const char * comment);
void emitLi(const char *reg, int value, const char * comment);
void emitLw(const char *reg1, const char *reg2, int offset, const char * comment);
void emitSw(const char *reg1, const char *reg2, int offset, const char * comment);
void emitLa(const char *reg, const char *name, const char * comment);
void emitSlt(const char *reg1, const char *reg2, const char* reg3, const char * comment);
void emitBeqz(const char *reg, const char *labelname, int labelnum, const char * comment);
void emitJ(const char *labelname, int labelnum, const char * comment);
void emitJr(const char *reg, const char * comment);
void emitJal(const char *name, const char * comment);


void emitPush(const char *reg);
void emitPop(const char *reg);

#endif
