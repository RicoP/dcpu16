#include <stdio.h>
#include <stdlib.h>
#include "cpu.h" 
#include "opcode.h"
#include "util.h" 

static char* getTargetName(OPVALUE opcode); 
static void notImplemented(Cpu* cpu, OPCODE op);
static word* getTarget(Cpu* cpu, OPVALUE value); 

static void cpuEXT(Cpu* cpu, word w); 
static void cpuSET(Cpu* cpu, word w); 
static void cpuADD(Cpu* cpu, word w); 
static void cpuSUB(Cpu* cpu, word w); 
static void cpuMUL(Cpu* cpu, word w); 
static void cpuDIV(Cpu* cpu, word w); 
static void cpuMOD(Cpu* cpu, word w); 
static void cpuSHL(Cpu* cpu, word w); 
static void cpuSHR(Cpu* cpu, word w); 
static void cpuAND(Cpu* cpu, word w); 
static void cpuBOR(Cpu* cpu, word w); 
static void cpuXOR(Cpu* cpu, word w); 
static void cpuIFE(Cpu* cpu, word w); 
static void cpuIFN(Cpu* cpu, word w); 
static void cpuIFG(Cpu* cpu, word w); 
static void cpuIFB(Cpu* cpu, word w); 

#define VALUE_A(opcode) ((OPVALUE)((opcode >> 4)  & 0x3F))
#define VALUE_B(opcode) ((OPVALUE)((opcode >> 10) & 0x3F))
 
