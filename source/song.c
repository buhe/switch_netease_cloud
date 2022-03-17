#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL_mixer.h>

#include "song.h"
#include "fetch.h"

extern int index_song;
extern Song *g_songs;
extern int song_len;

void play_next_song()
{
    printf("play next song\n");
    index_song++;
    if (g_songs && index_song == song_len)
    {
        printf("but currnt song is last..\n");
        return;
    }
    play2(&g_songs[index_song], play_next_song);
}
Mix_Music *audio = NULL;
void play2(const Song *s, void (*song_finished)(void))
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
        Mix_HookMusicFinished(song_finished);
    }
}

void pause2(const Song *s)
{
    if (audio)
    {
        Mix_PauseMusic();
    }
}

