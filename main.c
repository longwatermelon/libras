#include <libras/ras.h>
#include <libras/render.h>
#include <libras/mesh.h>


int main(int argc, char **argv)
{
    SDL_Window *w = SDL_CreateWindow("Libras demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer *r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    render_set_scrdim(800, 800);
    ras_init(r);

    ras_add_mesh((Vec3f){ 0, 0, 15 }, (Vec3f){ 0, 0, 0 }, "donut.obj", (SDL_Color){ 255, 0, 255 });
    ras_add_light((Vec3f){ 0, 0, 0 }, 1.f);

    struct Camera *cam = cam_alloc((Vec3f){ 0, 0, 0 }, (Vec3f){ 0, 0, 0 });
    ras_set_camera(cam);

    SDL_Texture *scrtex = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 800, 800);

    bool running = true;
    SDL_Event evt;

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

        const Uint8 *keys = SDL_GetKeyboardState(0);
        Vec3f move = { 0, 0, 0 };
        if (keys[SDL_SCANCODE_W]) move.z += 1.f;
        if (keys[SDL_SCANCODE_A]) move.x -= 1.f;
        if (keys[SDL_SCANCODE_S]) move.z -= 1.f;
        if (keys[SDL_SCANCODE_D]) move.x += 1.f;

        cam_move_angle(cam, move);

        if (keys[SDL_SCANCODE_SPACE]) cam->pos.y -= 1.f;
        if (keys[SDL_SCANCODE_LSHIFT]) cam->pos.y += 1.f;

        if (keys[SDL_SCANCODE_LEFT]) cam->angle.x -= .1f;
        if (keys[SDL_SCANCODE_RIGHT]) cam->angle.x += .1f;
        if (keys[SDL_SCANCODE_UP]) cam->angle.y += .1f;
        if (keys[SDL_SCANCODE_DOWN]) cam->angle.y -= .1f;

        SDL_RenderClear(r);

        ras_render(scrtex);
        SDL_RenderCopy(r, scrtex, 0, 0);

        SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
        SDL_RenderPresent(r);
    }

    cam_free(cam);

    ras_quit();

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);

    return 0;
}

