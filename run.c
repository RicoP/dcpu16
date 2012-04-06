#include <stdio.h> 
#include <stdlib.h> 
#include "cpu.h"

void loadFileIntoRam(Cpu* cpu, char* filename); 
void printHelp(); 

int main(int argc, char **argv) {
	Cpu* cpu = NULL; 
	int i; 

	if(argc == 2) {
		cpu = (Cpu*) malloc(sizeof(Cpu)); 			
		cpu->RAM = (char*) malloc(RAM_BYTES); 

		for(i = 0; i != RAM_BYTES; i++) {
			cpu->RAM[i] = 0; 
		}
	

		loadFileIntoRam(cpu, argv[1]); 
		cpuExecute(cpu); 
		return 0; 
	}

	printf("Run the App like that:\n\t%s FILENAME\n", argv[0]); 

	return 0; 
}

inline 
void loadFileIntoRam(Cpu* cpu, char* filename) {
	FILE* f = NULL; 
	int fileLen = 0; 
	f = fopen(filename, "r+b");
	if(!f) {
		fprintf(stderr, "Unable to open file %s\n", filename);
		exit(1); 
	}

	//Filelength
	fseek(f, 0, SEEK_END);
	fileLen=ftell(f);
	fseek(f, 0, SEEK_SET);		

	if(fileLen > RAM_BYTES) {
		fprintf(stderr, "File %s to Large. (> %dk)\n", filename, RAM_BYTES / 1024);
		fclose(f); 
		exit(1); 
	}

	fread(cpu->RAM, fileLen, 1, f); 
	fclose(f); 
} 

