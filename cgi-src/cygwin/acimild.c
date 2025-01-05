#include <stdio.h>
#include <stdlib.h>

#include "autocheck.h"

int main(int argc, char*argv[])
{
	unsigned char data[4 * 64];

	get_query();

	set_simple();
	set_mild(125, 8, 0, 0, 225, 10);

	get_colors(data);

	response(data);

	return 0;
}