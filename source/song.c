#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "song.h"
#include "fetch.h"

void play2(const Song *s) {
    request_song(s);
}

void pause2(const Song *s)
{

}
