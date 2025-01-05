#include <stdio.h>
#include <stdlib.h>

#include "autocheck.h"

int main(int argc, char*argv[])
{
	unsigned char data[4 * 64];
	
	/* QUERY_STRING から乱数シードを作ります */
	get_query();

	/* COMPLEX な作り方 */
	set_complex();

	/* 画像ビットデータを取り出します */
	get_colors(data);

	/* HTTP Response */
	response(data);

	return 0;
}
