#include "opcode.h" 
#include "util.h" 

INLINE
char* OpcodeName(OPCODE opcode) {
	static char* codes[] = {
		"EXT", 	
		"SET", 	
		"ADD", 	
		"SUB", 	
		"MUL", 	
		"DIV", 	
		"MOD", 	
		"SHL", 	
		"SHR", 	
		"AND", 	
		"BOR", 	
		"XOR", 	
		"IFE", 	
		"IFN", 	
		"IFG", 	
		"IFB"
	};

	return codes[opcode]; 
}

