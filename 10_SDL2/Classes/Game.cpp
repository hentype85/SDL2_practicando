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
    playerSprite.spriteTexture = load_texture("./Sprites/spritesheet00.png");
    playerSprite.currentframe = 0;
    playerSprite.currentRow = 0;
    zoom = 6.0f;
    rRect = { 0, 0, 0, 0 };
    // flag de colision
    collision_flag = false;
    // para fuente de letras
    bubbletxt.dialogRect = { 0, 0, 0, 0 }; // rectangulo de dialogo
    bubbletxt.textColor = { 0, 0, 0, 255 }; // color del texto
}

void Game::update() {
    // esperar hasta que sea tiempo de renderizar el siguiente frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));
    // delta time es la diferencia de tiempo entre frames en segundos
    delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
    // tiempo actual en milisegundos
    last_frame_time = SDL_GetTicks();


    // incrementar el frame actual del sprite
    playerSprite.currentframe = (int)((SDL_GetTicks() / 100) % 6); // 6 frames en las filas
    if (playerSprite.currentframe > 5) playerSprite.currentframe = 0; // regresar al primer frame


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
        playerSprite.currentRow = (playerSprite.textureheight / 5) * 1; // (altura de la textura / cantidad de filas) * fila deseada
    else if (sRect.moveUP)
        playerSprite.currentRow = (playerSprite.textureheight / 5) * 2; // (altura de la textura / cantidad de filas) * fila deseada
    else if (sRect.moveLEFT)
        playerSprite.currentRow = (playerSprite.textureheight / 5) * 3; // (altura de la textura / cantidad de filas) * fila deseada
    else if (sRect.moveRIGHT)
        playerSprite.currentRow = (playerSprite.textureheight / 5) * 4; // (altura de la textura / cantidad de filas) * fila deseada
    else
    {
        playerSprite.currentRow = 0; // fila 0 (idle)
        playerSprite.currentframe = (int)((SDL_GetTicks() / 1000) % 6); // 6 frames en las filas (idle) un poco mas lento
    }

    playerSprite.frameheight = playerSprite.textureheight / 5; // alto de cada frame
    playerSprite.framewidth = playerSprite.texturewidth / 6; // ancho de cada frame

    // definir rectangulos de origen y destino para renderizar la textura
    sRect.srcRect = {
        playerSprite.currentframe * playerSprite.framewidth, // mover frame en la fila actual
        playerSprite.currentRow, // fila actual
        playerSprite.framewidth, // ancho de cada frame
        playerSprite.frameheight // alto de cada frame
    };
    sRect.dstRect = {
        (int)(sRect.x - sRect.width * zoom / 2), // posicion x para aplicar movimiento y centrar el zoom
        (int)(sRect.y - sRect.height * zoom / 2), // posicion y para aplicar movimiento y centrar el zoom
        (int)(sRect.width * zoom), // aplicar zoom al ancho del sRect
        (int)(sRect.height * zoom) // aplicar zoom al alto del sRect
    };


    rRect = { 300, 400, 50, 50 }; // rectangulo para colision

    // checkeo colisiones
    if (check_collision(sRect.dstRect, rRect) == 1) {
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
    SDL_QueryTexture(playerSprite.spriteTexture, NULL, NULL, &playerSprite.texturewidth, &playerSprite.textureheight);

    // renderizar la textura del jugador con el rectagulo de destino modificado
    SDL_RenderCopy(renderer, playerSprite.spriteTexture, &sRect.srcRect, &sRect.dstRect);

    // renderizar el rectangulo perimetro del dstRect
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // color verde
    SDL_RenderDrawRect(renderer, &sRect.dstRect);

    // rectangulo estatico para colision
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // color rojo
    SDL_RenderDrawRect(renderer, &rRect);

    // renderizar dialogo de colision
    if (collision_flag) {
        dialog_box("Hola bebeeeee...");
    }

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

// cargar una fuente de letras
TTF_Font *Game::load_font(const char *path, int size) {
   bubbletxt.font = TTF_OpenFont(path, size);
    if (!bubbletxt.font) {
        printf("Error loading font: %s\n", TTF_GetError());
        return NULL;
    }

    return bubbletxt.font;
 }

// mostrar un dialogo en pantalla
void Game::dialog_box(const char *message) {

    bubbletxt.sizeFont = 24; // tamano de la fuente
    bubbletxt.font = load_font("./Fonts/font_small.ttf", bubbletxt.sizeFont); // cargar fuente de texto

    bubbletxt.h_font = bubbletxt.sizeFont; // altura del dialogo
    bubbletxt.w_font = (strlen(message) * bubbletxt.sizeFont); // ancho del dialogo

    // rectangulo del dialogo
    bubbletxt.dialogRect = {
        (WINDOW_WIDTH / 2) - (bubbletxt.w_font / 2),
        WINDOW_HEIGHT - bubbletxt.h_font * 4,
        bubbletxt.w_font,
        bubbletxt.h_font * 2
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // color blanco
    SDL_RenderFillRect(renderer, &bubbletxt.dialogRect);

    bubbletxt.textColor = { 0, 0, 0, 255 }; // color negro
    bubbletxt.textSurface = TTF_RenderText_Solid(bubbletxt.font, message, bubbletxt.textColor);

    TTF_SizeText(bubbletxt.font, message, &bubbletxt.textW, &bubbletxt.textH);
    bubbletxt.text_x = bubbletxt.dialogRect.x + (bubbletxt.dialogRect.w - bubbletxt.textW) / 2;
    bubbletxt.text_y = bubbletxt.dialogRect.y + (bubbletxt.dialogRect.h - bubbletxt.textH) / 2; 

    // crear textura a partir de superficie de texto
    bubbletxt.surfaceText = SDL_CreateTextureFromSurface(renderer, bubbletxt.textSurface);
    SDL_FreeSurface(bubbletxt.textSurface);

    // textura del mensaje en la posicion calculada
    SDL_Rect textRect = { bubbletxt.text_x, bubbletxt.text_y, bubbletxt.textW, bubbletxt.textH }; 
    // renderizar la textura del mensaje en el rectangulo del dialogo
    SDL_RenderCopy(renderer, bubbletxt.surfaceText, NULL, &textRect);
}

// destruir la ventana del juego
void Game::destroy_window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit(); // cerrar el subsistema de manejo de fuentes de texto
    SDL_Quit(); // cerrar SDL y limpiar cualquier recurso utilizado por SDL
}
