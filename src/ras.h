#ifndef RAS_H
#define RAS_H

#include "vector.h"
#include <SDL2/SDL.h>

struct Light;
typedef struct RenderInfoStruct RenderInfo;
struct Camera;

void ras_init(SDL_Renderer *r);
void ras_quit();

void ras_ri_init(SDL_Renderer *r);
RenderInfo *ras_ri();

void ras_render(SDL_Texture *scrtex);

struct Light *ras_add_light(Vec3f pos, float in);
void ras_remove_light(struct Light *l);
void ras_clear_lights();

struct Mesh *ras_add_mesh(Vec3f pos, Vec3f rot, const char *path, SDL_Color col);
void ras_remove_mesh(struct Mesh *l);
void ras_clear_meshes();

void ras_set_camera(struct Camera *c);

#endif

