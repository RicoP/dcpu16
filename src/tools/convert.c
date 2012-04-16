#include <stdio.h>
#include <stdlib.h>

#define ERROR(str, ...) \
	fprintf(stderr, "ERROR: " str "\n",##__VA_ARGS__ ); \
	exit(1);  

 
char hex(char c) {
	if(c >= '0' && c <= '9') 
		return c - '0'; 

	if(c >= 'A' && c <= 'F') 
		return 10 + c - 'A'; 

	if(c >= 'a' && c <= 'f') 
		return 10 + c - 'a'; 

	ERROR("Char '%c' is no Hex digit.", c); 
	return 0; 
}

void convert(char* from, char* to) {
	FILE* f = NULL; 
	int fileLen = 0; 
	int toSize = 0; 
	char* buffer; 

	f = fopen(from, "r+t");
	if(!f) {
		ERROR("Unable to open file %s\n", from);
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

	toSize = fileLen >> 1; 

	buffer = (char*) calloc(1, toSize); 
	int i; 

	for(i = 0; i != toSize;i++) {
		char c; 
		fread(&c, 1, 1, f); 
		printf("%c", c); 

		char high = hex(c); 				
		fread(&c, 1, 1, f); 
		printf("%c", c); 

		char low = hex(c); 		

		buffer[i] = (char)(high << 4 | low); 
		printf(" %02x\n", buffer[i] & 0xFF); 
	}
	fclose(f); 

	f = fopen(to, "w+b"); 
	fwrite (buffer , 1 , toSize, f );
	fclose(f); 
} 

int main(int argc, char** argv) {
	if(argc != 3) {
		ERROR("%s FROM TO\n", argv[0]); 
	}

	convert(argv[1], argv[2]); 
	return 0; 	
}
