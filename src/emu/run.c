#include <stdio.h> 
#include <stdlib.h> 

#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>

#include "cpu.h"
#include "util.h"

void loadFileIntoRam(Cpu* cpu, char* filename); 

int main(int argc, char **argv) {
	Cpu* cpu = NULL; 
	char* path; 
	char ok; 

	#ifdef EMSCRIPTEN 
		ok = 1; 
		path = "notch.bin"; 
	#else
		ok = argc == 2; 
		path = argv[1];
	#endif

	if(ok) {
		cpu = cpuGet(); 
		loadFileIntoRam(cpu, path); 
		cpuExecute(cpu); 
		return 0; 
	}

	printf("Run the App like that:\n\t%s FILENAME\n", argv[0]);

	return 0; 
}
 
void loadFileIntoRam(Cpu* cpu, char* filename) {
	FILE* f = NULL; 
	int fileLen = 0; 
	int amountWords = 0; 
	word* words = (word*) cpu->Ram; 

	f = fopen(filename, "r+b");
	if(!f) {
		ERROR("Unable to open file %s\n", filename);
	}

	//Filelength
	fseek(f, 0, SEEK_END);
	fileLen=ftell(f);
	fseek(f, 0, SEEK_SET);		

	if(fileLen & 1) {
		fprintf(stderr, "File needs even amount of bytes.\n");
		fclose(f); 
		exit(1); 
	}

	if(fileLen > RAM_BYTES) {
		fprintf(stderr, "File %s to Large. (> %dk)\n", filename, RAM_BYTES >> 10);
		fclose(f); 
		exit(1); 
	}

	amountWords = fileLen >> 1; 

	while(amountWords) {
		fread(words, sizeof(word), 1, f); 
		*words = ntohs( *words );

		words++; 
		amountWords--; 
	}
	fclose(f); 
} 

