#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <malloc_np.h>
#include <fortuna/fortuna.h>
#include <fortuna/px.h>
#include <fortuna/internal.h>


#define SYSTEM_RESEED_MIN   (20*60) /* 20 min */
#define SYSTEM_RESEED_MAX   (12*60*60) /* 12h */
#define SYSTEM_RESEED_CHECK_TIME   (10*60) /* 10 min */
#define SYSTEM_RESEED_CHANCE   (4) /* 256/4 * 10min ~ 10h */

/*
static time_t seed_time = 0;
static time_t check_time = 0;
*/

void system_reseed(void) {
       uint8	buf[1024];
       int	n;
       time_t	t;
       int	skip = 1;
       t = time(NULL);
       if (seed_time == 0)
              skip = 0;
       else if ((t - seed_time) < SYSTEM_RESEED_MIN)
              skip = 1;
       else if ((t - seed_time) > SYSTEM_RESEED_MAX)
              skip = 0;
       else if (!check_time || (t - check_time) > SYSTEM_RESEED_CHECK_TIME)
       {
              check_time = t;

              /* roll dice */
              px_get_random_bytes(buf, 1);
              skip = buf[0] >= SYSTEM_RESEED_CHANCE;
       }
       /* clear 1 byte */
       memset(buf, 0, sizeof(buf));

       if (skip)
              return;

       n = px_acquire_system_randomness(buf);
       if (n > 0)
              fortuna_add_entropy(buf, n);

       seed_time = t;
       memset(buf, 0, sizeof(buf));
}

int
px_get_random_bytes(uint8 *dst, unsigned count)
{
        system_reseed();
        fortuna_get_bytes(count, dst);
        return 0;
}

int
px_find_digest(const char *name, PX_MD **res)
{
	const struct int_digest *p;
	PX_MD *h;
	for (p = int_digest_list; p->name; p++) {
		if (strcasecmp(p->name, name) == 0) {
			h = malloc(sizeof(*h));
			p->init(h);
			*res = h;
			return 0;
		}
	}
	return PXE_NO_HASH;
}

int
px_add_entropy(const uint8 *data, unsigned count)
{
        system_reseed();
        fortuna_add_entropy(data, count);
        return 0;
}


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
	uint8 rnd[3];
	char data[1024];
	strcpy(data,"This is a test");

	if (px_get_random_bytes(rnd, 3) < 0) {
		printf("No random bytes\n");
		exit(1);
	}
	if (px_find_digest("sha1", &md) < 0) {
		printf("No sha\n");
		exit(1);
	} 

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
