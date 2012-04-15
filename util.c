#include "util.h" 
#include "cpu.h" 
#include <stdio.h> 

void CpuInfo(Cpu* cpu) {
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
