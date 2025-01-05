#include <stdio.h>
#include <stdlib.h>
#include <libpng16/png.h>


int weave(
	FILE * fp,
	int w, int h,
	int twill0, int twill1,
	int flip,
	int render,
	int blend,
	unsigned char *warp,
	unsigned char *weft);

int is_data(const char *s)
{
	int exor = 0;
	exor |= s[0] ^ 'd';
	exor |= s[1] ^ 'a';
	exor |= s[2] ^ 't';
	exor |= s[3] ^ 'a';
	return exor == 0;
}

enum
{
	NAME_DATA,

	NORMAL_CHAR,
	HI4BITS,
	LO4BITS,

	END_OF_ENUM
};

int hex2int(char c)
{
	if(c <= '9')
	{
		return c - '0';
	}
	if(c >= 'a')
	{
		return c - 'a' + 10;
	}
	return c - 'A' + 10;
}

int main(int argc, char *argv[])
{
	int clen;
	const char *env;
	int m;

	int i;
	char *content;
	int c;
	char *param;
	char *data = "";
	int datlen = 11;
	char *p;
	
	FILE *fp;
	unsigned long length;
	unsigned char buf[2048];
	
	int mode = NAME_DATA;

	env = getenv("CONTENT_LENGTH");
	clen = atoi(env);
	
	content = (char*)malloc(sizeof(char) * (clen + 1));
	c = 0;
	p = content + c;
	for(m = 0; m < clen; m++)
	{
		i = fgetc(stdin);
		if(i < 0)
		{
			printf(
				"Content-type: text/xml\n"
				"\n"
				"<Check status=\"Error1\"/>"
				);
			return 0;
		}
		switch(mode)
		{
		case NAME_DATA:
			if(i == '=')
			{
				if(is_data(p))
				{
					mode = NORMAL_CHAR;
					data = content + c;
					p = data;
				}
				else
				{
					printf(
						"Content-type: text/xml\n"
						"\n"
						"<Check status=\"Error2\"/>"
						);
					return 0;
				}
			}
			else
			{
				content[c] = (char)i; c++;
			}
			break;

		case NORMAL_CHAR:
			if(i == '%')
			{
				mode = HI4BITS;
			}
			else if(i == '&')
			{
				content[c] = '\0'; c++;
				if(p != NULL)
				{
					datlen = c;
				}
				p = content + c;
				mode = NAME_DATA;
			}
			else if(i == '+')
			{
				content[c] = ' '; c++;
			}
			else
			{
				content[c] = (char)i; c++;
			}
			break;

		case HI4BITS:
			content[c] = hex2int(i) << 4;
			mode = LO4BITS;
			break;

		case LO4BITS:
			mode = NORMAL_CHAR;
			content[c] |= hex2int(i); c++;
			break;
		}
	}
	if(p != NULL)
	{
		datlen = content + c - p;
	}
	content[c] = '\0';
	if(data == NULL)
	{
		printf(
			"Content-type: text/xml\n"
			"\n"
			"<Check status=\"Error4\"/>"
			);
		return 0;
	}

	if(datlen < 12)
	{
		printf(
			"Content-type: text/xml\n"
			"\n"
			"<Check status=\"Error5\"/>"
			);
		return 0;
	}
	
#define DECODE(v_, d_) v_ = (*d_ - 65) << 4; d_++; v_ |= (*d_ - 65); d_++;
	if(*data != '3')
	{
		printf(
			"Content-type: text/xml\n"
			"\n"
			"<Check status=\"Error6\"/>"
			);
		return 0;
	}
	data++;
	
	int w, h;
	DECODE(w, data); w++;
	DECODE(h, data); h++;
	
	int twill0, twill1;
	DECODE(twill0, data);
	DECODE(twill1, data);
	
	int flip;
	flip = *data; data++;

	int render;
	render = *data; data++;

	int blend;
	blend = *data; data++;
	
	unsigned char *warp;
	unsigned char *weft;
	warp = (unsigned char*)malloc(4 * w);
	weft = (unsigned char*)malloc(4 * h);
	int x, y;
	for(x = 0; x < w; x++)
	{
		DECODE(warp[x * 4 + 0], data);
		DECODE(warp[x * 4 + 1], data);
		DECODE(warp[x * 4 + 2], data);
		DECODE(warp[x * 4 + 3], data);
	}
	for(y = 0; y < h; y++)
	{
		DECODE(weft[y * 4 + 0], data);
		DECODE(weft[y * 4 + 1], data);
		DECODE(weft[y * 4 + 2], data);
		DECODE(weft[y * 4 + 3], data);
	}
	
	fp = tmpfile();
	if(weave(fp,
		 w, h,
		 twill0, twill1,
		 flip,
		 render,
		 blend,
		 warp,
		 weft) != 0)
	{
		printf("Content-type: text/xml\n"
		       "\n"
		       "<Checker status=\"error\"/>\n");
		return 0;
	}
	
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

	return 0;
}
