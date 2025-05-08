#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "nuklear.h"

#include <stdio.h>
#include "gui.h"

void render_widget_list(struct nk_context **ctx, int win_width, int win_height)
{
    if (nk_begin(*ctx, "list1", nk_rect(0, 0, win_width, win_height), NK_WINDOW_BORDER | NK_WINDOW_TITLE))
    {
        struct nk_rect bounds = nk_window_get_bounds(*ctx);
        struct nk_rect content = nk_window_get_content_region(*ctx);
        
        float title_bar_height = content.y - bounds.y;
        float padding = (*ctx)->style.window.header.padding.y;

        nk_layout_row_dynamic(*ctx, win_height - title_bar_height - padding * 5, 1);

        if (nk_group_begin(*ctx, "list1", 0)) {
            for (int i = 0; i < 100; i++) {
                nk_layout_row_dynamic(*ctx, 30, 1);
                char buffer[256];
                sprintf(buffer, "Element %d", i);
                nk_label(*ctx, buffer, NK_TEXT_LEFT);
            }
            
            nk_group_end(*ctx);
        }
    }
    nk_end(*ctx);
}

