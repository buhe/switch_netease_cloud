typedef struct
{
    int id;
    char name[100];
    char url[100];
    /* data */
} Song;
void play() {

}

void pause() {

}

enum status {
    download,
    playing,
    paused,
} s;