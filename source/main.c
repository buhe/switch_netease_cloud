// Include the most common headers from the C standard library
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// Include the main libnx system header, for Switch development
#include <switch.h>
#include <curl/curl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "fetch.h"

#define SCREEN_W 1280
#define SCREEN_H 720

// some switch buttons
#define JOY_A 0
#define JOY_B 1
#define JOY_X 2
#define JOY_Y 3
#define JOY_PLUS 10
#define JOY_MINUS 11
#define JOY_LEFT 12
#define JOY_UP 13
#define JOY_RIGHT 14
#define JOY_DOWN 15

extern int display_qr;
extern char *fetch_err;
extern char *check_msg;

// Main program entrypoint
int main(int argc, char *argv[])
{
    // This example uses a text console, as a simple way to output text to the screen.
    // If you want to write a software-rendered graphics application,
    //   take a look at the graphics/simplegfx example, which uses the libnx Framebuffer API instead.
    // If on the other hand you want to write an OpenGL based application,
    //   take a look at the graphics/opengl set of examples, which uses EGL instead.
    // consoleInit(NULL);

    // // Configure our supported input layout: a single player with standard controller styles
    // padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    // // Initialize the default gamepad (which reads handheld mode inputs as well as the first connected controller)
    // PadState pad;
    // padInitializeDefault(&pad);

    // socketInitializeDefault();
    // struct stat st = {0};

    // if (stat("/song", &st) == -1)
    // {
    //     mkdir("/song", 0700);
    // }

    // // network_request();
    // // fetch_songs_by_playlist("1");
    // printf("curl init\n");
    // curl_global_init(CURL_GLOBAL_DEFAULT);
    // login();
    // curl_global_cleanup();
    // // Main loop
    // while (appletMainLoop())
    // {
    //     // Scan the gamepad. This should be done once for each frame
    //     padUpdate(&pad);

    //     // padGetButtonsDown returns the set of buttons that have been
    //     // newly pressed in this frame compared to the previous one
    //     u64 kDown = padGetButtonsDown(&pad);

    //     if (kDown & HidNpadButton_Plus)
    //         break; // break in order to return to hbmenu

    //     // Your code goes here

    //     // Update the console, sending a new frame to the display
    //     consoleUpdate(NULL);
    // }

    // socketExit();
    // // Deinitialize and clean up resources used by the console (important!)
    // consoleExit(NULL);
    // return 0;
    struct stat st = {0};
    if (stat("/song", &st) == -1)
    {
        mkdir("/song", 0700);
    }

    socketInitializeDefault();
    romfsInit();
    // chdir("romfs:/");
    
    int wait = 16;
    int exit_requested = 0;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    SDL_JoystickEventState(SDL_ENABLE);
    SDL_JoystickOpen(0);

    SDL_Event event;
    SDL_Color colors[] = {
        {128, 128, 128, 0}, // gray
        {255, 255, 255, 0}, // white
        {255, 0, 0, 0},     // red
        {0, 255, 0, 0},     // green
        {0, 0, 255, 0},     // blue
        {255, 255, 0, 0},   // brown
        {0, 255, 255, 0},   // cyan
        {255, 0, 255, 0},   // purple
    };
    

    // load font from romfs
    TTF_Font *font = TTF_OpenFont("romfs:/data/LeroyLetteringLightBeta01.ttf", 36);

    SDL_Window *window = SDL_CreateWindow("sdl2+mixer+image+ttf demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    
    
    SDL_Texture *helloworld_tex = NULL;
    SDL_Texture *check_tex = NULL;
    SDL_Texture *sdllogo_tex = NULL;
    
 
    printf("curl init\n");
    curl_global_init(CURL_GLOBAL_DEFAULT);
    login();
    curl_global_cleanup();
    // Main loop
    while (!exit_requested && appletMainLoop())
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                exit_requested = 1;

            // use joystick
            if (event.type == SDL_JOYBUTTONDOWN)
            {
                if (event.jbutton.button == JOY_PLUS)
                    exit_requested = 1;

                if (event.jbutton.button == JOY_Y)
                    check();
            }
        }
        if (check_msg != NULL) {
            SDL_Surface *check_surface;
            SDL_Rect check_rect = {0, 300, 0, 0};
            check_surface = TTF_RenderText_Solid(font, check_msg, colors[3]);
            check_tex = SDL_CreateTextureFromSurface(renderer, check_surface);
            (&check_rect)->w = check_surface->w;
            (&check_rect)->h = check_surface->h;

            SDL_FreeSurface(check_surface);
            if (check_tex)
                SDL_RenderCopy(renderer, check_tex, NULL, &check_rect);
        }

        if (fetch_err != NULL)
        {
            SDL_Surface *surface;
            SDL_Rect rect = {0, 200, 0, 0};
            surface = TTF_RenderText_Solid(font, fetch_err, colors[3]);
            helloworld_tex = SDL_CreateTextureFromSurface(renderer, surface);
            (&rect)->w = surface->w;
            (&rect)->h = surface->h;

            SDL_FreeSurface(surface);
            if (helloworld_tex)
                SDL_RenderCopy(renderer, helloworld_tex, NULL, &rect);
        }

        if (display_qr)
        {
            SDL_Surface *sdllogo = IMG_Load("/song/qr.png");
            SDL_Rect sdl_pos = {0, 0, 0, 0};
            if (sdllogo)
            {
                sdl_pos.w = sdllogo->w;
                sdl_pos.h = sdllogo->h;
                sdllogo_tex = SDL_CreateTextureFromSurface(renderer, sdllogo);
                SDL_FreeSurface(sdllogo);
            }
            // put logos on screen
            if (sdllogo_tex)
                SDL_RenderCopy(renderer, sdllogo_tex, NULL, &sdl_pos);
        }
        SDL_RenderPresent(renderer);

        SDL_Delay(wait);
    }

    // clean up your textures when you are done with them
    if (helloworld_tex)
        SDL_DestroyTexture(helloworld_tex);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    romfsExit();
    socketExit();
    return 0;
}