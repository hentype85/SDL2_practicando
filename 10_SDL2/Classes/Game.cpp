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

    if (TTF_Init() == -1) { // inicializar SDL_ttf
        printf("Error initializing SDL_ttf: %s\n", TTF_GetError());
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
    // cargar la textura de sprites
    spriteTexture = load_texture("./Sprites/spritesheet00.png");
    currentframe = 0;
    currentRow = 0;
    zoom = 5.0f;
    rRect = { 0, 0, 0, 0 };
    // flag de colision
    collision_flag = false;
    // cargar fuente de letras
    font = load_font("./Fonts/font.ttf", 24); // fuente de 24 de tamano
    dialogRect = { 0, 0, 0, 0 }; // rectangulo de dialogo
    textColor = { 0, 0, 0, 255 }; // color del texto
}

void Game::update() {
    // esperar hasta que sea tiempo de renderizar el siguiente frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));
    // delta time es la diferencia de tiempo entre frames en segundos
    delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
    // tiempo actual en milisegundos
    last_frame_time = SDL_GetTicks();


    // incrementar el frame actual del sprite
    currentframe = (int)((SDL_GetTicks() / 100) % 6); // 6 frames en las filas
    if (currentframe > 5) currentframe = 0; // regresar al primer frame


    // aplicar movimiento utilizando delta_time solo si la tecla correspondiente esta presionada
    if (sRect.moveUP)
        sRect.y -= sRect.spd * delta_time;
    if (sRect.moveDOWN)
        sRect.y += sRect.spd * delta_time;
    if (sRect.moveLEFT)
        sRect.x -= sRect.spd * delta_time;
    if (sRect.moveRIGHT)
        sRect.x += sRect.spd * delta_time;


    // aplicar logica de para sprites de jugador
    if (sRect.moveDOWN)
        currentRow = (textureheight / 5) * 1; // (altura de la textura / cantidad de filas) * fila deseada
    else if (sRect.moveUP)
        currentRow = (textureheight / 5) * 2; // (altura de la textura / cantidad de filas) * fila deseada
    else if (sRect.moveLEFT)
        currentRow = (textureheight / 5) * 3; // (altura de la textura / cantidad de filas) * fila deseada
    else if (sRect.moveRIGHT)
        currentRow = (textureheight / 5) * 4; // (altura de la textura / cantidad de filas) * fila deseada
    else
    {
        currentRow = 0; // fila 0 (idle)
        currentframe = (int)((SDL_GetTicks() / 1000) % 6); // 6 frames en las filas (idle) un poco mas lento
    }

    frameheight = textureheight / 5; // alto de cada frame
    framewidth = texturewidth / 6; // ancho de cada frame

    // definir rectangulos de origen y destino para renderizar la textura
    srcRect = {
        currentframe * framewidth, // mover frame en la fila actual
        currentRow, // fila actual
        framewidth, // ancho de cada frame
        frameheight // alto de cada frame
    };
    dstRect = {
        (int)(sRect.x - sRect.width * zoom / 2), // posicion x para aplicar movimiento y centrar el zoom
        (int)(sRect.y - sRect.height * zoom / 2), // posicion y para aplicar movimiento y centrar el zoom
        (int)(sRect.width * zoom), // aplicar zoom al ancho del sRect
        (int)(sRect.height * zoom) // aplicar zoom al alto del sRect
    };


    rRect = { 300, 400, 50, 50 }; // rectangulo para colision

    // checkeo colisiones
    if (check_collision(dstRect, rRect) == 1) {
        collision_flag = true;
    } else {
        collision_flag = false;
    }
}

void Game::render() {
    // limpiar la pantalla (renderer, <Red>, <Green>, <Blue>, <Alpha/Transparencia>)
    SDL_SetRenderDrawColor(renderer, 41, 129, 6, 255); // color verde oscuro
    SDL_RenderClear(renderer);

    // aqui se comienzan a dibujar los objetos del juego

    // cargar ancho y alto de spriteTexture en texturewidth y textureheight
    SDL_QueryTexture(spriteTexture, NULL, NULL, &texturewidth, &textureheight);

    // renderizar la textura del jugador con el rectagulo de destino modificado
    SDL_RenderCopy(renderer, spriteTexture, &srcRect, &dstRect);

    // renderizar el rectangulo perimetro del dstRect
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // color verde
    SDL_RenderDrawRect(renderer, &dstRect);

    // rectangulo estatico para colision
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // color rojo
    SDL_RenderDrawRect(renderer, &rRect);

    // renderizar dialogo de colision
    /*
    if (collision_flag) {
        SDL_RenderCopy(renderer, surfaceText, NULL, &dialogRect);
        dialog_box("Hola Mundo", rRect.x, rRect.y, 100, 50, 24);
    }
    */

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

// cargar una fuente de letras
TTF_Font *Game::load_font(const char *path, int size) {
    font = TTF_OpenFont(path, size);
    if (!font) {
        printf("Error loading font: %s\n", TTF_GetError());
        return NULL;
    }

    return font;
 }

// checkear colision entre dos rectangulos
int Game::check_collision(SDL_Rect myRect, SDL_Rect rect) {
    // colision entre rectangulos
    if (SDL_HasIntersection(&myRect, &rect)) {
        // revertir el movimiento si hay colision
        if (sRect.moveUP) sRect.y += sRect.spd * delta_time;
        if (sRect.moveDOWN) sRect.y -= sRect.spd * delta_time;
        if (sRect.moveLEFT) sRect.x += sRect.spd * delta_time;
        if (sRect.moveRIGHT) sRect.x -= sRect.spd * delta_time;

        return 1;
    }

    return 0;
}

// mostrar un dialogo en pantalla
/*
void Game::dialog_box(const char *message, int x, int y, int w, int h, int size) {
    dialogRect = { x + 50, y - 10, w, h };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // color blanco
    SDL_RenderFillRect(renderer, &dialogRect);

    textColor = { 0, 0, 0, 255 }; // color negro
    textSurface = TTF_RenderText_Solid(font, message, textColor);

    // crear textura a partir de superficie de texto
    surfaceText = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
}
*/

// destruir la ventana del juego
void Game::destroy_window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
