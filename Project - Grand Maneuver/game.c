/* game.c 
 This file written 2026 by Samat Chowdhury
 */
#include "game.h"
#include "levels.h"
#include "display.h"

// Global constanste
#define CAR_W 22.0f //Width of car
#define CAR_H 14.0f //Heigth of car
#define ACCEL 0.16f //acceleration
#define FRICTION 0.85f //Friction to break when stop gas
#define MAX_SPEED 2.4f //max speed

typedef struct { float x,y,w,h; } Box;

//A box on the car. To indicate a collision
static Box car_box(const Car *c) {
    Box b;
    b.x = c->x; 
    b.y = c->y;
    
    // Checks if the Car is vertical, 90 or 270 degrees. Needs to change shape of car
    if (c->angle == 90.0f || c->angle == 270.0f) {
        b.w = CAR_H; 
        b.h = CAR_W; 
    } else {
        b.w = CAR_W; 
        b.h = CAR_H; 
    }

    return b;
}

//Collision detection on rectangular objects. If the car A goes more than object B on cordinate, it will crash. Return 1
static int overlap(Box a, Box b) {
    return a.x < b.x+b.w && a.x+a.w > b.x && a.y < b.y+b.h && a.y+a.h > b.y;
}


//Collision detection for circular object as tree. cx and cy mittpunkt for circle. R radius. b car
static int circle_box(float cx,float cy,float r,Box b) {
    float px=cx, py=cy;
    if(px < b.x) px=b.x; else if(px>b.x+b.w) px=b.x+b.w;
    if(py < b.y) py=b.y; else if(py>b.y+b.h) py=b.y+b.h;

    //checks distance of the tree and the car cordinates 
    float dx=cx-px, dy=cy-py;
    return dx*dx+dy*dy < r*r; 
}

//initializing the startplace for the car
void game_init(Car *car, int level) {
    car->x = (float)get_car_start_x(level); 
    car->y = (float)get_car_start_y(level);
    car->vx = car->vy = 0.0f; 
    car->angle = 0.0f; 
}

void game_update(Car *car, InputState in) {
    static int last_rotate_switch = 0;

    // If sw2 is up
    if (in.right && !last_rotate_switch) {
        car->angle += 90.0f; 
        if (car->angle >= 360.0f) car->angle = 0.0f;
        
        //Makes the speed 0 when it rotates
        car->vx = 0.0f;
        car->vy = 0.0f;
    }
    last_rotate_switch = in.right;
    
    // om sw1 uppe
    if (in.forward) { 
        if (car->angle == 0.0f) {
            car->vx += ACCEL; // Drives right
        } else if (car->angle == 90.0f) {
            car->vy += ACCEL; // Drives down
        } else if (car->angle == 180.0f) {
            car->vx -= ACCEL; // drives left
        } else if (car->angle == 270.0f) {
            car->vy -= ACCEL; // drives up
        }
    }
    
    car->vx *= FRICTION; 
    car->vy *= FRICTION;
    
    // Makes car stop if it is very low
    if (car->vx > -0.05f && car->vx < 0.05f) car->vx = 0.0f;
    if (car->vy > -0.05f && car->vy < 0.05f) car->vy = 0.0f;
    
    // Speed limit
    if (car->vx > MAX_SPEED) car->vx = MAX_SPEED;
    if (car->vx < -MAX_SPEED) car->vx = -MAX_SPEED;
    if (car->vy > MAX_SPEED) car->vy = MAX_SPEED;
    if (car->vy < -MAX_SPEED) car->vy = -MAX_SPEED;
    
    //Changes cordinations when it drives
    car->x += car->vx; 
    car->y += car->vy;
}

int game_collision(const Car *car, int level) {
    Box b = car_box(car);
    //If you drive overr the screen
    if (b.x < 1 || b.y < 1 || b.x + b.w > 319 || b.y + b.h > 239) return 1;
    
    //Checks the trees in level 1
    if (level == 1) {
        for (int i = 0; i < 5; i++) {
            if (circle_box(30 + i * 60, 25, 12, b)) return 1;
        }
        for (int i = 0; i < 5; i++) {
            if (circle_box(30 + i * 60, 215, 12, b)) return 1;
        }
    } 

    //The obstacles in level 2
    else if (level == 2) {
        if (overlap(b, (Box){10, 10, 290, 8})) return 1;
        if (overlap(b, (Box){10, 60, 220, 8})) return 1;
        if (overlap(b, (Box){290, 10, 8, 170})) return 1;
        if (overlap(b, (Box){220, 60, 8, 110})) return 1;
        if (overlap(b, (Box){10, 170, 280, 8})) return 1;
        if (circle_box(50, 115, 12, b)) return 1;
        if (circle_box(110, 115, 12, b)) return 1;
        if (circle_box(170, 115, 12, b)) return 1;
    } 

    //obstacle in level 3
    else if (level == 3) {
        if (overlap(b, (Box){90, 0, 15, 130})) return 1;
        if (overlap(b, (Box){190, 90, 15, 150})) return 1;
    }
    return 0;
}



int game_is_parked(const Car *car, int level) {
    Box b = car_box(car), p;
    int stopped = (car->vx == 0.0f && car->vy == 0.0f);
    
    //Checks if it is parked in the box pp
    if (level == 1 && stopped) { 
        p = (Box){240, 109, 36, 21}; 
    }
    else if (level == 2 && stopped) {
        p = (Box){245, 120, 21, 36};
    }
    else if (level == 3 && stopped) {
        p = (Box){240, 180, 21, 36};
    }
    else return 0;
    

    float x1, y1, x2, y2, area;

    //If left side of car is outside keep the cordinate 
    if (b.x > p.x) {
        x1 = b.x;
    } else {
        //or take the parking cordinate for upper left corner
        x1 = p.x;
    }

   
    //top
    if (b.y > p.y) {
        y1 = b.y;
    } else {
        y1 = p.y;
    }

    //If right side outside keep it
    if ((b.x + b.w) < (p.x + p.w)) {
        x2 = b.x + b.w;
    } else {
        //if inside keep parking right corner
        x2 = p.x + p.w;
    }

    //backside
    if ((b.y + b.h) < (p.y + p.h)) {
        y2 = b.y + b.h;
    } else {
        y2 = p.y + p.h;
    }

    //Counts the area
    if (x2 > x1 && y2 > y1) {
        area = (x2 - x1) * (y2 - y1);
    } else {
        area = 0.0f;
    }
    
    //Area, how much percent is inside
    return area / (b.w * b.h) >= 0.65f;
}