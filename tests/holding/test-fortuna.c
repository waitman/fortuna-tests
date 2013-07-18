#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <malloc_np.h>
#include <fortuna/fortuna.h>
#include <fortuna/px.h>
#include <fortuna/internal.h>


time_t seed_time = 0;
time_t check_time = 0;


static void
add_block_entropy(PX_MD *md, char *data)
{
	uint8 sha1[20];
	px_md_reset(md);
	px_md_update(md, (uint8 *) data, sizeof(data));
	px_md_finish(md, sha1);
	px_add_entropy(sha1, 20);
	memset(sha1, 0, 20);
}

int
main (int argc, char** argv) {

/*
	uint8 data[1024];
	unsigned len = 18;
	
	for (int i=0;i<len;i++) {
		printf("%i\n",(int)data[i]);
	}
	return(0);

}

*/

	PX_MD *md;
	uint8 rnd[16];
	char data[1024];
	int i;

	strcpy(data,"This is a test");

	if (px_get_random_bytes(rnd, 16) < 0) {
		printf("No random bytes\n");
		exit(1);
	}
	if (px_find_digest("sha1", &md) < 0) {
		printf("No sha\n");
		exit(1);
	} 

	for (i=0;i<16;i++) {
		printf("%i ",rnd[i]);
	}
	printf("\n");

	/*
	 * Try to make the feeding unpredictable.
	 *
	 * Prefer data over keys, as it's rather likely that key is same in
	 * several calls.
	*/

	
	if (data && rnd[0] >= 32) {
		add_block_entropy(md, data);
	}
	if (data && rnd[1] >= 160) {
		add_block_entropy(md, data);
	}
	if (data && rnd[2] >= 160)
		add_block_entropy(md, data);
	px_md_free(md);
	memset(rnd, 0, sizeof(rnd));

}
