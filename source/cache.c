#include <stdio.h>
#include <unistd.h>

int song_cache(int id){
    char file_name[200] = {0};
    snprintf(file_name, sizeof(file_name), "%d%s", id, ".mp3");

    // has file and file size >= 1024b

    if (access(file_name, F_OK) == 0)
    {
        // file exists
        FILE *file = fopen(file_name, "r");
        char buffer[1024];
        int r = fread(buffer, 1,1024,file);
        printf("read %d \n", r);
        return r == 1024;
    }
    else
    {
        // file doesn't exist
        return 0;
    }
}