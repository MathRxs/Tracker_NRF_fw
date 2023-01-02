#include "timer.h"
struct k_timer timer;
uint8_t waked_up = 0;


/**
 * The timer callback function is called when the timer expires. It sets the waked_up variable to 0 and
 * stops the timer
 * 
 * @param timer_id The timer that triggered the callback.
 */
static void timer_callback(struct k_timer *timer_id){
    // printf("Timer0 expired\n");
    waked_up = 0;
    timer_stop();
}

/**
 * It initializes the timer.
 */
void timer_init(void){
    k_timer_init(&timer, timer_callback, NULL);
}

/**
 * "If the timer is running, stop it, then start it with the new timeout value."
 * 
 * The function is called from the main loop when the user presses the button
 * 
 * @param timeout_sec The number of seconds to wait before the timer expires.
 */
void timer_start(uint16_t timeout_sec){
    k_timer_start(&timer, K_SECONDS(timeout_sec), K_NO_WAIT);
}

/**
 * It stops the timer.
 */
void timer_stop(void){
    k_timer_stop(&timer);
}

/**
 * "If the timer is running, stop it, then start it with the new timeout value."
 * 
 * The function is called from the main loop when the user presses the button
 * 
 * @param timeout_sec The number of seconds to wait before the timer expires.
 */
void timer_restart(uint16_t timeout_sec){
    timer_stop();
    timer_start(timeout_sec);
}

