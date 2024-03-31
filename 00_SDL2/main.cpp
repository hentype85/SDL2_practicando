#include <stdio.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FALSE 0
#define TRUE 1

// variables globales
int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer *renderer = NULL;


int initialize_window() {
    // inicializar SDL con SDL_INIT_EVERYTHING que inicializa todos los subsistemas de SDL
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return FALSE;
    }

    // crear ventana SDL con titulo, posicion x, posicion y, ancho, alto y flag de ventana
    window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);
    if (!window) {
        printf("Error creating SDL window: %s\n", SDL_GetError());
        return FALSE;
    }

    // crear un renderizador que dibuja la ventana, -1 especifica que queremos cargar el controlador de video por defecto, 0 indica que no queremos habilitar la aceleracion de hardware
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        printf("Error creating SDL renderer: %s\n", SDL_GetError());
        return FALSE;
    }    

    return TRUE;
}

void process_input(){
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type) {
        // si el evento es SDL_QUIT, cerrar la ventana
        case SDL_QUIT:
            game_is_running = FALSE;
            break;
        // si el evento es SDL_KEYDOWN, verificar que tecla fue presionada
        case SDL_KEYDOWN:
            // si la tecla presionada es ESCAPE, cerrar la ventana
            if (event.key.keysym.sym == SDLK_ESCAPE)
                game_is_running = FALSE;
            break;
    }
}

void setup() {

}

void update() {

}

void render() {

}

void destroy_window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    game_is_running = initialize_window();

    setup();

    // bucle principal del juego
    while(game_is_running) {
        process_input();
        update();
        render();
    }

    // limpiar la ventana y cerrar SDL
    destroy_window();

    return 0;
}