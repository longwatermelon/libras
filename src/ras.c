#include "ras.h"
#include "render.h"
#include "light.h"
#include "mesh.h"
#include <stdio.h>
#include <SDL2/SDL.h>

RenderInfo *g_ri = 0;

struct Mesh **g_meshes = 0;
size_t g_nmeshes = 0;

void ras_init(SDL_Renderer *r)
{
    SDL_Point scrdim = render_get_scrdim();

    if (scrdim.x == 0 && scrdim.y == 0)
        fprintf(stderr, "[ras_init]: Screen dimensions are not initialized.\n");

    ras_ri_init(r);
}


void ras_quit()
{
    ras_clear_lights();
    ras_clear_meshes();

    free(g_ri);
}


void ras_ri_init(SDL_Renderer *r)
{
    g_ri = malloc(sizeof(struct RenderInfoStruct));
    g_ri->rend = r;
    g_ri->lights = 0;
    g_ri->nlights = 0;
    
    SDL_Point scrdim = render_get_scrdim();
    g_ri->scr = malloc(sizeof(uint32_t) * scrdim.x * scrdim.y);
    g_ri->zbuf = malloc(sizeof(float) * scrdim.x * scrdim.y);
    render_reset_buffers(g_ri);
}


RenderInfo *ras_ri()
{
    return g_ri;
}


void ras_render(SDL_Texture *scrtex)
{
    render_reset_buffers(g_ri);

    for (size_t i = 0; i < g_nmeshes; ++i)
        mesh_render(g_meshes[i], g_ri);

    SDL_Point scrdim = render_get_scrdim();
    SDL_UpdateTexture(scrtex, 0, g_ri->scr, scrdim.x * sizeof(uint32_t));
}


struct Light *ras_add_light(Vec3f pos, float in)
{
    g_ri->lights = realloc(g_ri->lights, sizeof(struct Light*) * ++g_ri->nlights);
    g_ri->lights[g_ri->nlights - 1] = light_alloc(pos, in);
    return g_ri->lights[g_ri->nlights - 1];
}


void ras_remove_light(struct Light *l)
{
    for (size_t i = 0; i < g_ri->nlights; ++i)
    {
        if (g_ri->lights[i] == l)
        {
            light_free(g_ri->lights[i]);
            memmove(g_ri->lights + i, g_ri->lights + i + 1, (--g_ri->nlights - i) * sizeof(struct Light*));
            break;
        }
    }
}


void ras_clear_lights()
{
    for (size_t i = 0; i < g_ri->nlights; ++i)
        light_free(g_ri->lights[i]);

    free(g_ri->lights);
}


struct Mesh *ras_add_mesh(Vec3f pos, Vec3f rot, const char *path, SDL_Color col)
{
    g_meshes = realloc(g_meshes, sizeof(struct Light*) * ++g_nmeshes);
    g_meshes[g_nmeshes - 1] = mesh_alloc(pos, rot, path, col);
    return g_meshes[g_nmeshes - 1];
}


void ras_remove_mesh(struct Mesh *l)
{
    for (size_t i = 0; i < g_nmeshes; ++i)
    {
        if (g_meshes[i] == l)
        {
            mesh_free(g_meshes[i]);
            memmove(g_meshes + i, g_meshes + i + 1, (--g_nmeshes - i) * sizeof(struct Mesh*));
            break;
        }
    }
}


void ras_clear_meshes()
{
    for (size_t i = 0; i < g_nmeshes; ++i)
        mesh_free(g_meshes[i]);

    free(g_meshes);
}


void ras_set_camera(struct Camera *c)
{
    g_ri->cam = c;
}

