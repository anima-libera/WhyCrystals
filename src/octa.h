
#ifndef WHYCRYSTALS_HEADER_OCTA__
#define WHYCRYSTALS_HEADER_OCTA__

struct flags_bit_set_t
{
	unsigned int exists: 1;
	#define OBJ_FLAG_EXISTS (1 << 0)
};
typedef struct flags_bit_set_t flags_bit_set_t;

union flags_t
{
	unsigned int plain;
	flags_bit_set_t bit_set;
};
typedef union flags_t flags_t;

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

struct ptis_t
{
	unsigned int len;
	pti_t* arr; /* Should always be sorted. */
	/* TODO: Remove the indirection, so that colt_t is faster. */
};
typedef struct ptis_t ptis_t;

void ptis_add(ptis_t* ptis, pti_t pti);
void ptis_copy(const ptis_t* src, ptis_t* dst);
int pti_eq(const ptis_t* a, const ptis_t* b);
void ptis_print(const ptis_t* ptis);

struct colt_t
{
	unsigned int row_count;
	ptis_t ptis;
	void** col_data_arr; /* TODO: Remove one indirection. */
};
typedef struct colt_t colt_t;

void colt_init(colt_t* colt, const ptis_t* ptis);
void colt_lengthen(colt_t* colt, unsigned int by_how_much);
int colt_does_obj_exist(const colt_t* colt, unsigned int row_index);
void colt_print(const colt_t* colt);

struct octa_t
{
	unsigned int len;
	colt_t* colt_arr;
};
typedef struct octa_t octa_t;

extern octa_t g_octa;

struct oi_t
{
	unsigned int colt_index;
	unsigned int row_index;
};
typedef struct oi_t oi_t;

unsigned int octa_add_colt(const ptis_t* ptis);
oi_t octa_alloc_obj(const ptis_t* ptis);
void* octa_get_obj_prop(oi_t oi, pti_t pti);
void octa_print(void);

#endif /* WHYCRYSTALS_HEADER_OCTA__ */
