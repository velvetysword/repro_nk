#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define NK_BUTTON_TRIGGER_ON_RELEASE

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION

#include "nuklear.h"
#include "nuklear_sdl_gl3.h"

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

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

int init_sdl_win(SDL_Window **win, SDL_GLContext *glContext, struct nk_context **ctx) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    *win = SDL_CreateWindow(
        "Repro",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 800,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    *glContext = SDL_GL_CreateContext(*win);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        SDL_Log("Failed to initialize GLEW");
        return -1;
    }

    *ctx = nk_sdl_init(*win);
    
    { // font
        struct nk_font_atlas *atlas;
        nk_sdl_font_stash_begin(&atlas);
        nk_sdl_font_stash_end();
    }

    return 0;
}

void render_sdl_ui(SDL_Window **win, struct nk_context **ctx) {
    // gui
    int win_width, win_height;
    SDL_GetWindowSize(*win, &win_width, &win_height);

    // same 2 functions
    render_widget_list(ctx, win_width, win_height);

    // render
    glViewport(0, 0, win_width, win_height);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.24f, 0.24f, 0.24f, 1.0f);
    nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
    SDL_GL_SwapWindow(*win);

}

int cleanup(SDL_Window **win, SDL_GLContext *glContext) {
    nk_sdl_shutdown();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(*win);
    SDL_Quit();
    return 0;
}

int main(void) {
    SDL_Window *win;
    SDL_GLContext glContext;
    struct nk_context *ctx;

    if (init_sdl_win(&win, &glContext, &ctx) != 0) {
        return -1;
    }

    while (1) {
        SDL_Event evt;

        nk_input_begin(ctx);
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_QUIT) return cleanup(&win, &glContext);
            nk_sdl_handle_event(&evt);
        }
        nk_input_end(ctx);

        render_sdl_ui(&win, &ctx);
    }
}

