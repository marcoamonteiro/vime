#ifndef __UTIL_H
#define __UTIL_H

// An unsigned integer type for convenience.
typedef unsigned int uint;
typedef char *string;

// Allocate a new struct on the heap and return a pointer to it.
#define oalloc(type) malloc(sizeof(type))

#endif
