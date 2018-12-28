#include <stdio.h>
/*
 * Only include emscripten file if we are using that compiler.
 * Also include any emscripten specific libraries here.
 */
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#endif

/**
 * Inverse square root of two, for normalising velocity
 */
#define REC_SQRT2 0.7071067811865475 

/**
 * Set of input states
 */
enum input_state
{
    NOTHING_PRESSED = 0,
    UP_PRESSED = 1,
    DOWN_PRESSED = 1<<1,
    LEFT_PRESSED = 1<<2,
    RIGHT_PRESSED = 1<<3
};

/**
 * Context structure that will be passed to the loop handler
 */
struct context
{
    SDL_Renderer *renderer;

    /**
     * Rectangle that the owl texture will be rendered into
     */
    SDL_Rect dest;
    SDL_Texture *owl_tex;

    /**
     * Font that is used for rendering text, and
     * a texture the text is rendered into
     */
    TTF_Font *font;
    SDL_Texture *text_tex;

    enum input_state active_state;

    /**
     * x and y components of owl's velocity
     */
    int owl_vx;
    int owl_vy;
    bool running;
};

/**
 * Loads the owl texture into the context
 */
int get_owl_texture(struct context *ctx)
{
  SDL_Surface *image = IMG_Load("assets/owl.png");
  if (!image)
  {
     printf("IMG_Load: %s\n", IMG_GetError());
     return 0;
  }
  ctx->owl_tex = SDL_CreateTextureFromSurface(ctx->renderer, image);
  ctx->dest.w = image->w;
  ctx->dest.h = image->h;

  SDL_FreeSurface (image);

  return 1;
}

/**
 * Set the context's text texture to show the text 'text' 
 */
void set_font_text(struct context *ctx, const char *text)
{
	SDL_Color fg = {0,0,0,255};
    SDL_Surface *text_surface = TTF_RenderText_Blended(ctx->font, text, fg);
    ctx->text_tex = SDL_CreateTextureFromSurface(ctx->renderer, text_surface);
    SDL_FreeSurface(text_surface);
}

/**
 * Load the font we're going to use and set the text to
 * be "Hello owl!"
 */
int get_font_texture(struct context *ctx)
{
    ctx->font = TTF_OpenFont("assets/FreeSans.ttf", 30);
    set_font_text(ctx, "Hello owl!");
    return 1;
}

/**
 * Processes the input events and sets the velocity
 * of the owl accordingly
 */
void process_input(struct context *ctx)
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.key.keysym.sym)
        {
            case SDLK_UP:
                if (event.key.type == SDL_KEYDOWN)
                    ctx->active_state = (input_state) (ctx->active_state | UP_PRESSED);
                else if (event.key.type == SDL_KEYUP)
                    ctx->active_state = (input_state) (ctx->active_state ^ UP_PRESSED);
                break;
            case SDLK_DOWN:
                if (event.key.type == SDL_KEYDOWN)
                    ctx->active_state = (input_state) (ctx->active_state | DOWN_PRESSED);
                else if (event.key.type == SDL_KEYUP)
                    ctx->active_state = (input_state) (ctx->active_state ^ DOWN_PRESSED);
                break;
            case SDLK_LEFT:
                if (event.key.type == SDL_KEYDOWN)
                    ctx->active_state = (input_state) (ctx->active_state | LEFT_PRESSED);
                else if (event.key.type == SDL_KEYUP)
                    ctx->active_state = (input_state) (ctx->active_state ^ LEFT_PRESSED);
                break;
            case SDLK_RIGHT:
                if (event.key.type == SDL_KEYDOWN)
                    ctx->active_state = (input_state) (ctx->active_state | RIGHT_PRESSED);
                else if (event.key.type == SDL_KEYUP)
                    ctx->active_state = (input_state) (ctx->active_state ^ RIGHT_PRESSED);
                break;
            case SDLK_ESCAPE:
                #ifdef __EMSCRIPTEN__
                emscripten_cancel_main_loop();
                #endif
                ctx->running = false;
            default:
                break;
        }
    }

    ctx->owl_vy = 0;
    ctx->owl_vx = 0;
    if (ctx->active_state & UP_PRESSED)
        ctx->owl_vy = -5;
    if (ctx->active_state & DOWN_PRESSED)
        ctx->owl_vy = 5;
    if (ctx->active_state & LEFT_PRESSED)
        ctx->owl_vx = -5;
    if (ctx->active_state & RIGHT_PRESSED)
        ctx->owl_vx = 5;

    if (ctx->owl_vx != 0 && ctx->owl_vy != 0)
    {
        ctx->owl_vx *= REC_SQRT2;
        ctx->owl_vy *= REC_SQRT2;
    }
}

