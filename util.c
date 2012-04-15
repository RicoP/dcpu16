#include <stdio.h> 
#include "util.h" 
#include "cpu.h" 

void cpuInfo(Cpu* cpu) {
	printf(
		"CPU\n"
		"A:%04x "
		"B:%04x "
		"C:%04x "
		"X:%04x "
		"Y:%04x "
		"Z:%04x "
		"I:%04x "
		"J:%04x | "
		"PC:%04x "
		"SP:%04x "
		"O:%04x\n",
		cpu->A, 
		cpu->B, 
		cpu->C, 
		cpu->X, 
		cpu->Y, 
		cpu->Z, 
		cpu->I, 
		cpu->J, 

		cpu->PC, 
		cpu->SP, 
		cpu->O 
	);
}

static 
Cpu* allocCpu() {
	Cpu*cpu; 
	cpu = (Cpu*) malloc(sizeof(Cpu)); 			
	cpu->Ram = malloc(RAM_BYTES); 
	cpu->PC = 0; 
	cpu->SP = 0xFFFF; 	
	return cpu; 
} 

Cpu* cpuGet() {
	static Cpu* cpu = NULL;
	static char init = 0; 
	if(!init) {
		init = 1; 
		cpu = allocCpu(); 
	} 
	return cpu; 
}


