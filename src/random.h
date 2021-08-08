
/* Random number generator.
 * Implementations and explainations in "random.c". */

#ifndef WHYCRYSTALS_HEADER_RANDOM__
#define WHYCRYSTALS_HEADER_RANDOM__

#include <stdint.h> /* uint32_t */

/* Random generator state. */
struct rg_t
{
	uint32_t seed; /* Not actually used. */
	#define RG_SIZE 41
	uint32_t lcg_array[RG_SIZE];
	int cur;
};
typedef struct rg_t rg_t;

/* The global random generator.
 * TODO: Remove it ? */
extern rg_t* g_rg;

/* Initialises the given rg with the given seed. */
void rg_seed(rg_t* rg, uint32_t seed);

/* Initialises the given rg with the timestamp as the seed. */
void rg_time_seed(rg_t* rg);

/* Performs a rg step and returns a random int greater than or equeal to inf
 * and less than or equal to sup. */
int rg_int(rg_t* rg, int inf, int sup);

/* Performs a rg step and returns a random float between inf and sup. */
float rg_float(rg_t* rg, float inf, float sup);

/* Performs a rg step and returns 32 random bits. */
uint32_t rg_uint32_full(rg_t* rg);

#endif /* WHYCRYSTALS_HEADER_RANDOM__ */
