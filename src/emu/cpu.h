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
	void* Ram;  
} Cpu; 

void cpuExecute(Cpu* cpu); 

#endif 
