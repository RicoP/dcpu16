#include <stdio.h>
#include <stdlib.h>
#include "cpu.h" 
#include "opcode.h"
#include "util.h" 

#define VALUE_A(opcode) ((opcode >> 4)  & 0x3F)
#define VALUE_B(opcode) ((opcode >> 10) & 0x3F)
 
#define OPCODE(VAR) \
	case VAR: \
	cpu##VAR(cpu, operation); \
	break; 

INLINE void cpuExecute(Cpu* cpu) {
	word* words = (word*)cpu->Ram; 
	word operation = 0;
	for(;;) {
		operation = words[cpu->PC++]; 
		switch(operation & 0xF) {
			OPCODE(EXT);	
			OPCODE(SET);	
			OPCODE(ADD);	
			OPCODE(SUB);	
			OPCODE(MUL);
			OPCODE(DIV);
			OPCODE(MOD);
			OPCODE(SHL);
			OPCODE(SHR);
			OPCODE(AND);
			OPCODE(BOR);
			OPCODE(XOR);
			OPCODE(IFE);
			OPCODE(IFN);
			OPCODE(IFG);
			OPCODE(IFB);			
		}
	}
}


static void notImplemented(OPCODE op);
static word* getTarget(Cpu* cpu, OPVALUE value); 

INLINE void cpuEXT(Cpu* cpu, word w) { notImplemented(EXT); } 

INLINE void cpuSET(Cpu* cpu, word w) { 
	char a = VALUE_A(w); 
	char b = VALUE_B(w); 
	word* dest; 
	word* orig; 

	dest = getTarget(cpu, (OPVALUE) a); 

	printf("SET: w:%04x a:%d, b:%d\n", w, a, b); 
} 

INLINE void cpuADD(Cpu* cpu, word w) { notImplemented(ADD); } 
INLINE void cpuSUB(Cpu* cpu, word w) { notImplemented(SUB); } 
INLINE void cpuMUL(Cpu* cpu, word w) { notImplemented(MUL); } 
INLINE void cpuDIV(Cpu* cpu, word w) { notImplemented(DIV); } 
INLINE void cpuMOD(Cpu* cpu, word w) { notImplemented(MOD); } 
INLINE void cpuSHL(Cpu* cpu, word w) { notImplemented(SHL); } 
INLINE void cpuSHR(Cpu* cpu, word w) { notImplemented(SHR); } 
INLINE void cpuAND(Cpu* cpu, word w) { notImplemented(AND); } 
INLINE void cpuBOR(Cpu* cpu, word w) { notImplemented(BOR); } 
INLINE void cpuXOR(Cpu* cpu, word w) { notImplemented(XOR); } 
INLINE void cpuIFE(Cpu* cpu, word w) { notImplemented(IFE); } 
INLINE void cpuIFN(Cpu* cpu, word w) { notImplemented(IFN); } 
INLINE void cpuIFG(Cpu* cpu, word w) { notImplemented(IFG); } 
INLINE void cpuIFB(Cpu* cpu, word w) { notImplemented(IFB); } 

#define GET_REG(REG) case REG: return &cpu->REG; 
#define GET_REG_READ(REG) case READ_ ## REG: return (word*)(cpu->Ram + cpu->REG); 

INLINE static
word* getTarget(Cpu* cpu, OPVALUE value) {
	switch(value) {
		GET_REG(A); GET_REG(B); 
		GET_REG(C); GET_REG(X); 
		GET_REG(Y); GET_REG(Z); 
		GET_REG(I); GET_REG(J); 
		GET_REG_READ(A); GET_REG_READ(B); 
		GET_REG_READ(C); GET_REG_READ(X); 
		GET_REG_READ(Y); GET_REG_READ(Z); 
		GET_REG_READ(I); GET_REG_READ(J); 

	}
}

static 
void notImplemented(OPCODE op) {
	ERROR("Opcode %s has not been implemented jet.", OpcodeName(op));\
	exit(1); 
}

#undef VALUE_A 
#undef VALUE_B
#undef OPCODE 
