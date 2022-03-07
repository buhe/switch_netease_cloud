#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL_mixer.h>

#include "song.h"
#include "fetch.h"

Mix_Music *audio = NULL;
void play2(const Song *s)
{
    request_song(s);
    char url[S_STR_SIZE] = {0};
    snprintf(url, sizeof(url), "%d.mp3", s->id);
    printf("audio:%s\n", url);
    if(audio){
        Mix_FreeMusic(audio);
        audio = NULL;
    }
    audio = Mix_LoadMUS(url);
    if(audio){
        Mix_PlayMusic(audio, 1); // Play the audio file
    }
}

void pause2(const Song *s)
{
    if (audio)
    {
        Mix_PauseMusic();
    }
}

