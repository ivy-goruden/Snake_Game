
#include <stdio.h>
#include <stdlib.h>
#include "../brick_game/snake/snake.hpp"
#include "../brick_game/models/controller.h"
#include "desktop/snake.hpp"
int main(){
    int game;
    Controller c;
    while (true){

    }
    printf("Choose game:\n");
    printf("    1.Tetris\n");
    printf("    2.Snake\n");
    Render render;
    switch (game){
        case 1:
            printf("bad choice\n");
            break;
        case 2:
            Snake_Game &game = Snake_Game<10, 20>();
            Snake_Render &render = Snake_Render<10, 20, 0, 0>(game);
            Controller c = s21::Controller(game, render);
            c.Run();

        
        
    }
    return 0;
}