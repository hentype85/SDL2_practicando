#include "Game.h"

// constructor
Game::Game() {
    game_is_running = FALSE;
    window = NULL;
    renderer = NULL;
    spriteTexture = NULL;
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
            game_is_running = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    game_is_running = false;
                    break;
                case SDLK_w:
                    sRect.moveUP = true;
                    break;
                case SDLK_s:
                    sRect.moveDOWN = true;
                    break;
                case SDLK_a:
                    sRect.moveLEFT = true;
                    break;
                case SDLK_d:
                    sRect.moveRIGHT = true;
                    break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
                case SDLK_w:
                    sRect.moveUP = false;
                    break;
                case SDLK_s:
                    sRect.moveDOWN = false;
                    break;
                case SDLK_a:
                    sRect.moveLEFT = false;
                    break;
                case SDLK_d:
                    sRect.moveRIGHT = false;
                    break;
            }
            break;

        case SDL_MOUSEMOTION:
            // printf("mouse coords: x=%d, y=%d\n", event.motion.x, event.motion.y);
            sRect.mousePosX = event.motion.x;
            sRect.mousePosY = event.motion.y;
            break;
    }
}

void Game::setup() {
    sRect.width = 15;
    sRect.height = 15;
    sRect.x = WINDOW_WIDTH / 2; // rectangulo en el centro de la pantalla
    sRect.y = WINDOW_HEIGHT / 2; // rectangulo en el centro de la pantalla
    sRect.angle = 0.0f; // angulo
    sRect.spd = 150.0f; // velocidad
    // movimiento
    sRect.moveUP = false;
    sRect.moveDOWN = false;
    sRect.moveLEFT = false;
    sRect.moveRIGHT = false;
    // cargar la textura del personaje
    spriteTexture = load_texture("./Sprites/spritesheet00.png");
}

void Game::update() {

    // esperar hasta que sea tiempo de renderizar el siguiente frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));
    // delta time es la diferencia de tiempo entre frames en segundos
    delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
    // tiempo actual en milisegundos
    last_frame_time = SDL_GetTicks();


    // aplicar movimiento utilizando delta_time solo si la tecla correspondiente esta presionada
    if (sRect.moveUP)
        sRect.y -= sRect.spd * delta_time;
    if (sRect.moveDOWN)
        sRect.y += sRect.spd * delta_time;
    if (sRect.moveLEFT)
        sRect.x -= sRect.spd * delta_time;
    if (sRect.moveRIGHT)
        sRect.x += sRect.spd * delta_time;
}

void Game::render() {
    // limpiar la pantalla (renderer, <Red>, <Green>, <Blue>, <Alpha/Transparencia>)
    SDL_SetRenderDrawColor(renderer, 41, 129, 6, 255); // color verde oscuro
    SDL_RenderClear(renderer);

    // aqui se comienzan a dibujar los objetos del juego

    // definir rectangulos de cada frame de la animacion
    SDL_Rect srcRect = { 
        (int)(sRect.x - sRect.width * zoom * 2 / 2), // posicion x para centrar el zoom
        (int)(sRect.y - sRect.height * zoom / 2), // posicion y para centrar el zoom
        (int)(sRect.width * zoom * 2), // zoom al ancho
        (int)(sRect.height * zoom) // zoom al alto
    };
    // renderizar la textura del jugador con el rectagulo de destino modificado
    SDL_RenderCopy(renderer, spriteTexture, NULL, &srcRect);

    // mostrar el renderizado
    SDL_RenderPresent(renderer);
}

// cargar una textura
SDL_Texture *Game::load_texture(const char* path) {
    SDL_Surface *surface = IMG_Load(path);
    if (!surface) {
        printf("Error loading texture: %s\n", IMG_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        printf("Error creating texture: %s\n", SDL_GetError());
        return NULL;
    }

    return texture;
}

// destruir la ventana del juego
void Game::destroy_window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}