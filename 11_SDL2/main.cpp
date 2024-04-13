#include "Classes/Game.h"

int main(int argc, char* argv[]) {
    // crear un objeto de la clase Game
    Game game;
    // inicializar la ventana del juego
    game.initialize_window();

    // inicializar configuracion de elementos del juego
    game.setup();

    // bucle principal
    while (game.game_is_running) {
        game.process_input();
        game.update();
        game.render();
    }

    return 0;
}