#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


static int running;
static SDL_Rect squares[SQUARE_COUNT];
static SDL_Rect initial[SQUARE_COUNT];

static inline int rng(int min, int max)
{
    return (rand() % (max - min + 1) + min);
}

static int linear_ease(int current_time, int initial_val, int change_val, int duration)
{
    return change_val * current_time / duration + initial_val;
}







static void init_app(void)
{
    for (int i = 0; i < SQUARE_COUNT; ++i)
    {
        int x_pos = rng(0, WINDOW_WIDTH - SQUARE_DIM);
        int y_pos = rng(0, WINDOW_HEIGHT - SQUARE_DIM);

        /*
        squares[i] = (SDL_Rect) {
            .x = x_pos,
            .y = y_pos,
            .w = SQUARE_DIM,
            .h = SQUARE_DIM
        };

        initial[i] = (SDL_Rect) {
            .x = x_pos,
            .y = y_pos,
            .w = SQUARE_DIM,
            .h = SQUARE_DIM
        };
        */

        app->squares[i] = (square_t) {
            .x = x_pos,
            .y = y_pos,
            .w = SQUARE_DIM,
            .h = SQUARE_DIM,
            .initial_x = x_pos,
            .initial_y = y_pos,
            .color = color(255, 255, 255, 255);
        };
    }
}

static void update_app(void)
{
    static unsigned int start_time = 0;
    static int tweening = 0;
    static int target_x = 0;
    static int target_y = 0;

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                running = 0;
            } break;

            case SDL_MOUSEBUTTONDOWN:
            {
                start_time = SDL_GetTicks(); 
                tweening = 1;
                int mouse_x, mouse_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);
                target_x = mouse_x - (SQUARE_DIM / 2);
                target_y = mouse_y - (SQUARE_DIM / 2);
            } break;

            default: break;
        }
    }

    int time = SDL_GetTicks() - start_time;

    if (tweening)
    {
        if (time < DURATION)
        {
            for (int i = 0; i < SQUARE_COUNT; ++i)
            {
                int change_x = target_x - initial[i].x;
                int change_y = target_y - initial[i].y;
                squares[i].x = linear_ease(time, initial[i].x, change_x, DURATION);
                squares[i].y = linear_ease(time, initial[i].y, change_y, DURATION);
            }
        }
        else
        {
            for (int i = 0; i < SQUARE_COUNT; ++i)
            {
                initial[i].x = target_x;
                initial[i].y = target_y;
            }
        }
    }
}

static void render_app(SDL_Renderer *renderer)
{
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int i = 0; i < SQUARE_COUNT; ++i)
    {
        SDL_RenderFillRect(renderer, &squares[i]);
    }

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderPresent(renderer);
}

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Squares",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    srand(time(NULL));
    running = 1;

    init_app();

    while (running)
    {
        update_app();
        render_app(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
