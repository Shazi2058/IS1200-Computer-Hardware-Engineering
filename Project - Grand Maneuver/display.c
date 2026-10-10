/* display.c 
 This file written 2026 by Muhammad Shahwaiz Abbasi 
 */
#include "display.h"
#include <stdint.h>

static uint32_t draw = frame_2; // Start by drawing frame_2 which is the back buffer

volatile uint8_t *backBuffer(void) { // Give access to the framebuffer it is currently drawing into
    return (volatile uint8_t*) (uintptr_t) draw; 
}

void bufferSwap(void) {
    *vgaBackbuffer = draw; // Tell the hardware which buffer to display
    *vgaBuffer = 1;            // Trigger the swap
    while(((*vgaStatusReg) & 1) == 1); // Wait until the swap is complete
    
    if (draw == frame_1) {
        draw = frame_2;     // Toggle drawing back to frame2
    } else {
        draw = frame_1;     // Toggle drawing back to frame1
    }
}

void init_vga(void) {
    draw = frame_2;                         // Start drawing to frame2
    clear_screen(COLOR_BLACK);             // Fill drawing buffer with black
    bufferSwap();                         // Present black frame
    *vgaBackbuffer = frame_2;            // Select frame2 as hardware back buffer
    clear_screen(COLOR_BLACK);          // Clear new drawing buffer
}

void draw_pixel(int x, int y, int color) {  
    if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT) { // The memonry is in 1D but teh screen is in 2D so we calculate pixel to what we need
        backBuffer()[y * VGA_WIDTH + x] = (uint8_t) color;   // Write directly to current backBuffer() 
    }
}
 
void clear_screen(int color) {  // Clear teh entire screen
    volatile uint8_t *fb = backBuffer();
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) { // Every pixel
        fb[i] = (uint8_t) color;
    }
}

void draw_rect(int x, int y, int width, int height, int color) { // Draws the rectangle
    for (int i = 0; i < height; i++) { // Row by Row
        for (int j = 0; j < width; j++) { // Column by Column
            draw_pixel(x + j, y + i, color);
        }
    }
}

void draw_circle(int x0, int y0, int radius, int color) { // Draws the circle
    for (int y = -radius; y <= radius; y++) {  // Starts from top and going to bottom
        for (int x = -radius; x <= radius; x++) { // Starts from left and going to righ
            if (x * x + y * y <= radius * radius) {  // x^2 + y^2 = r^2
                draw_pixel(x0 + x, y0 + y, color);
            }
        }
    }
}

void draw_car(int x, int y, int angle, int body_color) { // Draws the car
    if (angle == 0 || angle == 180) { // If horizontal
        draw_rect(x + 3, y + 2, 18, 10, body_color);  // Main body 
        draw_rect(x + 7, y + 3, 9, 8, COLOR_BLUE);    // Roof panorama
        draw_rect(x + 4, y, 4, 2, COLOR_BLACK);       // Top left tyre
        draw_rect(x + 15, y, 4, 2, COLOR_BLACK);      // Top right tyre
        draw_rect(x + 4, y + 12, 4, 2, COLOR_BLACK);  // Bottom left tyre
        draw_rect(x + 15, y + 12, 4, 2, COLOR_BLACK); // Bottom right tyre

        if (angle == 0) { // looking right
            draw_rect(x + 21, y + 2, 1, 3, COLOR_YELLOW); // Front light
            draw_rect(x + 21, y + 9, 1, 3, COLOR_YELLOW); // Front light
            draw_rect(x + 2, y + 2, 1, 3, COLOR_RED);     // Tail light
            draw_rect(x + 2, y + 9, 1, 3, COLOR_RED);     // Tail light
        } else {   // looking left
            draw_rect(x + 2, y + 2, 1, 3, COLOR_YELLOW);  // Front light
            draw_rect(x + 2, y + 9, 1, 3, COLOR_YELLOW);  // Front light
            draw_rect(x + 21, y + 2, 1, 3, COLOR_RED);    // Tail light
            draw_rect(x + 21, y + 9, 1, 3, COLOR_RED);    // Tail light
        }
    } else {  // If verticle
        draw_rect(x + 2, y + 3, 10, 18, body_color);  
        draw_rect(x + 3, y + 7, 8, 9, COLOR_BLUE);    
        draw_rect(x, y + 4, 2, 4, COLOR_BLACK);       // Top left tyre
        draw_rect(x + 12, y + 4, 2, 4, COLOR_BLACK);  // Top right tyre
        draw_rect(x, y + 15, 2, 4, COLOR_BLACK);      // Bottom left tyre
        draw_rect(x + 12, y + 15, 2, 4, COLOR_BLACK); // Bottom right tyre

        if (angle == 90) {  // looking down
            draw_rect(x + 2, y + 21, 3, 1, COLOR_YELLOW); 
            draw_rect(x + 9, y + 21, 3, 1, COLOR_YELLOW);
            draw_rect(x + 2, y + 2, 3, 1, COLOR_RED);     
            draw_rect(x + 9, y + 2, 3, 1, COLOR_RED);
        } else {            // looking Up
            draw_rect(x + 2, y + 2, 3, 1, COLOR_YELLOW);  
            draw_rect(x + 9, y + 2, 3, 1, COLOR_YELLOW);
            draw_rect(x + 2, y + 21, 3, 1, COLOR_RED);    
            draw_rect(x + 9, y + 21, 3, 1, COLOR_RED);
        }
    }
}

void draw_wall(int x, int y, int width, int height) {
    draw_rect(x, y, width, height, COLOR_GREEN);

    for (int i = 0; i < width; i++) {
        draw_pixel(x + i, y, COLOR_BLACK);              // Top border
        draw_pixel(x + i, y + height - 1, COLOR_BLACK); // Bottom border
    }
    for (int i = 0; i < height; i++) {
        draw_pixel(x, y + i, COLOR_BLACK);             // Left border
        draw_pixel(x + width - 1, y + i, COLOR_BLACK); // Right border[
    }
}

void draw_tree(int x, int y, int radius) {
    draw_circle(x, y, radius, COLOR_DARKGREEN); 
    draw_circle(x, y, radius - 3, COLOR_GREEN); 
    draw_circle(x, y, radius / 3, COLOR_YELLOW); 
}

void draw_level_outlines() {
    for (int x = 0; x < VGA_WIDTH; x++) {
        draw_pixel(x, 0, COLOR_WHITE);               // Top border
        draw_pixel(x, VGA_HEIGHT - 1, COLOR_WHITE);  // bottom border 
    }
    for (int y = 0; y < VGA_HEIGHT; y++) {
        draw_pixel(0, y, COLOR_WHITE);              // Left border
        draw_pixel(VGA_WIDTH - 1, y, COLOR_WHITE);  // Right border
    }
}