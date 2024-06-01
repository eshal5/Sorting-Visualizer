
// main.cpp
#include "visualizer.hpp"
#include <SDL.h>
#define WIN_WIDTH 800 // screen size
#define WIN_HEIGHT 800
int main(int argc, char **argv)
{

    
    
    Visualizer v; // 
    srand(time(NULL));
    if (!v.init())
    {
        printf("Failed to initialize!\n");
        return 0;
    }
    v.loadMedia();
    v.run();  
    v.close(); 

    return 0;
}
//command
// g++ *.cpp -IC:\mingw_dev_lib\include\SDL2 -LC:\mingw_dev_lib\lib -w -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer