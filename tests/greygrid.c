#include <fortuna/internal.h>
#include <wand/magick_wand.h>

/* pixel dimensions of image */

#define XSIZE   1024
#define YSIZE   1024

int
main (int argc, char** argv) {

        uint8 rnd[XSIZE * YSIZE];
        
	/* fill array with random values */
        if (px_get_random_bytes(rnd, sizeof(rnd)) < 0) {
                printf("No random bytes\n");
                exit(1);
        }

	MagickWand *m_wand = NULL;
	PixelWand *p_wand = NULL;
	
	
	char hex[128];

	MagickWandGenesis();

	p_wand = NewPixelWand();
	PixelSetColor(p_wand,"white");
	m_wand = NewMagickWand();
	MagickNewImage(m_wand,XSIZE,YSIZE,p_wand);
	
	PixelIterator *iterator = NULL;
	iterator=NewPixelIterator(m_wand);
	for(int y=0;y<YSIZE;y++) {
		size_t width;
		PixelWand **pixels = NULL;
		pixels = PixelGetNextIteratorRow(iterator,&width);
		for(int x=0;x<XSIZE;x++) {
			int i = y * XSIZE + x;
			int gray = rnd[i];
			sprintf(hex,"#%02x%02x%02x",gray,gray,gray);
			PixelSetColor(pixels[x],hex);
		}
		PixelSyncIterator(iterator);
	}
	MagickWriteImage(m_wand,"randgreyimage.png");

	/* Clean up */
	iterator=DestroyPixelIterator(iterator);
	DestroyMagickWand(m_wand);
	MagickWandTerminus();
	memset(rnd, 0, sizeof(rnd));
}

