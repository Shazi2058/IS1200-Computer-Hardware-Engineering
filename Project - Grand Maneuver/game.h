/* game.h 
 This file written 2026 by Samat Chowdhury
 */
#ifndef GAME_H
#define GAME_H
#include "input.h"

typedef struct {
    float x, y;       
    float vx, vy;
    float angle;      
} Car;

// Function prototypes
//Tells other files, these are the function in game
void game_init(Car *car, int level);
void game_update(Car *car, InputState input);
int game_collision(const Car *car, int level);
int game_is_parked(const Car *car, int level);

#endif
