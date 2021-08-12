
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

enum pti_t
{
	PTI_FLAGS,
	PTI_POS,
	PTI_COLOR,
	PROP_TYPE_COUNT
};
typedef enum pti_t pti_t;

void pti_print(pti_t pti);

struct porp_info_t
{
	const char* name;
	unsigned int size; /* Would be sizeof(flags_t) if name was "flags". */
};
typedef struct porp_info_t porp_info_t;

extern const porp_info_t g_prop_info_table[PROP_TYPE_COUNT];

struct pti_set_t
{
	unsigned int len;
	pti_t* arr; /* Should always be sorted. */
};
typedef struct pti_set_t pti_set_t;

void pti_set_add(pti_set_t* pti_set, pti_t pti);
void pti_set_copy(const pti_set_t* src, pti_set_t* dst);
int pti_eq(const pti_set_t* a, const pti_set_t* b);
void pti_set_print(const pti_set_t* pti_set);

struct col_table_t
{
	unsigned int col_len;
	pti_set_t pti_set;
	void** col_data_arr;
};
typedef struct col_table_t col_table_t;

void col_table_init(col_table_t* col_table, const pti_set_t* pti_set);
void col_table_lengthen(col_table_t* col_table, unsigned int by_how_much);
int col_table_does_obj_exist(const col_table_t* col_table, unsigned int row_index);
void col_table_print(const col_table_t* col_table);

struct octa_t
{
	unsigned int len;
	col_table_t* col_table_arr;
};
typedef struct octa_t octa_t;

extern octa_t g_octa;

struct obj_index_t
{
	unsigned int col_table_index;
	unsigned int row_index;
};
typedef struct obj_index_t obj_index_t;

unsigned int octa_add_col_table(const pti_set_t* pti_set);
obj_index_t octa_alloc_obj(const pti_set_t* pti_set);
void* octa_get_obj_prop(obj_index_t oi, pti_t pti);
void octa_print(void);

#endif /* WHYCRYSTALS_HEADER_OBJ__ */
