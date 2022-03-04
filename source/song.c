#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "song.h"
#include "fetch.h"

void play2(const Song *s) {
    request_song(s);
    printf("url:%s", s->url);
}

void pause2(const Song *s)
{

}
