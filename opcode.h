#ifndef OPCODE_H 
#define OPCODE_H 

typedef enum {
	//0x00-0x07: register (A, B, C, X, Y, Z, I or J, in that order)
	A = 0, 
	B, 
	C, 
	X, 
	Y, 
	Z, 
	I, 
	J, 
    //0x08-0x0f: [register]
	READ_A, 
	READ_B, 
	READ_C, 
	READ_X, 
	READ_Y, 
	READ_Z, 
	READ_I, 
	READ_J, 
    //0x10-0x17: [next word + register]
	NEXT_A, 
	NEXT_B, 
	NEXT_C, 
	NEXT_X, 
	NEXT_Y, 
	NEXT_Z, 
	NEXT_I, 
	NEXT_J, 	
    //0x18: POP / [SP++]
	POP, 
    //0x19: PEEK / [SP]
	PEEK, 
    //0x1a: PUSH / [--SP]
	PUSH, 
    //0x1b: SP
	SP,
    //0x1c: PC
	PC, 
    //0x1d: O
	O, 
    //0x1e: [next word]
	READ_NEXT, 
    //0x1f: next word (literal)
	NEXT, 
    //0x20-0x3f: literal value 0x00-0x1f (literal)	
	LITERAL
} OPVALUE;

typedef enum {
	EXT = 0, //Extended
	SET, 
	ADD, 
 	SUB, 
	MUL,
	DIV,
	MOD,
	SHL,
	SHR,
	AND,
	BOR,
	XOR,
	IFE,
	IFN,
	IFG,
	IFB
} OPCODE; 

char* OpcodeName(OPCODE opcode); 

#endif 
