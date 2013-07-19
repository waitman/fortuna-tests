#include <fortuna/internal.h>
#include <wand/magick_wand.h>

/* pixel dimensions of image */

#define XSIZE	1024
#define YSIZE 	1024

int
main (int argc, char** argv) {

        uint8 rnd[(XSIZE*YSIZE)*3+1];
        int i;

	/* fill array with random values */
        if (px_get_random_bytes(rnd, sizeof(rnd)) < 0) {
                printf("No random bytes\n");
                exit(1);
        }

	MagickWand *m_wand = NULL;
	PixelWand *p_wand = NULL;
	PixelIterator *iterator = NULL;
	PixelWand **pixels = NULL;
	int y,red,green,blue;
	size_t x;
	char hex[128];

	MagickWandGenesis();

	p_wand = NewPixelWand();
	PixelSetColor(p_wand,"white");
	m_wand = NewMagickWand();
	MagickNewImage(m_wand,XSIZE,YSIZE,p_wand);
	iterator=NewPixelIterator(m_wand);
	for(y=0;y<YSIZE;y++) {
		pixels=PixelGetNextIteratorRow(iterator,&x);
		for(x=0;x<XSIZE;x++) {
			red = rnd[x*y];
			green = rnd[x*y*2];
			blue = rnd[x*y*3];
			sprintf(hex,"#%02x%02x%02x",red,green,blue);
			PixelSetColor(pixels[x],hex);
		}
		PixelSyncIterator(iterator);
	}
	MagickWriteImage(m_wand,"randcolorimage.png");

	/* Clean up */
	iterator=DestroyPixelIterator(iterator);
	DestroyMagickWand(m_wand);
	MagickWandTerminus();
	memset(rnd, 0, sizeof(rnd));
}

