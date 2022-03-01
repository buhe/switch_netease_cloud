#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

void show_login_qr();

SDL_Texture *render_text(SDL_Renderer *renderer, const char *text, TTF_Font *font, SDL_Color color, SDL_Rect *rect);

SDL_Texture *render_image(SDL_Renderer *renderer, const char *path, SDL_Rect *rect);