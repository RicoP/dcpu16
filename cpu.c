#include <stdio.h>
#include <stdlib.h>
#include "cpu.h" 
#include "opcode.h"

#define NOT_IMPLEMENTED(VAR) \
	fprintf(stderr, "Opcode %s has not been implemented jet.\n", OpcodeName(VAR));\
	exit(1); 

#define OPCODE(VAR) \
	case VAR: \
	cpu##VAR(cpu, w); \
	break; 

void cpuExecute(Cpu* cpu) {
	word w; 
	for(;;) {
		w = cpu->RAM[cpu->PC++]; 
		switch(w & 0xF) { //w & 0xF == opcode
			OPCODE(EXT)	
			OPCODE(SET)	
			OPCODE(ADD)	
			OPCODE(SUB)	
			OPCODE(MUL)	
			OPCODE(DIV)	
			OPCODE(MOD)	
			OPCODE(SHL)	
			OPCODE(SHR)	
			OPCODE(AND)	
			OPCODE(BOR)	
			OPCODE(XOR)	
			OPCODE(IFE)	
			OPCODE(IFN)	
			OPCODE(IFG)	
			OPCODE(IFB)				
			default: 
				fprintf(stderr, "Opcode %d undefined!\n", w & 0xF); 
				exit(1); 
		}
	}
}

void cpuEXT(Cpu* cpu, word w) { NOT_IMPLEMENTED(EXT); } 
void cpuSET(Cpu* cpu, word w) { NOT_IMPLEMENTED(SET); } 
void cpuADD(Cpu* cpu, word w) { NOT_IMPLEMENTED(ADD); } 
void cpuSUB(Cpu* cpu, word w) { NOT_IMPLEMENTED(SUB); } 
void cpuMUL(Cpu* cpu, word w) { NOT_IMPLEMENTED(MUL); } 
void cpuDIV(Cpu* cpu, word w) { NOT_IMPLEMENTED(DIV); } 
void cpuMOD(Cpu* cpu, word w) { NOT_IMPLEMENTED(MOD); } 
void cpuSHL(Cpu* cpu, word w) { NOT_IMPLEMENTED(SHL); } 
void cpuSHR(Cpu* cpu, word w) { NOT_IMPLEMENTED(SHR); } 
void cpuAND(Cpu* cpu, word w) { NOT_IMPLEMENTED(AND); } 
void cpuBOR(Cpu* cpu, word w) { NOT_IMPLEMENTED(BOR); } 
void cpuXOR(Cpu* cpu, word w) { NOT_IMPLEMENTED(XOR); } 
void cpuIFE(Cpu* cpu, word w) { NOT_IMPLEMENTED(IFE); } 
void cpuIFN(Cpu* cpu, word w) { NOT_IMPLEMENTED(IFN); } 
void cpuIFG(Cpu* cpu, word w) { NOT_IMPLEMENTED(IFG); } 
void cpuIFB(Cpu* cpu, word w) { NOT_IMPLEMENTED(IFB); } 

