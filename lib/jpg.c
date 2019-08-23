#include "common.h"
#include "jpeglib.h"

static int lcd;
static int lcd_w, lcd_h, lcd_bpp, screen_size;
static struct fb_var_screeninfo vsinfo;
static char * lcdmem;
static bool first = true;

void display(char *jpg)
{
	int jpgfd = open(jpg, O_RDONLY);

	int jpg_size = lseek(jpgfd, 0L, SEEK_END);
	lseek(jpgfd, 0L, SEEK_SET);

	char *jpg_buffer = calloc(1, jpg_size);

	int total = jpg_size;
	while(total > 0)
	{
		int n = read(jpgfd, jpg_buffer+(
		jpg_size-total), total);
		total -= n;
	}

	close(jpgfd);

	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);


	jpeg_mem_src(&cinfo, jpg_buffer, jpg_size);

	int ret = jpeg_read_header(&cinfo, true);

	if(ret != 1)
	{
		fprintf(stderr,"[%d]:jepg_read_head failed:""%s\n",
			 __LINE__, strerror(errno));
		exit(0);
	}

	jpeg_start_decompress(&cinfo);

	cinfo.output_width;
	cinfo.output_height;
	cinfo.output_components;

	int row_stride = cinfo.output_width * cinfo.output_components;
	
	unsigned char * rgb_buffer = calloc(1, row_stride*cinfo.output_height);

	int line = 0;
	while(cinfo.output_scanline < cinfo.output_height)
	{
		unsigned char *buffer_array[1];
	        buffer_array[0] = rgb_buffer +
	        (cinfo.output_scanline)*row_stride;
		jpeg_read_scanlines(&cinfo, buffer_array, 1);	
	}
	

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	free(jpg_buffer);

	if(first)
	{
		lcd = open("/dev/fb0", O_RDWR);

		bzero(&vsinfo, sizeof(vsinfo));
		ioctl(lcd, FBIOGET_VSCREENINFO, &vsinfo);

		lcd_w = vsinfo.xres;
		lcd_h = vsinfo.yres;
		lcd_bpp = vsinfo.bits_per_pixel;
		screen_size = lcd_w * lcd_h * lcd_bpp/8;

		lcdmem = mmap(NULL, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, lcd, 0);
		first = false;
	}
 	bzero(lcdmem, screen_size);

	int red_offset = vsinfo.red.offset;
	int green_offset = vsinfo.green.offset;
	int blue_offset = vsinfo.blue.offset;
	
	int w = (lcd_w)<cinfo.output_width?(lcd_w):cinfo.output_width;
	int h = (lcd_h)<cinfo.output_height?(lcd_h):cinfo.output_height;
	
	char *rgbtmp = rgb_buffer;
	
	char *lcdtmp;
	//小的放中间
	if(lcd_w >= cinfo.output_width && 
	lcd_h >=cinfo.output_height)
	{
		
		int x = (lcd_w-cinfo.output_width)/2;
		int y = (lcd_h-cinfo.output_height)/2;
		lcdtmp = lcdmem+(y*800+x)*4;
		
		for(int j=0; j<h; j++)
		{
			for(int i=0; i<w; i++)
			{
				memcpy(lcdtmp+4*i+red_offset/8,
					rgbtmp+3*i+0,1);		
				memcpy(lcdtmp+4*i+green_offset/8,
					rgbtmp+3*i+1,1);		
				memcpy(lcdtmp+4*i+blue_offset/8,
					rgbtmp+3*i+2,1);		
			}
			lcdtmp += (lcd_w*lcd_bpp/8);
			rgbtmp += (row_stride);
		}
	}
	else
	{		
		lcdtmp = lcdmem;
		
		for(int j=0; j<h; j++)
		{
			for(int i=0; i<w; i++)
			{
				memcpy(lcdtmp+4*i+red_offset/8,
					rgbtmp+3*i+0,1);		
				memcpy(lcdtmp+4*i+green_offset/8,
					rgbtmp+3*i+1,1);		
				memcpy(lcdtmp+4*i+blue_offset/8,
					rgbtmp+3*i+2,1);		
			}
			lcdtmp += (lcd_w*lcd_bpp/8);
			rgbtmp += (row_stride);
		}	
	}
	free(rgb_buffer);
}

void freelcd(void)
{
	if(!first)
	{
		close(lcd);
		munmap(lcdmem, screen_size);
	}
	first = true;
}
