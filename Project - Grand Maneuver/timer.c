/* timer.c 
 This file written 2026 by Muhammad Shahwaiz Abbasi
 */
#include "timer.h"

/* Below functions are external and found in other files. */
extern void enable_interrupt(void);
extern void tick(int*);

const char seg_table[10] = { // 7-segment
    0xC0, // 0
    0xF9, // 1
    0xA4, // 2
    0xB0, // 3
    0x99, // 4
    0x92, // 5
    0x82, // 6
    0xF8, // 7
    0x80, // 8
    0x90  // 9
};
volatile int game_seconds = 0; // Remaining seconds for current level
void handle_interrupt(unsigned cause) {
}

void set_displays(int display_number, int value) {
    volatile int *display = (volatile int *) (0x04000050 + (display_number * 0x10)); 
    *display = value;
}

// Set timer for each level
void set_timer_seconds(int seconds) {
    game_seconds = seconds;
    update_displays(game_seconds);
}

// Update the 7 segment display with seconds and minutes
void update_displays(int seconds) {
    if (seconds < 0) {
        seconds = 0;
    }
    int total_sec = seconds % 60;
    int mins = seconds / 60;

    // Seconds
    set_displays(0, seg_table[total_sec % 10]);       // Seconds ones
    set_displays(1, seg_table[total_sec / 10]);       // Seconds tens

    // Minutes
    set_displays(2, seg_table[mins % 10]);          // Minutes ones
    set_displays(3, seg_table[(mins / 10) % 60]);   // Minutes tens

    // Hours
    set_displays(4, seg_table[0]);
    set_displays(5, seg_table[0]);
}

// Remaining time
int get_timer_seconds(void) {
    return game_seconds;
}

void labinit(void) {
    volatile int *periodh       = (volatile int *) 0x0400002C;
    volatile int *periodl       = (volatile int *) 0x04000028;
    volatile int *timer_control = (volatile int *) 0x04000024;
    volatile int *timer_status  = (volatile int *) 0x04000020;  

    *timer_control = 0x8; // STOP bit
    *timer_status  = 0x0; // Clear 

    // 3,000,000 cycles
    *periodh = 0x002D;
    *periodl = 0xC6C0;

    
    *timer_control = 0x6;
    enable_interrupt();
}