#include <stdio.h>
#include <string.h>

void main(int argc, char** argv){
	const unsigned long MAX_LEN = 0x7fff;	//the max positive size of a short value
	printf("This program tells you if your input is small enough to fit into a char array with length no longer than %ld\n", MAX_LEN);

	if (argc < 2) {
		printf("usage: size_check <string input here>\n");
		return;
	}

	// dangerous because size could be larger than max value of short
	short len = strlen(argv[1]); // strlen returns a size_t, which is an unsigned type!

	printf("The size of your input was: %d\n", len);

	// this comparison will force len to a long value
	if (len < MAX_LEN) {
		printf("Length %d is less than max length %ld\n", len, MAX_LEN);
	}
	else {
		printf("Length %d was too long compared to max %ld!\n", len, MAX_LEN);
	}
}
