
#include "smata.h"
#include "random.h"
#include "spw.h"
#include <stdlib.h>
#include <GL/glew.h>

pixel_t* g_smata_atlas_data = NULL;
GLuint g_smata_atlas_id = 0;

int init_smata(void)
{
	#define ATLAS_SIDE 256
	g_smata_atlas_data = calloc(ATLAS_SIDE * ATLAS_SIDE, sizeof(pixel_t));

	for (unsigned int x = 0; x < ATLAS_SIDE; x++)
	for (unsigned int y = 0; y < ATLAS_SIDE; y++)
	{
		g_smata_atlas_data[x + ATLAS_SIDE * y].r = x;
		g_smata_atlas_data[x + ATLAS_SIDE * y].g = y;
		g_smata_atlas_data[x + ATLAS_SIDE * y].b = 255;
		g_smata_atlas_data[x + ATLAS_SIDE * y].a = 255;
	}

	glGenTextures(1, &g_smata_atlas_id);
	glBindTexture(GL_TEXTURE_2D, g_smata_atlas_id);
	glTexImage2D(GL_TEXTURE_2D,
		0, GL_RGBA, ATLAS_SIDE, ATLAS_SIDE, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, g_smata_atlas_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_smata_atlas_id);
	swp_update_atlas(0);

	return 0;
}

