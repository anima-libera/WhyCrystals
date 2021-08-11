
#ifndef WHYCRYSTALS_HEADER_OBJ__
#define WHYCRYSTALS_HEADER_OBJ__

struct flags_t
{
	unsigned int flags;
	#define OBJ_FLAG_EXISTS 0x1
};
typedef struct flags_t flags_t;

struct pos_t
{
	float x, y, z;
};
typedef struct pos_t pos_t;

struct color_t
{
	float r, g, b;
};
typedef struct color_t color_t;

enum prop_type_id_t
{
	PTI_FLAGS,
	PTI_POS,
	PTI_COLOR,
	PROP_TYPE_COUNT
};
typedef enum prop_type_id_t prop_type_id_t;

struct porp_info_t
{
	const char* name;
	unsigned int size; /* Would be sizeof(flags_t) if name was "flags". */
};
typedef struct porp_info_t porp_info_t;

extern const porp_info_t g_prop_info_table[PROP_TYPE_COUNT];

#

struct prop_type_id_set_t
{
	unsigned int len;
	prop_type_id_t* arr;
};
typedef struct prop_type_id_set_t prop_type_id_set_t;

struct col_table_t
{
	unsigned int each_col_len;
	prop_type_id_set_t prop_set;
	void* col_arr;
};
typedef struct col_table_t col_table_t;

struct octa_t
{
	unsigned int len;
	col_table_t* col_table_arr;
};
typedef struct octa_t octa_t;

extern octa_t g_octa;

struct obj_index_t
{
	unsigned int table_index;
	unsigned int row_index;
};
typedef struct obj_index_t obj_index_t;

#endif /* WHYCRYSTALS_HEADER_OBJ__ */
