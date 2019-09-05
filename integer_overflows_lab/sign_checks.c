#include <stdio.h>
#include <stdint.h>

// *****************************************************************
// be aware, not only does the casting change the interpretation,
// so does the printf function!!! The format string matters!
// Makes the most sense to step through it in gdb/assembly
// *****************************************************************
void main()
{
	char a = 0xff;
	printf("signed char a is: %d\n", a);

	// smaller signed to larger signed
	int b = a;
	printf("signed char a to signed int b is: %d\n", b);

	// signed to unsigned
	unsigned char c = a;
	printf("signed char a to unsigned char c is: %u\n", c);

	// signed to larger unsighed
	unsigned int d = a;
	printf("signed char a to unsigned int d is: %u\n", d);

	unsigned char e = 0xff;
	printf("unsigned char e is: %u\n", e);

	// smaller unsigned to larger unsigned
	unsigned int f = e;
	printf("unsigned char e to unsigned int f is: %u\n", f);

	// unsigned to signed
	char g = e ;
	printf("unsigned char e to signed char g is: %d\n", g);

	// smaller unsigned to larger signed
	int h = e;
	printf("unsigned char e to signed int h is: %d\n", h);


	// larger to smaller
	int64_t i = 0xf000000000000005;
	printf("signed 64-bit int i is: %lld\n", i);

	short j = i;
	printf("signed 64-bit int i to signed short j is: %d\n", j);

	unsigned short k = i;
	printf("signed 64-bit int i to unsigned short k is: %d\n", k);
}
