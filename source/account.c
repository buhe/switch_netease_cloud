#include <stdio.h>
#include <unistd.h>

#include "account.h"

int is_login() {
    // file exist?
    if (access(LOGIN, F_OK) == 0)
    {
        // file exists
        return 1;
    }
    else
    {
        // file doesn't exist
        return 0;
    }
}

void login2() {
    // set file
    FILE *file = fopen(LOGIN, "wb");
    if (file != NULL)
    {
        fclose(file);
    }
}

void logout2() {
    // rm file
    remove(LOGIN);
}