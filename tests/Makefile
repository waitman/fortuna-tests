CC?=		clang
PREFIX?=	/usr/local
ECHO?=		echo
CFLAGS?=	-Werror -Wall -O2 

all: test-static test-shared test-image

test-static:
	cc -I$(PREFIX)/include $(CFLAGS) -o test-fortuna-static test-fortuna.c $(PREFIX)/lib/libfortuna.a

test-shared:
	cc -I$(PREFIX)/include $(CFLAGS) -o test-fortuna-shared test-fortuna.c -L$(PREFIX)/lib -lfortuna

test-image:
	cc -I$(PREFIX)/include $(CFLAGS) -I$(PREFIX)/include/ImageMagick -o greygrid greygrid.c -L$(PREFIX)/lib -lfortuna -lMagickWand
	cc -I$(PREFIX)/include $(CFLAGS) -I$(PREFIX)/include/ImageMagick -o colorgrid colorgrid.c -L$(PREFIX)/lib -lfortuna -lMagickWand

clean:
	rm -f test-fortuna-static
	rm -f test-fortuna-shared
	rm -f greygrid
	rm -f colorgrid
	rm -f randcolorimage.png
	rm -f randgreyimage.png

run-tests:

	$(ECHO) 'static'
	./test-fortuna-static

	$(ECHO) 'shared'
	./test-fortuna-shared

