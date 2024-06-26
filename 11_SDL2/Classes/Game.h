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

        /* metodos publicos */

        void initialize_window();
        void setup();
        void process_input();
        void update();
        void render();
        void destroy_window();
        SDL_Texture *load_texture(const char *path);

        /* variables publicas */

        int game_is_running; // flag para saber si el juego esta corriendo

        // struct para rectangulo de animacion
        struct sRect
        {
            SDL_Rect srcRect;
            SDL_Rect dstRect;
            float x, y, width, height;
        } sRect;

        // manejo de sprites de la animacion
        struct SpriteManager
        {
            SDL_Texture *spriteTexture;
            SDL_Texture *texture;
            int framewidth, frameheight;
            int texturewidth, textureheight;
            int currentframe;
            int currentRow;
            int total_frames;
            int total_rows;
            int frames_per_row;
            int animation_spd;
        } spriteManager;

        float zoom; // zoom de la animacion

    private:

        // variables privadas

        SDL_Window *window;
        SDL_Renderer *renderer;

        float delta_time;
        int last_frame_time;
};

#endif
