
#include <stddef.h> /* NULL */
#include "agteon.h"

void anim_start(anim_state_t* anim_state, const anim_t* anim,
	unsigned int* target_sprite_id)
{
	*anim_state = (anim_state_t){
		.anim = anim,
		.inanim_time = anim->initial_inanim_time,
		.step_index = anim->initial_step_index,
	};
	*target_sprite_id = anim->step_arr[anim->initial_step_index].sprite_id;
}

void anim_iterate(anim_state_t* anim_state, unsigned int* target_sprite_id)
{
	const anim_t* anim = anim_state->anim;
	if (anim == NULL)
	{
		return;
	}

	const unsigned int previous_step_index = anim_state->step_index;
	const unsigned int nsst =
		anim->step_arr[anim_state->step_index].next_step_start_time;
	//const unsigned int total_duration = 
	//	anim->step_arr[anim->step_count-1].next_step_start_time;

	anim_state->inanim_time++;
	if (anim_state->inanim_time >= nsst)
	{
		anim_state->step_index++;
		if (anim_state->step_index >= anim->step_count)
		{
			anim_state->step_index = 0;
			anim_state->inanim_time = 0;
		}
	}

	if (previous_step_index != anim_state->step_index)
	{
		*target_sprite_id = anim->step_arr[anim_state->step_index].sprite_id;
	}
}
