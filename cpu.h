#ifndef CPU_H 
#define CPU_H 

#define RAM_WORDS 0x10000
#define RAM_BYTES (RAM_WORDS * 2)

typedef unsigned short word; 

typedef struct {
	word A, B, C, X, Y, Z, I, J; 
	word PC;
	word SP; 
	word O; 
	char* RAM;  
} Cpu; 

void cpuExecute(Cpu* cpu); 

void cpuEXT(Cpu* cpu, word w); 
void cpuSET(Cpu* cpu, word w); 
void cpuADD(Cpu* cpu, word w); 
void cpuSUB(Cpu* cpu, word w); 
void cpuMUL(Cpu* cpu, word w); 
void cpuDIV(Cpu* cpu, word w); 
void cpuMOD(Cpu* cpu, word w); 
void cpuSHL(Cpu* cpu, word w); 
void cpuSHR(Cpu* cpu, word w); 
void cpuAND(Cpu* cpu, word w); 
void cpuBOR(Cpu* cpu, word w); 
void cpuXOR(Cpu* cpu, word w); 
void cpuIFE(Cpu* cpu, word w); 
void cpuIFN(Cpu* cpu, word w); 
void cpuIFG(Cpu* cpu, word w); 
void cpuIFB(Cpu* cpu, word w); 



#endif 
