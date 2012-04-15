#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdio.h> 
#include "cpu.h"

void cpuInfo(Cpu* cpu); 
Cpu* cpuGet(); 

#define ERROR(str, ...) \
	fprintf(stderr, "[ERROR] " str "\n",##__VA_ARGS__ ); \
	cpuInfo(cpuGet()); \
	exit(1);  

#define INFO(str, ...) \
	fprintf(stderr, "[INFO] " str "\n",##__VA_ARGS__ );   


#define INLINE inline 

#endif 
