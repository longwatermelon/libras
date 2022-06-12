#include "render.h"

#define swapp(x, y) { \
    SDL_Point tmp = x; \
    x = y; \
    y = tmp; }

#define swapf(x, y) { \
    float tmp = x; \
    x = y; \
    y = tmp; }

RTI g_r01, g_r02, g_r12;

int g_sw = 0, g_sh = 0;

SDL_Point render_project_point(Vec3f p)
{
    float px = 0.f, py = 0.f;

    if (p.z != 0.f)
    {
        px = p.x / p.z;
        py = p.y / p.z;
    }

    SDL_Point proj = {
        (px + .5f) * (float)g_sw,
        (py + .5f) * (float)g_sh
    };

    return proj;
}


Vec3f render_rotate_ccw(Vec3f p, Vec3f angle)
{
    if (vec_len(angle) == 0.f) return p;

    float rotx[3][3] = {
        { 1, 0, 0 },
        { 0, cosf(angle.y), -sinf(angle.y) },
        { 0, -rotx[1][2], rotx[1][1] }
    };

    float roty[3][3] = {
        { cosf(angle.x), 0, sinf(angle.x) },
        { 0, 1, 0 },
        { -roty[0][2], 0, roty[0][0] }
    };

    float rotz[3][3] = {
        { cosf(angle.z), -sinf(angle.z), 0 },
        { -rotz[0][1], rotz[0][0], 0 },
        { 0, 0, 1 }
    };

    return render_matmul_rotation_opt(rotz, render_matmul_rotation_opt(rotx, render_matmul_rotation_opt(roty, p)));
}


Vec3f render_rotate_cc(Vec3f p, Vec3f angle)
{
    if (vec_len(angle) == 0.f) return p;
    angle = vec_mulf(angle, -1.f);

    float rotx[3][3] = {
        { 1, 0, 0 },
        { 0, cosf(angle.y), -sinf(angle.y) },
        { 0, -rotx[1][2], rotx[1][1] }
    };

    float roty[3][3] = {
        { cosf(angle.x), 0, sinf(angle.x) },
        { 0, 1, 0 },
        { -roty[0][2], 0, roty[0][0] }
    };

    float rotz[3][3] = {
        { cosf(angle.z), -sinf(angle.z), 0 },
        { -rotz[0][1], rotz[0][0], 0 },
        { 0, 0, 1 }
    };

    return render_matmul_rotation_opt(roty, render_matmul_rotation_opt(rotx, render_matmul_rotation_opt(rotz, p)));
}


void render_filled_tri(SDL_Point p[3], float z[3], uint32_t *screen, float *zbuf, SDL_Color col)
{
    SDL_Point p0 = p[0], p1 = p[1], p2 = p[2];
    float z0 = z[0], z1 = z[1], z2 = z[2];

    if (p0.y > p1.y)
    {
        swapp(p0, p1);
        swapf(z0, z1);
    }

    if (p0.y > p2.y)
    {
        swapp(p0, p2);
        swapf(z0, z2);
    }

    if (p1.y > p2.y)
    {
        swapp(p1, p2);
        swapf(z1, z2);
    }

    g_r02 = (RTI){ .x = p0.x, .z = z0, .sx = (float)(p2.y - p0.y) / (p2.x - p0.x), .sz = (float)(z2 - z0) / (p2.y - p0.y) };
    g_r01 = (RTI){ .x = p0.x, .z = z0, .sx = (float)(p1.y - p0.y) / (p1.x - p0.x), .sz = (float)(z1 - z0) / (p1.y - p0.y) };
    g_r12 = (RTI){ .x = p1.x, .z = z1, .sx = (float)(p2.y - p1.y) / (p2.x - p1.x), .sz = (float)(z2 - z1) / (p2.y - p1.y) };

    render_fill_edges(p0.y, p1.y, &g_r02, &g_r01, screen, zbuf, col);
    render_fill_edges(p1.y, p2.y, &g_r02, &g_r12, screen, zbuf, col);
}


void render_fill_edges(int top, int bot, RTI *l1, RTI *l2, uint32_t *screen, float *zbuf, SDL_Color col)
{
    uint32_t hex_col = 0x00000000 | col.r << 16 | col.g << 8 | col.b;

    for (int y = top; y < bot; ++y)
    {
        int min = roundf(l1->x > l2->x ? l2->x : l1->x);
        int max = roundf(l1->x > l2->x ? l1->x : l2->x);

        float z = l1->z;
        float sz = (l2->z - l1->z) / (l2->x - l1->x);

        for (int i = min; i < max; ++i)
        {
            z += sz;

            if (i < 0)
            {
                z += sz * -i;
                i = 0;
                continue;
            }

            if (i >= g_sw) break;

            int idx = y * g_sw + i;

            if (idx >= 0 && idx < g_sw * g_sh)
            {
                if (z < zbuf[idx])
                {
                    screen[idx] = hex_col;
                    zbuf[idx] = z;
                }
            }
        }

        l1->x += 1.f / l1->sx;
        l2->x += 1.f / l2->sx;

        l1->z += l1->sz;
        l2->z += l2->sz;
    }
}


Vec3f render_matmul(float mat[3][3], Vec3f p)
{
    Vec3f ret;

    ret.x = p.x * mat[0][0] + p.y * mat[1][0] + p.z * mat[2][0];
    ret.y = p.x * mat[0][1] + p.y * mat[1][1] + p.z * mat[2][1];
    ret.z = p.x * mat[0][2] + p.y * mat[1][2] + p.z * mat[2][2];

    return ret;
}


Vec3f render_matmul_rotation_opt(float mat[3][3], Vec3f p)
{
    if (mat[0][0] == 1 && mat[1][1] == 1 && mat[2][2] == 1)
        return p;

    return render_matmul(mat, p);
}


void render_set_scrdim(int w, int h)
{
    g_sw = w;
    g_sh = h;
}


SDL_Point render_get_scrdim()
{
    return (SDL_Point){ g_sw, g_sh };
}


void render_reset_buffers(RenderInfo *ri)
{
    for (int i = 0; i < g_sw * g_sh; ++i)
    {
        ri->scr[i] = 0x00000000;
        ri->zbuf[i] = INFINITY;
    }
}

