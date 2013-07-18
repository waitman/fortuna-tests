CC?=		clang
PREFIX?=	/usr/local
ECHO?=		echo

all: test-static test-shared

test-static:
	cc -I$(PREFIX)/include -o test-fortuna-static test-fortuna.c $(PREFIX)/lib/libfortuna.a

test-shared:
	cc -I$(PREFIX)/include -o test-fortuna-shared test-fortuna.c -L$(PREFIX)/lib -lfortuna

clean:
	rm -f test-fortuna-static
	rm -f test-fortuna-shared

run-tests:

	$(ECHO) 'static'
	./test-fortuna-static

	$(ECHO) 'shared'
	./test-fortuna-shared

