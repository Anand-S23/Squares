#include <stdio.h>
#include <stdlib.h>

#define GS_IMPL
#include <gs/gs.h>

#define GS_IMMEDIATE_DRAW_IMPL
#include <gs/util/gs_idraw.h>

gs_command_buffer_t  cb   = {0};
gs_immediate_draw_t  gsi  = {0};

void init()
{
    cb = gs_command_buffer_new(); 
    gsi = gs_immediate_draw_new();
}

void update()
{
    gsi_camera2D(&gsi);


    gsi_render_pass_submit(&gsi, &cb, gs_color(10, 10, 10, 255));
    gs_graphics_submit_command_buffer(&cb);
}

gs_app_desc_t gs_main(int32_t argc, char** argv)
{
    return (gs_app_desc_t) {
        .init = init,
        .update = update
    };
}   

