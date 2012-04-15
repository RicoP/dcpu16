#ifndef UTIL_H
#define UTIL_H 

#include <stdlib.h>
#include <stdio.h> 
#include "cpu.h"

void CpuInfo(Cpu* cpu); 
Cpu* cpuGlobal; 

#define ERROR(str, ...) \
	fprintf(stderr, "[ERROR] " str "\n",##__VA_ARGS__ ); \
	CpuInfo(cpuGlobal); \
	exit(1);  

#define INFO(str, ...) \
	fprintf(stderr, "[INFO] " str "\n",##__VA_ARGS__ );   


#define INLINE inline 

#endif 
