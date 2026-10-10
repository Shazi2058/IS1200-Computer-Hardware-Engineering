/* main.c 
 This file written 2026 by Muhammad Shahwaiz Abbasi and Samat Chowdhury
 */
#include "display.h"
#include "levels.h"
#include "timer.h"
#include "game.h"
#include "input.h"

extern void print_dec(unsigned int);
extern void print(const char*);
// Muhammad Shahwaiz Abbasi
void check_timer(void) {
    volatile int *timer_status = (volatile int *) 0x04000020; 
    static int timeoutcount = 0;

    if (*timer_status & 0x1) {
        *timer_status = 0; 
        timeoutcount++;

        if (timeoutcount >= 10) { 
            timeoutcount = 0;
            int current_time = get_timer_seconds();
            if (current_time > 0) {
                set_timer_seconds(current_time - 1); 
            }
        }
    }
}


// Muhammad Shahwaiz Abbasi
void delay(int cycles) {
    for (volatile int i = 0; i < cycles; i++) {}
}
// Samat Chowdhury
int main(void) {
    init_vga(); // Initialize VGA display
    labinit();  // Initialize timer
    input_init(); // Initialize input system

    //Start on level 1
    static int current_level = 1; 

    //
    static int prev_switch_level = -1;
    
    Car player;
    static int completed_times[3] = {0, 0, 0};
    static int best_times[3] = {25, 65, 80}; 
    
    // 0 playing, 1 game over, 2 level complete
    int game_over = 0;

    current_level = get_level_number();
    if (current_level < 1 || current_level > 3) {
        current_level = 1; //Secuiry that switch doesn
    }
    //Remember current switch lvl
    prev_switch_level = current_level;

    //Initize time and car
    game_init(&player, current_level);
    set_timer_seconds(time_limit(current_level)); 
    // Display instructions
    while (1){
        print("Greetings, dear player! Welcome to the realm of Grand Maneuver.\n");
        delay(3000000); // Some delay
        print("Game Instructions:\n");
        delay(3000000); // Some delay
        print("Use the switches 9 and 8 and press the button to select a level (1-3).\n");
        delay(3000000); // Some delay
        print("Press the switch 8 to restart the level.\n");
        delay(3000000); // Some delay
        print("Use the switches 1 to move the car forward.\n");
        delay(3000000); // Some delay
        print("Use the switches 2 to turn the car right (90 degrees clockwise).\n");
        delay(3000000); // Some delay
        print("Your goal is to park the car in the designated parking spot.\n");
        delay(3000000); // Some delay
        print("You have a time limit to complete each level.\n");
        delay(3000000); // Some delay
        print("Good luck!\n");
        print("-----------------------------------------------------------------------\n");
        print("\n");
        delay(3000000); // Some delay
        break;
    }
    while (1) {
        check_timer();

        int selected_level = get_level_number(); 
        int do_reset = reset_level(); 

        //Checks if switch or reset has changed
        if (selected_level != prev_switch_level || do_reset) { 
            //If the selected level changed, update
            if (selected_level != prev_switch_level) {
                current_level = selected_level;       
                prev_switch_level = selected_level;   
            }
            
            //Resets
            int level_time = time_limit(current_level); 
            set_timer_seconds(level_time); 
            game_init(&player, current_level);
            game_over = 0;
        }

        InputState keys = input_read();
        
        //if reset tapped, resets game
        if (keys.reset) {
            game_init(&player, current_level);
            set_timer_seconds(time_limit(current_level));
            game_over = 0;
        } else if (!game_over) {
            if (get_timer_seconds() <= 0) {
                game_over = 1; //Time out, timer reaches 0
            } else {
                game_update(&player, keys);

                //Checks if the car has crashed
                if (game_collision(&player, current_level)) {
                    game_over = 1;
                } else if (game_is_parked(&player, current_level)) { //Checks if the car is parked
                    int attempt_time = time_limit(current_level) - get_timer_seconds();
                    completed_times[current_level - 1] = attempt_time;
                    
                    if (attempt_time < best_times[current_level - 1]) {
                        best_times[current_level - 1] = attempt_time;
                    }
                    
                    //Level done
                    game_over = 2;
                }
            }
        }

        if (game_over == 1) { 
            // Game Over, Red Screen
            clear_screen(COLOR_RED); 
            bufferSwap();
            delay(5000000); 
            
            //Reset game
            game_init(&player, current_level);
            set_timer_seconds(time_limit(current_level));
            game_over = 0;
        } else if (game_over == 2) { 
            // Level Complete,  Green Screen nd Score
            clear_screen(COLOR_GREEN); 
            bufferSwap();
            
            print("Level ");
            print_dec(current_level);
            print(" Complete!\n");
            
            print("Your Time: ");
            print_dec(completed_times[current_level - 1]);
            print(" seconds\n");
            
            print("Best Time: ");
            print_dec(best_times[current_level - 1]);
            print(" seconds\n");
            print("------------------\n");
            print("\n");
            
            //If level 3 done, total time written
            if (current_level == 3) {
                int total_time = completed_times[0] + completed_times[1] + completed_times[2];
                print("Total game time: ");
                print_dec(total_time);
                print(" seconds\n");
                print("------------------\n");
                print("\n");
            }
            
            delay(10000000); //Delay for green screen
            
            //Changes level autmatically
            if (current_level < 3) {
                current_level++;
            } else {
                current_level = 1; 
            }
            
            // Secures that ur in the level, even if swith is done
            prev_switch_level = get_level_number(); 
            game_init(&player, current_level);
            set_timer_seconds(time_limit(current_level));
            game_over = 0;
        } else {
            // Normal Gameplay Frame Rendering
            clear_screen(COLOR_GRAY);
            draw_level(current_level);
            draw_car((int)player.x, (int)player.y, (int)player.angle, COLOR_RED);
            
            // Present frame onto the display
            bufferSwap();
        }

        delay(1000); 
    }

    return 0;
}