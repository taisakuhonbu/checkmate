#include <stdio.h>
#include <stdlib.h>

#include <mysql/mysql.h>
#include "SHA1Sum.h"

int weave(
	FILE *fp,
	int w, int h,
	int twill0, int twill1,
	int flip,
	int render,
	int blend,
	unsigned char *warp,
	unsigned char *weft);

struct _Color
{
	int r;
	int g;
	int b;
};
typedef struct _Color Color;

static int total = 0;

static int skip;
static int shift;
static int mask[4];
int get_random()
{
	static int m = 0;
	m++;
	int n;
	int i;
	for(i = 0; i < skip; i++)
	{
		total++;
		rand();
	}
	n = rand();

	n = n >> shift;
	n &= 0xFF;
	return n ^ mask[m & 3];
}

double distance(Color a, Color b)
{
	int d = 0;
	d += (a.r - b.r) * (a.r - b.r);
	d += (a.g - b.g) * (a.g - b.g);
	d += (a.b - b.b) * (a.b - b.b);
	return (double)d / 255.0 / 255.0;
}

bool inc(char *v, char *count, int i)
{
	if(count[i] == '\0')
	{
		v[i] = '\0';
		return true;
	}
	else
	{
		if(inc(v, count, i + 1))
		{
			if(count[i] == '9')
			{
				v[i] = '0';
				return true;
			}
			else
			{
				v[i] = count[i] + 1;
				return false;
			}
		}
		v[i] = count[i];
		return false;
	}
	
}

void countup(char *v, char *count)
{
	if(inc(v, count, 0))
	{
		v[-1] = '1';
	}
}

char passwd[] = {102,97,113,45,110,108,106,37,127,101,114,99,};
char * get_password()
{
  passwd[0] ^= 11;
  passwd[1] ^= 0;
  passwd[2] ^= 8;
  passwd[3] ^= 0;
  passwd[4] ^= 15;
  passwd[5] ^= 2;
  passwd[6] ^= 14;
  passwd[7] ^= 8;
  passwd[8] ^= 6;
  passwd[9] ^= 10;
  passwd[10] ^= 7;
  passwd[11] ^= 99;
  return passwd;
}

