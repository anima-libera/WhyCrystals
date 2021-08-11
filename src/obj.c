
#include "obj.h"

const porp_info_t g_prop_info_table[PROP_TYPE_COUNT] = {
	[PTI_FLAGS] = {.name = "flags", .size = sizeof(flags_t)},
	[PTI_POS] = {.name = "pos", .size = sizeof(pos_t)},
	[PTI_COLOR] = {.name = "color", .size = sizeof(color_t)},
};

octa_t g_octa = {0};


