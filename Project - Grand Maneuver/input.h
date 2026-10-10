/* input.h 
 This file written 2026 by Samat Chowdhury 
 */
#ifndef INPUT_H
#define INPUT_H

//All movement stored in a struct. A package that is a variable
typedef struct {
    int forward, right, reset;
} InputState;

// Function prototypes
//Tells other files, these are the function in input
void input_init(void);
InputState input_read(void);

#endif
