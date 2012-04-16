#include <stdio.h>
#include <stdlib.h>
#include "cpu.h" 
#include "opcode.h"
#include "util.h" 

static char* getTargetName(OPVALUE opcode); 
static void notImplemented(Cpu* cpu, OPCODE op);
static word* getTarget(Cpu* cpu, OPVALUE value); 

static word peekOp(Cpu* cpu); 
static word opSize(word op); 
static word opValueSize(OPVALUE w); 

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
	for(;;) {
		word* words = (word*)cpu->Ram; 
		word operation = words[cpu->PC++]; 
		switch(operation & 0xF) {
			case EXT: 		
			INFO("%x EXT %s", operation, getTargetName(VALUE_A(operation))); 
			cpuEXT(cpu, operation); 
			break; 

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
void cpuEXT(Cpu* cpu, word w) {
	if(w == 0) {
		ERROR("BREAK"); 
	}	

	if( VALUE_A(w) != 1 ) {
		ERROR("Unknown EXT Opcode %x.", w); 
	}

	OPVALUE a = (OPVALUE)(w >> 10); 

	word* ram = (word*)cpu->Ram; 

	//Push Address of next Instruction 
	ram[--cpu->SP] = cpu->PC + opValueSize(a); 	

	word* target = getTarget(cpu, a);

	cpu->PC = *target; 
} 

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
 
	cpu->O = *dest > 0xFFFF - *orig ? 1 : 0; 

	*dest += *orig; 
}

INLINE static
void cpuSUB(Cpu* cpu, word w) { 
	word* dest = getTarget(cpu, VALUE_A(w)); 
	word* orig = getTarget(cpu, VALUE_B(w)); 
 
	cpu->O = *dest < *orig ? 0xFFFF : 0; 

	*dest -= *orig; 
}

INLINE static
void cpuMUL(Cpu* cpu, word w) { 
	word* dest = getTarget(cpu, VALUE_A(w)); 
	word* orig = getTarget(cpu, VALUE_B(w)); 
 
	cpu->O = ((*dest * *orig)>>16)&0xFFFF; 

	*dest *= *orig; 
}

INLINE static
void cpuDIV(Cpu* cpu, word w) { 
	word* dest = getTarget(cpu, VALUE_A(w)); 
	word* orig = getTarget(cpu, VALUE_B(w)); 
 
	cpu->O = (( (*dest<<16) / *orig))&0xFFFF; 

	*dest /= *orig; 
}

INLINE static
void cpuMOD(Cpu* cpu, word w) { 
	word* dest = getTarget(cpu, VALUE_A(w)); 
	word* orig = getTarget(cpu, VALUE_B(w)); 
 
	if(*orig) 
		*dest %= *orig; 
	else
		*dest = 0; 
}

INLINE static
void cpuSHL(Cpu* cpu, word w) { 
	word* dest = getTarget(cpu, VALUE_A(w)); 
	word* orig = getTarget(cpu, VALUE_B(w)); 

	cpu->O = ((*dest << *orig) >> 16) & 0xFFFF; 
 
	*dest = *dest << *orig; 
}

INLINE static
void cpuSHR(Cpu* cpu, word w) { 
	word* dest = getTarget(cpu, VALUE_A(w)); 
	word* orig = getTarget(cpu, VALUE_B(w)); 
 
	cpu->O = ((*dest << 16) >> *orig) & 0xFFFF; 

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
void cpuIFE(Cpu* cpu, word w) {
	word* dest = getTarget(cpu, VALUE_A(w)); 
	word* orig = getTarget(cpu, VALUE_B(w)); 

	if(*dest == *orig) {
		return; 
	} 

	word op = peekOp(cpu); 
	word size = opSize(op); 
	cpu->PC += size; 	
} 

INLINE static
void cpuIFN(Cpu* cpu, word w) {
	word* dest = getTarget(cpu, VALUE_A(w)); 
	word* orig = getTarget(cpu, VALUE_B(w)); 

	if(*dest != *orig) {
		return; 
	} 

	word op = peekOp(cpu); 
	word size = opSize(op); 
	cpu->PC += size; 	
} 

INLINE static
void cpuIFG(Cpu* cpu, word w) {
	word* dest = getTarget(cpu, VALUE_A(w)); 
	word* orig = getTarget(cpu, VALUE_B(w)); 

	if(*dest > *orig) {
		return; 
	} 

	word op = peekOp(cpu); 
	word size = opSize(op); 
	cpu->PC += size; 	
}

INLINE static
void cpuIFB(Cpu* cpu, word w) {
	word* dest = getTarget(cpu, VALUE_A(w)); 
	word* orig = getTarget(cpu, VALUE_B(w)); 

	if(*dest & *orig) {
		return; 
	} 

	word op = peekOp(cpu); 
	word size = opSize(op); 
	cpu->PC += size; 	
}

INLINE static 
word peekOp(Cpu* cpu) {
	return ((word*)cpu->Ram)[cpu->PC];
}

INLINE static 
word opValueSize(OPVALUE w) {
	if(w >= 0x10 && w <= 0x17 || w == 0x1e || w == 0x1f) 
		return 1; 
	return 0; 
}

INLINE static 
word opSize(word op) {
	OPVALUE A = VALUE_A(op); 
	OPVALUE B = VALUE_B(op); 

	return opValueSize(A) + opValueSize(B) + 1; 
}

#define CASE_REG_VAL(REG) \
	case REG:            \
	return &cpu->REG; 

#define CASE_REG_READ(REG) \
	case READ_ ## REG:    \
	return &wram[cpu->REG]; 

#define CASE_NEXT_WORD_PLUS_REG(REG) \
	case NEXT_ ## REG:              \
	return &wram[wram[cpu->REG + cpu->PC++]]; 

#define CASE_REG(REG)   \
	CASE_REG_VAL(REG);  \
	CASE_REG_READ(REG); \
	CASE_NEXT_WORD_PLUS_REG(REG); 

INLINE static
word* getTarget(Cpu* cpu, OPVALUE value) {
	static word literal = 0; 
	word* wram = (word*)cpu->Ram; 
	switch(value) {
		CASE_REG(A); 
		CASE_REG(B); 
		CASE_REG(C); 
		CASE_REG(X); 
		CASE_REG(Y); 
		CASE_REG(Z); 
		CASE_REG(I); 
		CASE_REG(J); 
	
		case PC: 
			return &cpu->PC; 
	
		case SP:
			return &cpu->SP; 

		case O: 
			return &cpu->O;

		case POP: 
			return &wram[cpu->SP++]; 

		case PEEK: 
			return &wram[cpu->SP]; 

		case PUSH: 
			return &wram[--cpu->SP]; 

		case NEXT_WORD: 
			return &wram[wram[cpu->PC++]]; 

		case NEXT_LITERAL: 
			literal = wram[cpu->PC++]; 
			return &literal; 
	
		default: // 0x0 - 0x1F
			literal = value - LITERAL; 
			return &literal;  
	}
}

#undef CASE_REG_VAL
#undef CASE_REG_READ
#undef CASE_NEXT_WORD_PLUS_REG

#define CASE_REG_VAL(REG) \
	case REG:             \
	return #REG; 

#define CASE_REG_READ(REG) \
	case READ_ ## REG:     \
	return "[" #REG "]"; 

#define CASE_NEXT_WORD_PLUS_REG(REG) \
	case NEXT_ ## REG:               \
	return "[" #REG " + WORD]"; 

#define CASE_REG(REG)   \
	CASE_REG_VAL(REG);  \
	CASE_REG_READ(REG); \
	CASE_NEXT_WORD_PLUS_REG(REG); 

INLINE static
char* getTargetName(OPVALUE value) {
	switch(value) {
		CASE_REG(A); 
		CASE_REG(B); 
		CASE_REG(C); 
		CASE_REG(X); 
		CASE_REG(Y); 
		CASE_REG(Z); 
		CASE_REG(I); 
		CASE_REG(J); 

		case O: 
			return "O";
	
		case PC: 
			return "PC"; 
	
		case SP: 
			return "SP"; 

		case POP: 
			return "POP"; 

		case PEEK: 
			return "PEEK"; 

		case PUSH: 
			return "PUSH"; 

		case NEXT_WORD: 
			return "NEXT_WORD"; 

		case NEXT_LITERAL: 
			return "NEXT_LITERAL"; 
	
		default: 
			return "LITERAL";  
	}
}

static 
void notImplemented(Cpu* cpu, OPCODE op) {
	cpuInfo(cpu); 
	ERROR("Opcode %s has not been implemented jet.", OpcodeName(op));\
}

#undef VALUE_A
#undef VALUE_B
#undef OPCODE

#undef CASE_REG_VAL
#undef CASE_REG_READ
#undef CASE_NEXT_WORD_PLUS_REG
#undef CASE_REG