int main(int argc, char*argv[])
{
	const char *env;
	int n;
	char *filename;
	int i;
	FILE *fp;
	unsigned char hash[32];
	int seed;
	Color rgb[64];
	int nColors;
	int iCurrent;
	unsigned char data[64 * 4];
	SHA1Sum sha1;
	int length;
	unsigned char buf[2048];
	MYSQL_RES *result;
	MYSQL_ROW row;
	MYSQL *connection;
	MYSQL mysql;
	char update[] = "UPDATE `auto` SET `image`=                      ";
	
	env = getenv("QUERY_STRING");
	n = 0;
	while(env[n] != '\0') n++;
	
	sha1.Reset();
	sha1.Input((uint8_t*)env, n);
	sha1.Result((uint8_t*)hash);

	nColors = 0;
	rgb[0].r = hash[0];
	rgb[0].g = hash[1];
	rgb[0].b = hash[2];
	rgb[1].r = hash[3];
	rgb[1].g = hash[4];
	rgb[1].b = hash[5];
	rgb[2].r = hash[6];
	rgb[2].g = hash[7];
	rgb[2].b = hash[8];
	rgb[3].r = hash[0];
	rgb[3].g = hash[10];
	rgb[3].b = hash[11];
	nColors = 4;

	iCurrent = hash[12];
	iCurrent ^= hash[12] >> 2;
	iCurrent &= 0x3;
	skip = hash[12] >> 4;
	skip &= 0x7;
	shift = hash[12] >> 7;
	shift |= (hash[13] >> 6) & 0x3;
	seed <<= 24;
	seed |= hash[14] << 16;
	seed |= hash[15] << 8;
	seed |= hash[16];

	mask[0] = hash[17];
	mask[1] = hash[18];
	mask[2] = hash[19];
	mask[3] = 0;
	srand(seed);
	
	enum
	{
		SAME,
		EXIST,
		OTHER,
		DIFFERENT,
	} prev, pp;
	pp = OTHER;
	prev = SAME;
	int bounds[4];
	for(i = 0; i < 64; i++)
	{
		switch(pp)
		{
		case SAME:
			switch(prev)
			{
			case SAME:
				bounds[0] = 144;
				bounds[1] = 208;
				bounds[2] = 232;
				bounds[3] = 256;
				break;
			case EXIST:
			case OTHER:
			case DIFFERENT:
				bounds[0] = 220;
				bounds[1] = 232;
				bounds[2] = 244;
				bounds[3] = 256;
				break;
			}
			break;
		case EXIST:
			switch(prev)
			{
			case SAME:
				bounds[0] = 160;
				bounds[1] = 224;
				bounds[2] = 240;
				bounds[3] = 256;
				break;
			case EXIST:
			case OTHER:
			case DIFFERENT:
				bounds[0] = 220;
				bounds[1] = 232;
				bounds[2] = 244;
				bounds[3] = 256;
				break;
			}
			break;
		case OTHER:
			switch(prev)
			{
			case SAME:
				bounds[0] = 144;
				bounds[1] = 208;
				bounds[2] = 232;
				bounds[3] = 256;
				break;
			case EXIST:
			case OTHER:
			case DIFFERENT:
				bounds[0] = 220;
				bounds[1] = 232;
				bounds[2] = 244;
				bounds[3] = 256;
				break;
			}
		case DIFFERENT:
			switch(prev)
			{
			case SAME:
				bounds[0] = 48;
				bounds[1] = 232;
				bounds[2] = 244;
				bounds[3] = 256;
				break;
			case EXIST:
			case OTHER:
			case DIFFERENT:
				bounds[0] = 220;
				bounds[1] = 232;
				bounds[2] = 244;
				bounds[3] = 256;
				break;
			}
		}
		pp = prev;
		n = get_random();

		if(n < bounds[0])
		{
			prev = SAME;
		}
		else if(n < bounds[1])
		{
			iCurrent = n % nColors;
			prev = EXIST;
		}
		else if(n < bounds[2])
		{
			rgb[nColors].r = get_random();
			rgb[nColors].g = get_random();
			rgb[nColors].b = get_random();
			iCurrent = nColors;
			nColors++;
			prev = OTHER;
		}
		else
		{
			double d;
			Color c;
			double limit = 1.0101;
			do
			{
				limit /= 1.0025;
				c.r = get_random();
				c.g = get_random();
				c.b = get_random();
				d = distance(c, rgb[iCurrent]);
			} while(d < limit);
			rgb[nColors].r = c.r;
			rgb[nColors].g = c.g;
			rgb[nColors].b = c.b;
			iCurrent = nColors;
			nColors++;
			prev = DIFFERENT;
		}
		data[i * 4 + 0] = rgb[iCurrent].r ;
		data[i * 4 + 1] = rgb[iCurrent].g;
		data[i * 4 + 2] = rgb[iCurrent].b;
		data[i * 4 + 3] = 0xFF;
		i++;
		data[i * 4 + 0] = rgb[iCurrent].r ;
		data[i * 4 + 1] = rgb[iCurrent].g;
		data[i * 4 + 2] = rgb[iCurrent].b;
		data[i * 4 + 3] = 0xFF;
	}
	
	mysql_init(&mysql);
	connection = mysql_real_connect(&mysql, "localhost", "s", get_password(), "checker", 0, NULL, 0);
	mysql_query(connection, "LOCK TABLES `auto` WRITE");
	mysql_query(connection, "SELECT `image` FROM `auto`");
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);
	countup(update + 27, row[0]);
	mysql_query(connection, "UNLOCK TABLES");
	mysql_close(connection);
	
	fp = tmpfile();
	weave(fp,
		64, 64,
		1, 1,
		'0',
		'0',
		'A',
		data,
		data);
	length = ftell(fp);
	rewind(fp);
	
	printf("Content-type: image/png\x0d\x0a");
	printf("Content-length: %d\x0d\x0a", length);
	printf("\x0d\x0a");
	
	while((n = fread(buf, 1, 2048, fp)) != 0)
	{
		fwrite(buf, 1, n, stdout);
	}
	
	fclose(fp);

	return 0;
}
