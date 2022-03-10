#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "song.h"

void show_login_qr();

void dispose_login_qr();

void render_text(SDL_Renderer *renderer, const char *text, TTF_Font *font, SDL_Color color, SDL_Rect *rect);

void render_image(SDL_Renderer *renderer, const char *path, SDL_Rect *rect);

int render_list(SDL_Renderer *renderer, const Song *song, const int song_len, const int index, TTF_Font *font, SDL_Color color, SDL_Color selected_color, SDL_Rect *rect, int item_size, int start);

void render_progress(SDL_Renderer *renderer, const float progress, TTF_Font *font, SDL_Color color, SDL_Color b_color, SDL_Rect *rect);