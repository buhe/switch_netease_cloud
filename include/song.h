#define download 0
#define playing 1
#define paused 2

typedef struct
{
    int id;
    char name[100];
    char url[100];
    int status;
} Song;