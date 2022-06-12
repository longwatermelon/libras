#ifndef RAS_RENDER_H
#define RAS_RENDER_H

#include "vector.h"
#include "camera.h"
#include <SDL2/SDL.h>

typedef struct RenderInfoStruct
{
    SDL_Renderer *rend;
    struct Light **lights;
    size_t nlights;

    uint32_t *scr;
    float *zbuf;

    struct Camera *cam;
} RenderInfo;

typedef struct
{
    float x, z, sx, sz;
} RTI;

SDL_Point render_project_point(Vec3f p);

Vec3f render_rotate_ccw(Vec3f p, Vec3f angle);
Vec3f render_rotate_cc(Vec3f p, Vec3f angle);

void render_filled_tri(SDL_Point p[3], float z[3], uint32_t *screen, float *zbuf, SDL_Color col);
void render_fill_edges(int top, int bot, RTI *l1, RTI *l2, uint32_t *screen, float *zbuf, SDL_Color col);

Vec3f render_matmul(float mat[3][3], Vec3f p);
Vec3f render_matmul_rotation_opt(float mat[3][3], Vec3f p);

void render_set_scrdim(int w, int h);
SDL_Point render_get_scrdim();

void render_reset_buffers(RenderInfo *ri);

#endif

