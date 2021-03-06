// fbtest.c
// writes a test pattern to the display

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <unistd.h>                                                                                                        
#include <sys/ioctl.h>

#define BITS_PER_BYTE 8

int fbfd;
struct fb_var_screeninfo vinfo;

int fb_width;
int fb_height;
int fb_bpp;
long fb_size;
char *fbdata;

int main(int argc, char *argv[])
{

    if (argc != 2) {
		printf("Usage: ./fbinfo /dev/fb0\n");
		exit(1);
	}
 
	if (strstr(argv[1], "fb") == NULL) {
		printf("Only frame buffers (fb) are supported with this example\n");
		exit(1);
	}		

    // Open the file for reading and writing
	fbfd = open(argv[1], O_RDWR);
	if (fbfd == -1) {
		perror("Error: cannot open framebuffer device");
		goto handle_allocate_error;
	}
 
	// Get variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
		perror("Error: reading variable information");
		goto handle_ioctl_error;
	}
 
	// Figure out the size size
    fb_width = vinfo.xres;
    fb_height = vinfo.yres;
    fb_bpp = vinfo.bits_per_pixel;
    fb_size = (fb_bpp / BITS_PER_BYTE) * fb_width * fb_height;

    fprintf(stdout, "Screen width  = %d\n", fb_width);
    fprintf(stdout, "Screen height = %d\n", fb_height);
    fprintf(stdout, "Bits per pixel = %d\n", fb_bpp);
    fprintf(stdout, "Frame buffer %s size in bytes = %ld\n", argv[1], fb_size);
    
	// Map the device to memory
	fbdata = (char *)mmap(0, fb_size, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, (off_t)0);
	if (fbdata == MAP_FAILED) {
		perror("Error: failed to map framebuffer device to memory");
		goto handle_ioctl_error;
	}
	
	// clear display
	memset (fbdata, 0, fb_size);

	munmap(fbdata,fb_size);

	// sleep
	sleep(5);

	close(fbfd);
    return 0;

handle_ioctl_error:
	close(fbfd);
handle_allocate_error:
	exit(1);                          


}