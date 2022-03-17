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
#include <SDL_mixer.h>

#include "fetch.h"
#include "ui.h"
#include "base64.h"
#include "main.h"
#include "account.h"
#include "song.h"

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
extern int song_len;
extern char *qr_msg;
extern char *check_msg;
extern Song *g_songs;
extern float percent;

int check_en = 1;
int fetch_song_en = 1;

int start = 0;
int index_song = 0;

    // Main program entrypoint
int main(int argc, char *argv[])
{
    socketInitializeDefault();
    nxlinkStdio();

    curl_global_init(CURL_GLOBAL_DEFAULT);
    romfsInit();
    
    int wait = 25;
    int exit_requested = 0;
    

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    Mix_Init(MIX_INIT_MP3);

    // open 44.1KHz, signed 16bit, system byte order,
    //  stereo audio, using 4096 byte chunks
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
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
        {0,0,0,0},
    };
    

    // load font from romfs
    TTF_Font *font = TTF_OpenFont("romfs:/data/simhei.ttf", 36);

    SDL_Window *window = SDL_CreateWindow("music", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (is_login())
    {
        printf("login\n");
        qr_msg = "enter X fetch songs, enter - logout";
        check_en = 0;
    }
    else
    {
        printf("try login\n");
        show_qr();
    }
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

                if (event.jbutton.button == JOY_Y) {
                    if (check_en) {
                        check_en = 0;
                        check();
                        check_en = 1;
                    }
                }

                if (event.jbutton.button == JOY_X)
                {
                    if (fetch_song_en)
                    {
                        fetch_song_en = 0;
                        fetch_songs_by_playlist("72614739");
                        fetch_song_en = 1;
                    }
                }

                if (event.jbutton.button == JOY_UP)
                {
                    index_song--;
                    if (index_song < 0) {
                        index_song = 0;
                    }
                }

                if (event.jbutton.button == JOY_DOWN)
                {
                    index_song++;
                    if (index_song >= song_len)
                    {
                        index_song = song_len - 1;
                    }
                }

                if (event.jbutton.button == JOY_A)
                {
                    //play
                    play2(&g_songs[index_song], play_next_song);
                }

                if (event.jbutton.button == JOY_B)
                {
                    //pause
                    pause2(&g_songs[index_song]);
                }

                if (event.jbutton.button == JOY_MINUS)
                {
                    // logout
                    logout2();
                    check_en = 1;
                    show_qr();
                }
            }
        }
        SDL_RenderClear(renderer);
        if (check_msg) {
            SDL_Rect t2_pos = {0, 44, 0, 0};
            render_text(renderer, check_msg, font, colors[3], &t2_pos);
        }
        if (qr_msg) {
            SDL_Rect t1_pos = {0, 0, 0, 0};
            render_text(renderer, qr_msg, font, colors[1], &t1_pos);
        }
        if (g_songs)
        {
            SDL_Rect t4_pos = {0, 132, 0, 0};
            start = render_list(renderer, g_songs, song_len, index_song, font, colors[1], colors[2], &t4_pos, 10, start);
        }
        if (display_qr)
        {
            SDL_Rect qr_pos = {0, 256, 0, 0};
            render_image(renderer, QR, &qr_pos);
        }
        if(percent > 0) {
            SDL_Rect p_pos = {12, 88, 600, 40};
            render_progress(renderer, percent, font, colors[1], colors[8], &p_pos);
        }
        SDL_RenderPresent(renderer);

        SDL_Delay(wait);
    }
    
    TTF_CloseFont(font);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    romfsExit();
    curl_global_cleanup();
    socketExit();
    base64_cleanup();
    return 0;
}