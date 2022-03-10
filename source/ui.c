#include "ui.h"

int display_qr = 0;

void show_login_qr()
{
    display_qr = 1;
}

void dispose_login_qr()
{
    display_qr = 0;
}
void render_image(SDL_Renderer *renderer, const char *path, SDL_Rect *rect)
{
    SDL_Surface *surface;
    SDL_Texture *texture;
    surface = IMG_Load(path);
    rect->w = surface->w;
    rect->h = surface->h;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, rect);
    SDL_DestroyTexture(texture);
}
void render_text(SDL_Renderer *renderer, const char *text, TTF_Font *font, SDL_Color color, SDL_Rect *rect)
{
    SDL_Surface *surface;
    SDL_Texture *texture;

    surface = TTF_RenderUTF8_Solid(font, text, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect->w = surface->w;
    rect->h = surface->h;

    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, rect);
    SDL_DestroyTexture(texture);
}

void render_progress(SDL_Renderer *renderer, const float progress, TTF_Font *font, SDL_Color color, SDL_Color b_color, SDL_Rect *rect)
{
    // printf("renderer list\n");
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(renderer, rect);
    SDL_SetRenderDrawColor(renderer, b_color.r, b_color.g, b_color.b, b_color.a);
    SDL_Surface *surface;
    SDL_Texture *texture;
    char text[100] = {0};
    snprintf(text, sizeof(text), "%3.0f%%", progress);
    surface = TTF_RenderUTF8_Solid(font, text, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect->x = rect->w / 2 + rect->x - 44;
    rect->w = surface->w;
    rect->h = surface->h;

    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, rect);
    SDL_DestroyTexture(texture);
}

int render_list(SDL_Renderer *renderer, const Song *song, const int song_len, const int index, TTF_Font *font, SDL_Color color, SDL_Color selected_color, SDL_Rect *rect, int item_size, int start)
{
    if (song_len < item_size) {
        item_size = song_len;
    }
    if(start + item_size - 1 < index){
        start = index - (item_size - 1);
    } 
    if(start > index){
        start = index;
    }
    int i, phy_index;
    for (i = 0; i < item_size; i++)
    {
        
        SDL_Surface *surface;
        SDL_Texture *texture;
        phy_index = start + i;

        if (phy_index == index) {
            surface = TTF_RenderUTF8_Solid(font, song[phy_index].name, selected_color);
        } else{
            surface = TTF_RenderUTF8_Solid(font, song[phy_index].name, color);
        }

        texture = SDL_CreateTextureFromSurface(renderer, surface);
        rect->w = surface->w;
        rect->h = surface->h;
        
        SDL_FreeSurface(surface);
        SDL_RenderCopy(renderer, texture, NULL, rect);
        rect->y = rect->y + 44;
        SDL_DestroyTexture(texture);
    }
    return start;
}