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
#include <SDL2/SDL_ttf.h>

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
        SDL_Texture *load_texture(const char *path);
        int check_collision(SDL_Rect myRect, SDL_Rect rect);
        void dialog_box(const char *message);
        TTF_Font *load_font(const char *path, int size);

        // variables publicas

        // flag para saber si el juego esta corriendo
        int game_is_running;

        // rectangulo de colision
        SDL_Rect rRect;

        // burbuja de dialogo
        struct bubbleDialogTxt
        {
            SDL_Rect dialogRect;
            SDL_Color textColor;
            SDL_Surface *textSurface;
            SDL_Texture *surfaceText;
            TTF_Font *font;
            int sizeFont, h_font, w_font;
            int textW, textH, text_x, text_y;

        } bubbletxt;    

        // struct para rectangulo de jugador
        struct sRect
        {
            SDL_Rect srcRect;
            SDL_Rect dstRect;
            bool moveUP, moveDOWN, moveLEFT, moveRIGHT;
            float x, y, width, height, angle, spd;
            float mousePosX, mousePosY;
        } sRect;

        // manejo de sprites del jugador
        struct playerSpriteManager
        {
            SDL_Texture *spriteTexture;
            SDL_Texture *texture;
            int framewidth, frameheight;
            int texturewidth, textureheight;
            int currentframe;
            int currentRow;
        } playerSprite;


    private:

        // variables privadas

        SDL_Window *window;
        SDL_Renderer *renderer;

        float delta_time;
        int last_frame_time;

        float zoom;
        bool collision_flag;
};

#endif
