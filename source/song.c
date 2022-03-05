#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "song.h"
#include "fetch.h"
// extern int g_id;
// extern const char *g_url;
void play2(const Song *s) {
    request_song(s);
    // printf("%d%s!!!!\n", g_id, g_url);
    // fetch_song(g_id,g_url);
    
}

void pause2(const Song *s)
{

}
