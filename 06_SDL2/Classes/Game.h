#ifndef GAME_H
#define GAME_H

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FALSE 0
#define TRUE 1

#define FPS 60 // frames por segundo
#define FRAME_TARGET_TIME (1000 / FPS) // tiempo que debe pasar para que se actualice un frame

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <math.h>


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
        SDL_Texture* load_texture(const char* path);

        // variables publicas
        int game_is_running;
        int last_frame_time;
        float delta_time;
        float zoom = 10.0f;

        // struct para un rectangulo
        struct sRect
        {
            bool moveUP, moveDOWN, moveLEFT, moveRIGHT;
            float x, y, width, height, angle, spd;
            float mousePosX, mousePosY;
        } sRect;

    private:

        // variables privadas
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *playerTexture;
};

#endif
