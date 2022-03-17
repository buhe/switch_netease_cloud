#define download 0
#define ready 1
#define playing 2
#define paused 3

#ifndef SONG
#define SONG

typedef struct
{
    int id;
    char name[100];
    char url[100];
    int status;
} Song;

#endif

void play2(const Song *s, void (*song_finished)(void));

void pause2(const Song *s);

void play_next_song();