/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */


/* Below functions are external and found in other files. */
extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void tick(int*);
extern void delay(int);
extern int nextprime( int );
void set_leds(int led_mask);
void set_displays(int display_number, int value);
int get_sw(void);
int get_btn(void);
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

int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) 
{}

/* Add your code here for initializing interrupts. */
void labinit(void)
{}
void set_leds(int led_mask){
  volatile int *led = (volatile int *) 0x04000000; // pointer to the LED 
  *led = led_mask & 0x3FF; // masking the 10 LSB to set the LEDS
}
void set_displays(int display_number, int value){
  volatile int *display = (volatile int *) (0x04000050 + (display_number * 0x10)); // pointer to the display with offset of 0x10
  *display = value;
}
int get_sw(void){
  volatile int *sw = (volatile int *) 0x04000010;
   int switch_value = *sw & 0x3FF;
   return switch_value;
}
int get_btn(void){
    volatile int *btn = (volatile int *) 0x040000d0;
    return (*btn) & 1;
}



/* Your code goes into main as well as any needed functions. */
int main() {
  // Call labinit()
  labinit();
  int led_count = 0;
  set_leds(led_count);

while (led_count < 15) {
    delay(2000); // 1 approximate second
    led_count++;
    set_leds(led_count);
}
int seconds = 0;
int minutes = 0;
int hours = 0;
while (1) {
set_displays(0, seg_table[seconds % 10]); // Second digit 1
set_displays(1, seg_table[(seconds / 10) % 10]); // Second digit 2


set_displays(2, seg_table[minutes % 10]); // Minute digit 1
set_displays(3, seg_table[(minutes / 10) % 10]); // Minute digit 2


set_displays(4, seg_table[hours % 10]); // Hour digit 1
set_displays(5, seg_table[(hours / 10) % 10]);// Hour digit 2

int sw = get_sw(); // Get the value of the switches

if (get_btn()) { // Check if button is pressed
    int sw7 = (sw >> 7) & 0x1;
    if (sw7){
        break;
    }
    int mode = (sw >> 8) & 0x3; // Get the mode from switches 9 and 10 and keeps the 2 bits
    int val = sw & 0x3F;        // Get the value from switches 0-4

    if (mode == 1) {        // 01  update seconds
        seconds = val % 60;
    } else if (mode == 2) { // 10 update minutes
        minutes = val % 60;
    } else if (mode == 3) { // 11  update hours
        hours = val % 24;
    }
}
delay(2000);          // Delays 1 sec (adjust this value)
seconds++;
    if (seconds >= 60) { // If seconds reach 60 reset to 0 and increment minutes
        seconds = 0;
        minutes++;
        if (minutes >= 60) { // If minutes reach 60 reset to 0 and increment hours
            minutes = 0;
            hours = (hours + 1) % 24; // If hours reach 24 reset to 0
        }
    }
}

  // Enter a forever loop
  while (1) {
    time2string( textstring, mytime ); // Converts mytime to string
    display_string( textstring ); //Print out the string 'textstring'
    delay(2000);          // Delays 1 sec (adjust this value)
    tick( &mytime );     // Ticks the clock once
  }
}


/*  jtagconfig
make
dtekv-run main.bin
*/