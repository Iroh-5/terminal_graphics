#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#define WIDTH 64
#define HEIGHT 64
#define FPS 15

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
	{
	    line[x] = table[buffer[2 * y * WIDTH + x]][buffer[(2 * y + 1) * WIDTH + x]];
	}
	fwrite(line, WIDTH, 1, stdout);
	fputc('\n', stdout);
    }

    get_cursor_back();
}

void draw_rect(int x1, int y1, int x2, int y2)
{
    assert(x1 >= 0 && x1 < WIDTH && y1 >= 0 && y1 < HEIGHT); 
    assert(x2 >= 0 && x2 < WIDTH && y2 >= 0 && y2 < HEIGHT); 

    for (int y = y1; y <= y2; ++y)
	for (int x = x1; x <= x2; ++x)
	    buffer[y * WIDTH + x] = FULL;
}

int main(void)
{
    while (1)
    {
	buffer_clear();
	draw_rect(20, 20, 40, 40);
	buffer_show();
	wait();
    }

    return 0;
}
