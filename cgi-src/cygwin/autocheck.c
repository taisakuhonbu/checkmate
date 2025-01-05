#include "autocheck.h"

#include <stdio.h>
#include <stdlib.h>

#include "SHA1Sum.h"

struct _Color
{
	int r;
	int g;
	int b;
};
typedef struct _Color Color;

static unsigned char hash[32];

static int skip;
static int shift;
static int mask[4];

static Color rgb[64];
static int nColors;
static int iCurrent;

static int gray_level = 0;
static int gray_apply = 0;
static int dark_level = 0;
static int dark_apply = 0;
static int light_level = 0;
static int light_apply = 0;

enum _Selecting
{
	SAME,
	EXIST,
	OTHER,
	DIFFERENT,
};
typedef _Selecting Selecting;
static Selecting prev, pp;

static int domain[32];

extern void set_complex()
{
	domain[ 0] = 144;
	domain[ 1] = 208;
	domain[ 2] = 232;
	domain[ 3] = 256;

	domain[ 4] = 220;
	domain[ 5] = 232;
	domain[ 6] = 244;
	domain[ 7] = 256;

	domain[ 8] = 160;
	domain[ 9] = 224;
	domain[10] = 240;
	domain[11] = 256;

	domain[12] = 220;
	domain[13] = 232;
	domain[14] = 244;
	domain[15] = 256;

	domain[16] = 144;
	domain[17] = 208;
	domain[18] = 232;
	domain[19] = 256;

	domain[20] = 220;
	domain[21] = 232;
	domain[22] = 244;
	domain[23] = 256;

	domain[24] = 48;
	domain[25] = 232;
	domain[26] = 244;
	domain[27] = 256;

	domain[28] = 220;
	domain[29] = 232;
	domain[30] = 244;
	domain[31] = 256;
}

extern void set_simple()
{
	domain[ 0] = 184;
	domain[ 1] = 216;
	domain[ 2] = 224;
	domain[ 3] = 256;

	domain[ 4] = 220;
	domain[ 5] = 232;
	domain[ 6] = 244;
	domain[ 7] = 256;

	domain[ 8] = 200;
	domain[ 9] = 232;
	domain[10] = 240;
	domain[11] = 256;

	domain[12] = 220;
	domain[13] = 232;
	domain[14] = 244;
	domain[15] = 256;

	domain[16] = 200;
	domain[17] = 240;
	domain[18] = 248;
	domain[19] = 256;

	domain[20] = 200;
	domain[21] = 240;
	domain[22] = 248;
	domain[23] = 256;

	domain[24] = 48;
	domain[25] = 240;
	domain[26] = 248;
	domain[27] = 256;

	domain[28] = 224;
	domain[29] = 240;
	domain[30] = 248;
	domain[31] = 256;
}

extern void set_hash()
{
	int seed;

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
	seed = 0;
	seed |= hash[14] << 16;
	seed |= hash[15] << 8;
	seed |= hash[16];

	mask[0] = hash[17];
	mask[1] = hash[18];
	mask[2] = hash[19];
	mask[3] = 0;
	srand(seed);

	pp = OTHER;
	prev = SAME;
}

static void mild(Color *c)
{
	if(gray_apply <= 0 && dark_apply <= 0 && light_apply <= 0) return;

	float r = c->r / 254.999;
	float g = c->g / 254.999;
	float b = c->b / 254.999;
	float max = r > g ? r : g;
	max = max > b ? max : b;
	float min = r < g ? r : g;
	min = min < b ? min : b;
	float h = max - min;
	if (h > 0.0f)
	{
		if (max == r)
		{
			h = (g - b) / h;
			if (h < 0.0f)
			{
				h += 6.0f;
			}
		}
		else if (max == g)
		{
			h = 2.0f + (b - r) / h;
		}
		else
		{
			h = 4.0f + (r - g) / h;
		}
	}
	h /= 6.0f;
	float s = (max - min);
	if (max != 0.0f)
	s /= max;
	float v = max;

	if(gray_apply > 0)
	{
		s = s * (255.0 - gray_level) / 255.0;
		gray_apply--;
	}
	if(dark_apply > 0)
	{
		v = v * (255.0 - dark_level) / 255.0;
		dark_apply--;
	}
	if(light_apply > 0)
	{
		v += (1.0 - v) * (light_level / 255.0);
		light_apply--;
	}

	r = v;
	g = v;
	b = v;
	if (s > 0.0f)
	{
		h *= 6.0f;
		int i = (int) h;
		float f = h - (float) i;
		switch (i)
		{
		default:
		case 0:
			g *= 1 - s * (1 - f);
			b *= 1 - s;
			break;
		case 1:
			r *= 1 - s * f;
			b *= 1 - s;
			break;
		case 2:
			r *= 1 - s;
			b *= 1 - s * (1 - f);
			break;
		case 3:
			r *= 1 - s;
			g *= 1 - s * f;
			break;
		case 4:
			r *= 1 - s * (1 - f);
			g *= 1 - s;
			break;
		case 5:
			g *= 1 - s;
			b *= 1 - s * f;
			break;
		}
	}
	c->r = (int)(r * 255.001);
	c->g = (int)(g * 255.001);
	c->b = (int)(b * 255.001);
}