/**
 * Loop handler that gets called each animation frame,
 * process the input, update the position of the owl and 
 * then render the texture
 */
void loop_handler(void *arg)
{
    struct context *ctx = (struct context *) arg;

    int vx = 0;
    int vy = 0;
    //move the owl
    process_input(ctx);

    ctx->dest.x += ctx->owl_vx;
    ctx->dest.y += ctx->owl_vy;

    //clear the screen
    SDL_RenderClear(ctx->renderer);
    //copy the image to renderer in the new location described by the destination
    SDL_RenderCopy(ctx->renderer, ctx->owl_tex, NULL, &ctx->dest);

    SDL_Rect text_dest = {.x = 50, .y = 175, .w = 0, .h = 0};
    SDL_QueryTexture(ctx->text_tex,
            NULL, NULL, &text_dest.w, &text_dest.h);
    SDL_RenderCopy(ctx->renderer, ctx->text_tex, NULL, &text_dest);

    SDL_RenderPresent(ctx->renderer);
}

struct context initializeSDL() {
    SDL_Window *window;
    struct context ctx;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_CreateWindowAndRenderer(600, 400, 0, &window, &ctx.renderer);
    SDL_SetRenderDrawColor(ctx.renderer, 255, 255, 255, 255);

    get_owl_texture(&ctx);
    get_font_texture(&ctx);
    ctx.active_state = NOTHING_PRESSED;
    ctx.dest.x = 200;
    ctx.dest.y = 100;
    ctx.owl_vx = 0;
    ctx.owl_vy = 0;
    ctx.running = true;
    return ctx;
}

#ifdef __EMSCRIPTEN__
#define BROWSER_REFRESH_RATE_FOR_FPS -1
//#define SIMULATE_INFINITE_LOOP_TRUE 1
static int SIMULATE_INFINITE_LOOP_TRUE = 1;
extern "C" void mainWasm()
{
    struct context ctx = initializeSDL();
    try {
        /**
         * Schedule the main loop handler to get 
         * called on each animation frame.
         * 1st arg = looped method
         * 2nd arg = the context we are passing to the looped method
         * 3rd arg = frames per second, -1 sets this to the browser's animation refresh rate
         * 4th arg = determines whether an infinite loop is simulated of the loop is run asynchronously from this method
         */
        emscripten_set_main_loop_arg(loop_handler, &ctx, BROWSER_REFRESH_RATE_FOR_FPS, SIMULATE_INFINITE_LOOP_TRUE);
    } catch (...) {
        printf("Failure in main loop! Exiting...");
    }
}
#else
#define DESIRED_FRAME_RATE 60
#define MS_PER_FRAME = 1000 / DESIRED_FRAME_RATE
int main(int argc, char* args[])
{
    SDL_Window* window = NULL;
    struct context ctx = initializeSDL();
    while(ctx.running) {
        Uint32 endTime = SDL_GetTicks() + MS_PER_FRAME;
        loop_handler(&ctx);
        while(SDL_GetTicks() < endTime) {
            SDL_Delay(endTime - SDL_GetTicks());
        }
    }
}
#endif
