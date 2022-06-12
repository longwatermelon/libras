#ifndef RAS_UTIL_H
#define RAS_UTIL_H

#include "vector.h"
#include "light.h"
#include <time.h>
#include <sys/types.h>
#include <SDL2/SDL.h>

float util_tri_area(Vec3f points[3]);

Vec3f util_barycentric_coefficients(Vec3f points[3], Vec3f p);

float util_p_line_sdist(Vec3f p, Vec3f p0, Vec3f p1);

#endif

