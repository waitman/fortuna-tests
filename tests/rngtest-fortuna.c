#include <stdio.h>
#include <fortuna/internal.h>

uint8 rnd[2500064];
int
main (int argc, char** argv) {

	int i;

	if (px_get_random_bytes(rnd, sizeof(rnd)) < 0) {
		printf("No random bytes\n");
		exit(1);
	}

	for (i=0;i<sizeof(rnd);i++) {
		printf("%c",(char) rnd[i]);
	}

	memset(rnd, 0, sizeof(rnd));

}
