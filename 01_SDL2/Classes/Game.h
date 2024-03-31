#ifndef GAME_H
#define GAME_H

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FALSE 0
#define TRUE 1

#include <SDL2/SDL.h>
#include <stdio.h>

class Game {

    public:

        Game(); // constructor
        ~Game(); // destructor

        // metodos publicos
        void initialize_window();
        void setup();
        void process_input();
        void update();
        void render();
        void destroy_window();

        // variables publicas
        int game_is_running;

    private:

        // variables privadas
        SDL_Window *window;
        SDL_Renderer *renderer;
};

#endif
