#ifndef GAME_H
#define GAME_H

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FALSE 0
#define TRUE 1

#define FPS 60 // frames por segundo
#define FRAME_TARGET_TIME (1000 / FPS) // un segundo dividido por los frames por segundo

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
        void check_collision(SDL_Rect rect);

        // variables publicas
        int game_is_running;

        SDL_Rect rRect; // rectangulo para colision

    private:

        // variables privadas
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *spriteTexture;

        float delta_time;
        int last_frame_time;
    
        float zoom;
        int framewidth, frameheight;
        int texturewidth, textureheight;
        int currentframe;
        int currentRow;

        // rectangulos para jugador
        SDL_Rect srcRect;
        SDL_Rect dstRect;     

        // struct para rectangulo de jugador
        struct sRect
        {
            bool moveUP, moveDOWN, moveLEFT, moveRIGHT;
            float x, y, width, height, angle, spd;
            float mousePosX, mousePosY;
        } sRect;

};

#endif
