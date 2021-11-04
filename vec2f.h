#ifndef MATHS_H
#define MATHS_H

#include <math.h>

float sqrf(float val);

typedef struct { float x, y; } vec2f;
vec2f vec2f_add(vec2f lhs, vec2f rhs);
vec2f vec2f_sub(vec2f lhs, vec2f rhs);
vec2f vec2f_mul(vec2f v, float val);
vec2f vec2f_div(vec2f v, float val);
vec2f vec2f_floor(vec2f v);
vec2f vec2f_ceil(vec2f v);
float vec2f_sqrlen(vec2f v);
float vec2f_len(vec2f v);

#define vec2f(...) (vec2f){__VA_ARGS__}

#ifdef VEC2F_IMPL

float sqrf(float val)
{
    return val * val;
}

vec2f vec2f_add(vec2f lhs, vec2f rhs)
{
    return vec2f(lhs.x + rhs.x, lhs.y + rhs.y);
}

vec2f vec2f_sub(vec2f lhs, vec2f rhs)
{
    return vec2f(lhs.x - rhs.x, lhs.y - rhs.y);
}

vec2f vec2f_mul(vec2f v, float val)
{
    return vec2f(v.x * val, v.y * val);
}

vec2f vec2f_div(vec2f v, float val)
{
    return vec2f_mul(v, 1 / val);
}

vec2f vec2f_floor(vec2f v)
{
    return vec2f(floorf(v.x), floorf(v.y));
}

vec2f vec2f_ceil(vec2f v)
{
    return vec2f(ceilf(v.x), ceilf(v.y));
}

float vec2f_sqrlen(vec2f v)
{
    return sqrf(v.x) + sqrf(v.y);
}

float vec2f_len(vec2f v)
{
    return sqrtf(vec2f_sqrlen(v));
}

#endif

#endif
