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
            game_is_running = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    game_is_running = false;
                    break;
            }
            break;
    }
}

void Game::setup() {
    sRect.width = WINDOW_WIDTH;
    sRect.height = WINDOW_HEIGHT;
    sRect.x = WINDOW_WIDTH / 2; // rectangulo en el centro de la pantalla
    sRect.y = WINDOW_HEIGHT / 2; // rectangulo en el centro de la pantalla
    // cargar la textura de sprites
    spriteManager.spriteTexture = load_texture("./Sprites/intro00.png");
    spriteManager.currentframe = 0;
    spriteManager.currentRow = 0;
    zoom = 1.0f;
}

void Game::update() {
    // esperar hasta que sea tiempo de renderizar el siguiente frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));
    // delta time es la diferencia de tiempo entre frames en segundos
    delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
    // tiempo actual en milisegundos
    last_frame_time = SDL_GetTicks();

    // calcular el frame actual y la fila actual
    spriteManager.total_frames = 4; // total de frames por fila
    spriteManager.total_rows = 9; // total de filas
    spriteManager.frames_per_row = spriteManager.total_frames * spriteManager.total_rows;

    spriteManager.currentframe = (int)((last_frame_time / 100 % spriteManager.frames_per_row));
    spriteManager.currentRow = spriteManager.currentframe / spriteManager.total_frames;

    printf("Frame: %d, Row: %d\n", spriteManager.currentframe % spriteManager.total_frames, spriteManager.currentRow);

    // ancho y alto de cada frame
    spriteManager.framewidth = spriteManager.texturewidth / spriteManager.total_frames; // ancho de cada frame
    spriteManager.frameheight = spriteManager.textureheight / spriteManager.total_rows; // alto de cada frame

    // Definir rectángulos de origen y destino para renderizar la textura
    sRect.srcRect = {
        (spriteManager.currentframe % spriteManager.total_frames) * spriteManager.framewidth, // posicion x del frame actual
        spriteManager.currentRow * spriteManager.frameheight, // fila actual
        spriteManager.framewidth, // ancho de cada frame
        spriteManager.frameheight // alto de cada frame
    };
    sRect.dstRect = {
        (int)(sRect.x - sRect.width * zoom / 2), // posicion x con zoom centrado
        (int)(sRect.y - sRect.height * zoom / 2), // posicion y con zoom centrado
        (int)(sRect.width * zoom), // aplicar zoom al ancho del sRect
        (int)(sRect.height * zoom) // aplicar zoom al alto del sRect
    };

}


void Game::render() {
    // limpiar la pantalla
    SDL_RenderClear(renderer);

    // aqui se comienzan a dibujar los objetos del juego

    // cargar ancho y alto de spriteTexture en texturewidth y textureheight
    SDL_QueryTexture(spriteManager.spriteTexture, NULL, NULL, &spriteManager.texturewidth, &spriteManager.textureheight);

    // renderizar la textura del jugador con el rectagulo de destino modificado
    SDL_RenderCopy(renderer, spriteManager.spriteTexture, &sRect.srcRect, &sRect.dstRect);

    // mostrar el renderizado
    SDL_RenderPresent(renderer);
}

// cargar una textura desde un archivo
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
    SDL_Quit(); // cerrar SDL y limpiar cualquier recurso utilizado por SDL
}