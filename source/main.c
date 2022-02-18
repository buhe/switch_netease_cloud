#include <string.h>
#include <stdio.h>

#include <switch.h>

int main(int argc, char* argv[]) {
    consoleInit(NULL);              // Initialize the console; redirect printf to the console

    printf("Hello bugu!\n");       // Print Hello World to the console

    while (appletMainLoop()) {      // while the application hasn't received an exit request...
        consoleUpdate(NULL);        // Update the screen
    }

    consoleExit(NULL);              // Clean up
    return 0;
}