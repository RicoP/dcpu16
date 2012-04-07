#ifndef UTIL_H
#define UTIL_H 

#include <stdlib.h>
#include <stdio.h> 

#define ERROR(str, ...) \
	fprintf(stderr, "ERROR: " str "\n",##__VA_ARGS__ ); \
	exit(1);  

#define INLINE inline 

#endif 
