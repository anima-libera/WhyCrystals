
/* AGTEON - Animation Global Table Eon
 * Implementations are in "agteon.c". */

#ifndef WHYCRYSTALS_HEADER_AGTEON__
#define WHYCRYSTALS_HEADER_AGTEON__

struct anim_step_t
{
	unsigned int sprite_id;
	unsigned int next_step_start_time;
};
typedef struct anim_step_t anim_step_t;

struct anim_t
{
	unsigned int step_count;
	anim_step_t* step_arr;
	unsigned int initial_inanim_time;
	unsigned int initial_step_index;
};
typedef struct anim_t anim_t;

struct anim_state_t
{
	const anim_t* anim;
	unsigned int inanim_time;
	unsigned int step_index;
};
typedef struct anim_state_t anim_state_t;

void anim_start(anim_state_t* anim_state, const anim_t* anim,
	unsigned int* target_sprite_id);

void anim_iterate(anim_state_t* anim_state, unsigned int* target_sprite_id);

#endif /* WHYCRYSTALS_HEADER_AGTEON__ */
