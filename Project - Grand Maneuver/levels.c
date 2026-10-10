/* levels.c 
 This file written 2026 by Muhammad Shahwaiz Abbasi
 */
#include "levels.h"
#include "display.h"

int time_limit(int level) { // the limit for each level
    if (level == 1) {
      return 20; // 20 second for level 1
    }
    if (level == 2) {
      return 60; // 30 second for level 2
    }
    if (level == 3) {
      return 75; // 75 second for level 3
    }
    return 30;
}

int get_car_start_x(int level) { // the X positon of the car spawn point
    if (level == 2) {
        return 25; 
    }
    return (level == 3) ? 25 : 30;
}

int get_car_start_y(int level) { // the Y positon of the car spawn point
    if (level == 1) {
        return 113;
    } else if (level == 2) {
        return 30; // Top channel
    } else {
        return 155;
    }
}

void draw_level(int level) { // Drawing the levels
    draw_level_outlines();
    switch (level){
        case 1:
            // 5 Trees along the Top Row 
            for (int i = 0; i < 5; i++) {
                draw_tree(30 + i * 60, 25, 12);
            }

            // 5 Trees along the Bottom Row 
            for (int i = 0; i < 5; i++) {
                draw_tree(30 + i * 60, 215, 12);
            }
            draw_rect(240, 109, 36, 21, COLOR_PARKING_ZONE); // Outer part of parking
            draw_rect(242, 111, 32, 17, COLOR_GRAY);         // Inner part of parking
            break;
      
      case 2:
            
            draw_wall(10, 10, 290, 8);     // Top wall
            draw_wall(10, 60, 220, 8);     // Divider between top lane and middle

           
            draw_wall(290, 10, 8, 170);    // The most right wall
            draw_wall(220, 60, 8, 110);    // The inner right wall
            
            draw_wall(10, 170, 280, 8);    // Bottom wall

            // the trees for decoration
            for (int i = 50; i <=170; i += 60) {
                draw_tree(i, 115, 12);
            }
            // The Parking
            draw_rect(245, 120, 21, 36, COLOR_PARKING_ZONE); 
            draw_rect(247, 122, 17, 32, COLOR_GRAY);         
            break;
        case 3: 
            draw_rect(240, 180, 21, 36, COLOR_PARKING_ZONE); // Outer part of parking
            draw_rect(242, 182, 17, 32, COLOR_GRAY); // Inner part of parking

            draw_wall(90, 0, 15, 130); // Top wall
            draw_wall(190, 90, 15, 150); // Bottom wall
            break;
    }

}
int get_sw(void){   // The switches
    volatile int *sw = (volatile int *) 0x04000010;
    return (*sw) & 0x3FF;
}
int get_btn(void){  // The Button
    volatile int *btn = (volatile int *) 0x040000d0;
    return (*btn) & 1;
}
int reset_level(void) { // Resets the level
    static int last_btn_state = 0; // if the button is not pressed
    int current_btn = get_btn(); 
    int sw = get_sw();
    int reset_triggered = 0; 

    // Trigger reset ONLY on button press not release and when SW0 is UP
    if (current_btn && !last_btn_state && (sw & 0x1)) {
        reset_triggered = 1;
    }

    last_btn_state = current_btn;
    return reset_triggered;
}
int get_level_number(){
    int sw = get_sw(); // Get the value of the switches
    static int level = 1;
    if (get_btn()) { // Check if button is pressed
        if (!(sw & 0x1)) {  // is switch0 is on then dont change mode turn off switch to change lvl
            int mode = (sw >> 8) & 0x3;// Get the mode from switches 9 and 10 and keeps the 2 bits
            if (mode == 1) {        // 01  update seconds
                level = 1;
            } else if (mode == 2) { // 10 update minutes
                level = 2;
            } else if (mode == 3) { // 11  update hours
            level = 3;
            }
        }
    }
    return level;
}