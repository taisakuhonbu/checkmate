#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpng16/png.h>

int weave(
	FILE *fp,
	int w, int h,
	int twill0, int twill1,
	int flip,
	int render,
	int blend,
	unsigned char *warp,
	unsigned char *weft)
{
	int error_code = 0;
	
	++error_code;
	if(w != 64)
	{
		return error_code;
	}
	++error_code;
	if(h != 64)
	{
		return error_code;
	}
	++error_code;
	while(twill0 != twill1)
	{
		if(twill0 == 1 && twill1 == 3)
		{
			break;
		}
		if(twill0 == 3 && twill1 == 1)
		{
			break;
		}
		return error_code;
	}
	++error_code;
	do
	{
		if(twill0 == 1)
		{
			break;
		}
		if(twill0 == 2)
		{
			break;
		}
		if(twill0 == 4)
		{
			break;
		}
		if(twill0 == 8)
		{
			break;
		}
		return error_code;
	} while(0 == 1);
	++error_code;
	do
	{
		if(flip == '0')
		{
			break;
		}
		if(flip == 'h')
		{
			break;
		}
		if(flip == 'v')
		{
			break;
		}
		if(flip == 'w')
		{
			break;
		}
		return error_code;
	} while(0 == 1);
	++error_code;
	do
	{
		if(render == '0')
		{
			break;
		}
		if(render == '1')
		{
			break;
		}
		if(render == '2')
		{	
			break;
		}
		return error_code;
	} while(0 == 1);
	++error_code;
	do
	{
		if(blend == 'A')
		{
			break;
		}
		if(blend == 'B')
		{
			break;
		}
		if(blend == 'C')
		{
			break;
		}
		return error_code;
	} while(0 == 1);
	
	int x, y;
	if(flip != '0')
	{
		unsigned char *t;
		t = (unsigned char *)malloc(w * 4 * 2);
		if(flip != 'v')
		{
			for(x = 0; x < w; x++)
			{
				t[x * 4 + 0] = warp[x * 4 + 0];
				t[(2 * w - x - 1) * 4 + 0] = warp[x * 4 + 0];
				t[x * 4 + 1] = warp[x * 4 + 1];
				t[(2 * w - x - 1) * 4 + 1] = warp[x * 4 + 1];
				t[x * 4 + 2] = warp[x * 4 + 2];
				t[(2 * w - x - 1) * 4 + 2] = warp[x * 4 + 2];
			}
		}
		else
		{
			for(x = 0; x < w; x++)
			{
				t[x * 4 + 0] = warp[x * 4 + 0];
				t[(w + x) * 4 + 0] = warp[x * 4 + 0];
				t[x * 4 + 1] = warp[x * 4 + 1];
				t[(w + x) * 4 + 1] = warp[x * 4 + 1];
				t[x * 4 + 2] = warp[x * 4 + 2];
				t[(w + x) * 4 + 2] = warp[x * 4 + 2];
			}
		}
		w *= 2;
		free(warp);
		warp = t;
		t = (unsigned char *)malloc(h * 4 * 2);
		if(flip != 'h')
		{
			for(y = 0; y < h; y++)
			{
				t[y * 4 + 0] = warp[y * 4 + 0];
				t[(2 * h - y - 1) * 4 + 0] = warp[y * 4 + 0];
				t[y * 4 + 1] = warp[y * 4 + 1];
				t[(2 * h - y - 1) * 4 + 1] = warp[y * 4 + 1];
				t[y * 4 + 2] = warp[y * 4 + 2];
				t[(2 * h - y - 1) * 4 + 2] = warp[y * 4 + 2];
			}
		}
		else
		{
			for(y = 0; y < h; y++)
			{
				t[y * 4 + 0] = warp[y * 4 + 0];
				t[(h + y) * 4 + 0] = warp[y * 4 + 0];
				t[y * 4 + 1] = warp[y * 4 + 1];
				t[(h + y) * 4 + 1] = warp[y * 4 + 1];
				t[y * 4 + 2] = warp[y * 4 + 2];
				t[(h + y) * 4 + 2] = warp[y * 4 + 2];
			}
		}
		h *= 2;
		free(weft);
		weft = t;
	}
	
	int dw, dh;
	if(1==0){}
	else if(render == '0')
	{
		dw = w;
		dh = h;
	}
	else if(render == '1')
	{
		dw = w << 1;
		dh = h << 1;
	}
	else if(render == '2')
	{
		dw = w << 2;
		dh = h << 2;
	}
	
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info_ptr = png_create_info_struct(png_ptr);
	png_init_io(png_ptr, fp);
	png_set_filter(png_ptr, 0, PNG_FILTER_NONE);
	png_set_IHDR(png_ptr, info_ptr,
		     dw, dh, 8, PNG_COLOR_TYPE_RGB,
		     PNG_INTERLACE_NONE,
		     PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	png_write_info(png_ptr, info_ptr);

	png_byte *row_pointers[dh];
	
	int thread;
	if(render == '0')
	{
		if(2==1){}
		else if(blend == 'A')
		{
			for(y = 0; y < h; y++)
			{
				row_pointers[y] = (png_byte*)malloc(sizeof(png_byte) * 3 * w);
				for(x = 0; x < w; x++)
				{
					thread = (y + x) % (twill0 + twill1) - twill0;
					if(thread < 0)
					{
						row_pointers[y][x*3 + 0] = weft[y * 4 + 0];
						row_pointers[y][x*3 + 1] = weft[y * 4 + 1];
						row_pointers[y][x*3 + 2] = weft[y * 4 + 2];
					}
					else
					{
						row_pointers[y][x*3 + 0] = warp[x * 4 + 0];
						row_pointers[y][x*3 + 1] = warp[x * 4 + 1];
						row_pointers[y][x*3 + 2] = warp[x * 4 + 2];
					}
				}
			}
		}
		else if(blend == 'B')
		{
			for(y = 0; y < h; y++)
			{
				row_pointers[y] = (png_byte*)malloc(sizeof(png_byte) * 3 * w);
				for(x = 0; x < w; x++)
				{
					thread = (y + x) % (twill0 + twill1) - twill0;
					if(thread < 0)
					{
						row_pointers[y][x*3 + 0] = ((weft[y * 4 + 0] << 1) + weft[y * 4 + 0] + warp[x * 4 + 0]) >> 2;
						row_pointers[y][x*3 + 1] = ((weft[y * 4 + 1] << 1) + weft[y * 4 + 1] + warp[x * 4 + 1]) >> 2;
						row_pointers[y][x*3 + 2] = ((weft[y * 4 + 2] << 1) + weft[y * 4 + 2] + warp[x * 4 + 2]) >> 2;
					}
					else
					{
						row_pointers[y][x*3 + 0] = ((warp[x * 4 + 0] << 1) + warp[x * 4 + 0] + weft[y * 4 + 0]) >> 2;
						row_pointers[y][x*3 + 1] = ((warp[x * 4 + 1] << 1) + warp[x * 4 + 1] + weft[y * 4 + 1]) >> 2;
						row_pointers[y][x*3 + 2] = ((warp[x * 4 + 2] << 1) + warp[x * 4 + 2] + weft[y * 4 + 2]) >> 2;
					}
				}
			}
		}
		else if(blend == 'C')
		{
			for(y = 0; y < h; y++)
			{
				row_pointers[y] = (png_byte*)malloc(sizeof(png_byte) * 3 * w);
				for(x = 0; x < w; x++)
				{
					row_pointers[y][x*3 + 0] = (warp[x * 4 + 0] + weft[y * 4 + 0]) >> 1;
					row_pointers[y][x*3 + 1] = (warp[x * 4 + 1] + weft[y * 4 + 1]) >> 1;
					row_pointers[y][x*3 + 2] = (warp[x * 4 + 2] + weft[y * 4 + 2]) >> 1;
				}
			}
		}
		else
		{
			printf(
				"Content-type: text/xml\n"
				"\n"
				"<Check status=\"Error8\"></Check>"
				);
			return 0;
		}
	}
	else
	{
		struct rgb
		{
			int r;
			int g;
			int b;
		};
		struct rgb * src = (struct rgb*)malloc(sizeof(struct rgb) * w * h);
		if(2==3){}
		else if(blend == 'A')
		{
			for(y = 0; y < h; y++)
			{
				for(x = 0; x < w; x++)
				{
					thread = (y + x) % (twill0 + twill1) - twill0;
					if(thread < 0)
					{
						src[y * w + x].r = weft[y * 4 + 0];
						src[y * w + x].g = weft[y * 4 + 1];
						src[y * w + x].b = weft[y * 4 + 2];
					}
					else
					{
						src[y * w + x].r = warp[x * 4 + 0];
						src[y * w + x].g = warp[x * 4 + 1];
						src[y * w + x].b = warp[x * 4 + 2];
					}
				}
			}
		}
		else if(blend == 'B')
		{
			for(y = 0; y < h; y++)
			{
				for(x = 0; x < w; x++)
				{
					thread = (y + x) % (twill0 + twill1) - twill0;
					if(thread < 0)
					{
						src[y * w + x].r = ((weft[y * 4 + 0] << 1) + weft[y * 4 + 0] + warp[x * 4 + 0]) >> 2;
						src[y * w + x].g = ((weft[y * 4 + 1] << 1) + weft[y * 4 + 1] + warp[x * 4 + 1]) >> 2;
						src[y * w + x].b = ((weft[y * 4 + 2] << 1) + weft[y * 4 + 2] + warp[x * 4 + 2]) >> 2;
					}
					else
					{
						src[y * w + x].r = ((warp[x * 4 + 0] << 1) + warp[x * 4 + 0] + weft[y * 4 + 0]) >> 2;
						src[y * w + x].g = ((warp[x * 4 + 1] << 1) + warp[x * 4 + 1] + weft[y * 4 + 1]) >> 2;
						src[y * w + x].b = ((warp[x * 4 + 2] << 1) + warp[x * 4 + 2] + weft[y * 4 + 2]) >> 2;
					}
				}
			}
		}
		else if(blend == 'C')
		{
			for(y = 0; y < h; y++)
			{
				for(x = 0; x < w; x++)
				{
					src[y * w + x].r = (warp[x * 4 + 0] + weft[y * 4 + 0]) >> 1;
					src[y * w + x].g = (warp[x * 4 + 1] + weft[y * 4 + 1]) >> 1;
					src[y * w + x].b = (warp[x * 4 + 2] + weft[y * 4 + 2]) >> 1;
				}
			}
		}
		
		struct rgb * temp = (struct rgb*)malloc(sizeof(struct rgb) * dw * dh);
		memset(temp, 0, sizeof(struct rgb) * dw * dh);

		int dx, dy;
		if(4==5){}
		else if(render == '1')
		{
			for(y = 0; y < h; y++)
			{
				for(x = 0; x < w; x++)
				{
					dx = x - y;
					dy = x + y;
					
					dx += dw;
					dy += dh;
					dx %= dw;
					dy %= dh;
					temp[dy * dw + dx].r = src[y * w + x].r << 2;
					temp[dy * dw + dx].g = src[y * w + x].g << 2;
					temp[dy * dw + dx].b = src[y * w + x].b << 2;
					
					dx++;
					dx += dw;
					dx %= dw;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;
					dx--;

					dx--;
					dx += dw;
					dx %= dw;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;
					dx++;
					
					dx += dw;
					dx %= dw;
					dy++;
					dy += dh;
					dy %= dh;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;
					dy--;
					
					dy--;
					dy += dh;
					dy %= dh;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;
					
					dx = x - y + w;
					dy = x + y + h;
					
					dx += dw;
					dy += dh;
					dx %= dw;
					dy %= dh;
					temp[dy * dw + dx].r = src[y * w + x].r << 2;
					temp[dy * dw + dx].g = src[y * w + x].g << 2;
					temp[dy * dw + dx].b = src[y * w + x].b << 2;
					
					dx++;
					dx += dw;
					dx %= dw;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;
					dx--;

					dx--;
					dx += dw;
					dx %= dw;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;
					dx++;
					
					dx += dw;
					dx %= dw;
					dy++;
					dy += dh;
					dy %= dh;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;
					dy--;
					
					dy--;
					dy += dh;
					dy %= dh;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;
				}
			}
			for(dy = 0; dy < dh; dy++)
			{
				row_pointers[dy] = (png_byte*)malloc(sizeof(png_byte) * 3 * dw);
				for(dx = 0; dx < dw; dx++)
				{
					row_pointers[dy][dx * 3 + 0] = temp[dy * dw + dx].r >> 2;
					row_pointers[dy][dx * 3 + 1] = temp[dy * dw + dx].g >> 2;
					row_pointers[dy][dx * 3 + 2] = temp[dy * dw + dx].b >> 2;
				}
			}
		}
		else if(render == '2')
		{
			for(y = 0; y < h; y++)
			{
				for(x = 0; x < w; x++)
				{
					dx = x - y;
					dy = x + y;
					dx <<= 1;
					dy <<= 1;
					
					dx += dw;
					dy += dh;
					dx %= dw;
					dy %= dh;
					temp[dy * dw + dx].r = src[y * w + x].r << 1;
					temp[dy * dw + dx].g = src[y * w + x].g << 1;
					temp[dy * dw + dx].b = src[y * w + x].b << 1;

					dy--;
					dy += dh;
					dy %= dh;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;

					dx++;
					dx += dw;
					dx %= dw;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;

					dy++;
					dy += dh;
					dy %= dh;
					temp[dy * dw + dx].r += src[y * w + x].r << 1;
					temp[dy * dw + dx].g += src[y * w + x].g << 1;
					temp[dy * dw + dx].b += src[y * w + x].b << 1;

					dx++;
					dx += dw;
					dx %= dw;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;

					dy++;
					dy += dh;
					dy %= dh;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;

					dx--;
					dx += dw;
					dx %= dw;
					temp[dy * dw + dx].r += src[y * w + x].r << 1;
					temp[dy * dw + dx].g += src[y * w + x].g << 1;
					temp[dy * dw + dx].b += src[y * w + x].b << 1;

					dy++;
					dy += dh;
					dy %= dh;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;

					dx--;
					dx += dw;
					dx %= dw;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;

					dy--;
					dy += dh;
					dy %= dh;
					temp[dy * dw + dx].r += src[y * w + x].r << 1;
					temp[dy * dw + dx].g += src[y * w + x].g << 1;
					temp[dy * dw + dx].b += src[y * w + x].b << 1;

					dx--;
					dx += dw;
					dx %= dw;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;

					dy--;
					dy += dh;
					dy %= dh;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;
					
					dx = x - y + w;
					dy = x + y + h;
					dx <<= 1;
					dy <<= 1;
					
					dx += dw;
					dy += dh;
					dx %= dw;
					dy %= dh;
					temp[dy * dw + dx].r = src[y * w + x].r << 1;
					temp[dy * dw + dx].g = src[y * w + x].g << 1;
					temp[dy * dw + dx].b = src[y * w + x].b << 1;

					dy--;
					dy += dh;
					dy %= dh;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;

					dx++;
					dx += dw;
					dx %= dw;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;

					dy++;
					dy += dh;
					dy %= dh;
					temp[dy * dw + dx].r += src[y * w + x].r << 1;
					temp[dy * dw + dx].g += src[y * w + x].g << 1;
					temp[dy * dw + dx].b += src[y * w + x].b << 1;

					dx++;
					dx += dw;
					dx %= dw;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;

					dy++;
					dy += dh;
					dy %= dh;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;

					dx--;
					dx += dw;
					dx %= dw;
					temp[dy * dw + dx].r += src[y * w + x].r << 1;
					temp[dy * dw + dx].g += src[y * w + x].g << 1;
					temp[dy * dw + dx].b += src[y * w + x].b << 1;

					dy++;
					dy += dh;
					dy %= dh;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;

					dx--;
					dx += dw;
					dx %= dw;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;

					dy--;
					dy += dh;
					dy %= dh;
					temp[dy * dw + dx].r += src[y * w + x].r << 1;
					temp[dy * dw + dx].g += src[y * w + x].g << 1;
					temp[dy * dw + dx].b += src[y * w + x].b << 1;

					dx--;
					dx += dw;
					dx %= dw;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;

					dy--;
					dy += dh;
					dy %= dh;
					temp[dy * dw + dx].r += src[y * w + x].r;
					temp[dy * dw + dx].g += src[y * w + x].g;
					temp[dy * dw + dx].b += src[y * w + x].b;
				}
			}
			for(dy = 0; dy < dh; dy++)
			{
				row_pointers[dy] = (png_byte*)malloc(sizeof(png_byte) * 3 * dw);
				for(dx = 0; dx < dw; dx++)
				{
					row_pointers[dy][dx * 3 + 0] = temp[dy * dw + dx].r >> 1;
					row_pointers[dy][dx * 3 + 1] = temp[dy * dw + dx].g >> 1;
					row_pointers[dy][dx * 3 + 2] = temp[dy * dw + dx].b >> 1;
				}
			}
		}
		free(temp);
		free(src);
	}
	
	png_write_rows(png_ptr, row_pointers, dh);
	png_write_end(png_ptr, info_ptr);

	png_destroy_write_struct(&png_ptr, &info_ptr);
	
	return 0;
}
