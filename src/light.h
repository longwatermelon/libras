#ifndef RAS_LIGHT_H
#define RAS_LIGHT_H

#include "vector.h"

struct Light
{
    Vec3f pos;
    float in;
};

struct Light *light_alloc(Vec3f pos, float in);
void light_free(struct Light *l);

#endif

