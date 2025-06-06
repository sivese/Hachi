#include <stdio.h>
#include <emscripten.h>

extern "C" {
    EMSCRIPTEN_KEEPALIVE void reset() {
        printf("Resetting...\n");
    }

    EMSCRIPTEN_KEEPALIVE void pause() {
        printf("Pausing...\n");
    }

    EMSCRIPTEN_KEEPALIVE void load_rom() {
        printf("Loading ROM...\n");
    }
}

int main() {
    printf("Hello World!\n");
    return 0;
}