#define OPCODE(VAR) \
	case VAR: \
	INFO("%x " #VAR " %s, %s", operation, getTargetName(VALUE_A(operation)), getTargetName(VALUE_B(operation))); \
	cpu##VAR(cpu, operation); \
	break; 

INLINE
void cpuExecute(Cpu* cpu) {
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

INLINE static
void cpuEXT(Cpu* cpu, word w) { notImplemented(cpu, EXT); } 

INLINE static
void cpuSET(Cpu* cpu, word w) { 
	word* dest = getTarget(cpu, VALUE_A(w)); 
	word* orig = getTarget(cpu, VALUE_B(w)); 
 
	*dest = *orig; 
} 

INLINE static
void cpuADD(Cpu* cpu, word w) { 
	word* dest = getTarget(cpu, VALUE_A(w)); 
	word* orig = getTarget(cpu, VALUE_B(w)); 
 
	*dest += *orig; 
}

INLINE static
void cpuSUB(Cpu* cpu, word w) { 
	word* dest = getTarget(cpu, VALUE_A(w)); 
	word* orig = getTarget(cpu, VALUE_B(w)); 
 
	*dest -= *orig; 
}

INLINE static
void cpuMUL(Cpu* cpu, word w) { 
	word* dest = getTarget(cpu, VALUE_A(w)); 
	word* orig = getTarget(cpu, VALUE_B(w)); 
 
	*dest *= *orig; 
}

INLINE static
void cpuDIV(Cpu* cpu, word w) { 
	word* dest = getTarget(cpu, VALUE_A(w)); 
	word* orig = getTarget(cpu, VALUE_B(w)); 
 
	*dest /= *orig; 
}

INLINE static
void cpuMOD(Cpu* cpu, word w) { 
	word* dest = getTarget(cpu, VALUE_A(w)); 
	word* orig = getTarget(cpu, VALUE_B(w)); 
 
	*dest %= *orig; 
}

INLINE static
void cpuSHL(Cpu* cpu, word w) { 
	word* dest = getTarget(cpu, VALUE_A(w)); 
	word* orig = getTarget(cpu, VALUE_B(w)); 
 
	*dest = *dest << *orig; 
}

INLINE static
void cpuSHR(Cpu* cpu, word w) { 
	word* dest = getTarget(cpu, VALUE_A(w)); 
	word* orig = getTarget(cpu, VALUE_B(w)); 
 
	*dest = *dest >> *orig; 
}

INLINE static
void cpuAND(Cpu* cpu, word w) { 
	word* dest = getTarget(cpu, VALUE_A(w)); 
	word* orig = getTarget(cpu, VALUE_B(w)); 
 
	*dest &= *orig; 
}

INLINE static
void cpuBOR(Cpu* cpu, word w) { 
	word* dest = getTarget(cpu, VALUE_A(w)); 
	word* orig = getTarget(cpu, VALUE_B(w)); 
 
	*dest |= *orig; 
}

INLINE static
void cpuXOR(Cpu* cpu, word w) { 
	word* dest = getTarget(cpu, VALUE_A(w)); 
	word* orig = getTarget(cpu, VALUE_B(w)); 
 
	*dest ^= *orig; 
}

INLINE static
void cpuIFE(Cpu* cpu, word w) { notImplemented(cpu, IFE); } 

INLINE static
void cpuIFN(Cpu* cpu, word w) { notImplemented(cpu, IFN); } 

INLINE static
void cpuIFG(Cpu* cpu, word w) { notImplemented(cpu, IFG); } 

INLINE static
void cpuIFB(Cpu* cpu, word w) { notImplemented(cpu, IFB); } 

#define GET_REG_VAL(REG) \
	case REG: \
	return &cpu->REG; 

#define GET_REG_READ(REG) \
	case READ_ ## REG: \
	return &wram[cpu->REG]; 

#define GET_NEXT_WORD_PLUS_REG(REG) \
	case NEXT_ ## REG: \
	return &wram[cpu->REG + (cpu->PC++)]; 

#define GET_REG(REG) \
	GET_REG_VAL(REG); \
	GET_REG_READ(REG); \
	GET_NEXT_WORD_PLUS_REG(REG); 

INLINE static
word* getTarget(Cpu* cpu, OPVALUE value) {
	static word literal = 0; 
	word* wram = (word*)cpu->Ram; 
	switch(value) {
		GET_REG(A); 
		GET_REG(B); 
		GET_REG(C); 
		GET_REG(X); 
		GET_REG(Y); 
		GET_REG(Z); 
		GET_REG(I); 
		GET_REG(J); 

		case POP: 
			return &wram[cpu->SP++]; 

		case PEEK: 
			return &wram[cpu->SP]; 

		case PUSH: 
			return &wram[--cpu->SP]; 

		case O: 
			return &cpu->O;

		case NEXT_WORD: 
			return &wram[wram[cpu->PC++]]; 

		case NEXT_LITERAL: 
			literal = wram[cpu->PC++]; 
			return &literal; 
	
		default: 
			ERROR("can't compute Target 0x%02x.", value); 
	}
}

#undef GET_REG_VAL
#undef GET_REG_READ
#undef GET_NEXT_WORD_PLUS_REG

#define GET_REG_VAL(REG) \
	case REG: \
	return #REG; 

#define GET_REG_READ(REG) \
	case READ_ ## REG: \
	return "[" #REG "]"; 

#define GET_NEXT_WORD_PLUS_REG(REG) \
	case NEXT_ ## REG: \
	return "[" #REG " + WORD]"; 

#define GET_REG(REG) \
	GET_REG_VAL(REG); \
	GET_REG_READ(REG); \
	GET_NEXT_WORD_PLUS_REG(REG); 

INLINE static
char* getTargetName(OPVALUE value) {
	switch(value) {
		GET_REG(A); 
		GET_REG(B); 
		GET_REG(C); 
		GET_REG(X); 
		GET_REG(Y); 
		GET_REG(Z); 
		GET_REG(I); 
		GET_REG(J); 

		case POP: 
			return "POP"; 

		case PEEK: 
			return "PEEK"; 

		case PUSH: 
			return "PUSH"; 

		case O: 
			return "O";

		case NEXT_WORD: 
			return "NEXT_WORD"; 

		case NEXT_LITERAL: 
			return "NEXT_LITERAL"; 
	
		default: 
			ERROR("can't compute Target Name 0x%02x.", value); 
	}
}

static 
void notImplemented(Cpu* cpu, OPCODE op) {
	CpuInfo(cpu); 
	ERROR("Opcode %s has not been implemented jet.", OpcodeName(op));\
}

#undef VALUE_A
#undef VALUE_B
#undef OPCODE

#undef GET_REG_VAL
#undef GET_REG_READ
#undef GET_NEXT_WORD_PLUS_REG
#undef GET_REG
