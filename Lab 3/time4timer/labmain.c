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
int timeoutcount = 0;
int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) 
{}

/* Add your code here for initializing interrupts. */
void labinit(void){
    volatile int *periodh = (volatile int *) 0x0400002C; // pointer to the timer period high register
    volatile int *periodl = (volatile int *) 0x04000028; // pointer to the timer period low register Since the timer rgsiter is 16 bit, we need to set the low and high registers separately
    *periodh = 0x002D; // Higher bits for 3 Million
    *periodl = 0xC6C0; // lower bits for 3 Million

    volatile int *timer_control   = (volatile int *) 0x04000024; // pointer to the timer control register
    *timer_control = 0x6; //start / continuous 
}
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
    return (*sw) & 0x3FF;
}
int get_btn(void){
    volatile int *btn = (volatile int *) 0x040000d0;
    return (*btn) & 1;
}

/* Your code goes into main as well as any needed functions. */
int main() {
    volatile int *timer_status = (volatile int *) 0x04000020;
    labinit();

    int led_count = 0;
    set_leds(led_count);
    while (led_count < 15) {
        if (*timer_status & 0x1) {
            *timer_status = 0;
            led_count++;
            set_leds(led_count);
        }
    }

    while (1) {
        int sw = get_sw();

        if (get_btn()) { //  Update mytime based on switch values
            int sw7 = (sw >> 7) & 0x1;
            if (sw7){
                break;
            }
            int mode = (sw >> 8) & 0x3; // Switches 8 and 9 determine the mode for seconds minutes and hours
            int val = sw & 0x3F;        // Get the value from switches 0-4

            if (mode == 1) { // Seconds
                mytime = (mytime & 0xFFFF00) | ((val / 10) << 4) | (val % 10);
            } else if (mode == 2) { // Minutes
                mytime = (mytime & 0xFF00FF) | (((val / 10) << 12) | ((val % 10) << 8));
            } else if (mode == 3) { // Hours
                mytime = (mytime & 0x00FFFF) | (((val / 10) << 20) | ((val % 10) << 16));
            }
        }

    
        if (*timer_status & 0x1) { // Check if the timer interrupt flag is set
            *timer_status = 0; 
            timeoutcount++; 

            
            if (timeoutcount >= 10) { // Update time every 10 timeouts (1 second)
                timeoutcount = 0;

                
                time2string(textstring, mytime);  // Update string 
                display_string(textstring);
                tick(&mytime);

                set_displays(0, seg_table[mytime & 0xF]); // Second first
                set_displays(1, seg_table[(mytime >> 4) & 0xF]); // Second Second

                // display 2 & 3: Minutes LSB & MSB
                set_displays(2, seg_table[(mytime >> 8) & 0xF]); // Minute first
                set_displays(3, seg_table[(mytime >> 12) & 0xF]); // Minute Second

                // display 4 & 5: Hours LSB & MSB
                set_displays(4, seg_table[(mytime >> 16) & 0xF]); // Hour first
                set_displays(5, seg_table[(mytime >> 20) & 0xF]); // Hour Second
            }
        }
    }
    return 0;
}



/*  jtagconfig
make
dtekv-run main.bin
*/