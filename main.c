#include <ras.h>
#include <render.h>
#include <mesh.h>


int main(int argc, char **argv)
{
    ras_init_sdl();
    SDL_Window *w = SDL_CreateWindow("Libras demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer *r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    render_set_scrdim(800, 800);

    bool running = true;
    SDL_Event evt;

    struct Mesh *m = mesh_alloc((Vec3f){ 0, 2.5, 5 }, (Vec3f){ 0, 0, 0 }, "cube.obj", (SDL_Color){ 255, 0, 255 });
    struct Light **lights = malloc(sizeof(struct Light*));
    lights[0] = light_alloc((Vec3f){ 0, 0, 0 }, 1.f);

    uint32_t *scr = malloc(sizeof(uint32_t) * 800 * 800);;
    float *zbuf = malloc(sizeof(float) * 800 * 800);
    SDL_Texture *scrtex = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 800, 800);

    RenderInfo ri = { .rend = r, .font = TTF_OpenFont("font.ttf", 16), .scr = scr, .zbuf = zbuf, .lights = lights, .nlights = 1 };
    struct Camera *cam = cam_alloc((Vec3f){ 0, 0, 0 }, (Vec3f){ 0, 0, 0 });

    while (running)
    {
        while (SDL_PollEvent(&evt))
        {
            switch (evt.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            }
        }

        SDL_RenderClear(r);

        render_reset_buffers(&ri);

        mesh_render(m, &ri, cam);
        SDL_UpdateTexture(scrtex, 0, scr, 800 * sizeof(uint32_t));
        SDL_RenderCopy(r, scrtex, 0, 0);

        SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
        SDL_RenderPresent(r);
    }

    cam_free(cam);

    free(scr);
    free(zbuf);

    light_free(lights[0]);
    free(lights);

    TTF_CloseFont(ri.font);

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
    ras_quit_sdl();

    return 0;
}

