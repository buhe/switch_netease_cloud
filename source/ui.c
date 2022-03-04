#include "ui.h"

int display_qr = 0;

void show_login_qr()
{
    display_qr = 1;
}
SDL_Texture *render_image(SDL_Renderer *renderer, const char *path, SDL_Rect *rect)
{
    SDL_Surface *surface;
    SDL_Texture *texture;
    surface = IMG_Load(path);
    rect->w = surface->w;
    rect->h = surface->h;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
SDL_Texture *render_text(SDL_Renderer *renderer, const char *text, TTF_Font *font, SDL_Color color, SDL_Rect *rect)
{
    SDL_Surface *surface;
    SDL_Texture *texture;

    surface = TTF_RenderUTF8_Solid(font, text, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect->w = surface->w;
    rect->h = surface->h;

    SDL_FreeSurface(surface);

    return texture;
}

int render_list(SDL_Renderer *renderer, const Song *song, const int song_len, const int index, TTF_Font *font, SDL_Color color, SDL_Color selected_color, SDL_Rect *rect, int item_size, int start)
{
    if (song_len < item_size) {
        item_size = song_len;
    }
    if(start + item_size < index){
        start = index - item_size;
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
            // color.a = 255;
            surface = TTF_RenderUTF8_Solid(font, song[phy_index].name, selected_color);
        } else{
            surface = TTF_RenderUTF8_Solid(font, song[phy_index].name, color);
        }

        
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        rect->w = surface->w;
        rect->h = surface->h;
        rect->y = rect->y + 44;
        SDL_FreeSurface(surface);
        SDL_RenderCopy(renderer, texture, NULL, rect);
        SDL_DestroyTexture(texture);
    }
    return start;
}