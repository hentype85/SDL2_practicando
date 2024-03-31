#include "Game.h"

// constructor
Game::Game() {
    game_is_running = FALSE;
    window = NULL;
    renderer = NULL;
}
// destructor
Game::~Game() {
    destroy_window();
}

// metodos

// inicializar la ventana del juego
void Game::initialize_window() {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        game_is_running = FALSE;
        return;
    }

    window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);
    if (!window) {
        printf("Error creating SDL window: %s\n", SDL_GetError());
        game_is_running = FALSE;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        printf("Error creating SDL renderer: %s\n", SDL_GetError());
        game_is_running = FALSE;
        return;
    }

    game_is_running = TRUE;
}

// procesar el input del usuario
void Game::process_input() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
        game_is_running = FALSE;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            game_is_running = FALSE;
        break;
    }


}

void Game::setup() {
    sRect.width = 15;
    sRect.height = 15;
    sRect.x = WINDOW_WIDTH / 2; // rectangulo en el centro de la pantalla
    sRect.y = WINDOW_HEIGHT / 2; // rectangulo en el centro de la pantalla
    sRect.angle = 0.0f; // angulo
    sRect.spd = 5.0f; // velocidad
}

void Game::update() {

    // esperar hasta que sea tiempo de renderizar el siguiente frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));

    // delta time es la diferencia de tiempo entre frames en segundos
    delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    // tiempo actual en milisegundos
    last_frame_time = SDL_GetTicks();

    // coordenadas basadas en el angulo del rectangulo
    sRect.x = WINDOW_WIDTH / 2; // centrar rectangulo en x
    sRect.y = WINDOW_HEIGHT / 2; // centrar rectangulo en y
    // sBall.x += 10 * delta_time; // mover el rectangulo 10 pixeles hacia la derecha
    // sBall.y += 10 * delta_time; // mover el rectangulo 10 pixeles hacia abajo
    sRect.x += static_cast<int>(20 * cos(sRect.angle)); // oscilar el rectangulo en x
    sRect.y += static_cast<int>(20 * sin(sRect.angle)); // oscilar el rectangulo en y

    sRect.angle += sRect.spd * delta_time; // incrementar el angulo para rotar el rectangulo

}

void Game::render() {
    // (renderer, <Red>, <Green>, <Blue>, <Alpha/Transparencia>)
    SDL_SetRenderDrawColor(renderer, 41, 129, 6, 255); // color verde oscuro
    // limpiar la pantalla
    SDL_RenderClear(renderer);

    // aqui se comienzan a dibujar los objetos del juego

    // dibujar rectangulo
    SDL_Rect obj_rect = {
        (int)sRect.x,
        (int)sRect.y,
        (int)sRect.width,
        (int)sRect.height
    };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // color negro
    SDL_RenderFillRect(renderer, &obj_rect); // dibujar el rectangulo

    // mostrar el renderizado
    SDL_RenderPresent(renderer);
}

// destruir la ventana del juego
void Game::destroy_window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
