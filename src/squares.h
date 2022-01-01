#ifndef SQUARES_H
#define SQUARES_H

// Definitions //

// Window Settings
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// Square info
#define SQUARE_DIM 24
#define SQUARE_COUNT 150
#define DURATION 500

// Structs // 

typedef struct color_t
{
    int r;
    int g;
    int b;
    int a;
} color_t;

typedef struct square_t
{
    int x;
    int y;
    int w;
    int h;
    int initial_x;
    int initial_y;
    color_t color;
} square_t; 

typedef struct app_t
{
    int running;
    int tweening;
    int finished_tweening;
    int target_x;
    int target_y;
    unsigned int start_time;
    square_t squares[SQUARE_COUNT];
} app_t;

// Inline helper functions //

static inline int rng(int min, int max)
{
    return (rand() % (max - min + 1) + min);
}

// Function declarations //

// Core
static void init_app(app_t *app);
static void update_app(app_t *app); 
static void render_app(SDL_Renderer *renderer, app_t *app);

// Tween functions
static int linear_ease(int current_time, int initial_val, int change_val, int duration);

#endif
