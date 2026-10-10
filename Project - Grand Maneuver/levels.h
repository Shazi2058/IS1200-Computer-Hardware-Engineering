/* levels.h 
 This file written 2026 by Muhammad Shahwaiz Abbasi
 */
#ifndef LEVELS_H
#define LEVELS_H

// Function prototypes
int get_car_start_x(int level);
int get_car_start_y(int level);
int get_car_vertical(int level);
void draw_level(int level);
int get_sw(void);
int get_btn(void);
int reset_level(void);
int get_level_number();
#endif