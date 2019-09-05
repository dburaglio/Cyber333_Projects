#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// from linux kernel macro https://elixir.bootlin.com/linux/v4.6/source/tools/include/linux/kernel.h#L9
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

/**
 * edited signficiantly to just show vuln
 * original code from linxu kernel
 * ring_buffer_resize - resize the ring buffer
 * @buffer: the buffer to resize.
 * @size: the new size.
 * @cpu_id: the cpu buffer to resize
 *
 * Minimum size is 2 * BUF_PAGE_SIZE.
 *
 * Returns 0 on success and < 0 on failure.
 * vuln from https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/commit/?id=59643d1535eb220668692a5359de22545af579f6
 * https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2016-9754
 */
int ring_buffer_resize(unsigned long size)
{
	unsigned long BUF_PAGE_SIZE  = 4080;	// using a constant page size of 4080 bytes
	unsigned long nr_pages;			// how many pages we will have once we resize to the requested size
	
	printf("Ring buffer size %lu (%lx) was requested, current page size is %lu\n", size, size, BUF_PAGE_SIZE);

	printf("Divide and round operands: (%lu + %lu - 1)/%lu\n", size, BUF_PAGE_SIZE, BUF_PAGE_SIZE);
	size = DIV_ROUND_UP(size, BUF_PAGE_SIZE);
	printf("The requested size results in %lu pages\n", size);
	
	size *= BUF_PAGE_SIZE;
	printf("The total size required is: requested size %lu\n", size);

	/* we need a minimum of two pages */
	printf("Checking to see if the newly required size %lu is less than the required 2 pages (4080*2 = 8160 bytes)\n", size);
	if (size < BUF_PAGE_SIZE * 2)
	{
		size = BUF_PAGE_SIZE * 2;
		printf("Need a minimum of two pages, new size: %lu\n", size);
	}
	else
	{
		printf("looks like we have more than 2 pages, all good\n");
	}

	printf("Calculating the required number of pages again in case it just changed with the <2 pages size check\n");
		
	nr_pages = DIV_ROUND_UP(size, BUF_PAGE_SIZE);	// the error is here, no need to call this twice after the size (at least 2) was just checked above and isn't checked again

	printf("Divide and round operands: (%lu + %lu - 1)/%lu = %lu\n", size, BUF_PAGE_SIZE, BUF_PAGE_SIZE, nr_pages);
		
	printf("new num pages in ring buffer: %lu\n", nr_pages);
	
	return nr_pages * BUF_PAGE_SIZE; 	// the final size after scaling, min total size needed

}

void main(int argc, char** argv)
{
	if (argc < 2){
		printf("Please supply test page size as arg\n");
		return;
	}

	unsigned long test_size = 0;
	sscanf(argv[1], "%lu", &test_size);

	unsigned long result = ring_buffer_resize(test_size);

	printf("Requested buffer size %lu requires %lu bytes of memory\n", test_size, result);
}