#include <stdio.h>

#include "interface/interface.hpp"

Interface application;

int main() {
    application.start();


    float delta = 0.0;
    do {
        // game logic I guess
        //printf("delta = %f\n", delta);

    } while (application.refresh(&delta));
    
    application.stop();
    return 0;   
}