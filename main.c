#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#define VEC2F_IMPL
#include "vec2f.h"

#define WIDTH 64 
#define HEIGHT 64 
#define FPS 30 
#define DELTA (1.0f / FPS)

typedef enum
{
    FULL = 0,
    BLANK = 1,
    PIXEL_COUNT = 2
} pixel;

static pixel buffer[WIDTH * HEIGHT];

void buffer_clear(void)
{
    for (int y = 0; y < HEIGHT; ++y)
	for (int x = 0; x < WIDTH; ++x)
	    buffer[y * WIDTH + x] = BLANK;
}

void get_cursor_back(void)
{
    printf("\x1b[%dD", WIDTH);
    printf("\x1b[%dA", HEIGHT / 2);
}

void wait(void)
{
    usleep(1000 * 1000 / FPS);
}

void buffer_show(void)
{
    static char table[2][2] = 
    {
	{ 'C', '^' },
	{ '_', ' ' }
    };

    static char line[WIDTH];
    for (int y = 0; y < HEIGHT / 2; ++y)
    {
	for (int x = 0; x < WIDTH; ++x)
	    line[x] = table[buffer[2 * y * WIDTH + x]][buffer[(2 * y + 1) * WIDTH + x]];

	fwrite(line, WIDTH, 1, stdout);
	fputc('\n', stdout);
    }

    get_cursor_back();
}

void draw_rect(int x1, int y1, int x2, int y2)
{
    // NOTE: This assertions were made 'cause I was too lazy
    //       to implement the case when part of the rectangle
    //       is out of screen as it is done with the circle.
    assert(x1 >= 0 && x1 < WIDTH && y1 >= 0 && y1 < HEIGHT); 
    assert(x2 >= 0 && x2 < WIDTH && y2 >= 0 && y2 < HEIGHT); 

    for (int y = y1; y <= y2; ++y)
	for (int x = x1; x <= x2; ++x)
	    buffer[y * WIDTH + x] = FULL;
}

void draw_circle(vec2f center, float radius)
{
    vec2f beg = vec2f_floor(vec2f_sub(center, vec2f(radius, radius)));
    vec2f end = vec2f_ceil(vec2f_add(center, vec2f(radius, radius)));

    for (int y = (int)beg.y; y <= (int)end.y; ++y)
    {
	for (int x = (int)beg.x; x <= (int)end.x; ++x)
	{
	    vec2f curr_pos = vec2f_add(vec2f(x, y), vec2f(0.5f, 0.5f));
	    vec2f dst_vec  = vec2f_sub(center, curr_pos);

	    if (vec2f_sqrlen(dst_vec) <= sqrf(radius))
		if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
		    buffer[y * WIDTH + x] = FULL;
	}
    }
}

vec2f collision_detection(vec2f pos, float radius, vec2f vel)
{
    float left_x  = pos.x - radius;
    float right_x = pos.x + radius;
    float up_y    = pos.y - radius;
    float down_y  = pos.y + radius;

    if (left_x <= 0)      vel.x *= -1;
    if (right_x >= WIDTH) vel.x *= -1;
    if (up_y >= 0)        vel.y *= -1;
    if (down_y <= HEIGHT) vel.y *= -1;

    return vel;
}

int main(void)
{
    float radius = 8.0f;
    vec2f pos = vec2f(20.0f + radius, radius);
    vec2f vel = vec2f(35, 35);

    while (1)
    {
	buffer_clear();
	draw_circle(pos, radius);
	buffer_show();

	pos = vec2f_add(pos, vec2f_mul(vel, DELTA));
	vel = collision_detection(pos, radius, vel);

	wait();
    }

    return 0;
}
