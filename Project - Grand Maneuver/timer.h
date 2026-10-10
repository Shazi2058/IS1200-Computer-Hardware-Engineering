/* timer.h 
 This file written 2026 by Muhammad Shahwaiz Abbasi
 */
#ifndef TIMER_H
#define TIMER_H

void labinit(void);
void set_timer_seconds(int seconds);
int get_timer_seconds(void);
void update_displays(int seconds);
int time_limit(int level);

#endif