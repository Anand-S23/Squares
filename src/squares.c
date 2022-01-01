#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "squares.h"

static app_t app;

// Tween Functions //

static int linear_ease(int current_time, int initial_val, int change_val, int duration)
{
    return change_val * current_time / duration + initial_val;
}

// Core // 

static void init_app(app_t *app)
{
    app->running = 1;
    app->tweening = 0;
    app->finished_tweening = 0;
    app->start_time = 0;
    app->target_x = 0;
    app->target_y = 0;

    for (int i = 0; i < SQUARE_COUNT; ++i)
    {
        int x_pos = rng(0, WINDOW_WIDTH - SQUARE_DIM);
        int y_pos = rng(0, WINDOW_HEIGHT - SQUARE_DIM);

        color_t color = (color_t) {
            .r = 255,
            .g = 255,
            .b = 255,
            .a = 255
        };

        app->squares[i] = (square_t) {
            .x = x_pos,
            .y = y_pos,
            .w = SQUARE_DIM,
            .h = SQUARE_DIM,
            .initial_x = x_pos,
            .initial_y = y_pos,
            .color = color
        };
    }
}

static void update_app(app_t *app)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                app->running = 0;
            } break;

            case SDL_MOUSEBUTTONDOWN:
            {
                if (app->tweening && app->finished_tweening)
                {
                    init_app(app);
                    return;
                }
                else
                {
                    // Initialize tween
                    app->tweening = 1;
                    app->start_time = SDL_GetTicks(); 

                    // Get click info
                    int mouse_x, mouse_y;
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    app->target_x = mouse_x - (SQUARE_DIM / 2);
                    app->target_y = mouse_y - (SQUARE_DIM / 2);
                }
            } break;

            default: break;
        }
    }

    int time = SDL_GetTicks() - app->start_time;

    if (app->tweening)
    {
        if (time < DURATION)
        {
            for (int i = 0; i < SQUARE_COUNT; ++i)
            {
                int initial_x = app->squares[i].initial_x;
                int initial_y = app->squares[i].initial_y;
                int change_x = app->target_x - initial_x;
                int change_y = app->target_y - initial_y;
                app->squares[i].x = linear_ease(time, initial_x, change_x, DURATION);
                app->squares[i].y = linear_ease(time, initial_y, change_y, DURATION);
            }
        }
        else
        {
            app->finished_tweening = 1;

            for (int i = 0; i < SQUARE_COUNT; ++i)
            {
                app->squares[i].initial_x = app->target_x;
                app->squares[i].initial_y = app->target_y;
            }
        }
    }
}

static void render_app(SDL_Renderer *renderer, app_t *app)
{
    SDL_RenderClear(renderer);

    for (int i = 0; i < SQUARE_COUNT; ++i)
    {
        SDL_Rect current_square = (SDL_Rect) {
            .x = app->squares[i].x,
            .y = app->squares[i].y,
            .w = app->squares[i].w,
            .h = app->squares[i].h
        };

        color_t color = app->squares[i].color;

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &current_square);
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
    init_app(&app);

    while (app.running)
    {
        update_app(&app);
        render_app(renderer, &app);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
