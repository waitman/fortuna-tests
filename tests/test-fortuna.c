#include <stdio.h>
#include <fortuna/internal.h>

int
main (int argc, char** argv) {

	uint8 rnd[1024];
	int i;

	if (px_get_random_bytes(rnd, sizeof(rnd)) < 0) {
		printf("No random bytes\n");
		exit(1);
	}

	for (i=0;i<sizeof(rnd);i++) {
		printf("%i ",rnd[i]);
	}
	printf("\n\n");

	memset(rnd, 0, sizeof(rnd));

}