extern void set_mild(int g, int nGray, int d, int nDark, int l, int nLight)
{
	gray_level = g;
	gray_apply = nGray;
	dark_level = d;
	dark_apply = nDark;
	light_level = l;
	light_apply = nLight;

	mild(rgb + iCurrent);
	mild(rgb + (iCurrent ^ 3));
	mild(rgb + (iCurrent ^ 2));
	mild(rgb + (iCurrent ^ 1));
}

static int get_random()
{
	static int m = 0;
	m++;
	int n;
	int i;
	for(i = 0; i < skip; i++)
	{
		rand();
	}
	n = rand();

	n = n >> shift;
	n &= 0xFF;
	return n ^ mask[m & 3];
}

static double distance(Color a, Color b)
{
	int d = 0;
	d += (a.r - b.r) * (a.r - b.r);
	d += (a.g - b.g) * (a.g - b.g);
	d += (a.b - b.b) * (a.b - b.b);
	return (double)d / 255.0 / 255.0;
}

extern void get_color(Color *current)
{
	int n;
	int bounds[4];

	switch(pp)
	{
	case SAME:
		switch(prev)
		{
		case SAME:
			bounds[0] = domain[ 0];
			bounds[1] = domain[ 1];
			bounds[2] = domain[ 2];
			bounds[3] = domain[ 3];
			break;
		case EXIST:
		case OTHER:
		case DIFFERENT:
			bounds[0] = domain[ 4];
			bounds[1] = domain[ 5];
			bounds[2] = domain[ 6];
			bounds[3] = domain[ 7];
			break;
		}
		break;
	case EXIST:
		switch(prev)
		{
		case SAME:
			bounds[0] = domain[ 8];
			bounds[1] = domain[ 9];
			bounds[2] = domain[10];
			bounds[3] = domain[11];
			break;
		case EXIST:
		case OTHER:
		case DIFFERENT:
			bounds[0] = domain[12];
			bounds[1] = domain[13];
			bounds[2] = domain[14];
			bounds[3] = domain[15];
			break;
		}
		break;
	case OTHER:
		switch(prev)
		{
		case SAME:
			bounds[0] = domain[16];
			bounds[1] = domain[17];
			bounds[2] = domain[18];
			bounds[3] = domain[19];
			break;
		case EXIST:
		case OTHER:
		case DIFFERENT:
			bounds[0] = domain[20];
			bounds[1] = domain[21];
			bounds[2] = domain[22];
			bounds[3] = domain[23];
			break;
		}
		break;
	case DIFFERENT:
		switch(prev)
		{
		case SAME:
			bounds[0] = domain[24];
			bounds[1] = domain[25];
			bounds[2] = domain[26];
			bounds[3] = domain[27];
			break;
		case EXIST:
		case OTHER:
		case DIFFERENT:
			bounds[0] = domain[28];
			bounds[1] = domain[29];
			bounds[2] = domain[30];
			bounds[3] = domain[31];
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
		mild(rgb + iCurrent);
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
	current->r = rgb[iCurrent].r;
	current->g = rgb[iCurrent].g;
	current->b = rgb[iCurrent].b;
}

void get_query()
{
	const char *env;
	SHA1Sum sha1;
	
	env = getenv("QUERY_STRING");
	unsigned int n = 0;
	while(env[n] != '\0') n++;
	
	sha1.Reset();
	sha1.Input((uint8_t*)env, n);
	sha1.Result((uint8_t*)hash);

	set_hash();
}

void get_colors(unsigned char data[4*64])
{
	int i;

	for(i = 0; i < 64; i++)
	{
		Color c;
		get_color(&c);

		data[i * 4 + 0] = c.r;
		data[i * 4 + 1] = c.g;
		data[i * 4 + 2] = c.b;
		data[i * 4 + 3] = 0xFF;
		i++;
		data[i * 4 + 0] = c.r;
		data[i * 4 + 1] = c.g;
		data[i * 4 + 2] = c.b;
		data[i * 4 + 3] = 0xFF;
	}
}

extern int weave(
	FILE *fp,
	int w, int h,
	int twill0, int twill1,
	int flip,
	int render,
	int blend,
	unsigned char *warp,
	unsigned char *weft);

void response(unsigned char data[4*64])
{
	FILE *fp;
	unsigned long length;
	unsigned char buf[2048];
	
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

	size_t n;
	while((n = fread(buf, 1, 2048, fp)) != 0)
	{
		fwrite(buf, 1, n, stdout);
	}
	
	fclose(fp);
}