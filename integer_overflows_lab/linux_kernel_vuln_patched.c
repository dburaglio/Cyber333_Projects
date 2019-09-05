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
	unsigned long BUF_PAGE_SIZE  = 4080;	// using a constant page size of 4080 bytes, a normal 4kB page
	unsigned long nr_pages;			// how many pages we will have once we resize to the requested size
	
	printf("Ring buffer size %lu (%lx) was requested, current page size is %lu\n", size, size, BUF_PAGE_SIZE);

	printf("Divide and round operands: (%lu + %lu - 1)/%lu\n", size, BUF_PAGE_SIZE, BUF_PAGE_SIZE);
	nr_pages = DIV_ROUND_UP(size, BUF_PAGE_SIZE);
	printf("The requested size results in %lu pages\n", nr_pages);

	/* we need a minimum of two pages */
	printf("Checking if the number of pages is less than the required 2 pages (4080*2 = 8160 bytes)\n", nr_pages);
	if (nr_pages < 2)
	{
		nr_pages = 2;
		printf("Need a minimum of two pages, new size: %lu\n", size);
	}
	else
	{
		printf("looks like we have at least 2 pages, all good\n");
	}
	
	size = nr_pages * BUF_PAGE_SIZE;
	
	printf("new num pages in ring buffer: %lu\n", nr_pages);
	return size; 	// the final size after scaling, min total size needed

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