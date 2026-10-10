/* input.c 
 This file written 2026 by Samat Chowdhury 
 */
#include "input.h"

//Adress to the switches
#define SWITCHES (*(volatile unsigned int *)0x04000010)

static InputState state;

//Starting with 0 when game starts
void input_init(void) {
    state.forward = 0;
    state.right = state.reset = 0;
}

//Reads the input from switches
InputState input_read(void) {

    //taking value from SWITCHES
    unsigned int sw = SWITCHES;
    
    // SW1 = Drives forward
    state.forward  = (sw & 0x2) ? 1 : 0;  
    
    // SW2 = Rotates 90 degrees clockwise
    state.right = (sw & 0x4) ? 1 : 0; 
    
    // SW0 = Reset the time
    static int last_sw0 = 0;
    state.reset = 0;

    //Reset when it goes from 0 to 1
    if ((sw & 0x1) && !last_sw0) {
        state.reset = 1;
    }
    last_sw0 = (sw & 0x1);

    return state;
}